# Thiết kế chương trình tetraDMO-Receiver

## 1. Mục tiêu: 
Một chương trình duy nhất thu tín hiệu từ thiết bị SDR (rtlsdr hoặc pluto sdr) và thực hiện đầy đủ các chức năng giải điều chế pi4dqpsk, decode tín hiệu tetraDMO và đẩy tín hiệu audio ra loa (đồng thời xử lý chỉ bật loa khi nhận được cuộc gọi và tắt loa khi cuộc gọi kết thúc. 

Chỉ thực hiện nhận cuộc gọi nhóm, và hiển thị Talkgroup ID, Caller ID khi nhận cuộc gọi, đồng thời với việc đẩy audio ra loa.

## 2. Lựa chọn ngôn ngữ lập trình và công cụ: 
Sau khi xem xét các lựa chọn tự viết chương trình trên C++, Python, và sử dụng GNURadio, quyết định lựa chọn PA sử dụng GNURadio kết hợp với viết các module OOT để xử lý các chức năng tetra decoder, speech codec decoder và đưa audio ra loa, với các lý do sau đây:
- GNURadio sử dụng flowchart với các block thực hiện các chức năng, có cấu trúc rõ ràng, mang tính trực quan
- Trên GNUradio đã có sẵn các khối pi4 demodulation với signal source cho rtlsdr và plutoSdr, cũng như AudioSink đẩy audio ra loa.
- Phát triển các OOT module trên GNURadio để nâng cao khả năng khai thác, sử dụng GNURadio
- Mặc dù GNURadio chạy python wrapper, nhưng các module viết trên c++, nên đảm bảo về tốc độ xử lý nhanh.

## 3. Thiết kế chương trình:

![image](https://github.com/user-attachments/assets/5c631208-7ae3-4c13-b656-f565dc518c70)

Giao tiếp giữa các module:  
(1) IQ signal stream of complex numbers at sampling rate  
(2) demoded s36kbit stream, each byte is 01 bit, at rate 36k samples per second  
(3) Tetra ACELP encoded speech frames (432 bits each): vector of 432 byte length, each byte represent 01 bit  
(4) raw speech data stream at rate 8ksps, each data sample is a int16 word  

