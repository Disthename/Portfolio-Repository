import pygame
from pygame.math import Vector2
import itertools
import math

class SingleForce:
    def __init__(self, objects_list=[]):
        self.objects_list = objects_list

    def apply(self):
        for obj in self.objects_list:
            force = self.force(obj)
            obj.add_force(force)

    def force(self, obj): # virtual function
        return Vector2(0, 0)

class Friction(SingleForce):
    def __init__(self, **kwargs):
        self.mew = 0.3
        self.grav = 98
        super().__init__(**kwargs)

    def force(self, obj):
        if obj.mass == math.inf or obj.vel==Vector2(0,0):
            return Vector2(0,0)
        return -self.mew*self.grav*obj.mass*obj.vel.normalize()

class PairForce:
    def __init__(self, objects_list=[]):
        self.objects_list = objects_list

    def apply(self):
        # Loop over all pairs of objects and apply the calculated force
        # to each object, respecting Newton's 3rd Law.  
        # Use either two nested for loops (taking care to do each pair once)
        # or use the itertools library (specifically, the function combinations).
        objectPairs = itertools.combinations(self.objects_list, 2)
        for a,b in objectPairs:
            force = self.force
            
        pass


    def force(self, a, b): # virtual function
        return Vector2(0, 0)


class BondForce:
    
    def __init__(self, pairs_list=[]):
        # pairs_list has the format [[obj1, obj2], [obj3, obj4], ... ]
        self.pairs_list = pairs_list
        
    def apply(self):
        # Loop over all pairs from the pairs list. 
        for a,b in self.pairs_list:
            a.add_force(self.force(a,b))
            b.add_force(self.force(b,a))
        # Apply the force to each member of the pair respecting Newton's 3rd Law.

    def force(self, a, b): # virtual function
        return Vector2(0, 0)

# Add Gravity, SpringForce, SpringRepulsion, AirDrag
class Gravity(SingleForce):
    def __init__(self, acc=(0,0), **kwargs):
        self.acc = Vector2(acc)
        super().__init__(**kwargs)

    def force(self, obj):
        if obj.mass == math.inf:
            return Vector2(0,0)
        return obj.mass*self.acc
        # Note: this will throw an error if the object has infinite mass.
        # Think about how to handle those.
class SpringForce(BondForce):
    def __init__(self, stiffness = 1, length = 1, damping = 1, **kwargs):
        self.k = stiffness
        self.l = length
        self.d = damping
        super().__init__(**kwargs)
    def force(self, a,b):
        r = (a.pos -b.pos)
        v = a.vel -b.vel
        if(r.magnitude()!=0):
            result = (-self.k*(r.magnitude()-self.l)-self.d*v*r.normalize())*r.normalize()
            return result
        return Vector2(0,0)
    def draw(self, window, a, b):
        pygame.draw.line(window, [255,255,255], a.pos, b.pos, 1)
class SpringRepulsion(BondForce):
    def __init__(self, **kwargs):
        self.fc = 10.0
        super().__init__(**kwargs)
    def force(self, a,b):
        r = a.pos-b.pos
        if a.radius+b.radius-r.magnitude()>0:
            return self.fc*(a.radius+b.radius-r.magnitude())*r.normalize()
        else:
            return Vector2(0,0)

class AirDrag(SingleForce):
    def __init__(self, windVel = 0, dragCo = 1, **kwargs):
        self.d = 1.225
        self.f = windVel
        self.a = math.pi * 10**2
        self.co = dragCo
        super().__init__(**kwargs)
    def force(self, obj):
        if(obj.mass!=0):
            v = obj.vel-self.f
        if(obj.vel!=Vector2(0,0)):
            result = -(1/2)*self.co*self.d*self.a*v.magnitude()*v
            return result
        return Vector2(0,0)