# CSCI - 3038
# Final Project
# Andrew Law
# Color map - holds the data for the color of each pixel in an image

# Imports
from vector2D import Vector2D as Vec2
from gradient import Gradient


class ColorMap:
    def __init__(self, render_size: Vec2):
        self.width = render_size.x
        self.height = render_size.y
        # list of lists containing color values at each position. Init all to 0 (black)
        self.map = [[0 for _ in range(self.height)] for _ in range(self.width)]
        self.min = 0
        self.max = 0

    def set_pixel_intensity(self, position_and_intensity: tuple):
        # save min and max for later processing
        if position_and_intensity[0] < self.min:
            self.min = position_and_intensity[0]
        if position_and_intensity[0] > self.max:
            self.max = position_and_intensity[0]

        # save intensity value into 2d list
        x_pos, y_pos = int(position_and_intensity[1]), int(position_and_intensity[2])
        self.map[x_pos][y_pos] = position_and_intensity[0]

    def normalize_pixel_values(self):
        value_range = self.max - self.min
        value_offset = self.min
        print(f'max: {self.max}, min:{self.min}')

        grad = Gradient()

        # for all values in the list of lists
        for y in range(self.height):
            for x in range(self.width):
                # replace value with intensity percentage * number of colors
                # this value will now map to a line in gradient_rgb.dat (an (r,g,b,a) value)
                self.map[x][y] = grad.get_gradient_value((self.map[x][y] + value_offset) / value_range)

    def get_map(self):
        return self.map
