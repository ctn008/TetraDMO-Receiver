/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DMO_TETRA_MAC_DECODER_LAYER_2_3_H
#define INCLUDED_DMO_TETRA_MAC_DECODER_LAYER_2_3_H

#include <gnuradio/DMO_TETRA/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace DMO_TETRA {

    /*!
     * \brief <+description of block+>
     * \ingroup DMO_TETRA
     *
     */
    class DMO_TETRA_API MAC_DECODER_LAYER_2_3 : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<MAC_DECODER_LAYER_2_3> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of DMO_TETRA::MAC_DECODER_LAYER_2_3.
       *
       * To avoid accidental use of raw pointers, DMO_TETRA::MAC_DECODER_LAYER_2_3's
       * constructor is in a private implementation
       * class. DMO_TETRA::MAC_DECODER_LAYER_2_3::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace DMO_TETRA
} // namespace gr

#endif /* INCLUDED_DMO_TETRA_MAC_DECODER_LAYER_2_3_H */
