#ifndef COMMON_LIB_H
#define COMMON_LIB_H

#include <ostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

namespace gr {
    namespace TETRA_DMO {

        const std::vector<uint8_t> NORMAL_TRAINING_SEQ_1       = {1,1,0,1,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0}; // n1..n22
        const std::vector<uint8_t> NORMAL_TRAINING_SEQ_2       = {0,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,1,1,0}; // p1..p22
        const std::vector<uint8_t> NORMAL_TRAINING_SEQ_3_BEGIN = {0,0,0,1,1,0,1,0,1,1,0,1};                     // q11..q22
        const std::vector<uint8_t> NORMAL_TRAINING_SEQ_3_END   = {1,0,1,1,0,1,1,1,0,0};                         // q1..q10

        // 9.4.4.3.4 Synchronisation training sequence
        const std::vector<uint8_t> SYNC_TRAINING_SEQ = {1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1}; // y1..y38

        const std::vector<uint8_t> PREAMBLE_P1  =   {0,0,1,1,0,0,1,0,0,0,1,1};   // j1..j12
        const std::vector<uint8_t> PREAMBLE_P2  =   {1,0,0,1,1,0,1,0,1,0,0,1};   // k1..k12
        const std::vector<uint8_t> PREAMBLE_P3  =   {0,0,0,1,0,1,0,0,0,1,1,1};   // I1..I12

        enum BurstType: int{
        INACTIVE = 0,
        DSB = 1,
        DNB = 2,
        DNB_SF = 3,
        };

        struct TetraTime {
            uint16_t tn;                                                            ///< time slot
            uint16_t fn;                                                            ///< frame number
            uint16_t mn;                                                            ///< multi-frame number
        };

        bool m_bPreSynchronized = false;
        bool m_bIsSynchronized = false;
        uint64_t m_syncBitCounter = 0; 
        bool m_frameFound;
        bool m_validBurstFound;
        void Time_increase(TetraTime time);

        int FRAME_LEN = 510;

        enum MacLogicalChannel {                                                    // CP only
            NULL_CH = 0,
            DSCH_SH = 10,       // synchronisation channel + half slot signaling channel
            DSCH_F  = 12,       // full slot signaling channel  DMO MAC Logical Channels
            DSTCH   = 13,       // stealing channel             DMO MAC Logical Channels
            DTCH_S  = 14,       // traffic channel - vocie only DMO MAC Logical Channels
            DLCH    = 15        // linearization channel        DMO MAC Logical Channels
        }; 
        void updateSynchronizer(bool frameFound);
        bool isFrameFound();
        uint32_t patternAtPositionScore(std::vector<uint8_t> data, std::vector<uint8_t> pattern, std::size_t position);
        std::vector<uint8_t> vectorExtract(const std::vector<uint8_t>& source, uint32_t pos, int32_t length);
        std::vector<uint8_t> vectorAppend(std::vector<uint8_t> vec1, std::vector<uint8_t> vec2);
    }//end of TETRA_DMO
}//end of gr

#endif 