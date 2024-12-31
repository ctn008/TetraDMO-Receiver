#include "gnuradio/TETRA_DMO/common_lib.h"

namespace gr {
    namespace TETRA_DMO {

        uint32_t patternAtPositionScore(std::vector<uint8_t> data, std::vector<uint8_t> pattern, std::size_t position)
        {
            uint32_t errors = 0;

            for (std::size_t idx = 0; idx < pattern.size(); idx++)
            {
                errors += (uint32_t)(pattern[idx] ^ data[position + idx]);
            }
        
            return errors;
            }

        std::vector<uint8_t> vectorExtract(const std::vector<uint8_t>& source, uint32_t pos, int32_t length)
        {
            if (length <= 0 || pos >= source.size()) {
                return {}; // Return an empty vector if invalid length or position
            }

            length = std::min(length, static_cast<int32_t>(source.size() - pos)); // Clamp length to remaining elements
            return std::vector<uint8_t>(source.begin() + pos, source.begin() + pos + length);
        }


        std::vector<uint8_t> vectorAppend(std::vector<uint8_t> vec1, std::vector<uint8_t> vec2)
        {
            std::vector<uint8_t> ret(vec1);
            ret.insert(ret.end(), vec2.begin(), vec2.end());
            return ret;
        }


        void updateSynchronizer(bool frameFound)
        {
            if (frameFound)
            {
                if (m_bPreSynchronized == true)
                {
                    m_bIsSynchronized = true;
                }
                else
                {
                    m_bPreSynchronized = true;
                }
                m_syncBitCounter  = 0;             // reset syncBitCounter if frameFound 
            }
            else
            {
                if ( (m_bPreSynchronized) || (m_bIsSynchronized) )
                {
                    if (m_syncBitCounter > FRAME_LEN * 8)   // if syncBitCounter > 8 frames, 
                    {
                        m_bIsSynchronized = false;          // ie. no frameFound in 8 frames
                        m_bPreSynchronized = false;         // then reset Synch status
                        m_syncBitCounter  = 0;
                    }
                }
            }
        }

        bool isFrameFound()
        {
        bool frameFound = false;
        /*
        uint32_t score_preamble_p1 = patternAtPositionScore(m_frame, PREAMBLE_P1, 0+34);  // do not count 34 guard bits, burst starts @ zero
        uint32_t score_preamble_p2 = patternAtPositionScore(m_frame, PREAMBLE_P2, 0+34);  // do not count 34 guard bits
        uint32_t score_preamble_p3 = patternAtPositionScore(m_frame, PREAMBLE_P3, 0+34);  // do not count 34 guard bits

        uint32_t scoreSync    = patternAtPositionScore(m_frame, SYNC_TRAINING_SEQ,     214+34);
        uint32_t scoreNormal1 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_1, 230+34);
        uint32_t scoreNormal2 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_2, 230+34);
        */
        uint32_t score_preamble_p1 = patternAtPositionScore(m_burst, PREAMBLE_P1, 0+34);  // do not count 34 guard bits, burst starts @ zero
        uint32_t score_preamble_p2 = patternAtPositionScore(m_burst, PREAMBLE_P2, 0+34);  // do not count 34 guard bits
        uint32_t score_preamble_p3 = patternAtPositionScore(m_burst, PREAMBLE_P3, 0+34);  // do not count 34 guard bits

        uint32_t scoreSync    = patternAtPositionScore(m_burst, SYNC_TRAINING_SEQ,     214+34);
        uint32_t scoreNormal1 = patternAtPositionScore(m_burst, NORMAL_TRAINING_SEQ_1, 230+34);
        uint32_t scoreNormal2 = patternAtPositionScore(m_burst, NORMAL_TRAINING_SEQ_2, 230+34);

        scoreNormal1 = (scoreNormal1*38)/22;                                           // multifly with 1.5 to be comparable with STS
        scoreNormal2 = (scoreNormal2*38)/22;

        scoreNormal1 += score_preamble_p1;                                             // combine bit errors of preamble & training sequences
        scoreNormal2 += score_preamble_p2;
        scoreSync += score_preamble_p3;

        // soft decision to detect burst: bit errors in preamble & training sequence less than 6
        uint32_t scoreMin = scoreSync;                                                  // if same score, Sync Burst is chosen
        m_burstType = DSB;                                                              // same enum name is used for TMO & DMO (DSB, DNB DNB_SF)

        if (scoreNormal1 < scoreMin)
        {
            scoreMin  = scoreNormal1;
            m_burstType = DNB;
        }
        if (scoreNormal2 < scoreMin)
        {
            scoreMin  = scoreNormal2;
            m_burstType = DNB_SF;
        }

        if (scoreMin <= FRAME_DETECT_THRESHOLD)                                                          // frame (burst) is matched and can be processed
        {                                                                           // max 1 error for preamble + 5 errors for sts 
            frameFound = true;
            m_validBurstFound = true;
        }
        else
        {
            if (m_bIsSynchronized)
            {
                m_burstType = IDLE;
            }
        }
        return frameFound;
        }

  

    }//end of DMO_TETRA
}//end of gr