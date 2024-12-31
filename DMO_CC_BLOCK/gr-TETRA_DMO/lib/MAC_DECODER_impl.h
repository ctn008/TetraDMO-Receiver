/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TETRA_DMO_MAC_DECODER_IMPL_H
#define INCLUDED_TETRA_DMO_MAC_DECODER_IMPL_H

#include <gnuradio/TETRA_DMO/MAC_DECODER.h>
#include "gnuradio/TETRA_DMO/Pdu.h"
#include "gnuradio/TETRA_DMO/tetracell.h"
#include "gnuradio/TETRA_DMO/uplane.h"
#include "gnuradio/TETRA_DMO/viterbicodec.h"


namespace gr {
  namespace TETRA_DMO {

    class MAC_DECODER_impl : public MAC_DECODER
    {
     private:
      // Nothing to declare in this block.
      std::shared_ptr<TetraCell> m_tetraCell;
      std::shared_ptr<UPlane> m_Uplane;
      std::shared_ptr<ViterbiCodec> m_viterbiCodec1614;
      TetraTime m_tetraTime;

     public:
      MAC_DECODER_impl();
      ~MAC_DECODER_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);

      void Mac_channel_decode(std::vector<uint8_t> bkn1, std::vector<uint8_t> bkn2, BurstType type, uint8_t *out, int &out_index);
      void Mac_service(Pdu pdu, MacLogicalChannel LogicalChannel, uint8_t *out, int &out_index);
      Pdu  pduProcessDmacSync(const Pdu pdu);
      Pdu  pduProcessDmacData(const Pdu pdu);

      std::vector<uint8_t> descramble(std::vector<uint8_t> data, const int len, const uint32_t scramblingCode);
      std::vector<uint8_t> deinterleave(std::vector<uint8_t> data, const uint32_t K, const uint32_t a);
      std::vector<uint8_t> depuncture23(std::vector<uint8_t> data, const uint32_t len);
      std::vector<uint8_t> viterbiDecode1614(std::vector<uint8_t> data);
      int checkCrc16Ccitt(std::vector<uint8_t> data, const int len);
      std::vector<uint8_t> nullPDU;
    };


  } // namespace TETRA_DMO
} // namespace gr

#endif /* INCLUDED_TETRA_DMO_MAC_DECODER_IMPL_H */
