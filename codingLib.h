#ifndef CODINGLIB
#define CODINGLIB
#include "viterbi.h"

extern "C" {
ViterbiCodec * m_viterbiCodec1614;                                      ///< Viterbi codec
int descramble_raw(uint8_t* in_data, int in_len, uint8_t* out_data, const int len, const uint32_t scramblingCode);
std::vector<uint8_t> descramble(std::vector<uint8_t> data, const int len, const uint32_t scramblingCode);
std::vector<uint8_t> scramble(std::vector<uint8_t> data, int len, uint32_t scramblingCode);

int deinterleave_raw(uint8_t* in_data, int in_len, uint8_t* out_data, const uint32_t K, const uint32_t a);
std::vector<uint8_t> deinterleave(std::vector<uint8_t> data, const uint32_t K, const uint32_t a);
std::vector<uint8_t> interleave(std::vector<uint8_t> data, const uint32_t K, const uint32_t a);

int depuncture23_raw(uint8_t* in_data, int in_len, uint8_t* out_data, const uint32_t len);
std::vector<uint8_t> depuncture23(std::vector<uint8_t> data, const uint32_t len);
std::vector<uint8_t> puncture23(std::vector<uint8_t> data, const uint32_t len);

int viterbiDecode1614_raw(uint8_t* in_data, int in_len, uint8_t* out_data);
std::vector<uint8_t> viterbiDecode1614(std::vector<uint8_t> data);
std::vector<uint8_t> motherEncode1614(std::vector<uint8_t> data, const uint32_t len);

std::vector<uint8_t> reedMuller3014Decode(std::vector<uint8_t> data);
std::vector<uint8_t> reedMuller3014Encode(std::vector<uint8_t> data);
uint32_t rm3014Compute(const uint16_t in);

bool checkCrc16Ccitt_raw(uint8_t* in_data, int in_len, const int len);
bool checkCrc16Ccitt(std::vector<uint8_t> data, const int len);
uint16_t computeCrc16Ccitt(std::vector<uint8_t> data, const int len);

}
#endif /* CODINGLIB */
