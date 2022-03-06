import pygame
from pygame import Vector2
from physics_objects import Circle, Wall
from contact import generate_contact, resolve_contact

# Create window
window = pygame.display.set_mode([800, 600])

# Clock object for timing
clock = pygame.time.Clock()
fps = 60
dt = 1/fps

# Objects
objects = []
objects.append(Circle(pos=(100,290), vel=(100,0), mass=10, radius=50, color=(255,0,0)))
objects.append(Circle(pos=(400,300), vel=(0,0), mass=14, radius=100, color=(0,0,255)))
objects.append(Wall(Vector2(750,50),Vector2(750,600-50),reverse = True, color=[0,255,0], width = 10))
# Game loop
running = True
while running:
    # Event handling loop
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
 
    # Physics
    ## Clear force from all particles
    ## Add forces
    ## Update particles
    for object in objects:
        object.update(dt)
    # Collisions
    for j in range(len(objects)):
        for i in range(j):
            c = generate_contact(objects[i], objects[j])
            resolve_contact(c, restitution=1)

    # Graphics
    window.fill([0,0,0]) # clear the window so we can redraw the picture fresh

    ## Draw objects
    for obj in objects:
        obj.draw(window)

    pygame.display.update()
    clock.tick(fps)
    