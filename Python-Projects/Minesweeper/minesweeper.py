# Name: Andrew Law
# Instructor: Dr. Smith
# Course: CSCI-3038
# Assignment: Project 3

# used for terminal animations
import time
# used for rendering display
import os
# required for game implementation
import random


def main():
    terminal_start_anim()
    terminal_load_anim('Loading Main Menu')
    print_start_menu()
    return

### Menus and Display ###
# clear terminal and print menu and return user's selection
def print_start():
    # clear the terminal and print menu/prompt user
    os.system('clear')
    print('=[CSCI-3038]=====================[Project-3]=')
    print('|                                           |')
    print('|                MINESWEEPER                |')
    print('|                                           |')
    print('|               BY ANDREW LAW               |')
    print('|                                           |')
    print('=============================================')
    print('Menu:\n[S]tart  [Q]uit')
    user_selection = input('Enter Command:')

    return user_selection


# call print_start and process user's selection
def print_start_menu():
    # print menu and get user input
    user_selection = print_start()
    while True:
        # handle user menu selection
        match (user_selection):
            case 'S' | 's':
                start_game()
                return
            case 'Q' | 'q':
                return
            case _:
                print('Invalid Command.',end='\n')
        user_selection = print_start()


# display the gameboard to the terminal
def render_game_board(grid, flags_placed):
    # clear the terminal
    os.system('clear')
    # print the game board
    print('=[CSCI-3038]====[MINESWEEPER]====[Project-3]=')
    for x in range(5):
        print('|       ||       ||       ||       ||       |')
        for y in range(5):
            print(f'|   {grid[x][y]}   |', end='')
        print('\n|       ||       ||       ||       ||       |')
        if x != 4:
            print('|-------++-------++-------++-------++-------|')
    print(f'=[Flags Placed: {flags_placed:>2}]==========================')

### Game and Game Utilities ###
# main game function
def start_game():
    game_loop_active = True
    game_victory = False
    flags_placed = 0
    action_list = []

    terminal_load_anim('Generating Latent Grid', 1)
    terminal_load_anim('Calculating Adjacentcies', 1)
    terminal_load_anim('Rendering Display', 1)
    
    # init grids
    latent_grid = generate_latent_grid()
    display_grid = [[' ', ' ', ' ', ' ', ' '], [' ', ' ', ' ', ' ', ' '], [' ', ' ', ' ', ' ', ' '], [' ', ' ', ' ', ' ', ' '], [' ', ' ', ' ', ' ', ' ']]
    checked_tiles = [[0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0], [0, 0, 0, 0, 0]]

    # print board and command prompt
    render_game_board(display_grid, str(flags_placed))
    print('Menu:\n[C]heck Tile  [F]lag Tile  [U]nflag Tile  [M]ain Menu')
    while game_loop_active:
        previous_command = ''

        # get and process user selection
        user_selection = input('Enter Command: ')
        match user_selection:
            case 'C' | 'c':
                # get coords, check coords, use coords to reveal a tile/end game (if mine)
                user_tile = input('\rEnter Tile to Reveal [format: x y]:')
                while not user_coords_valid(user_tile):
                    user_tile = input('\rInvalid Coordiantes. Enter Tile to Reveal [format: x y]:')
                user_tile = user_tile.split()
                user_x = int(user_tile[0])
                user_y = int(user_tile[1])
                previous_command += f'Check {user_x} {user_y}'

                if not latent_is_mine(user_x, user_y, latent_grid):
                    display_grid = check_and_display_adjacents(user_x, user_y, latent_grid, display_grid, checked_tiles)
                else:
                    game_loop_active = False # Game ends when mine is hit

            case 'F' | 'f':
                # check if max flags are reached
                if flags_placed == 4:
                    input('You cannot place more than 4 flags. Enter to continue.')
                else:
                    # get coords, check coords, place flag if valid
                    user_tile = input('\rEnter Tile to Flag [format: x y]:')
                    while not user_coords_valid(user_tile):
                        user_tile = input('\rInvalid Coordiantes. Enter Tile to Flag [format: x y]:')
                    user_tile = user_tile.split()
                    user_x = int(user_tile[0])
                    user_y = int(user_tile[1])
                    previous_command += f'Flag {user_x} {user_y}'

                    if display_grid[user_y][user_x] == ' ':
                        display_grid = flag_tile(user_x, user_y, display_grid)
                        flags_placed += 1
                    else:
                        if display_grid[user_y][user_x] == '?':
                            input(f'Flag already placed on tile ({user_x}, {user_y}). Enter to continue.')
                        else:
                            input(f'Tile already ({user_x}, {user_y}) revealed. Enter to continue.')

            case 'U' | 'u':
                # check if there are flags to be unflagged
                if flags_placed == 0:
                    input('There are no flags placed. Enter to continue.')
                else:
                    # get coords, check coords, remove flag if valid
                    user_tile = input('\rEnter Tile to Unflag [format: x y]:')
                    while not user_coords_valid(user_tile):
                        user_tile = input('\rInvalid Coordiantes. Enter Tile to Reveal [format: x y]:')
                        
                    user_tile = user_tile.split()
                    user_x = int(user_tile[0])
                    user_y = int(user_tile[1])
                    previous_command += f'Unflag {user_x} {user_y}'

                    if display_grid[user_y][user_x] == '?':
                        display_grid = unflag_tile(user_x, user_y, display_grid)
                        flags_placed -= 1
                    else:
                        input(f'No flag on tile ({user_x}, {user_y}). Enter to continue.')

            case 'M' | 'm':
                # return to main menu
                print_start_menu()
                game_loop_active = False
                return

            case _:
                print('Invalid Command.')
        
        action_list.append(previous_command) # Save user inputed commands to action_list

        # if the player reveals the entire grid, end loop in victory
        if all_grid_shown(display_grid):
            game_loop_active = False
            game_victory = True

        # reprint board and commands 
        render_game_board(display_grid, str(flags_placed))
        print('Menu:\n[C]heck Tile  [F]lag Tile  [U]nflag Tile  [M]ain Menu')

    # end of game processing
    flash_board(latent_grid)
    render_game_board(display_grid, flags_placed)
    if game_victory:
        print("You Win! Saving Game Report.")
        save_game_to_file(latent_grid, action_list, 'win')
    else:
        print("Game Over! Saving Game Report.")
        save_game_to_file(latent_grid, action_list, 'loss')
    user_selection = input('Menu:\n[M]ain Menu  [Q]uit\nEnter Command:')
    if user_selection == 'M' or user_selection == 'm':
        print_start_menu()
    return


# create and return hidden data grid
def generate_latent_grid():
    latent_grid = [['0', '0', '0', '0', '0'], ['0', '0', '0', '0', '0'], ['0', '0', '0', '0', '0'], ['0', '0', '0', '0', '0'], ['0', '0', '0', '0', '0']]
    mine_locations = set()
    # generate random locations for 4 mines
    while(len(mine_locations) < 4):
        location_x = random.randint(0,4)
        location_y = random.randint(0,4)
        location = (location_x, location_y)
        mine_locations.add(location) # use a set so duplication locations are not saved

    # place the mines and number of mines in the appropriate grid location
    for x in range(5):
        for y in range(5):
            if (x,y) in mine_locations:
                latent_grid[x][y] = 'X'
            else:
                latent_grid[x][y] = compute_adjacent_mines(x, y, mine_locations)

    return latent_grid


# determine the number of mines adjacent to a given tile (used for populating the hidden data grid)
def compute_adjacent_mines(x, y, mine_locations):
    adjacent_tile_locations = get_adjacent_set(x, y)
    adjacent_mines = 0
    for i in range(len(adjacent_tile_locations)):
        adjacent_mines += check_adjacent_mines(adjacent_tile_locations[i], mine_locations)

    return adjacent_mines
        

def get_adjacent_set(x, y):
    adjacent_set = []
    if x == 0:
        if y == 0: # top left
            adjacent_set = [(x + 1, y), (x + 1, y + 1), (x, y + 1)]
        elif y == 4: # bottom left
            adjacent_set = [(x, y - 1), (x + 1, y - 1), (x + 1, y)]
        else: # left side not corner
            adjacent_set = [(x, y - 1), (x + 1, y - 1), (x + 1, y), (x + 1, y + 1), (x, y + 1)]
    elif x == 4:
        if y == 0: # top right
            adjacent_set = [(x, y + 1), (x - 1, y + 1), (x - 1, y)]
        elif y == 4: # bottom right
            adjacent_set = [(x, y - 1), (x - 1, y), (x - 1, y - 1)]
        else: # right side not corner
            adjacent_set = [(x, y - 1), (x, y + 1), (x - 1, y + 1), (x - 1, y), (x - 1, y - 1)]
    else:
        if y == 0: # top row not corner
            adjacent_set = [(x + 1, y), (x + 1, y + 1), (x, y + 1), (x - 1, y + 1), (x - 1, y)]
        elif y == 4: # bottom row not corner
            adjacent_set = [(x, y - 1), (x + 1, y - 1), (x + 1, y), (x - 1, y), (x - 1, y - 1)]
        else: # internal
            adjacent_set = [(x, y - 1), (x + 1, y - 1), (x + 1, y), (x + 1, y + 1), (x, y + 1), (x - 1, y + 1), (x - 1, y), (x - 1, y - 1)]
    return adjacent_set


# check if there are mines adjacent to a location
def check_adjacent_mines(location, mine_locations):
    if location in mine_locations:
        return 1
    else: 
        return 0


# recusive function to check and display any tiles (and their adjacents) without adjacent mines
def check_and_display_adjacents(x, y, latent_grid, display_grid, checked_tiles):
    checked_tiles_marker = -1
    if latent_grid[y][x] == 0: # if no adjacent mines
        display_grid[y][x] = latent_grid[y][x]
        checked_tiles[y][x] = checked_tiles_marker # keep track of tiles already checked (prevent infinte loop)
        adjacent_tiles = get_adjacent_set(x, y)
        print(f'x, y: {x}, {y}')
        print(adjacent_tiles)
        for tile in adjacent_tiles:
            x = tile[0]
            y = tile[1]
            if checked_tiles[y][x] == 0:
                display_grid = check_and_display_adjacents(x, y, latent_grid, display_grid, checked_tiles)    
    display_grid[y][x] = latent_grid[y][x]
    return display_grid


# check if hidden data is a mine at a given location
def latent_is_mine(x, y, latent_grid):
    if latent_grid[y][x] == 'X':
        return True
    return False


# place flag on a tile
def flag_tile(x, y, display_grid):
    display_grid[y][x] = '?'
    return display_grid


# remove a flag from a tile
def unflag_tile(x, y, display_grid):
    display_grid[y][x] = ' '
    return display_grid


# check if the entire grid has been revealed
def all_grid_shown(grid):
    for x in range(5):
        for y in range(5):
            if grid[y][x] == ' ':
                return False
    return True


# ensure user entered coords are valid
def user_coords_valid(user_tile):
    user_tile = user_tile.split()
    if '0' <= user_tile[0] <= '4':
        if '0' <= user_tile[1] <= '4':
            return True
    return False


# save game data to a file
def save_game_to_file(grid, action_list, result):
    with open('previous_game.dat', 'w') as file:
        if result == 'win':
            file.write(f'===================[WIN!]====================\n')
        else:
            file.write(f'===================[LOSS]====================\n')
        for y in range(5):
            file.write('|       ||       ||       ||       ||       |\n')
            file.write(f'|   {grid[y][0]}   ||   {grid[y][1]}   ||   {grid[y][2]}   ||   {grid[y][3]}   ||   {grid[y][4]}   |\n')
            file.write('|       ||       ||       ||       ||       |\n')
            if y != 4:
                file.write('|-------++-------++-------++-------++-------|\n')
        file.write(f'=============================================\n')
        command_num = 0
        file.write('Command List:\n')
        for command in action_list:
            file.write(f'{command_num:>3}: {command}\n')
            command_num += 1
        file.close()

### Terminal Animations ###
# flash gameboard (used when game ends)
def flash_board(grid, num_cycles=1):
    for i in range(num_cycles*2):
        render_game_board(grid, 'XX')
        time.sleep(0.15)

        os.system('clear')
        print('=[CSCI-3038]====[MINESWEEPER]====[Project-3]=')
        for x in range(19):
            print('|                                           |')
        print('=============================================')
        time.sleep(0.15)


# start animation called at beginning of program
def terminal_start_anim():
    line_1_content = '_________________________________________________________________________________________________________________________________________________________________________________________________________'
    line_2_content = '    | | |                                     |\\\\   /|   ||   |\\\\   ||   ||     ]    \\\\  \\\\   ||    ||   ||     ]   ||     ]   ||    \    ||     ]   ||    \                                    | | |    '
    line_3_content = '    | | |                                     ||\\\\ /||   ||   ||\\\\  ||   ||           \\\\  /   ||    ||   ||         ||         ||    ||   ||         ||    ||                                   | | |    '
    line_4_content = '    | | |                                     || \\v ||   ||   || \\\\ ||   ||____        \\\\     || /^ ||   ||____     ||____     ||____/    ||____     ||____/                                    | | |    '
    line_5_content = '    | | |                                     ||    ||   ||   ||  \\\\||   ||         /   \\\\    ||// \||   ||         ||         ||         ||         || \\\\                                      | | |    '
    line_6_content = '____|_|_|_____________________________________||____||___||___||___\||___||_____]___\\\\__//____|//___\|___||_____]___||_____]___||_________||_____]___||__\\\\_____________________________________|_|_|____'

    for i in range(156):
        print('\n\n')
        print(line_1_content[i:i+45], line_2_content[i:i+45], line_3_content[i:i+45], line_4_content[i:i+45], line_5_content[i:i+45], line_6_content[i:i+45],sep='\n',end='\n')
        time.sleep(0.017)
        os.system('clear')


# simple load animation 
def terminal_load_anim(terminal_message='Loading', num_cycles=2):
    os.system('clear')
    loading_message = ''
    for i in range(num_cycles*4):
        match (i % 4):
            case 0:
                loading_message = f'{terminal_message} (-)'
            case 1:
                loading_message = f'{terminal_message} (\)'
            case 2:
                loading_message = f'{terminal_message} (|)'
            case 3:
                loading_message = f'{terminal_message} (/)'
        print(f'{loading_message:^45}', end='\r')
        time.sleep(0.15)
    os.system('clear')


if __name__ == '__main__':
    main()
