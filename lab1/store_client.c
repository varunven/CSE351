// Written by Porter Jones (pbjones@cs.washington.edu)
//
// This is a file for managing a store of various aisles,
// represented by an array of 64-bit integers

//TODO: Uncomment
#include <stddef.h> // To be able to use NULL
#include "aisle_manager.h"
#include "store_client.h"
#include "store_util.h"

// Number of aisles in the store
#define NUM_AISLES 10

// Number of sections per aisle
#define SECTIONS_PER_AISLE 4

// Number of items in the stockroom (2^6 different id combinations)
#define NUM_ITEMS 64

//Same number as from aisle_manager.c: NUM_SPACES = 10
#define NUM_SPACES 10

// Global array of aisles in this store. Each unsigned long in the array
// represents one aisle.
unsigned long aisles[NUM_AISLES];

// Array used to stock items that can be used for later. The index of the array
// corresponds to the item id and the value at an index indicates how many of
// that particular item are in the stockroom.
int stockroom[NUM_ITEMS];

/*
Static helper method to get the absolute change in elements between two sections in individual bits
*/
static int getChangeInItems(unsigned short section, unsigned short newSection)
{
    int diff = 0;
    for (int i = 0; i < NUM_SPACES; i++)
    {
        char bitInSpace = (section >> i) & 1;
        char bitInNewSpace = (newSection >> i) & 1;
        if (bitInNewSpace != bitInSpace)
        {
            diff += 1;
        }
    }
    return diff;
}

/* Starting from the first aisle, refill as many sections as possible using
 * items from the stockroom. A section can only be filled with items that match
 * the section's item id. Priotizes and fills sections with lower addresses
 * first. Sections with lower addresses should be fully filled (if possible)
 * before moving onto the next section.
 */
void refill_from_stockroom()
{
    // TODO: implement this function
    for (int i = 0; i < NUM_AISLES; i++)
    {
        for (int j = 0; j < SECTIONS_PER_AISLE; j++)
        {
            unsigned long *currAisle = aisles;
            currAisle = (currAisle + i);
            int id = get_id(currAisle, j);
            int currAmount = stockroom[id];
            unsigned short section = get_spaces(currAisle, j);
            add_items(currAisle, j, currAmount);
            unsigned short newSection = get_spaces(currAisle, j);
            int newIncrease = getChangeInItems(section, newSection);
            stockroom[id] = stockroom[id] - newIncrease;
        }
    }
}

/* Remove at most num items from sections with the given item id, starting with
 * sections with lower addresses, and return the total number of items removed.
 * Multiple sections can store items of the same item id. If there are not
 * enough items with the given item id in the aisles, first remove all the
 * items from the aisles possible and then use items in the stockroom of the
 * given item id to finish fulfilling an order. If the stockroom runs out of
 * items, you should remove as many items as possible.
 */
int fulfill_order(unsigned short id, int num)
{
    // TODO: implement this function
    int itemDecrease = 0;
    for (int i = 0; i < NUM_AISLES; i++)
    {
        for (int j = 0; j < SECTIONS_PER_AISLE; j++)
        {
            unsigned long *currAisle = aisles;
            currAisle = (currAisle + i);
            if (get_id(currAisle, j) == id && itemDecrease < num)
            {
                unsigned short section = get_spaces(currAisle, j);
                remove_items(currAisle, j, num - itemDecrease);
                unsigned short newSection = get_spaces(currAisle, j);
                itemDecrease += getChangeInItems(section, newSection);
            }
        }
    }
    if (itemDecrease < num)
    {
        int dif = num - itemDecrease;
        if (dif < stockroom[(int)id])
        {
            itemDecrease += dif;
            stockroom[(int)id] = stockroom[(int)id] - dif;
        }
        else
        {
            itemDecrease += stockroom[(int)id];
            stockroom[(int)id] = 0;
        }
    }
    return itemDecrease;
}

/* Return a pointer to the first section in the aisles with the given item id
 * that has no items in it or NULL if no such section exists. Only consider
 * items stored in sections in the aisles (i.e., ignore anything in the
 * stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short *empty_section_with_id(unsigned short id)
{
    // TODO: implement this function
    unsigned short *section = NULL;
    for (int i = 0; i < NUM_AISLES; i++)
    {
        for (int j = 0; j < SECTIONS_PER_AISLE; j++)
        {
            unsigned long *currAisle = aisles;
            currAisle = (currAisle + i);
            if (get_id(currAisle, j) == id && get_spaces(currAisle, j) == 0 && section == NULL)
            {
                section = (unsigned short *)currAisle;
                section = section + j;
            }
        }
    }
    return section;
}

/* Return a pointer to the section with the most items in the store. Only
 * consider items stored in sections in the aisles (i.e., ignore anything in
 * the stockroom). Break ties by returning the section with the lowest address.
 */
unsigned short *section_with_most_items()
{
    // TODO: implement this function
    unsigned short *section = (unsigned short *)aisles;
    int currMax = 0;
    for (int i = 0; i < NUM_AISLES; i++)
    {
        for (int j = 0; j < SECTIONS_PER_AISLE; j++)
        {
            unsigned long *currAisle = aisles;
            currAisle = (currAisle + i);
            unsigned short currSpace = get_spaces(currAisle, j);
            int itemsAtCurr = getChangeInItems(currSpace, 0);
            if (itemsAtCurr > currMax)
            {
                section = (unsigned short *)currAisle;
                section = section + j;
                currMax = itemsAtCurr;
            }
        }
    }
    return section;
}