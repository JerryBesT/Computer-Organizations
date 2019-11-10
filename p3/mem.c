////////////////////////////////////////////////////////////////////////////////
// Main File:        Mem.c
// This File:        Mem.c
// Other Files:      Mem.o, Mem.h
// Semester:         CS 354 Spring 2018
//
// Author:           Zhenyu Zou
// Email:            zzou24@wisc.edu
// CS Login:         zzou
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Independent 
//                   Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include "mem.h"

/*
 * This structure serves as the header for each allocated and free block
 * It also serves as the footer for each free block
 * The blocks are ordered in the increasing order of addresses 
 */
typedef struct blk_hdr {                         
    int size_status;

	/*
	 * Size of the block is always a multiple of 8
	 * => last two bits are always zero - can be used to store other information
	 *
	 * LSB -> Least Significant Bit (Last Bit)
	 * SLB -> Second Last Bit 
	 * LSB = 0 => free block
	 * LSB = 1 => allocated/busy block
	 * SLB = 0 => previous block is free
	 * SLB = 1 => previous block is allocated/busy
	 * 
	 * When used as the footer the last two bits should be zero
	 */

	/*
	 * Examples:
	 * 
	 * For a busy block with a payload of 20 bytes (i.e. 20 bytes data + an additional 4 bytes for header)
	 * Header:
	 * If the previous block is allocated, size_status should be set to 27
	 * If the previous block is free, size_status should be set to 25
	 * 
	 * For a free block of size 24 bytes (including 4 bytes for header + 4 bytes for footer)
	 * Header:
	 * If the previous block is allocated, size_status should be set to 26
	 * If the previous block is free, size_status should be set to 24
	 * Footer:
	 * size_status should be 24
	 * 
	 */
} blk_hdr;

/* Global variable - This will always point to the first block
 * i.e. the block with the lowest address */
blk_hdr *first_blk = NULL;

/*
 * Note: 
 *  The end of the available memory can be determined using end_mark
 *  The size_status of end_mark has a value of 1
 *
 */

/* 
 * Function for allocating 'size' bytes
 * Returns address of allocated block on success 
 * Returns NULL on failure 
 * Here is what this function should accomplish 
 * - Check for sanity of size - Return NULL when appropriate 
 * - Round up size to a multiple of 8 
 * - Traverse the list of blocks and allocate the best free block which can accommodate the requested size 
 * - Also, when allocating a block - split it into two blocks
 * Tips: Be careful with pointer arithmetic 
 */
void* Mem_Alloc(int size) {
    // check for the sanity of the size
    if  ((size <= 0) || (first_blk == NULL))
        return NULL;
    
    // plus 4 bytes for header and cast the size
    size = (int)size;
    size = size + 4;
    int blk_padding = 0;

    // rounding
    blk_padding = size % 8;

    // rounding up to multiple of 8
    if  (size <= 8)
        size = 8;
    else
        size = size + (8 - blk_padding); 

    // Traversal
    blk_hdr* curr = (blk_hdr*)first_blk;

    // pointer of the best blk
    blk_hdr* bestptr = NULL; 

    // update the total size
    int totalSize = first_blk->size_status - 2;

    // for comparison
    int min;

    // temp value for best blk
    int temp;
    
    // while the blk size is not 1
    while (curr->size_status != 1) {
        // if free
        if ((curr->size_status & 1) == 0) { 
            // if previous free is allocated

            if ((curr->size_status & 2) == 2) { 
                // actual payload + header
                temp = curr->size_status - 2;
            } else {
            // if previous free is not allocated
                temp = curr->size_status;
            }

            // if the best block is found, exit
            if (temp == size) {
                bestptr = curr;
                break;
            }

            // check if it is best fit
            if (temp > size) {
                if (bestptr == NULL) {
                    // start from the first free
                    bestptr = (blk_hdr*)curr;
                    min = temp - size;
                }
                // try to find the best size
                if ((temp - size) < min) {
                    min = temp - size;
                    bestptr = (blk_hdr*)curr;
                }
            }
            // move the blk
            curr = (blk_hdr*)((char*)curr + temp);
        } else { 
            // if busy
            // if previous is allocated
            if ((curr->size_status & 2) == 2) {
                // actual payload
                int temp3 = curr->size_status - 3;
                // proceed to the next block
                curr = (blk_hdr*)((char*)curr + temp3);
            }

            // if previous is not allocated
            if ((curr->size_status & 2) == 0) {
                // actual payload
                int temp4 = curr->size_status - 1;
                // proceed to the next block
                curr = (blk_hdr*)((char*)curr + temp4);
            }
        }
    }


    // alloc is successful
    if (bestptr != NULL) {
        // get the size of the block without status
        int allocSize = bestptr->size_status;
        if ((allocSize & 2) == 2)
            allocSize -= 2;
        // update total size
        totalSize = totalSize - size;

        // change status of next block
        blk_hdr* alloc_next = (blk_hdr*)((char*)bestptr + allocSize);

        // check if the next blk status is correct
        if ((alloc_next->size_status & 2) == 0 && alloc_next->size_status != 1)
            alloc_next->size_status += 2;

        // split
        if ((allocSize - size - 8) >= 0) {
            // split the next block
            blk_hdr* split_blk = (blk_hdr*)((char*)bestptr + size);
            split_blk->size_status = allocSize - size + 2;

            // reassign footer
            blk_hdr* split_footer = 
            (blk_hdr*)((char*)split_blk + allocSize - size - 4);
            split_footer->size_status = allocSize - size;
        }

        // check if previous is free or allocated
        blk_hdr* temptr = (blk_hdr*)((char*)bestptr - 4);
        bestptr->size_status = size + 1;

        // update the best block's status
        if ((temptr->size_status & 1) == 0)
            bestptr->size_status += 2;

        // only for personal testing
        // printf("\n\n<<<<<<<<<MemAlloc(%d)\n", size);
        // Mem_Dump();
        // printf(">>>>>>>>>>>>>>>>>>>>\n");

        // point to payload
        return (void*)bestptr + 4;
    } else {
        return NULL;
    }
}

/* 
 * Function for freeing up a previously allocated block 
 * Argument - ptr: Address of the block to be freed up 
 * Returns 0 on success 
 * Returns -1 on failure 
 * Here is what this function should accomplish 
 * - Return -1 if ptr is NULL
 * - Return -1 if ptr is not 8 byte aligned or if the block is already freed
 * - Mark the block as free 
 * - Coalesce if one or both of the immediate neighbours are free 
 */
int Mem_Free(void *ptr) { 
    // check if pointer is null
    if (ptr == NULL)
        return -1;
    // point to header
    blk_hdr* header = (blk_hdr*)((char*)ptr - 4);

    // check for sanity
    if ((header->size_status & 1) == 0)
        return -1;
    if (((int)ptr) % 8 != 0)
        return -1;

    // for checking if previous is free
    int tempX = header->size_status;

    // free up
    header->size_status -= 1;
    if ((header->size_status & 2) == 2) {
        // now is no status
        header->size_status -= 2;
    }

    // header points to header not payload
    // set the footer for the new freed block
    blk_hdr* footer = 
    (blk_hdr*)((char*)header + header->size_status - sizeof(blk_hdr));
    footer->size_status = header->size_status;

    // store the next blk
    blk_hdr* next_blk = (blk_hdr*)((char*)header + header->size_status);

    // coalescing
    // if previous is free
    if ((tempX & 2) == 0) {
        // update pointer
        blk_hdr* footer_of_previous = 
        (blk_hdr*)((char*)header - sizeof(blk_hdr));

        // without status
        int previous_size = footer_of_previous->size_status;
        blk_hdr* header_of_previous = (blk_hdr*)((char*)header - previous_size);

        // if next blk is busy
        if ((next_blk->size_status & 1) == 1) {
            // the header of memory with previous block, with status
            header_of_previous->size_status += header->size_status;
            footer->size_status = previous_size + header->size_status;

            // update header
            header = (blk_hdr*)header_of_previous;

            // change status
            blk_hdr* prev_next = (blk_hdr*)((char*)footer + sizeof(blk_hdr));
            prev_next->size_status -= 2;

        } else { 
            // if next blk is free
            int tempNext = next_blk->size_status - 2;

            // update footer
            blk_hdr* newfooter = 
            (blk_hdr*)((char*)header + 
            header->size_status + tempNext - sizeof(blk_hdr));

            // update new footer
            newfooter->size_status = 
            previous_size + header->size_status + tempNext;

            // the header of memory with previous block, with status
            header_of_previous->size_status = header_of_previous->size_status + 
                header->size_status + tempNext;
            header = header_of_previous;
        }
    } else {  
        // if prev not free
        // if next is free
        if ((next_blk->size_status & 1) == 0) {
            // get the size of next block
            int nextSize2 = next_blk->size_status - 2;
            header->size_status += nextSize2;

            // update footer
            footer = 
            (blk_hdr*)((char*)header + header->size_status - sizeof(blk_hdr));

            // update status
            footer->size_status = header->size_status;
            header->size_status += 2;
        } else { 
            // if next is busy succeed
            // change status of next next blk
            if (next_blk->size_status != 0)
                next_blk->size_status -= 2;
            header->size_status += 2;
        }
    }

    // for testing purpose
    // printf("\n\n<<<<<<<<<MemFree(%h)\n", (int)ptr - 4);
    // Mem_Dump();
    // printf(">>>>>>>>>>>>>>>>>>>>\n");
    return 0;
    fflush(stdout);
}

/*
 * Function used to initialize the memory allocator
 * Not intended to be called more than once by a program
 * Argument - sizeOfRegion: Specifies the size of the chunk which needs to be allocated
 * Returns 0 on success and -1 on failure 
 */
int Mem_Init(int sizeOfRegion) {                         
    int pagesize;
    int padsize;
    int fd;
    int alloc_size;
    void* space_ptr;
    blk_hdr* end_mark;
    static int allocated_once = 0;

    if (0 != allocated_once) {
        fprintf(stderr, 
        "Error:mem.c: Mem_Init has allocated space during a previous call\n");
        return -1;
    }
    if (sizeOfRegion <= 0) {
        fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
        return -1;
    }

    // Get the pagesize
    pagesize = getpagesize();

    // Calculate padsize as the padding required to round up sizeOfRegion 
    // to a multiple of pagesize
    padsize = sizeOfRegion % pagesize;
    padsize = (pagesize - padsize) % pagesize;

    alloc_size = sizeOfRegion + padsize;

    // Using mmap to allocate memory
    fd = open("/dev/zero", O_RDWR);
    if (-1 == fd) {
        fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
        return -1;
    }
    space_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, 
        fd, 0);
    if (MAP_FAILED == space_ptr) {
        fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
        allocated_once = 0;
        return -1;
    }

    allocated_once = 1;

    // for double word alignement and end mark 4088
    alloc_size -= 8;

    // To begin with there is only one big free block
    // initialize heap so that first block meets 
    // double word alignement requirement
    first_blk = (blk_hdr*) space_ptr + 1;
    end_mark = (blk_hdr*)((void*)first_blk + alloc_size);

    // Setting up the header
    first_blk->size_status = alloc_size;

    // Marking the previous block as busy
    first_blk->size_status += 2;

    // Setting up the end mark and marking it as busy
    end_mark->size_status = 1;

    // Setting up the footer
    blk_hdr *footer = (blk_hdr*) ((char*)first_blk + alloc_size - 4);
    footer->size_status = alloc_size;

    return 0;
}

/* 
 * Function to be used for debugging 
 * Prints out a list of all the blocks along with the following information i
 * for each block 
 * No.      : serial number of the block 
 * Status   : free/busy 
 * Prev     : status of previous block free/busy
 * t_Begin  : address of the first byte in the block (this is where the header starts)
 * t_End    : address of the last byte in the block
 * t_Size   : size of the block (as stored in the block header) (including the header/footer)
 */
void Mem_Dump() {
    int counter;
    char status[5];
    char p_status[5];
    char *t_begin = NULL;
    char *t_end = NULL;
    int t_size;

    blk_hdr *current = first_blk;
    counter = 1;

    int busy_size = 0;
    int free_size = 0;
    int is_busy = -1;

    fprintf(stdout, "************************************Block list***\
        ********************************\n");
    fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
    fprintf(stdout, "-------------------------------------------------\
        --------------------------------\n");

    while (current->size_status != 1) {
        t_begin = (char*)current;
        t_size = current->size_status;

        if (t_size & 1) {
            // LSB = 1 => busy block
            strcpy(status, "Busy");
            is_busy = 1;
            t_size = t_size - 1;
        } else {
            strcpy(status, "Free");
            is_busy = 0;
        }

        if (t_size & 2) {
            strcpy(p_status, "Busy");
            t_size = t_size - 2;
            } else {
            strcpy(p_status, "Free");
        }

        if (is_busy)
            busy_size += t_size;
        else
            free_size += t_size;

        t_end = t_begin + t_size - 1;

        fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%d\n", counter, status,
            p_status, (unsigned long int)t_begin, 
            (unsigned long int)t_end, t_size);

        current = (blk_hdr*)((char*)current + t_size);
        counter = counter + 1;
    }

    fprintf(stdout, "---------------------------------------------------\
        ------------------------------\n");
    fprintf(stdout, "***************************************************\
        ******************************\n");
    fprintf(stdout, "Total busy size = %d\n", busy_size);
    fprintf(stdout, "Total free size = %d\n", free_size);
    fprintf(stdout, "Total size = %d\n", busy_size + free_size);
    fprintf(stdout, "***************************************************\
        ******************************\n");
    fflush(stdout);

    return;
}
