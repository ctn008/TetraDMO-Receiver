# Project Title: TETRA Receiver (RTL-SDR)

## 📌 Overview
This project implements a **TETRA DMO receiver** using an **RTL-SDR device**, GNU Radio, and custom Python/C++ blocks.  
It can demodulate and decode TETRA signals from a handset and stream live audio to a PC.  

This work was carried out as part of a **graduation thesis project in Vietnam**, where I mentored undergraduate students.

---

## 🛠️ Features
- Real-time demodulation and decoding of TETRA DMO signals  
- Audio stream output to PC speakers  
- Custom GNU Radio blocks written in **Python** and **C++**  
- Tested with live RF signals from a TETRA handset  

---

## 🔧 Tools & Technologies
- Hardware: RTL-SDR USB dongle  
- Software: GNU Radio, Python, C++  
- OS: Linux (Ubuntu recommended)  
- Protocols: TETRA (Direct Mode Operation)  

---

## 🚀 How to Run
1. Clone this repo:  
   ```bash
   git clone https://github.com/yourusername/tetra-receiver.git

# Tetra DMO Receiver
Using RTL-SDR device to receive Tetra DMO signal and demodulated, decoded the received signal to retrieve audio and output to PC speakers.
The repo consists of 2 main programs: 
## 1. DMO Decoder
Fork from tetra-kit decoder and modify to include option to decode DMO tetra signal from file 

Chạy chương trình với command line option -D để chạy DMO mode.
./decoder -D -d n -i /path/filname
options:  -d n với n=1, 4 để các level debug khác nhau (sẽ output dữ liệu ra màn hình)

compile bằng lệnh make với makefile default đã có sẵn.

Các file dữ liệu mẫu để test:
RxDMO_pluto_390mhz_s36kBit.uint8
plutoDMO_bits.uint8
rtlsdrTMO_bits.uint8

khi chạy decoder, speech audio data sẽ được ghi ra file .out với tên file được generate tự động, và lưu vào subfolder /audio. 
File .out này là các tetra audio frame (tương ứng với 30ms), cần chạy Tetra speech decoder để chuyển đổi sang file .wav bằng chương trình out2wave.sh trong tetra-kit/recorder

## TDMO Receiver
A program runs on GNURadio toolkit with custom blocks to demodulate and decode Tetra signal received from Rtl-Sdr receiver.

Refer to https://github.com/ctn008/TetraDMO-Receiver/blob/main/tetraDMO-Receiver%20Design.md
