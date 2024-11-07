/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_H
#define INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_H

#include <gnuradio/DMO_TETRA/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace DMO_TETRA {

    /*!
     * \brief <+description of block+>
     * \ingroup DMO_TETRA
     *
     */
     enum OperationMode{
      DMO = 1,
      TMO = 0,
    };

    enum BurstType: int{
      INACTIVE = 0,
      DSB = 1,
      DNB = 2,
      DNB_SF = 3,
    };
    
    class DMO_TETRA_API DMO_burst_extract : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<DMO_burst_extract> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of DMO_TETRA::DMO_burst_extract.
       *
       * To avoid accidental use of raw pointers, DMO_TETRA::DMO_burst_extract's
       * constructor is in a private implementation
       * class. DMO_TETRA::DMO_burst_extract::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace DMO_TETRA
} // namespace gr

#endif /* INCLUDED_DMO_TETRA_DMO_BURST_EXTRACT_H */
