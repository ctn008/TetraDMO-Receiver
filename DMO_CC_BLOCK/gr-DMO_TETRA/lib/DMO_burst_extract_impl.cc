/* -*- c++ -*- */
/*
 * Copyright 2024 kien.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "DMO_burst_extract_impl.h"

namespace gr {
  namespace DMO_TETRA {

    // #pragma message("set the following appropriately and remove this warning")
    using input_type = uint8_t;
    // #pragma message("set the following appropriately and remove this warning")
    using output_type = uint8_t;
    DMO_burst_extract::sptr
    DMO_burst_extract::make()
    {
      return gnuradio::make_block_sptr<DMO_burst_extract_impl>(
        );
    }


    /*
     * The private constructor
     */
    DMO_burst_extract_impl::DMO_burst_extract_impl()
      : gr::sync_block("DMO_burst_extract",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(output_type)))
    {}

    /*
     * Our virtual destructor.
     */
    DMO_burst_extract_impl::~DMO_burst_extract_impl()
    {
    }

    uint32_t patternAtPositionScore(std::vector<uint8_t> data, std::vector<uint8_t> pattern, std::size_t position)
    {
      uint32_t errors = 0;

      for (std::size_t idx = 0; idx < pattern.size(); idx++)
      {
          errors += (uint32_t)(pattern[idx] ^ data[position + idx]);
      }

      return errors;
    }

    std::vector<uint8_t> vectorExtract(std::vector<uint8_t> source, const uint32_t pos, const int32_t length)
    {
        std::vector<uint8_t> ret;

        if (length > 0)                                                             // check if invalid length requested
        {
            int32_t len = (int32_t)source.size() - (int32_t)pos;                    // actual remaining bytes in vector after pos

            if (len > 0)                                                            // check if actual length is valid
            {
                if (len > length)                                                   // we have more bytes than requested
                {
                    len = length;                                                   // so return only the requested ones
                }

                std::copy(source.begin() + pos, source.begin() + pos + (uint32_t)len, back_inserter(ret));
            }
        }

        return ret;
    }




    int DMO_burst_extract_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      // Access input and output items
      const uint8_t *in = static_cast<const uint8_t *>(input_items[0]);
      uint8_t *out = static_cast<uint8_t *>(output_items[0]);

      int in_index = 0;
      int out_index = 0;
      int null_count = 0;

      BurstType burst_type;

      for (int i = 0; i<noutput_items; i++)
      {
        if (in[i] == 0){null_count++;}
        else {null_count =0;}
        
        std::vector<uint8_t> bkn1;
        std::vector<uint8_t> bkn2;
        std::vector<uint8_t> temp(520);

        std::copy(in + i + in_index, in + i + in_index+520, temp.begin()); // buffer 
        // for (int j =0; j<= temp.size(); j++)
        // {
        //   std::cout << static_cast<int>(temp[j]) << " ";
        // }

        if(patternAtPositionScore(temp, PREAMBLE_START,0) != 0) //check for PREAMBLE_START to 
        {
          continue;
        }
        else 
        {
          // printf("data_start \n");
          int pos = 0;
          pos +=9; 
          std::vector<uint8_t> type = vectorExtract(temp,pos,1);

          // for (int j =0; j< type.size(); j++)
          // {
          //   std::cout << "type: " << static_cast<int>(type[j]) << " ";
          // }

          burst_type = static_cast<BurstType>(type[0]); // cast out Burst Type from bit 10
          pos += 1;
          if(burst_type == DNB || burst_type == DNB_SF)
          {
            pos += 48;
            bkn1 = vectorExtract(temp,pos,216); //take out BKN1
            // std::cout <<"bkn1 start: \n";

            // for (int j =0; j<= bkn1.size(); j++)
            // {
            //   std::cout << static_cast<int>(bkn1[j]) << " ";
            // }
            // std::cout <<"\n length: " << bkn1.size() << "\n";
            // std::cout <<"\n";

            pos += 238;               //216 + 22
            bkn2 = vectorExtract(temp,pos,216); //take out BKN 2

            // std::cout <<"bkn2 start: \n";
            // for (int j =0; j<= bkn2.size(); j++)
            // {
            //   std::cout << static_cast<int>(bkn2[j]) << " ";
            // }
            // std::cout <<"\n";
            // std::cout <<"\n length: " << bkn2.size() << "\n";

          }
          else if(burst_type == DSB)
          {
            pos += 128;
            bkn1 = vectorExtract(temp,pos,120);

            // std::cout <<"bkn1 start: \n";
            // for (int j =0; j<= bkn1.size(); j++)
            // {
            //   std::cout << static_cast<int>(bkn1[j]) << " ";
            // }
            // std::cout <<"\n length: " << bkn1.size() << "\n";
            // std::cout <<"\n";

            pos += 158;               //120 + 38 
            bkn2 = vectorExtract(temp,pos,216);

            // std::cout <<"bkn2 start: \n";
            // for (int j =0; j<= bkn2.size(); j++)
            // {
            //   std::cout << static_cast<int>(bkn2[j]) << " ";
            // }
            // std::cout <<"\n length: " << bkn2.size() << "\n";
            // std::cout <<"\n";

          }

          //construct output vector 

          std::vector<uint8_t> result(520);
          // Initialize the preamble `10101`
          for (int i = 0; i < 5; i++) 
          {
            result[i] = (i % 2 == 0) ? 1 : 0;
          }

          // tag burst_type 
          pos = 5;
          for (int j = 1; j >=0; j--)
          {
            result[pos++] = (burst_type >> j & 1);
          }  

          // Parse in `lengthBKN1` (size of `bkn1` in bits)
          for (int j = 7; j >= 0; j--)
          {
            result[pos++] = (bkn1.size() >> j) & 1;
          }

          // Parse in `BKN1` data
          for (size_t i = 0; i < bkn1.size(); i++) 
          {
            result[pos++] = bkn1[i];
          }

          // Parse in `lengthBKN2` (size of `bkn2` in bits)
          for (int j = 7; j >= 0; j--)
          {
            result[pos++] = (bkn2.size() >> j) & 1;
          }
          // Parse in `BKN2` data
          for (size_t i = 0; i < bkn2.size(); i++)
          {
            result[pos++] = bkn2[i];
          }
          std::copy(result.begin(), result.end(), out + out_index);
        in_index +=520;
        out_index +=520;
        }
      }
    consume_each(noutput_items);

    return noutput_items;
    }
  } /* namespace DMO_TETRA */
} /* namespace gr */


