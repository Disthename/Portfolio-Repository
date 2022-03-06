from decimal import Clamped
from re import X
from turtle import circle
import pygame
#import numpy as np
import random
from sqlalchemy import null

from sympy import Max, Min
from physics_objects import Circle, Wall #Wall
from forces import *
from contact import Contact_Circle_Circle, Contact_Circle_Wall, generate_contact, resolve_contact

# Initialize pygame and open window
pygame.init()
# 2.5m * 3.0m
width, height = 500, 600
screen = pygame.display.set_mode([width, height])
bg_color = (0, 190, 0)


# Timing stuff
fps = 60
dt = 1/fps
clock = pygame.time.Clock()

# Stages
PLACING_BALL = 0
AIMING_THROW = 1
CHARGING_THROW = 2
BALLS_ROLLING = 3
DISPLAY_ROUND_RESULT = 4

# Max number of player balls
BALL_NUM = 8
# Constant used for throw calculations
POWER_CONSTANT = 150

# Set objects
#gray_circle
gray_circle = Circle(pos = (width/2, 445), radius = 100, color=[150,150,150], width=0)

#pallino 
pallino = Circle(mass = 1, radius=5, color=[255,255,255], width=0, pos=gray_circle.pos+Vector2(0,1))

# colors for each player's balls
PLAYER1COLOR = [255,0,0]
PLAYER2COLOR = [0,0,255]

#balls are 2 pixels per 1 cm
# balls are all in balls EXCEPT the pallino
balls = []

walls = []
# top
walls.append(Wall(Vector2(50,50),Vector2(width-50,50),reverse = False, color=[0,0,0], width = 10))

# left
walls.append(Wall(Vector2(50,50),Vector2(50,height-50),reverse = True, color=[0,0,0], width = 10))

# right
walls.append(Wall(Vector2(width-50,height-50),Vector2(width-50,50),reverse = True, color=[0,0,0], width = 10))

# bottom
walls.append(Wall(Vector2(width-50,height-50),Vector2(50,height-50),reverse = False, color=[0,0,0], width = 10))

# objects is the list of all balls and walls (including the pallino) and is used for collision detection and resolution
objects = walls+balls

# Set forces
# Friction
friction = Friction(objects_list = objects)

# Fonts/text related variables
currentPlayeris1 = True
currentPlayerText = 'Player 1'
pygame.font.init()
font = pygame.font.SysFont('comicsans-ms', 15, True, False) # bold, not italicized
instructionString = f'{currentPlayerText}\'s Turn'
InstructionText = font.render(instructionString, True, [255,255,255])

# balls belonging to player1
player1balls = []

# balls belonging to player2
player2balls = []

# Other game variables
# throwing power that increases with a held mouse button
current_power = 0

# variable that will hold the current closest ball for point/highlight calculations
closest_ball = null

# variables that tracks the current stage of a given turn
current_stage = PLACING_BALL

# boolean for main game loop
game_end = False

# variable that is used to color the balls of the active player, and is changed when appropriate
# it is initialized to player 1's color
currentPlayerColor = PLAYER1COLOR

# boolean that tracks whether or not the round is starting, meaning that the pallino should be thrown
first_turn = True

# integers that increment as players win rounds
player1Points = 0
player2Points = 0

# boolean that is set to true after points have been added for a given round, but before the next round starts
pointsAdded = False

# game loop
while not game_end:
    # EVENT loop
    for event in pygame.event.get():
        # Quitting game
        if (event.type == pygame.QUIT 
            or (event.type == pygame.KEYDOWN
                and event.key == pygame.K_ESCAPE)):
            game_end = True
        # Placing a new ball in the gray area
        if (current_stage == PLACING_BALL):
            # if this is the first ball, the pallino will be thrown first
            if first_turn == True:
                current_ball = pallino
                # adding pallino to collision list
                objects.append(current_ball)
                current_stage+=1
                # next throw will be player 1 ball
                first_turn = False
            elif(event.type == pygame.MOUSEBUTTONDOWN and (pygame.mouse.get_pos()-gray_circle.pos).magnitude()<=gray_circle.radius):
                # creates ball for reference during this turn
                current_ball = Circle(mass = 1,vel=Vector2(0,0),pos = pygame.mouse.get_pos(), radius=11, color=currentPlayerColor, width=0)
                # ball is added to collision list
                objects.append(current_ball)
                # ball is added to ball specific list
                balls.append(current_ball)
                # ball is added to player specific ball list
                if(current_ball.color == PLAYER1COLOR):
                    player1balls.append(current_ball)
                else:
                    player2balls.append(current_ball)
                current_stage+=1
                
        # Aiming ball throw
        elif (current_stage == AIMING_THROW):
            if(event.type == pygame.MOUSEBUTTONDOWN):
                current_stage+=1
                current_power = 0
                ballthrown = False
        # Charging a ball for a throw
        # stage handled outside of event loop
        elif (current_stage == CHARGING_THROW):
            pass

        # Balls rolling
        elif (current_stage == BALLS_ROLLING):
            # if the combined total of both player's balls equal the maximum amount, the round is considered over
            if(objects_stopped and player1balls.__len__()+player2balls.__len__() == BALL_NUM):
                current_stage+=1
            # otherwise, the next ball shall be thrown
            elif(objects_stopped):
                current_stage = 0

        # Going forward after the round ends
        elif (current_stage == DISPLAY_ROUND_RESULT):

            # if/else statements that determine the text based elements to display
            # upon the end of a round
            if(closest_ball.color == PLAYER1COLOR):
                currentPlayerText = 'Player 1'
                currentPlayerPoints = player1Points
            else:
                currentPlayerText = 'Player 2'
                currentPlayerPoints = player2Points

            # end of round text settings
            instructionString = f'{currentPlayerText} has won the round and have a total of {currentPlayerPoints} points!'
            InstructionText = font.render(instructionString, True, [255,255,255])
            scoreString = f'Player 1: {player1Points} Player 2: {player2Points}'
            scoreText = font.render(scoreString, True, [255,255,255])
            playAgainString = 'Click to play again!'
            playAgainText = font.render(playAgainString, True, [255,255,255])  
            
            # if the player clicks during the display screen, all elements (besides points) are reset, and a new round is started
            if(event.type == pygame.MOUSEBUTTONDOWN):
                balls = []
                player1balls = []
                player2balls = []
                objects = balls+walls
                current_stage = PLACING_BALL
                first_turn = True
                pointsAdded = False
                pallino.pos = gray_circle.pos+Vector2(0,1)
                closest_ball = null
                currentPlayerColor = PLAYER1COLOR
                currentPlayerText = 'Player 1'
                closest_ball_distance = Vector2(0,0)
                friction = Friction(objects_list = objects)

    # PHYSICS
    # clear all forces
    for object in objects:
        object.clear_force()

    # apply all forces
    friction.apply()
    for object in objects:
        object.update(dt)
    
    # process collisions
    for j in range(len(objects)):
        for i in range(j):
            c = generate_contact(objects[i], objects[j])

            # circle/wall collisions have a restitution of 0.30, all other collisions have a restitution of 0.50
            if(type(c) == Contact_Circle_Wall):
                resolve_contact(c, restitution=0.30)
            else:
                resolve_contact(c, restitution=0.50)

    # max power is reached after 3 seconds at 60 fps
    if(current_stage == CHARGING_THROW):
        # 0.1 power is gained for every frame of the program
        # if breakpoint is placed on following line, code will break during debug as it will not recognize a 'MOUSEBUTTONUP' from the user
        current_power+=0.1
        if(current_power>=18.0):
            current_power = 18.0

    # Releasing the ball to roll
    # Checks if mouse has been released during Ball Charge
    if (event.type == pygame.MOUSEBUTTONUP and current_stage == CHARGING_THROW):
        current_stage+=1
        # velocity calculation for current ball
        velCalc = Min(math.sqrt(current_power).real*POWER_CONSTANT, 500)
        current_ball.vel = velCalc*(pygame.mouse.get_pos()-current_ball.pos).normalize()
        
    # Check for balls rolling slow enough to stop
    # Check if all balls have been thrown and have stopped
    objects_stopped = True
    for object in objects:
        # if the object has a velocity magnitude that is less than or equal to 5.0, the object will be stopped
        if object.vel.magnitude() <= 5.0:
            object.vel = Vector2(0,0)
        # if an object is found with a velocity magnitude that is higher than 5.0, a boolean is marked to signify that fact, and the physics continue
        else:
            objects_stopped = False
            break

    
    
    
    # GAME
    
    

    # DRAW
    screen.fill(bg_color)
    gray_circle.draw(screen)
    
 
    # draw objects
    for object in objects:
        object.draw(screen)

    # draw aiming line
    if(current_stage == AIMING_THROW):
        pygame.draw.line(screen,[255,255,255],current_ball.pos,pygame.mouse.get_pos())

    # highlight balls
    # in the case of player 1 having the closest ball
    # each player's ball list is sorted by distance from the pallino
    player1balls.sort(key = lambda y: (y.pos - pallino.pos).magnitude())
    player2balls.sort(key = lambda z: (z.pos - pallino.pos).magnitude())
    if(balls.__len__()>1):
        if (closest_ball.color == PLAYER1COLOR and objects_stopped == True):
            # adds a point for each ball closer than the opposing player's closest ball
            for ball in player1balls:
                if(pallino.pos-ball.pos).magnitude()<=(pallino.pos-player2balls[0].pos).magnitude():
                    pygame.draw.circle(screen, [0,255,255], ball.pos, ball.radius, 5)
        # in the case of player 2 having the closest ball
        elif(objects_stopped == True):
            # adds a point for each ball closer than the opposing player's closest ball
            for ball in player2balls:
                if(pallino.pos-ball.pos).magnitude()<=(pallino.pos-player1balls[0].pos).magnitude():
                    pygame.draw.circle(screen, [0,255,255], ball.pos, ball.radius, 5)


    # vector meant to be overwritten immediately
    closest_ball_distance = Vector2(100000,100000)
    for ball in balls:
        if (pallino.pos-ball.pos).magnitude()<closest_ball_distance.magnitude():
            closest_ball_distance = (pallino.pos-ball.pos)
            closest_ball = ball
    # Start new turn if all balls are stopped
    # decides who shall play next based on who has the ball closest to the pallino, as long as that player has not thrown all of their balls
    if(closest_ball!=null and objects_stopped == True):
        if((closest_ball.color == PLAYER1COLOR or player1balls.__len__()==BALL_NUM/2) and player2balls.__len__()<BALL_NUM/2 and current_stage!=AIMING_THROW):
            currentPlayerColor = PLAYER2COLOR
            currentPlayerText = 'Player 2'
        elif((closest_ball.color == PLAYER2COLOR or player2balls.__len__()==BALL_NUM/2) and player1balls.__len__() < BALL_NUM/2 and current_stage!=AIMING_THROW):
            currentPlayerColor = PLAYER1COLOR
            currentPlayerText = 'Player 1'
    
    # if all balls have been thrown, the player with the ball closest to the pallino wins the round. The winning player's ball
    # that is the closest has their list of balls compared to the closest ball of the other player (the loser of the round).
    # The winning player is awarded a point for each ball that they have closer to the pallino than the opposing player's closest ball.
    if(current_stage == DISPLAY_ROUND_RESULT and pointsAdded == False):
        # in the case of player 1 having the closest ball
        if closest_ball.color == PLAYER1COLOR:
            # adds a point for each ball closer than the opposing player's closest ball
            for ball in player1balls:
                if(pallino.pos-ball.pos).magnitude()<=(pallino.pos-player2balls[0].pos).magnitude():
                    player1Points += 1
            pointsAdded = True
        # in the case of player 2 having the closest ball
        else:
            # adds a point for each ball closer than the opposing player's closest ball
            for ball in player2balls:
                if(pallino.pos-ball.pos).magnitude()<=(pallino.pos-player1balls[0].pos).magnitude():
                    player2Points += 1
            pointsAdded = True
    # Draw messages on screen
    # standard text display
    if(current_stage !=DISPLAY_ROUND_RESULT):
        instructionString = f'{currentPlayerText}\'s Turn'
        InstructionText = font.render(instructionString, True, [255,255,255])  
        screen.blit(InstructionText, [width/2-InstructionText.get_width()/2, 25-InstructionText.get_height()/2])
    # text display during the results screen
    else:
        screen.blit(InstructionText, [width/2-InstructionText.get_width()/2, 25-InstructionText.get_height()/2])
        # scoring text
        scoreString = f'Player 1: {player1Points} Player 2: {player2Points}'
        scoreText = font.render(scoreString, True, [255,255,255])
        screen.blit(scoreText, [width/2-scoreText.get_width()/2, height/2-scoreText.get_height()/2])
        # play-again text
        playAgainString = 'Click to play again!'
        playAgainText = font.render(playAgainString, True, [255,255,255])  
        screen.blit(playAgainText, [width/2-playAgainText.get_width()/2, 50+height/2-playAgainText.get_height()/2])        

    # update the display
    pygame.display.update()
    # delay for correct timing
    time = clock.tick(fps)
    