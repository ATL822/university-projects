# Name: Andrew Law
# Instructor: Dr. Smith
# Course: CSCI-3038
# Assignment: Project 1

def main():
    passcode = ''
    previous_segment = ''
    digit_1_set = False
    digit_2_set = False

    print('---CID Password Extractor---')
    
    # get initial user input
    user_input = input('Enter line (enter \'end\' to quit): ') 

    while user_input != 'end':
        # check each character in the string
        for ch in user_input:
            # if it is a digit save it and set flag
            if is_digit(ch):
                if not digit_1_set:
                    digit_1 = ch
                    digit_1_set = True
                else:
                    digit_2 = ch
                    digit_2_set = True
        
        # concat. the two digits in the correct order (second digit followed by first digit or first digit twice in case of only one digit read from passcode segment)
        if digit_1_set and not digit_2_set:
            passcode_segment = digit_1 + digit_1
        elif digit_1_set and digit_2_set:
            passcode_segment = digit_2 + digit_1 
        else:
            passcode_segment = '-1'
        
        # if passcode segment is -1, subtract -1 from the previous passcode segment, otherwise concat. the passcode segment to the end of the string 
        if passcode_segment == '-1':
            if previous_segment != '' and int(previous_segment) > 0: # if the passcode is not empty or 0 (cannot have a negative passcode)
                previous_segment = str(int(previous_segment) - 1) 
                if int(previous_segment) < 10:
                    previous_segment = '0' + previous_segment
            # if previous segment was reduced to 0, correct string from single 0 to double 0.
            if previous_segment != '' and int(previous_segment) == 0:
                previous_segment = '00'
        else:
            passcode = passcode + previous_segment
            previous_segment = passcode_segment
            
        
        # reset flags
        digit_1_set = False
        digit_2_set = False

        # print extracted segment
        print(f'\t{passcode_segment} extracted')

        # get next line of user input
        user_input = input('Enter line: ')

    # add the last segment to the passcode
    passcode = passcode + previous_segment

    if passcode != '':
        print('\nthe passcode is: ' + passcode)
    else:
        print('\nNo passcode was extracted.')


def is_digit(ch):
    if '0' <= ch <= '9':
        return True
    else:
        return False


if __name__ == '__main__':
    main()
