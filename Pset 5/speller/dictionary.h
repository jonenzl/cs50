/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Define our node for the trie
typedef struct node
{
    // Boolean value to indicate whether this node is the end of a valid word
    bool is_word;
    // Array of 27 pointers to other nodes (26 letters and an apostrophe)
    struct node *children[27];
}
node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Returns the index of the array for each character (ie. a = 0, b = 1...).
 */
int arrayIndex(const char c);

/**
 * Check to see whether node children can be freed.
 */
void freeNode(node *current_node);

#endif // DICTIONARY_H
