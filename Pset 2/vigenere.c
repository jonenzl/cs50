#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    // check whether we have the correct number of command-line arguments
    if (argc != 2)
    {
        printf("Missing command-line argument, please enter the cipher key.\n");
        return 1;
    }
    
    // check whether all the characters in the argument are alphabetical
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Key contains non-alphabetical characters. Please re-enter the cipher key.\n");
            return 1;
        }
    }
    
    // prompt user for the plaintext message
    printf("plaintext: ");
    string plaintext = get_string();
    
    // print label for the ciphertext
    printf("ciphertext: ");
    
    // encipher the message using Vigenere's algorithm --> ci = (pi + kj) % 26
    
    // initialize variable j in order to loop through the characters in the key
    int j = 0;
    
    // iterate through each character in the string
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        // loop through the key continuously
        j = j % strlen(argv[1]);
        
        // check if the character is alphabetical
        if (isalpha(plaintext[i]))
        {
            // check if the character is uppercase and the key character is uppercase, then encipher
            if (isupper(plaintext[i]) && isupper(argv[1][j]))
            {
                // get the i'th character's ascii value
                // subtract 65 to get the alphanumeric value
                // add the alphanumeric value of the j'th character of the key
                // get the remainder after dividing by 26
                // add 65 to get the character's ascii value
                printf("%c", (((plaintext[i] - 65) + (argv[1][j] - 65)) % 26) + 65);
            }
            
            // check if the character is uppercase and the key character is lowercase, then encipher
            else if (isupper(plaintext[i]) && islower(argv[1][j]))
            {
                // get the i'th character's ascii value
                // subtract 65 to get the alphanumeric value
                // add the alphanumeric value of the j'th character of the key
                // get the remainder after dividing by 26
                // add 65 to get the character's ascii value
                printf("%c", (((plaintext[i] - 65) + (argv[1][j] - 97)) % 26) + 65);
            }
            
            // check if the character is lowercase and the key character is uppercase, then encipher
            else if (islower(plaintext[i]) && isupper(argv[1][j]))
            {
                // get the i'th character's ascii value
                // subtract 97 to get the alphanumeric value
                // add the alphanumeric value of the j'th character of the key
                // get the remainder after dividing by 26
                // add 97 to get the character's ascii value
                printf("%c", (((plaintext[i] - 97) + (argv[1][j] - 65)) % 26) + 97);
            }
            
            // check if the character is lowercase and the key character is lowercase, then encipher
            else if (islower(plaintext[i]) && islower(argv[1][j]))
            {
                // get the i'th character's ascii value
                // subtract 97 to get the alphanumeric value
                // add the alphanumeric value of the j'th character of the key
                // get the remainder after dividing by 26
                // add 97 to get the character's ascii value
                printf("%c", (((plaintext[i] - 97) + (argv[1][j] - 97)) % 26) + 97);
            }
            
            // increment j to get the next character in the key
            j++;
        }
        
        // if the character is not alphabetical, just print it as is
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    
    printf("\n");
    return 0;
}