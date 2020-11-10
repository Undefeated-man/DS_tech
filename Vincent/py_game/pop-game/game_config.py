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

# set constants
IMAGE_SIZE = 128
SCREEN_SIZE = 512
NUM_TILES_SIDE = 4
NUM_TILES_TOTAL = 16
MARGIN = 4

ASSET_DIR = "./assets"
ASSET_FILES = [i for i in os.listdir(ASSET_DIR) if i[-3:].lower() == 'png']

assert len(ASSET_FILES) == 8