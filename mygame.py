from cmath import sqrt
import pygame
from sqlalchemy import null
from physics_objects import Circle, Particle
from pygame.math import Vector2
import random
from pygame.locals import *

SIZE = 420
WIDTH = 2*SIZE+1
MAX_PARTICLE_NUMBER = 10
# Create window
window = pygame.display.set_mode([WIDTH, WIDTH])

#list to hold particles
particles = []

#sun and ship circle objects are initialized
sun = Circle(mass = 1, color = [255,255,0], radius = SIZE/10, pos = (WIDTH/2,WIDTH/2))
ship = Circle(mass = 1, color = [0,192,255], radius = SIZE/30, pos = sun.pos +[-SIZE/2,-SIZE/2], vel=[100,0])

# Time variables
clock = pygame.time.Clock()
dt = 1/60
secondsPassed = 0
minutesPassed = 0

# variables to hold game results
resultString = ''
resultText = ''

#Vector from ship to sun
r = sun.pos-ship.pos

#force of gravity on ship from sun
#acceleration = size/15 pixels/s/s
g = r.magnitude()*ship.mass/15

#formula used to calculate initial velocity magnitude
orbitalspeed = sqrt((g*ship.mass)/(r.length()))

#vector perpendicular to r
initveldir = r.rotate(270.0)

# initial velocity for ship that allows it to maintain orbit (assuming no movement is made by the player)
ship.vel = Vector2(initveldir.x*orbitalspeed.real,initveldir.y*orbitalspeed.real)

# particles are initialized
for i in range(MAX_PARTICLE_NUMBER):
    particles.append(Circle(mass = 1,radius = SIZE/60, color = [255,255,255], pos =(random.randint(-300,300)+sun.pos.x,random.randint(-300,300)+sun.pos.y)))
    particleR = sun.pos-particles[i].pos
    particleForce = (particleR.magnitude()*particles[i].mass/15)
    particleSpeed = sqrt((particleForce*particles[i].mass)/(particleR.length()))
    initParticleDir = particleR.rotate(270.0)
    particles[i].vel = Vector2(initParticleDir.x*particleSpeed.real,initParticleDir.y*particleSpeed.real)
    print(particles[i].vel)

# thrust variables are initialized
thrustForce = Vector2(0,0)
thrustNormalized = Vector2(0,0)

# font is initialized and configured
pygame.font.init()
font = pygame.font.SysFont('comicsans-ms', 30, True, False) # bold, not italicized

# Main game variables
running = True
particlesCollected = 0
gameFinished = False
gameWon = False
playAgainMessage = False
fuelAmount = 3000
fuelText = ''
timesPlayed = 0

# Main game loop
while running:
    # Event handling loop
    for event in pygame.event.get():
        thrustForce = Vector2(0,0)
        if event.type == pygame.QUIT:
            running = False

        #Thrust control
        if(gameFinished==False or gameWon == True):        
            keyinput = pygame.key.get_pressed()
            if  (keyinput[pygame.K_w]):
                thrustForce.y-=1.0
            if  (keyinput[pygame.K_s]):
                thrustForce.y+=1.0
            if  (keyinput[pygame.K_d]):
                thrustForce.x+=1.0
            if  (keyinput[pygame.K_a]):
                thrustForce.x-=1.0
        
        # allows for the 'play again' message to be displayed after a short delay
        if (event.type == pygame.USEREVENT and gameFinished):
            playAgainMessage = True
        
        # reinitializes game 
        if(gameFinished and event.type == pygame.KEYDOWN):
            if (event.key == K_SPACE):
                timesPlayed += 1
                particles = []
                sun.pos = Vector2(WIDTH/2,WIDTH/2)
                ship.pos = sun.pos + Vector2(-SIZE/2,-SIZE/2)
                # Clock object for timing
                secondsPassed = 0
                minutesPassed = 0
                resultString = ''
                resultText = ''
                #acceleration = size/15 pixels/s/s
                #Vector from ship to sun
                r = sun.pos-ship.pos
                #force of gravity on ship from sun
                g = r.magnitude()*ship.mass/15
                #formula used to calculate initial velocity magnitude
                orbitalspeed = sqrt((g*ship.mass)/(r.length()))
                #vector perpendicular to r
                initveldir = r.rotate(270.0)
                ship.vel = Vector2(initveldir.x*orbitalspeed.real,initveldir.y*orbitalspeed.real)
                for i in range(MAX_PARTICLE_NUMBER):
                    particles.append(Circle(mass = 1,radius = SIZE/60, color = [255,255,255], pos =(random.randint(-300,300)+sun.pos.x,random.randint(-300,300)+sun.pos.y)))
                    particleR = sun.pos-particles[i].pos
                    particleForce = (particleR.magnitude()*particles[i].mass/15)
                    particleSpeed = sqrt((particleForce*particles[i].mass)/(particleR.length()))
                    initParticleDir = particleR.rotate(270.0)
                    particles[i].vel = Vector2(initParticleDir.x*particleSpeed.real,initParticleDir.y*particleSpeed.real)
                    print(particles[i].vel)
                thrustForce = Vector2(0,0)
                thrustNormalized = Vector2(0,0)
                particlesCollected = 0
                gameFinished = False
                fuelAmount = 3000
                if(gameWon == True):
                    fuelAmount-=(500*timesPlayed)
                gameWon = False
                playAgainMessage = False
                fuelText = ''
                #clears pygame events from queue
                pygame.event.clear()
            # closes game
            if (event.key == K_ESCAPE):
                running = False
    # Gravitational force added every frame
    # ship force calculations
    ship.clear_force()
    thrustNormalized = Vector2(0,0)

    #Vector from ship to sun
    r = sun.pos-ship.pos
    
    #force of gravity on ship from sun (including fuel calculations)
    g = r.magnitude()*ship.mass/15
    if(thrustForce !=Vector2(0,0)):
        fuelAmount-=1
        fuelText = font.render(str(fuelAmount), True, [255,255,255])
        thrustNormalized = thrustForce.normalize()
        ship.add_force(g*r.normalize()+thrustNormalized*100.0)
    
    #thrust added to ship force
    else:
        ship.add_force(g*r.normalize())
    ship.update(dt)
    
    #particle force calculations
    for part in particles:
        ## Clear force from all particles
        part.clear_force()
        particleR = (sun.pos-part.pos)
        particleForce = (particleR.magnitude()*part.mass/15)
        #force added to each particle
        part.add_force(particleForce*particleR.normalize())
        part.update(dt)

    # Timer calculations
    if(gameWon==False):
        timeElapsed = pygame.time.get_ticks()
    secondsPassed = int((timeElapsed/1000)%60)    
    minutesPassed = int((timeElapsed/60000))
    timerString = f"{minutesPassed}:{secondsPassed}"
    timerText = font.render(timerString, True, [255,255,255])
    
    ## Dot collection
    for i in range(particles.__len__()):
        if((ship.pos-particles[i].pos).magnitude()<=ship.radius+particles[i].radius):
            particles.pop(i)
            particlesCollected += 1
            break
    
    # Drawing
    window.fill([0,0,0]) # clear the window so we can redraw the picture fresh
    
    ## Play again text
    if(playAgainMessage):
        resultString = "Press space to play again."
        resultText = font.render(resultString,True,[255,255,255])
        window.blit(resultText, [400-resultText.get_width()/2, 50-resultText.get_height()/2])
    
    ## Winning handler + win text
    elif(particlesCollected==10 and gameFinished == False):
        gameFinished = True
        gameWon = True
        resultString = "You WON!"
        resultText = font.render(resultString,True,[255,255,255])
        window.blit(resultText, [400-resultText.get_width()/2, 50-resultText.get_height()/2])
        pygame.time.set_timer(pygame.USEREVENT, 1000)

    ## Losing handler + loss text
    elif((ship.pos-sun.pos).magnitude()<=ship.radius+sun.radius and (gameFinished == False and gameWon == False) or fuelAmount <= 0):
        gameFinished = True
        resultString = "You lost."
        resultText = font.render(resultString,True,[255,255,255])
        window.blit(resultText, [400-resultText.get_width()/2, 50-resultText.get_height()/2])
        pygame.time.set_timer(pygame.USEREVENT, 1000)

    #draws each particle
    for part in particles:
        part.draw(window)
    
    #draws sun and ship
    sun.draw(window)
    ship.draw(window)
    
    #draws line from sun to ship if they are over a certain distance apart
    if(r.magnitude()>WIDTH/2):
        pygame.draw.line(window, color = [0,255,0], start_pos=sun.pos, end_pos=ship.pos)
    
    #if there is a non-zero thrust vector, draw the thrust in the corresponding direction
    if(thrustNormalized!=Vector2(0,0)):
        pygame.draw.circle(window, color=[255,255,0], center= ship.pos+10*thrustNormalized.rotate(180), radius=ship.radius/2)
    
    #draws timer
    window.blit(timerText, [100-timerText.get_width()/2, 50-timerText.get_height()/2])
    if(resultString!=''):
        window.blit(resultText, [400-resultText.get_width()/2, 50-resultText.get_height()/2])
    if(fuelAmount>=0 and fuelText!=''):
        window.blit(fuelText, [700-fuelText.get_width()/2, 50-fuelText.get_height()/2])
    pygame.display.update() # update the screen with what was drawn (double buffer)
    clock.tick(60) # delay running to 60 frames per second
            