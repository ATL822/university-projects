# Name: Andrew Law
# Instructor: Dr. Smith
# Course: CSCI-3038
# Assignment: Project 2

import sys

def main():
    filename = ''
    tokens_won = 0
    
    try:
        filename = sys.argv[1]
    except IndexError:
        print(f'ERROR: missing arguments for correct usage.\nUSAGE: python nebulanumbers.py filename')
        return

    try:
        open_file = open(filename, 'r')
    except FileNotFoundError:
        print(f'ERROR: {filename} could not be opened...')
        return
       
    print('------Matches------')

    for line in open_file:
        bet_number, winning_set = parseline(line)
        print(f'{bet_number:>4}: ', end='')

        if winning_set == set():
            print('None')
        else:
            print(winning_set)
            tokens_won += 2 ** (len(winning_set)-1) 

    print(f'\nTokens Won: {tokens_won}')

    return


def parseline(readline):
    readline = readline[4:] # remove formatting from beginning of line ('Bet\t')
    bet_number, readline = readline.split(':') # split off the bet number and remaining data
    user_numbers, house_numbers = readline.split('=>') # split the sets of numbers into seperate variables
    user_numbers = set(user_numbers.split()) # place each user number into a set (remove dupes)
    house_numbers = set(house_numbers.split()) # place each house number into a set (remove dupes)
    return bet_number, (user_numbers & house_numbers) # return the bet number and the intersection of the two sets (shared values)
    

if __name__ == '__main__':
    main()
