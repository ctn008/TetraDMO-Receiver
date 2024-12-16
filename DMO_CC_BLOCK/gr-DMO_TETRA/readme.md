# This is C++ embedded block in GNURADIO for DMO TETRA processing. 

Work done: 

MAC_DECODER_LAYER_2_3: TETRA signal processing at layer 2 and 3, sending speech frame to TETRA speech CODEC. 

Issues: 

- Inaudible audio output

- Incorrect burst decoding after an amount of time running. 

# Guide:

- Navigate into working folder: gr-DMO_TETRA
- delete build folder: `rm -rf build`
- generate a new build folder: `mkdir build`
- Build the block:
  ```
  cd build
  make clean
  make
  sudo make install
  ```
- Run the GNUradio, navigate into DMO_TETRA module and choose MAC_DECODER_LAYER_2_3 

# Workflow: 

- at general_scope:

![general_scope](https://github.com/user-attachments/assets/bcd49960-f905-4e4c-8272-7e30ddbbfefa)

- at mac_channel_decode scope:

![mac_channel_decode_scope](https://github.com/user-attachments/assets/0632c2d8-a778-4262-b742-c7395a7c4c1a)

- at mac_service scope:

![mac_service_scope](https://github.com/user-attachments/assets/98fa7737-a129-45c1-9a06-4aea4f036ae6)




