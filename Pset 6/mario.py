#!/usr/bin/env python3

import cs50

def main():
    
    # get the height of the pyramid from the user
    while True:
        print("Height: ", end="")
        height = cs50.get_int()
        if height >= 0 or height <= 23:
            break
        
    # create the pyramid using loops
    for i in range(height):
        for j in range(height - 1 - i):
            print(" ", end="")
        for k in range(i + 2):
            print("#", end="")
        print("")
    
if __name__ == "__main__":
    main()