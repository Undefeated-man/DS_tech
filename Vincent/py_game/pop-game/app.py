"""
	###########################################################################
	#		                                                                  #
	#		Project: pygame                                                   #
	#		                                                                  #
	#		Filename: app.py                                                  #
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


import pygame
import game_config as gc
from pygame import display, event, image
from animals import Animal, re_play
from time import sleep, localtime, time
import diy

# to find the right img
def find_index(x, y):
    row = y // gc.IMAGE_SIZE
    col = x // gc.IMAGE_SIZE
    index = row * gc.NUM_TILES_SIDE + col
    return index

def showResult(s, e):
    timi = localtime(e-s)
    titleFont = pygame.font.Font('freesansbold.ttf', 65)
    titleSurf1 = titleFont.render('  Used time: %dmin%dsec  '%(timi[4], timi[5]), True, diy.result_bg_color, diy.result_font_color)
    screen.fill((230, 230, 230))
    degrees1 = diy.result_font_angle
    rotatedSurf1 = pygame.transform.rotate(titleSurf1, degrees1)
    rotatedRect1 = rotatedSurf1.get_rect()
    rotatedRect1.center = (Window_Width / 2, Window_Height / 2)
    screen.blit(rotatedSurf1, rotatedRect1)
    pygame.display.update()
    sleep(5)
 

# begin game
def main():
    loop = True
    timi = (0,0)
    
    while loop:
        running = True
        start_time = time()
        current_images = []
        matched = image.load("other_assets/matched.png")
        matched = pygame.transform.scale(matched, (diy.screen_size, diy.screen_size))
        screen.blit(matched, (0, 0))
        display.flip()
        
        tiles = [Animal(i) for i in range(0, gc.NUM_TILES_TOTAL)]
        
        # Start the game loop
        while running:
            current_events = event.get()
            
            # catch the keyboard and mouse event
            for e in current_events:
                # set a method to quit the game
                if e.type == pygame.QUIT:
                    running = False
                    return
                    
                if e.type == pygame.KEYDOWN:
                    if e.type == pygame.K_ESCAPE:
                        running = False
                
                if e.type == pygame.MOUSEBUTTONDOWN:
                    mouse_x, mouse_y = pygame.mouse.get_pos()
                    index = find_index(mouse_x, mouse_y)
                    if not index in current_images:
                        current_images.append(index)
                    if len(current_images) > 2:
                        current_images = current_images[1:]
                    
            screen.fill((255, 255, 255))
            total_skiped = 0
            for _, tile in enumerate(tiles):
                image_i = tile.image if _ in current_images else tile.box
                if not tile.skip:
                    screen.blit(image_i, (tile.col * gc.IMAGE_SIZE + gc.MARGIN,\
                    tile.row * gc.IMAGE_SIZE + gc.MARGIN))
                else:
                    total_skiped += 1
                
            display.flip()
            
            if len(current_images) == 2:
                idx1, idx2 = current_images
                if tiles[idx1].name == tiles[idx2].name:
                    tiles[idx1].skip = True
                    tiles[idx2].skip = True
                    sleep(0.2)
                    screen.blit(matched, (10, 10))
                    display.flip()
                    sleep(0.4)
                    current_images = []
            
            if total_skiped == len(tiles):
                running = False
                end_time = time()
                showResult(start_time, end_time)

            sleep(0.01)
        
        # would you like to replay?
        replay = pygame.transform.scale(image.load("other_assets/replay.png"), (diy.screen_size, diy.screen_size))
        screen.blit(replay, (0, 0))
        display.flip()
        sleep(0.01)
        
        inner_loop = True
        
        # continue to play?
        while inner_loop:
            current_events = event.get()
            for e in current_events:
                # set a method to quit the game
                if e.type == pygame.QUIT:
                    running = False
                    loop = False
                    inner_loop = False
                    return
                
                # set a method to quit the game            
                if e.type == pygame.KEYDOWN:
                    if e.type == pygame.K_ESCAPE:
                        running = False
                        loop = False
                        inner_loop = False
                    else:
                        running = True
                        loop = True
                        inner_loop = False
                        re_play()
                        break
            sleep(0.01)        
                
if __name__ == '__main__':
    pygame.init()
    display.set_caption("Animals")
    Window_Width = 800
    Window_Height = 800
    screen = display.set_mode((Window_Width, Window_Height))
    
    # Game begin
    main()
 
    # show some words when leave the game
    print("Goodbye!")
    # sleep(5)
