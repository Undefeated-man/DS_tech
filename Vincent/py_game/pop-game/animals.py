"""
	###########################################################################
	#		                                                                  #
	#		Project: pygame                                                   #
	#		                                                                  #
	#		Filename: animals.py                                              #
	#		                                                                  #
	#		Programmer: Vincent Holmes                                        #
	#		                                                                  #
	#		Description: 这只是一个简单的python小游戏                         #
	#		                                                                  #
	#		Start_date: 2020-06-28                                            #
	#		                                                                  #
	#		Last_update: 2020-06-28                                           #
	#		                                                                  #
	###########################################################################
"""


import os
import random
import game_config as gc
from pygame import image, transform
import diy

animals_count = dict((a, 0) for a in gc.ASSET_FILES)

def available_animals():
    return [a for a, c in animals_count.items() if c < 2]

class Animal:
    def __init__(self, index):
        self.index = index
        self.row = index // gc.NUM_TILES_SIDE
        self.col = index % gc.NUM_TILES_SIDE
        self.name = random.choice(available_animals())
        animals_count[self.name] += 1
        
        self.img_path = os.path.join(gc.ASSET_DIR, self.name)
        self.image = image.load(self.img_path)
        self.image = transform.scale(self.image, (gc.IMAGE_SIZE - diy.col_margin*gc.MARGIN,\
        gc.IMAGE_SIZE - diy.row_margin*gc.MARGIN)) # (column margin, row margin)
        self.box = self.image.copy()
        print(diy.block_color)
        self.box.fill(diy.block_color)
        self.skip = diy.skip
    
def re_play():
    for a in gc.ASSET_FILES:
        animals_count[a]=0