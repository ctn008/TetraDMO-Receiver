#include "decoder.h"
using namespace Tetra;

/**
 * @brief Tetra decoder
 *
 */

TetraDecoder::TetraDecoder(int socketFd, bool bRemoveFillBits, const LogLevel logLevel, bool bEnableWiresharkOutput,  bool bDmoMode)
{
    m_socketFd = socketFd;

    m_log       = new Log(logLevel);

    m_report    = new Report(m_socketFd, m_log);
    m_tetraCell = new TetraCell();

    m_sds    = new Sds(m_log, m_report);
    m_cmce   = new Cmce(m_log, m_report, m_sds);
    m_mm     = new Mm(m_log, m_report);
    m_sndcp  = new Sndcp(m_log, m_report);
    m_mle    = new Mle(m_log, m_report, m_cmce, m_mm, m_sndcp);
    m_llc    = new Llc(m_log, m_report, m_mle);
    m_uPlane = new UPlane(m_log, m_report);
    if (bEnableWiresharkOutput)
    {
        m_wireMsg = new WireMsg();
    }
    else
    {
        m_wireMsg = NULL;
    }

    m_mac    = new Mac(m_log, m_report, m_tetraCell, m_uPlane, m_llc, m_mle, m_wireMsg, bRemoveFillBits);

    m_frame.clear();

    m_bIsSynchronized = false;
    m_syncBitCounter  = 1;
    m_bDmoMode = bDmoMode;
    m_totalBits = 0;
    m_errorBits = 0;

}

/**
 * @brief Destructor
 *
 */

TetraDecoder::~TetraDecoder()
{
    delete m_mac;
    delete m_uPlane;
    delete m_llc;
    delete m_mle;
    delete m_sndcp;
    delete m_mm;
    delete m_cmce;
    delete m_sds;
    delete m_tetraCell;
    delete m_log;
    delete m_report;
    delete m_wireMsg;
}

/**
 * @brief Reset the synchronizer
 *
 * Burst was matched, we can reset the synchronizer to allow 50 missing frames (expressed in burst units = 50 * 510 bits)
 *
 */

void TetraDecoder::resetSynchronizer()
{
    m_bIsSynchronized = true;
    m_syncBitCounter  = FRAME_LEN * 50;                                         // allow 50 missing frames (in bits unit)
}

/**
 * @brief Process a received symbol.
 *
 * This function is called by "physical layer" when a bit is ready
 * to be processed.
 *
 * Note that "frame" is actually called "burst" in Tetra doc
 *
 * @return true if frame (burst) found, false otherwise
 *
 */

bool TetraDecoder::rxSymbol(uint8_t sym)
{
    m_frame.push_back(sym);                                                     // insert symbol at buffer end
    if (m_bIsSynchronized == true)
    {
        m_syncBitCounter--;
    }
    if (m_frame.size() < FRAME_LEN)                                             // not enough data to process
    {
        return 0;
    }

    m_frameFound = isFrameFound();
    if (m_frameFound == true)
    {
        resetSynchronizer();                                                    // reset missing sync synchronizer        
    }

    bool clearedFlag = false;

    if (m_frameFound || (m_bIsSynchronized && ((m_syncBitCounter % 510) == 0)))   // the frame can be processed either by presence of training sequence, either by synchronised and still allowed missing frames
    {
        /* Issue arises here with DMO mode, when burst not found, still processFrame of noise, 
        while processFrame do nothing and keep the old m_validBurtFound value, that cause serviceLowerMac is called wrongly.
        Technically, for DMO mode, no sync loss is tolerated ??? */
        
        if ((m_bDmoMode) && (!m_frameFound))
        {
            // ignore the data (just an empty burst in DMO mode)
            #if 0
            Tetra::TetraTime m_tetraTime = m_mac->getTime();
            m_log->print(LogLevel::NONE, "Ignore DMO unsync'ed burst : TN/FN/MN = %2u/%2u/%2u  DMO burst type=???\n",
                m_tetraTime.tn,
                m_tetraTime.fn,
                m_tetraTime.mn);
            #endif
        }
        else
        {
            processFrame();
            if (m_validBurstFound)      // valid burst found, send it to MAC

            {           
                m_mac->serviceLowerMac(m_frame, m_burstType);
            }
        }
        // frame has been processed, so clear it
        m_frame.clear();

        // set flag to prevent erasing first bit in frame
        clearedFlag = true;

        m_mac->incrementTn();
    }

    if (m_syncBitCounter <= 0)
    {
        // synchronization is lost
        if (m_bIsSynchronized)
        {
            printf("\n* synchronization lost...\n\n");
        }
        m_bIsSynchronized  = false;
        m_syncBitCounter = 0;
    }

    if (!clearedFlag)
    {
        // remove first symbol from buffer to make space for next one
        m_frame.erase(m_frame.begin());
    }

    return m_frameFound;
}

/**
 * @brief Report information to screen
 *
 */

void TetraDecoder::printData()
{
    std::string txt = "";
    for (int i = 0; i < 12; i++) txt += m_frame[i] == 0 ? "0" : "1";

    txt += " ";
    for (int i = 12; i < 64; i++) txt += m_frame[i] == 0 ? "0" : "1";

    txt += " ";
    for (int i = 510 - 11; i < 510; i++) txt += m_frame[i] == 0 ? "0" : "1";

    printf("%s", txt.c_str());
}

/**
 * @brief Process frame to decide which type of burst it is then service lower MAC
 *
 */

bool TetraDecoder::isFrameFound()
{
    bool frameFound = false;
    if (! m_bDmoMode)
    {
        uint32_t scoreBegin = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_3_BEGIN, 0);
        uint32_t scoreEnd   = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_3_END, 500);

        if ((scoreBegin + scoreEnd) < 2)                                            // frame (burst) is matched and can be processed
        {                                                                           // tolerate 1 bit error for q1..q22 
            frameFound = true;
        }
        // update errorBits if synchronisationStatus is ON
        if (m_bIsSynchronized)
        {
            updateBitErrs( 22, scoreBegin + scoreEnd);  // 22 = NTS length (number of bits) 
            if ((scoreBegin + scoreEnd) > 0)
            {
                printf("Bit errors found in NTS3 of TMO Burst: %u, TN/FN/MN: %u/%u/%u\n", scoreBegin+scoreEnd, m_mac->getTime().tn, m_mac->getTime().fn, m_mac->getTime().mn);
            }
        }

    }
    else    // DMO mode
    {
        uint32_t score_preamble_p1 = patternAtPositionScore(m_frame, PREAMBLE_P1, 0);  // do not count 34 guard bits, burst starts @ zero
        uint32_t score_preamble_p2 = patternAtPositionScore(m_frame, PREAMBLE_P2, 0);  // do not count 34 guard bits
        uint32_t score_preamble_p3 = patternAtPositionScore(m_frame, PREAMBLE_P3, 0);  // do not count 34 guard bits

        uint32_t scoreSync    = patternAtPositionScore(m_frame, SYNC_TRAINING_SEQ,     214);
        uint32_t scoreNormal1 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_1, 230);
        uint32_t scoreNormal2 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_2, 230);

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

        if (scoreMin <= 6)                                                          // frame (burst) is matched and can be processed
        {                                                                           // max 1 error for preamble + 5 errors for sts 
            frameFound = true;
            m_validBurstFound = true;
            // TEMPORARY RESULT. not good estimation - need to look into more details
            updateBitErrs( 12+38 , scoreMin);  // 12 + 38 = preamble + STS (NTS has ajdjusted to approx 38 bits) 

            if (scoreMin > 0)
            {
                printf("Bit errors found in DMO Burst: %u, TN/FN/MN: %u/%u/%u\n", scoreMin, m_mac->getTime().tn, m_mac->getTime().fn, m_mac->getTime().mn);
            }

        }

    }


    return frameFound;
}

void TetraDecoder::processFrame()
{

    if (!m_bDmoMode)    // only process Frame in TMO mode
    {
        uint32_t scoreSync    = patternAtPositionScore(m_frame, SYNC_TRAINING_SEQ,     214);
        uint32_t scoreNormal1 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_1, 244);
        uint32_t scoreNormal2 = patternAtPositionScore(m_frame, NORMAL_TRAINING_SEQ_2, 244);

        scoreNormal1 = (scoreNormal1*3) >> 1;                                          // multifly with 1.5 to be comparable with STS
        scoreNormal2 = (scoreNormal2*3) >> 1;

        // soft decision
        uint32_t scoreMin = scoreSync;

        if (scoreNormal1 < scoreMin)
        {
            scoreMin  = scoreNormal1;
            m_burstType = NDB;
        }

        if (scoreNormal2 < scoreMin)
        {
            scoreMin  = scoreNormal2;
            m_burstType = NDB_SF;
        }

        if (scoreMin <= 5)
        {
            m_validBurstFound = true;
            updateBitErrs( 38 , scoreMin);  // 38 =  STS length (NTS has ajdjusted to approx 38 bits) 

            if (scoreMin > 0)
            {
                printf("Bit errors found in TMO Burst: %u, TN/FN/MN: %u/%u/%u\n", scoreMin, m_mac->getTime().tn, m_mac->getTime().fn, m_mac->getTime().mn);
            }

        }


    }
}

/**
 * @brief Return pattern/data comparison errors count at position in data vector
 *
 * @param data      Vector to look in from pattern
 * @param pattern   Pattern to search
 * @param position  Position in vector to start search
 *
 * @return Score based on similarity with pattern (differences count between vector and pattern)
 *
 */

uint32_t TetraDecoder::patternAtPositionScore(std::vector<uint8_t> data, std::vector<uint8_t> pattern, std::size_t position)
{
    uint32_t errors = 0;

    for (std::size_t idx = 0; idx < pattern.size(); idx++)
    {
        errors += (uint32_t)(pattern[idx] ^ data[position + idx]);
    }

    return errors;
}

void TetraDecoder::updateBitErrs(uint16_t totalBits, uint16_t errorBits)
{
    m_totalBits += totalBits;
    m_errorBits += errorBits;
}

void TetraDecoder::getBitErrs(uint64_t* totalBits, uint64_t* errorBits)
{
    *totalBits = m_totalBits;
    *errorBits = m_errorBits;
}
