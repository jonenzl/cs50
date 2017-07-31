#!/usr/bin/env python3

import cs50
import sys

def main():
    
    # check whether we have the correct number of command-line arguments
    if len(sys.argv) != 2:
        print("Missing command-line argument, please enter the cipher key.")
        exit(1)
    
    # get cipher key from the command-line argument, convert to an integer
    key  = int(sys.argv[1]) # atoi returns 0 if given a non-integer string
    
    # prompt user for the plaintext message
    print("plaintext: ", end="")
    plaintext = cs50.get_string()
    
    # print label for the ciphertext
    print("ciphertext: ", end="")
    
    # encipher the message using Caesar's algorithm --> ci = (pi + k) % 26
    # iterate through each character in the string
    for char in plaintext:
        if char.isalpha():
            if char.isupper():
                print(chr(((ord(char) - 65 + key) % 26) + 65), end="")
            else:
                print(chr(((ord(char) - 97 + key) % 26) + 97), end="")
        else:
            print(char, end="")
        
    print()
    exit(0)
    
if __name__ == "__main__":
    main()