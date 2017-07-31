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
    
    // get cipher key from the command-line argument, convert to an integer
    int key  = atoi(argv[1]); // atoi returns 0 if given a non-integer string
    
    // prompt user for the plaintext message
    printf("plaintext: ");
    string plaintext = get_string();
    
    // print label for the ciphertext
    printf("ciphertext: ");
    
    // encipher the message using Caesar's algorithm --> ci = (pi + k) % 26
    // iterate through each character in the string
    for (int i = 0, length = strlen(plaintext); i < length; i++)
    {
        // check if the character is uppercase, then encipher
        if (isupper(plaintext[i]))
        {
            // get the i'th character's ascii value, add the key
            // subtract 65 to get the alphanumeric value
            // get the remainder after dividing by 26
            // add 65 to get the character's ascii value
            printf("%c", (((plaintext[i] + key) - 65) % 26) + 65);
        }
        
        // check if the character is lowercase, then encipher
        else if (islower(plaintext[i]))
        {
            // get the i'th character's ascii value, add the key
            // subtract 97 to get the alphanumeric value
            // get the remainder after dividing by 26
            // add 97 to get the character's ascii value
            printf("%c", (((plaintext[i] + key) - 97) % 26) + 97);
        }
        
        // if the character is not alphabetic, just print it as is
        else
        {
            printf("%c", plaintext[i]);
        }
    }
    
    printf("\n");
    return 0;
}
