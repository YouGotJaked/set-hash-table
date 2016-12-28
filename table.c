/* 
 * File:  table.c
 * Author: Jake Day
 * Class: COEN 12
 * Instructor: Atkinson
 * Section: T 5:15-8pm
 * Created on April 19, 2016, 5:29 PM
 * Description: This program implements a set abstract data type for strings using a hash table
 */

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "set.h"

//flags for each element are defined
#define EMPTY 0
#define FILLED 1
#define DELETED 2

int findElement(SET *sp, char *elt, bool *found);

struct set {
    int length;
    int count;
    char **elts;
    char *flags;
};

/*
 * Big-O Notation: O(n)
 * Creates a set by allocating memory and returns a pointer to this new set with a maximum capacity of maxElts
 */
SET *createSet(int maxElts) {
    SET *sp;

    sp = malloc(sizeof (SET));
    assert(sp != NULL);

    sp->count = 0;
    sp->length = maxElts;

    //allocates memory for the elements
    sp->elts = malloc(sizeof (char*) * maxElts);
    assert(sp->elts != NULL);

    //allocates memory for the flags
    sp->flags = malloc(sizeof (char) * maxElts);
    assert(sp->flags != NULL);

    //loops through the array and sets all the flags to empty
    for (int i = 0; i < maxElts; i++) {
        sp->flags[i] = EMPTY;
    }
    return sp;
}

/*
 * Big-O Notation: O(n)
 * Destroys a set by deallocating memory associated with the set pointed to by sp 
 */
void destroySet(SET *sp) {
    assert(sp != NULL);

    //loops through the array and frees all the characters individually
    for (int i = 0; i < sp->length; i++) {
        if (sp->flags[i] == FILLED) {
            free(sp->elts[i]);
        }
    }

    //frees all the memory associated with each set
    free(sp->elts);
    free(sp->flags);
    free(sp);
}

/*
 * Big-O Notation: O(1)
 * Returns the number of elements in the set pointed to by sp
 */
int numElements(SET *sp) {
    assert(sp != NULL);
    return sp->count;
}

/*
 * Big-O Notation: O(n)
 * Assigns a char value to an unsigned hash value and returns the hash value
 */
unsigned hashString(char *s) {
    unsigned hash = 0;

    while (*s != '\0') {
        hash = 31 * hash + *s++;
    }

    return hash;
}

/*
 * Big-O Notation: O(1) [best], O(n) [worst]
 * Returns the location of an element in the set 
 */
int findElement(SET *sp, char *elt, bool *found) {
    assert(sp != NULL && elt != NULL);

    int position;
    int first = -1;
    unsigned hash;

    //creates a hash value for element
    hash = hashString(elt);

    //loops through all the positions in the array
    for (int i = 0; i < sp->length; i++) {
        position = (hash + i) % sp->length;

        //if the flag is deleted and this is the first position, remember this position
        if (sp->flags[position] == DELETED && first == -1) {
            first = position;
        }

        //if the flag is empty, return the first deleted position if possible or the current position
        if (sp->flags[position] == EMPTY) {
            *found = false;

            if (first != -1) {
                return first;
            }
            return position;

        }

        //if the flag is filled and the element is in the current position, then the element has been found
        if (sp->flags[position] == FILLED && strcmp(elt, sp->elts[position]) == 0) {
            *found = true;
            return position;
        }
    }

    //if none of the other conditions are met, the element was not found and the first deleted position is returned
    *found = false;
    return first;

}

/*
 * Big-O Notation: O(1) [best], O(n) [worst]
 * Returns whether elt is a member of the set pointed to by sp 
 */
bool hasElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);

    bool found;

    findElement(sp, elt, &found);
    return found;
}

/*
 * Big-O Notation: O(1) [best], O(n) [worst]
 * Adds elt to the set pointed to by sp and returns whether the set changed 
 */
bool addElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);

    //returns false if the set is full
    if (sp->count == sp->length) {
        return false;
    }

    bool found;
    int position = findElement(sp, elt, &found);

    //returns false if the element is already in the set
    if (found) {
        return false;
    }

    elt = strdup(elt);
    assert(elt != NULL);

    //if the element is not found, it is copied into the position given by findElement, the position is flagged as filled, and the count is incremented by one
    sp->elts[position] = elt;
    sp->flags[position] = FILLED;
    sp->count++;
    return true;
}

/*
 * Big-O Notation: O(1) [best], O(n) [worst]
 * Removes elt from the set pointed to by sp and returns whether the set changed 
 */
bool removeElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);

    //returns false if the set is empty
    if (sp->count == 0) {
        return false;
    }

    bool found;
    int position = findElement(sp, elt, &found);

    //if the element is found, its position in memory is freed, the position is flagged as deleted, and the count is decremented by one
    if (found) {
        free(sp->elts[position]);
        sp->flags[position] = DELETED;
        sp->count--;
        return true;
    }
    return false;

}