"""
	###########################################################################
	#		                                                                  #
	#		Project: pygame                                                   #
	#		                                                                  #
	#		Filename: game_config.py                                          #
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
import diy

# set constants
IMAGE_SIZE = diy.img_size
SCREEN_SIZE = diy.screen_size
NUM_TILES_SIDE = diy.col_num # 1~4
NUM_TILES_TOTAL = diy.total_num  # 1~16
MARGIN = diy.margin

ASSET_DIR = "./assets"
ASSET_FILES = [i for i in os.listdir(ASSET_DIR) if i[-3:].lower() == 'png']

assert len(ASSET_FILES) == 8