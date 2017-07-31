/****************************************************************************
 * recover.c
 *
 * HarvardX - CS50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image
 ***************************************************************************/
 
#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 512

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    
    // open the forensic image (the card file)
    FILE *inptr = fopen(argv[1], "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open card.raw.\n");
        return 2;
    }
    
    // create the buffer array
    uint8_t buffer[BUFFER_SIZE];
    
    // create the output file counter
    int counter = 0;
    
    // create the output file
    FILE *outptr = NULL;
    
    // iterate through the contents of the card file 512 bytes at a time
    while (fread(buffer, BUFFER_SIZE, 1, inptr))
    {
        // read the first four bytes of the buffer and check to see if we have found the JPEG signature
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close the file if it is already open
            if (outptr != NULL)
            {
                fclose(outptr);
            }
            
            // create the filename
            char filename[8];
            sprintf(filename, "%03i.jpg", counter);
            
            // open the output file
            outptr = fopen(filename, "w");
            
            // increment the file counter
            counter++;
        }
        
        // start writing the output file 512 bytes at a time
        if (outptr != NULL)
        {
            fwrite(&buffer, BUFFER_SIZE, 1, outptr);
        }
    }
    
    // close the input file
    fclose(inptr);

    // close the output file
    fclose(outptr);

    // success
    return 0;
}