"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!

dmoBurstSync
- input is a stream of demodulated bits (each bit in one byte)
- output is a stream of consecutive 510 bits (bytes) after burst synchronisation is detected.
- consider to pass tag along the data stream to indicate the type of burst ? Alternatively, can use 34 guard bit as a burst header to fill in the necessary info or even use extra bytes (on top of 510 bytes) as burst header. Using burst header has the advantage of these headers can be stored together with data streams in output files, maybe more convenient & easy for debug.
"""

import numpy as np
from gnuradio import gr

DSB = 1
DNB = 2
DNB_SF = 3
INACTIVE = 0

class dmoBurstSync(gr.basic_block):  # other base classes are basic_block, decim_block, interp_block

    NORMAL_TRAINING_SEQ_1       = (1,1,0,1,0,0,0,0,1,1,1,0,1,0,0,1,1,1,0,1,0,0)  # n1..n22
    NORMAL_TRAINING_SEQ_2       = (0,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,0,1,1,1,1,0)  # p1..p22
    NORMAL_TRAINING_SEQ_3_BEGIN = (0,0,0,1,1,0,1,0,1,1,0,1)                      # q11..q22
    NORMAL_TRAINING_SEQ_3_END   = (1,0,1,1,0,1,1,1,0,0)                          # q1..q10

    SYNC_TRAINING_SEQ = (1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,0,1,1,1,0,1,0,0,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1)  # y1..y38

    PREAMBLE_P1  =   (0,0,1,1,0,0,1,0,0,0,1,1)    # j1..j12
    PREAMBLE_P2  =   (1,0,0,1,1,0,1,0,1,0,0,1)    # k1..k12
    PREAMBLE_P3  =   (0,0,0,1,0,1,0,0,0,1,1,1)    # I1..I12

    BURST_DETECT_THRESHOLD = 4;

    def __init__(self, dmoMode = True): 
        gr.basic_block.__init__(
            self,
            name='dmoBurstSync',     # will show up in GRC
            in_sig=[np.uint8],
            out_sig=[np.uint8]
        )
        self.dmoMode = dmoMode       # False means tmoMode
        self.burstActive = False
        self.burstSynced = False
        
    def patternAtPositionScore(self, bitstream, pattern, position):
        patternScore = 0
        for i in range(len(pattern)):
            if bitstream[position+i] != pattern[i]:
                patternScore += 1
        return patternScore

    def isValidBurst(self, bitstream, pointer):
        
        score_preamble_p1 = self.patternAtPositionScore(bitstream, dmoBurstSync.PREAMBLE_P1, pointer+34)   # don't count 34 guard bits, starts @ zero
        score_preamble_p2 = self.patternAtPositionScore(bitstream, dmoBurstSync.PREAMBLE_P2, pointer+34)   # don't count 34 guard bits
        score_preamble_p3 = self.patternAtPositionScore(bitstream, dmoBurstSync.PREAMBLE_P3, pointer+34)   # don't count 34 guard bits

        scoreSync    = self.patternAtPositionScore(bitstream, dmoBurstSync.SYNC_TRAINING_SEQ,     214 + pointer+34) # STS position in DMO burst
        scoreNormal1 = self.patternAtPositionScore(bitstream, dmoBurstSync.NORMAL_TRAINING_SEQ_1, 230 + pointer+34) # NTS1 position in DMO burst
        scoreNormal2 = self.patternAtPositionScore(bitstream, dmoBurstSync.NORMAL_TRAINING_SEQ_2, 230 + pointer+34) # NTS2 position in DMO burst
     
        scoreNormal1 = int((scoreNormal1*38)/22)                                       # multifly with 1.5 to be comparable with STS
        scoreNormal2 = int((scoreNormal2*38)/22) 

        scoreNormal1 += score_preamble_p1                                              # combine bit errors of preamble & training sequences
        scoreNormal2 += score_preamble_p2 
        scoreSync    += score_preamble_p3 

        # soft decision to detect burst: bit errors in preamble & training sequence less than 6
        scoreMin = scoreSync                                                   # if same score, Sync Burst is chosen
        burstType = DSB         

        if (scoreNormal1 < scoreMin):
            scoreMin  = scoreNormal1 
            burstType = DNB 
        if (scoreNormal2 < scoreMin):
            scoreMin  = scoreNormal2 
            burstType = DNB_SF 

        if (scoreMin > dmoBurstSync.BURST_DETECT_THRESHOLD):                  # frame (burst) is matched and can be processed
            burstType = INACTIVE
            
        return (burstType, scoreMin)


    def general_work(self, input_items, output_items):

        in_index = 0
        out_index = 0

        while (len(input_items[0]) >= in_index + 510) and (len(output_items[0]) >= out_index + 520):
            # only process the code below if more than 510 input data samples
        
            bitStream = input_items[in_index:]
            if (not self.burstSynced):
                for in_index in range(len(bitStream) - 286): # 286 = 34 + 12 + 2 + 216 + 22
                    (burstType, scoreMin) = self.isValidBurst(bitStream, in_index)
                    if (burstType):
                        self.burstSynced = True
                        print("first burst detected, type: ", burstType, "in_index: ", in_index, "errors: ", scoreMin)
                        break
            else:          
                (burstType, scoreMin) = self.isValidBurst(bitStream, 0)
                if (burstType):
                    self.burstActive = True
                else:
                    self.burstActive = False

                print("Burst type: ", burstType, "errors: ", scoreMin)
                output_items[0][out_index] = burstType
                for i in range(1,10):
                    output_items[0][out_index+i] = 0
            
                for i in range(10, 520):
                    if burstType :
                        output_items[0][out_index+i] = input_items[0][in_index+i-10]
                    else:
                        output_items[0][out_index+i] = 0
                in_index += 510
                out_index += 520
            
        self.consume(0, in_index) #consume port 0 input
        return out_index
