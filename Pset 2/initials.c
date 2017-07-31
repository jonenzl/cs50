#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    // get user to input their name
    string name = get_string();
    
    // calculate the length of the string
    int length = strlen(name);
    
    if (name != NULL)
    {
        // print the first character as the first initial if it isn't a space
        if (name[0] != ' ')
        {
            printf("%c", toupper(name[0]));
        }
        
        // iterate through the string character by character
        for (int i = 0; i < length; i++)
        {   
           // check whether the current character is a space
           if (name[i] == ' ')
            {
                // if the following character is not a space, print it as an initial
                if (i + 1 < length && name[i + 1] != ' ')
                {
                    printf("%c", toupper(name[i + 1]));
                }
            }
        }
    }
    printf("\n");
}