# CSCI - 3038 
# Final Project
# Andrew Law
# Simple 2D vector

class Vector2D:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def get_vector(self):
        vector = (self.x, self.y)
        return vector

    def __add__(self, other):
        self.x += other.x
        self.y += other.y
        return self

    def __sub__(self, other):
        self.x -= other.x
        self.y -= other.y
        return self
