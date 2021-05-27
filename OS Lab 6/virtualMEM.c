#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define INPUT_SIZE 7
#define PAGE_ENTRIES 256
#define PAGE_SIZE 256
#define MEM_SIZE 65536
#define TLB_size 16

int PageTable[PAGE_ENTRIES];
signed char mem[MEM_SIZE];
signed char *backing;

struct TLB_unit
{
	char pageNo;
	unsigned int frameNo;
};

typedef struct
{
	struct TLB_unit TLB_tbl[TLB_size];
	unsigned int size;
	int hits;
} TLB_s;

int main(int argc, char *argv[])
{

	int addr;
	int pageno;
	int offset;
	int frameno;
	int i;
	int iterations;

	TLB_s TLB = {0};

	FILE *fp;					   // file pointer to open addresses file
	char input[INPUT_SIZE];		   // input buffer
	unsigned char page_demand = 0; // counter for counting page demands

	char *backing_file = "BACKING_STORE.bin";							// backing file for page demanding
	int backing_fd = open(backing_file, O_RDONLY);						// opening backing file with read only
	backing = mmap(0, MEM_SIZE, PROT_READ, MAP_PRIVATE, backing_fd, 0); // creating a mapping of backing file as an array
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
			printf("File doesn't exists.");
			return -1;
		}
	}

	while (fgets(input, INPUT_SIZE, fp) != NULL)
	{
	    iterations++;
		addr = atoi(input);			// getting logical address from file
		pageno = (addr >> 8) & 255; // extracting page number from logical address i.e. bit 15 to bit 8
		offset = (addr & 255);		// extracting offset from logical address i.e. bit 7 to bit 0
		for (i = 0; i < TLB.size; i++)
		{
			if (TLB.TLB_tbl[i].pageNo == pageno)
			{
				frameno = TLB.TLB_tbl[i].frameNo;
				TLB.hits++;
				break;
			}
		}
		if (PageTable[pageno] == -1 && i == TLB.size)
		{ // when page is not found in page table
			frameno = page_demand++;

			// Copy page from backing file into physical memory
			memcpy(mem + frameno * PAGE_SIZE, backing + pageno * PAGE_SIZE, PAGE_SIZE);
			PageTable[pageno] = frameno; // saving page in page table
			TLB.TLB_tbl[TLB.size].pageNo = pageno;
			TLB.TLB_tbl[TLB.size++].frameNo = frameno;
			if (TLB.size == 16)
				TLB.size = 0;
		}
		else if (i == TLB.size)
		{ // when page is availabe in page table
			frameno = PageTable[pageno];
			TLB.TLB_tbl[TLB.size].pageNo = pageno;
			TLB.TLB_tbl[TLB.size++].frameNo = frameno;
			if (TLB.size == 16)
				TLB.size = 0;
		}
		int physical_addr = (frameno * 256) + offset; // replacing frame number in place of page no for physical address
		signed char value = mem[physical_addr];		  // getting value stored at physical aaddress
													  // printing addresses and value
		printf("Virtual address: %d Physical address: %d Value: %d\n", addr, physical_addr, value);
	}
	fclose(fp);
	printf("TLB Hit rate: %0.2f\n", (TLB.hits*1.0/iterations)*100);
	printf("Page-fault rate: %0.2f\n", (page_demand*1.0/iterations)*100);

	return -1;
}
