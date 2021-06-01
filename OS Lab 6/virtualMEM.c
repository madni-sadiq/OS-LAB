#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <assert.h>

#define INPUT_SIZE 7
#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define MAX_FRAMES 128
#define MAX_SIZE 65536
#define TLB_size 16

struct TLB_unit
{
    unsigned char pageNo;
    unsigned char frameNo;
};

typedef struct
{
    struct TLB_unit TLB_tbl[TLB_size];
    unsigned char size;
    unsigned int hits;
} TLB_s;

int main(int argc, char *argv[])
{
    int addr;
    int pageno;
    int offset;
    int frameno;
    int i;
    int iterations = 0;
    char printBuf[80];
    signed char value;
    unsigned int physical_addr;
    int page_demand = 0; // counter for counting page demands

    int PageTable[PAGE_ENTRIES];
    signed char mem[MAX_FRAMES * PAGE_SIZE];
    signed char *backing;

    char input[INPUT_SIZE]; // input buffer

    char *backing_file = "BACKING_STORE.bin"; // backing file for page demanding

    TLB_s TLB = {0};
    FILE *fp; // file pointer to open addresses file

    FILE *correct;
    char correct_line[80];

    int backing_fd = open(backing_file, O_RDONLY); // opening backing file with read only

    backing = mmap(0, MAX_SIZE, PROT_READ, MAP_PRIVATE, backing_fd, 0); // creating a mapping of backing file as an array
    close(backing_fd);
    for (i = 0; i < PAGE_ENTRIES; i++)
    {
        // assigning every entry of page table a value of -1
        PageTable[i] = -1;
    }

    if (argc == 2)
    {
        fp = fopen(argv[1], "r"); // opening addresses file in read mode
        if (fp < 0)
        {
            puts("File doesn't exists.");
            return -1;
        }
    }

    if ((correct = fopen("./correct.txt", "r")) == NULL)
    {
        puts("Can't open correct.txt");
        exit(1);
    }

    while (fgets(input, INPUT_SIZE, fp) != NULL)
    {
        fgets(correct_line, 80, correct);
        iterations++;
        addr = atoi(input);         // getting logical address from file
        pageno = (addr >> 8) & 255; // extracting page number from logical address i.e. bit 15 to bit 8
        offset = (addr & 255);      // extracting offset from logical address i.e. bit 7 to bit 0
        for (i = 0; i < TLB.size; i++)
        {
            if (TLB.TLB_tbl[i].pageNo == pageno)
            {
                frameno = TLB.TLB_tbl[i].frameNo;
                TLB.hits++;
                goto mem_hit;
            }
        }
        if (PageTable[pageno] == -1)
        { // when page is not found in page table
            frameno = page_demand++;

            // Fault paging
            if (frameno >= MAX_FRAMES)
            {
                frameno %= MAX_FRAMES;
                // Find the previous page table entry for this frame no and reset it
                for (i = 0; i < PAGE_ENTRIES; i++)
                {
                    if (PageTable[i] == frameno)
                        PageTable[i] = -1;
                }
            }

            // Copy page from backing file into physical memory
            memcpy(mem + frameno * PAGE_SIZE, backing + pageno * PAGE_SIZE, PAGE_SIZE);
            PageTable[pageno] = frameno; // saving page in page table
            TLB.TLB_tbl[TLB.size].pageNo = pageno;
            TLB.TLB_tbl[TLB.size++].frameNo = frameno;
            if (TLB.size == 16)
                TLB.size = 0;
        }
        else
        { // when page is availabe in page table
            frameno = PageTable[pageno];
            TLB.TLB_tbl[TLB.size].pageNo = pageno;
            TLB.TLB_tbl[TLB.size++].frameNo = frameno;
            if (TLB.size == 16)
                TLB.size = 0;
        }
    mem_hit:
        physical_addr = (frameno * PAGE_SIZE) + offset; // replacing frame number in place of page no for physical address
        value = mem[physical_addr];                     // getting value stored at physical aaddress
                                                        // printing addresses and value
        snprintf(printBuf, sizeof(printBuf), "Virtual address: %d Physical address: %d Value: %d\n", addr, physical_addr, value);
        printf("%s", printBuf);
        printf("%s", correct_line);
        // assert(strcmp(printBuf, correct_line) == 0);
    }
    fclose(fp);
    fclose(correct);
    printf("Page demand : %d \nTotal Hits: %d \nTotal Iterations: %d \n", page_demand, TLB.hits, iterations);
    printf("TLB Hit rate: %.2f\n", (TLB.hits / (1.0 * iterations)) * 100);
    printf("Page-fault rate: %.2f\n", (page_demand / (1.0 * iterations)) * 100);
    return 0;
}
