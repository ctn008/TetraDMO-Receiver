"""
Embedded Python Blocks:

Each time this file is saved, GRC will instantiate the first class it finds
to get ports and parameters of your block. The arguments to __init__  will
be the parameters. All of them are required to have default values!
"""

import numpy as np
from gnuradio import gr


class blk(gr.basic_block):  # other base classes are basic_block, decim_block, interp_block
    """Embedded Python Block example - a simple multiply const"""

    def __init__(self, step = 4800, change = False):  # only default arguments here
        """arguments to this function show up as parameters in GRC"""
        gr.basic_block.__init__(
            self,
            name='Push_To_View',   # will show up in GRC
            in_sig=[np.float32],
            out_sig=[np.float32]
        )
        # if an attribute with the same name as a parameter is found,
        # a callback is registered (properties work, too).
        self.step= step
        self.change = change
        self.prechange = False

    def general_work(self, input_items, output_items):
        """example: multiply with constant"""
        next_view = False
        if self.change != self.prechange :
            print("Next View")
            next_view = True
            self.prechange = self.change
        if next_view and (len(output_items[0]) >= self.step) and (len(input_items[0]) >= self.step):
           
            for i in range(self.step):
                output_items[0][i] = input_items[0][i]
        
            self.consume(0, self.step) 
            return self.step
        else:
            return 0
