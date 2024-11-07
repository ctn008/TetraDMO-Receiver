/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_IMPL_H
#define INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_IMPL_H

#include <gnuradio/DMO_TETRA/DMO_burst_extract.h>


namespace gr {
  namespace DMO_TETRA {
    // ETSI EN 300 396-2 V1.4.1 DMO 9.4.3.3.4 Normal training sequence & 9.4.3.3.4 Synchronisation training sequence
    const std::vector<uint8_t> PREAMBLE_P1  =   {0,0,1,1,0,0,1,0,0,0,1,1};   // j1..j12
    const std::vector<uint8_t> PREAMBLE_P2  =   {1,0,0,1,1,0,1,0,1,0,0,1};   // k1..k12
    const std::vector<uint8_t> PREAMBLE_P3  =   {0,0,0,1,0,1,0,0,0,1,1,1};   // I1..I12

    //for GNU data sync
    const std::vector<uint8_t> PREAMBLE_START = {1,0,1,0,1,0,0,0};

    class DMO_burst_extract_impl : public DMO_burst_extract
    {
     private:
      // Nothing to declare in this block.
      bool OP_DMO;
      int burstType;
      
     public:
      DMO_burst_extract_impl();
      ~DMO_burst_extract_impl();

      void set_burstType(int burstType){this->burstType = burstType;};
      int get_burstType(){return burstType;};
      void set_OP(bool OP){OP_DMO = OP;};
      bool get_OP(){return OP_DMO;};

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace DMO_TETRA
} // namespace gr

#endif /* INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_IMPL_H */
