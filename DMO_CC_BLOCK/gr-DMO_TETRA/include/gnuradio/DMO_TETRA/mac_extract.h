/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_DMO_TETRA_MAC_EXTRACT_H
#define INCLUDED_DMO_TETRA_MAC_EXTRACT_H

#include <gnuradio/DMO_TETRA/api.h>
#include <gnuradio/sync_block.h>

#include "gnuradio/DMO_TETRA/common/tetra.h"
#include "gnuradio/DMO_TETRA/common/layer.h"
#include "gnuradio/DMO_TETRA/common/base64.h"
#include "gnuradio/DMO_TETRA/common/log.h"
#include "gnuradio/DMO_TETRA/common/pdu.h"
#include "gnuradio/DMO_TETRA/common/report.h"
#include "gnuradio/DMO_TETRA/common/tetracell.h"
#include "gnuradio/DMO_TETRA/common/utils.h"


namespace gr {
  namespace DMO_TETRA {

    /*!
     * \brief <+description of block+>
     * \ingroup DMO_TETRA
     *
     */

     enum MacLogicalChannel {                                                    // CP only
        AACH    = 0,
        BLCH    = 1,
        BNCH    = 2,
        BSCH    = 3,
        SCH_F   = 4,
        SCH_HD  = 5,
        STCH    = 6,
        TCH_S   = 7,
        TCH     = 8,
        unknown = 9,
        DSCH_SH = 10,       // synchronisation channel + half slot signaling channel
        DSCH_F  = 12,       // full slot signaling channel  DMO MAC Logical Channels
        DSTCH   = 13,       // stealing channel             DMO MAC Logical Channels
        DTCH_S  = 14,       // traffic channel - vocie only DMO MAC Logical Channels
        DLCH    = 15        // linearization channel        DMO MAC Logical Channels
    }; 

    

    class DMO_TETRA_API mac_extract : virtual public gr::sync_block
    {
     public:
      typedef std::shared_ptr<mac_extract> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of DMO_TETRA::mac_extract.
       *
       * To avoid accidental use of raw pointers, DMO_TETRA::mac_extract's
       * constructor is in a private implementation
       * class. DMO_TETRA::mac_extract::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace DMO_TETRA
} // namespace gr

#endif /* INCLUDED_DMO_TETRA_MAC_EXTRACT_H */
