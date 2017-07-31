#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    int spaces;
    int hashes;
    
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height > 23);
    
    // loop for the rows
    for (int i = 0; i < height; i++)
    {
        // print spaces for left pyramid
        for (spaces = ((height - 1) - i); spaces > 0; spaces--)
        {
            printf(" ");
        }
        // print hashes for left pyramid
        for (hashes = 1; hashes <= (i + 1); hashes++)
        {
            printf("#");
        }
        // print gap
        printf("  ");
        // print hashes for right pyramid
        for (hashes = 1; hashes <= (i + 1); hashes++)
        {
            printf("#");
        }
        // print new line
        printf("\n");
    }
}