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

    def clear_force(self):
        self.force = Vector2(0,0)


    def add_force(self, force):
        self.force += force

    def update(self, dt):
        self.vel += (self.force/self.mass)*dt
        # update velocity using the current force
        self.pos += self.vel*dt
        # update position using the newly updated velocity


class Circle(Particle):
    def __init__(self, radius = 10, color = [255,255,255], width = 0, **kwargs):
        # **kwargs is a dictionary to catch all the other keyword arguments
        super().__init__(**kwargs) # calls the superclass constructor
        # **kwargs unpacking the kwargs dictionary into key=value, key=value, etc.
        self.radius = radius
        self.color = color
        self.width = width

    def draw(self, window):
        pygame.draw.circle(window, self.color, self.pos, self.radius, self.width)