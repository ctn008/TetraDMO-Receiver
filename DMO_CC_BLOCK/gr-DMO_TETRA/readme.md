This is C++ embedded block in GNURADIO for DMO TETRA processing. 

Work done: 

DMO burst extract block, which takes input as output of burst Sync of CTN008's block and parse out extracted BKN block.

currently working:

MAC_extract block, which takes input as output of DMO burst extact block. Expecting to process information inside the burst, as well as voice data. 

Optimization: 
The block needs to push data in a controllable way instead of continous stream. 

