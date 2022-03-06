from turtle import circle
from pygame.math import Vector2

# Returns a new contact object of the correct type
# This function has been done for you.
def generate_contact(a, b):
    # Check if a's type comes later than b's alphabetically.
    # We will label our collision types in alphabetical order, 
    # so the lower one needs to go first.
    if b.contact_type < a.contact_type:
        a, b = b, a
    # This calls the class of the appropriate name based on the two contact types.
    return globals()[f"Contact_{a.contact_type}_{b.contact_type}"](a, b)
    
# Resolves a contact (by the default method) and returns True if it needed to be resolved
def resolve_contact(contact, restitution=0):
    m = 1/((1/contact.a.mass)+(1/contact.b.mass))
    relativeVelocity = contact.a.vel - contact.b.vel
    #Resolves overlap
    if contact.overlap()>0:
        contact.a.pos+=(m/contact.a.mass)*contact.overlap()*contact.normal()
        contact.b.pos+=(-m/contact.b.mass)*contact.overlap()*contact.normal()
        #Resolve velocity
        if relativeVelocity.dot(contact.normal())<0:
            relativeVelocity = contact.a.vel-contact.b.vel
            j = -(1+restitution)*m*(relativeVelocity*contact.normal())
            imp = j*contact.normal()
            contact.a.impulse(imp)
            contact.b.impulse(-imp)


# Generic contact class, to be overridden by specific scenarios
class Contact():
    def __init__(self, a, b):
        self.a = a
        self.b = b
 
    def overlap(self):  # virtual function
        return 0

    def normal(self):  # virtual function
        return Vector2(0, 0)

# Contact class for a circle and a wall
class Contact_Circle_Wall(Contact):
    def __init__(self, a, b):
        super().__init__(a, b)
        self.circle = a
        self.wall = b    

    def overlap(self):
        return self.circle.radius-(self.circle.pos-self.wall.pos)*self.wall.normal

    def normal(self):
        return self.wall.normal

# Contact class for two circles
class Contact_Circle_Circle(Contact):
    def __init__(self, a, b):
        super().__init__(a, b)    

    def overlap(self):
        distance = self.a.pos-self.b.pos
        radialSum = self.a.radius+self.b.radius
        return radialSum-distance.magnitude()
        

    def normal(self):
        if (self.a.pos-self.b.pos)!=Vector2(0,0):
            normal = (self.a.pos-self.b.pos).normalize()
            return normal # Fill in the appropriate expression
        else:
            return Vector2(0,0)
        

#Empty class for wall-wall collisions
#since we aren't overiding overlap() and nomral(), these should always not be overlapped
class Contact_Wall_Wall(Contact):
    def __init__(self, a, b):
        super().__init__(a,b)