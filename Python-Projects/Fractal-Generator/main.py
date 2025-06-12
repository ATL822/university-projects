# CSCI - 3038 
# Final Project
# Andrew Law
# This project is a simple fractal renderer implemented with pygame. It takes heavy inspiration from and is essentially my attempt at porting the fractal renderer
# created by youtube user 'Pezzza's Work'. The video referenced can be found at this link https://youtu.be/uc2yok_pLV4?si=LhUKBJtmzSKYY6Cf
# Credit for several functions goes to the creator of that video. I simply rewrote his C++ code in python.

# imports
from vector2D import Vector2D as Vec2
import config
import fractal_utils
import pygame
import time
import tkinter as tk

# globals
# global GUI vars
root = tk.Tk()
root.title('Fractal Creator')
root.configure(background='darkgrey')
root.minsize(500,350)
root.maxsize(500,350)
root.geometry('500x500+0+700') # set start position of the window\

fractal_num_x = tk.StringVar()
fractal_num_y = tk.StringVar()

# other globals
WINDOW_DIMENSIONS = Vec2(config.WIDTH, config.HEIGHT)
RENDER_SIZE = Vec2(config.WIDTH, config.HEIGHT)


def main():
    # GUI loop
    prompt = tk.Label(root, text='Enter the values for your fractal!\nValues between -1 and 1 are recommended.', font=('Helvetica', 18), justify='center', bg='lightgrey', fg='Navy')
    prompt.pack()

    prompt = tk.Label(root, text='Enter x value', font=('Helvetica', 18), justify='center', bg='lightgrey', fg='Navy')
    prompt.pack()

    x_value_field = tk.Entry(root, font=('Helvetica', 18), textvariable=fractal_num_x)
    x_value_field.pack()

    prompt = tk.Label(root, text='Enter y value', font=('Helvetica', 18), justify='center', bg='lightgrey', fg='Navy')
    prompt.pack()

    y_value_field = tk.Entry(root, font=('Helvetica', 18), textvariable=fractal_num_y)
    y_value_field.pack()

    render_button = tk.Button(root, text='Submit and Render', font=('Helvetica', 18), command=render_and_display, bg='lightgrey', fg='Navy', activebackground='Navy', activeforeground='lightgrey')
    render_button.pack()

    prompt = tk.Label(root, text='Suggested Values:\n\tx: -0.80 y: 0.20\n\tx: -0.90 y: 0.27\n\tx: -0.90 y: 0.12', font=('Helvetica', 18), justify='center', bg='lightgrey', fg='Navy')
    prompt.pack()

    close_window_button = tk.Button(root, text='Close Display Window', font=('Helvetica', 18), command=close_window, bg='lightgrey', fg='Navy', activebackground='Navy', activeforeground='lightgrey')
    close_window_button.pack()

    root.mainloop()


def render_and_display():
    # py game initialization stuff
    pygame.init()
    pygame.display.set_caption('Fractal Display')
    dimensions = pygame.Vector2(WINDOW_DIMENSIONS.get_vector())  # convert from custom vector to pygame vector2
    screen = pygame.display.set_mode(dimensions)

    # zoom variables - feature not implemented in this build
    camera_size = RENDER_SIZE
    x_offset, y_offset = 0, 0 # used for adjusting were in the image you are zoomed into, if you change the camear size

    # fractal constant used in fractal math
    fractal_num = Vec2(float(fractal_num_x.get()), float(fractal_num_y.get()))

    start = time.time()
    # render the image
    fractal_utils.render_color_map(camera_size, fractal_num, y_offset, x_offset, screen)

    # display the image and print info to terminal
    pygame.display.flip()
    end = time.time()
    print(f'time: {end - start}')
    print(f'fractal constant values: {fractal_num.x}, {fractal_num.y}')


def close_window():
    pygame.quit()

if __name__ == '__main__':
    main()
