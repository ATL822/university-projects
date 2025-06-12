# CSCI - 3038 
# Final Project
# Andrew Law
# Gradient class used to build a list of colors used in the gradient

class Gradient:
    def __init__(self):
        self.color_list = []
        with open('gradient_rgb.dat') as rgb_data:
            for line in rgb_data:
                r, g, b, a = line.split(sep=',')
                color_tuple = tuple((int(r), int(g), int(b), int(a)))
                if color_tuple not in self.color_list:
                    self.color_list.append(color_tuple)
        self.num_colors = len(self.color_list)

    def get_gradient_value(self, value) -> tuple:
        return self.color_list[int(value * self.num_colors)]
