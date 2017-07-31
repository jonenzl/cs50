/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

// Create our root node
node *root;

// Define a global variable to track the number of words in the dictionary
unsigned int word_count = 0;

// Define a global variable to track whether the dictionary has loaded properly
bool loaded = false;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Set current_node to root
    node *current_node = root;
    
    // For each character in the input word
    for (int i = 0; word[i] != '\0'; i++)
    {
        // Find the index of the current character
        int index = arrayIndex(word[i]);
        
        // If the character at the current node is NULL, the world is misspelled
        if (current_node->children[index] == NULL)
        {
            return false;
        }
        
        // If not NULL, move to the next character
        current_node = current_node->children[index];
    }
    
    // At the end of the word, check if is_word is true
    return current_node->is_word;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary) 
{    
    // Read the dictionary
    FILE *fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        printf("Could not open %s.\n", dictionary);
        unload();
        return false;
    }
    
    // Create space for root
    root = malloc(sizeof(node));
    // Check to make sure the pointer to the node does not return NULL
    if (root == NULL)
    {
        unload();
        return false;
    }
    
    // Set current_node to root
    node *current_node = root;
    
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // Find the index of the current character
        int index = arrayIndex(c);
        
        // Check whether it is a new line
        if(c != '\n')
        {
            // Check whether a node already exists for the character
            if(current_node->children[index] == NULL)
            {
                // Create a new node
                current_node->children[index] = malloc(sizeof(node));
                // Check to make sure the pointer to the node does not return NULL
                if (current_node->children[index] == NULL)
                {
                    unload();
                    return false;
                    
                }
                // Move to the next node
                current_node = current_node->children[index];
            }
            else 
            {
                // Move to the next node
                current_node = current_node->children[index];
            }   
        }        
        else
        {
            // Mark as a word
            current_node->is_word = true;
            
            // Increment the word count
            word_count++;
            
            // Reset the current_node to root to traverse the trie again
            current_node = root;           
        }
    }   
    //Close the dictionary
    fclose(fp); 
    
    //The dictionary has load successfully
    loaded = true;
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (loaded)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{   
    freeNode(root);
    return true;
}

/**
 * Returns the index of the array for each character (ie. a = 0, b = 1...).
 */
int arrayIndex(const char c)
{
    // If the character is an apostrophe, return the index for the last element in the array
    if (c == '\'')
    {
        return 26;
    }
    else
    {
        /* Convert all characters to lower case. The character's index is the remainder of the character's ascii value
        after dividing by the ascii value of "a" (ie. g = 103 % 97 = 6) */
        return tolower(c) % 'a';
    }
}

/**
 * Check to see whether node children can be freed.
 */
void freeNode(node* current_node)
{
    // Iterate through the nodes children 
    for(int i = 0; i < 27; i ++)
    {
        // If child is a pointer, iterate recursively through it as well
        if(current_node->children[i] != NULL)
        {
            freeNode(current_node->children[i]);
        }
    }
    
    // If all children are NULL, free the node
    free(current_node);
}