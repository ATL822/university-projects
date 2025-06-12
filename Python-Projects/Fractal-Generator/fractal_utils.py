# CSCI - 3038
# Final Project
# Andrew Law
# Utility functions for computing fractal math
# More information on the project can be found in main.py

# imports
from vector2D import Vector2D as Vec2
import math
import config
import color_map
import pygame
from concurrent.futures import ProcessPoolExecutor

# Fractal Math - Python port of function from inspiration video (see main.py)
def compute_next(current: Vec2, constant: Vec2) -> Vec2:
    zr = current.x * current.x - current.y * current.y
    zi = 2.0 * current.x * current.y

    next_item = Vec2(zr, zi) + constant
    return next_item

# Helper function
def mod2(z: Vec2) -> float:
    return z.x * z.x + z.y * z.y

# Fractal Math - Python port of function from inspiration video (see main.py)
def compute_iterations_smooth(z0: Vec2, constant: Vec2, max_iterations: int, position: Vec2) -> tuple:
    zn = z0
    iteration = 0
    while mod2(zn) < 4.0 and iteration < max_iterations:
        zn = compute_next(zn, constant)
        iteration += 1

    mod = math.sqrt(mod2(zn))
    smooth_iteration = float(iteration) - math.log2(max(1.0, math.log2(mod)))

    value = (smooth_iteration, position)

    return value

# Function wrapper to use parallel processing
def compute_iteration_wrapper(data: tuple) -> tuple:
    pixel_coord = data[0]
    constant = data[1]
    max_iterations = data[2]
    position = data[3]

    return compute_iterations_smooth(pixel_coord, constant, max_iterations, position)

# Compute all colors in the display map
def compute_color_map(constant: Vec2, render_size: Vec2) -> color_map.ColorMap:
    c_map = color_map.ColorMap(render_size)
    # scale of the coordinates
    scale = 1.0 / float(render_size.y / 2)
    
    # prepare values for parallel processing
    wrapper_values = []
    for y in range(render_size.y):
        for x in range(render_size.x):
            px = (x - render_size.x / 2) * scale
            py = (y - render_size.y / 2) * scale
            pixel_coord = Vec2(px, py)
            position = Vec2(x, y)
            wrapper_values.append((pixel_coord, constant, config.MAX_ITERATION, position))

    # send values to parallel process executor
    futures = []
    max_workers=32
    with ProcessPoolExecutor(max_workers=max_workers) as executor:
        futures = executor.map(compute_iteration_wrapper, wrapper_values, chunksize=len(wrapper_values)//max_workers)

        for future in futures:
            intensity, position = future
            result = (intensity, position.x, position.y)
            c_map.set_pixel_intensity(result)

    return c_map

# set all pixels to correct color
def render_color_map(render_size: Vec2, constant: Vec2, y_offset: int, x_offset: int, screen) -> None:
    c_map = compute_color_map(constant, render_size)
    c_map.normalize_pixel_values()
    # render each pixel in the color map as an individual rectangle
    for y in range(render_size.y):
        if not y + y_offset >= render_size.y:
            for x in range(render_size.x):
                if not x + x_offset >= render_size.x:
                    pygame.draw.rect(screen, pygame.Color(c_map.map[x + x_offset][y + y_offset]), (x, y, 1, 1))
