/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DMO_TETRA_MAC_EXTRACT_IMPL_H
#define INCLUDED_DMO_TETRA_MAC_EXTRACT_IMPL_H

#include <gnuradio/DMO_TETRA/mac_extract.h>


namespace gr {
  namespace DMO_TETRA {

    class mac_extract_impl : public mac_extract
    {
     private:
      // Nothing to declare in this block.

     public:
      mac_extract_impl();
      ~mac_extract_impl();

      void MAC_lower(std::vector<uint8_t> bkn1, int lengthBKN1, std::vector<uint8_t> bkn2, int lengthBKN2);
      void MAC_upper(Tetra::Pdu Pdu, Tetra::MacLogicalChannel MacLogicalChannel);

      // Where all the action really happens
      int work(
              int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
      );
    };

  } // namespace DMO_TETRA
} // namespace gr

#endif /* INCLUDED_DMO_TETRA_MAC_EXTRACT_IMPL_H */
