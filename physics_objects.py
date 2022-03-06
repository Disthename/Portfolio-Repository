import math
from turtle import pos
import pygame
from pygame.math import Vector2

class Particle:
    def __init__(self, mass = 1, pos = (0,0), vel=(0,0)):
        self.mass = mass
        self.pos = Vector2(pos)
        self.vel = Vector2(vel)
        self.clear_force()
        #Used by generate_contacts to generate
        #the appropriate collision detection function
        #self.contact_type = "Circle"

    def impulse(self, impulse):
        self.vel+=impulse/self.mass

    def clear_force(self):
        self.force = Vector2(0,0)


    def add_force(self, force):
        self.force += force

    def update(self, dt):
        if self.mass != 0:
            self.vel += (self.force/self.mass)*dt
        # update velocity using the current force
        self.pos += self.vel*dt
        # update position using the newly updated velocity

class Wall(Particle):
    def __init__(self, point1, point2, reverse=False, mass = 1000, color=[0,0,0], width=1):
        # Two endpoints of the wall (visually)
        # Wall behaves as if it's infinite
        self.point1 = Vector2(point1)
        self.point2 = Vector2(point2)
        self.contact_type = "Wall"
        super().__init__(pos=(self.point1 + self.point2)/2)
        self.width = width
        self.color = color
        self.mass = mass
        # Normal is perpendicular to the wall (clockwise 90)
        self.normal = (point2 - point1).normalize().rotate(90)
        if reverse:
            self.normal *= -1

    def draw(self, screen):
        pygame.draw.line(screen, self.color, self.point1, self.point2, self.width)

class Circle(Particle):
    def __init__(self, radius = 10, color = [255,255,255], width = 0, mouseMode = False, **kwargs):
        # **kwargs is a dictionary to catch all the other keyword arguments
        super().__init__(**kwargs) # calls the superclass constructor
        # **kwargs unpacking the kwargs dictionary into key=value, key=value, etc.
        self.radius = radius
        self.color = color
        self.width = width
        self.contact_type = "Circle"

    def draw(self, window):
        pygame.draw.circle(window, self.color, self.pos, self.radius, self.width)