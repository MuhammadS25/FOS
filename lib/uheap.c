#include <inc/lib.h>

// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

struct pages
{
	uint32 Address;
	uint32 Size;
	int map;
}pages[(USER_HEAP_MAX-USER_HEAP_START)/PAGE_SIZE];
uint32 startAddress = USER_HEAP_START;

bool canAllocateAtTop(uint32 size)
{
	return USER_HEAP_MAX - startAddress >= size;
}
void gettingBestFrag(uint32* Address,uint32 size,uint32* bestSize)
{
	uint32 currntSize = 0;
	for(uint32 i = 0; i<(startAddress - USER_HEAP_START)/PAGE_SIZE; i++)
	{
		if(pages[i].map == 0)
		{
			currntSize += PAGE_SIZE;
		}
		else
		{
			if(currntSize  >= size)
			{
				if((uint32)bestSize>currntSize)
				{
					*Address = USER_HEAP_START + (i*PAGE_SIZE) - currntSize;
					*bestSize = currntSize;
				}
			}
			currntSize = 0;
			if(i != (startAddress - USER_HEAP_START)/PAGE_SIZE-1)
				i+= pages[i].Size / PAGE_SIZE -1;
		}
	}
}

void allocateAtPaggy(uint32 Address,uint32 size)
{
	for(int i = 0;i<size/PAGE_SIZE;i++)
		pages[(Address - USER_HEAP_START)/PAGE_SIZE + i].map = 1;

	pages[((Address - USER_HEAP_START)/PAGE_SIZE)].Address = Address;
	pages[(Address - USER_HEAP_START)/PAGE_SIZE].Size = size;
}
void* malloc(uint32 size)
{
	//TODO: [PROJECT 2021 - [2] User Heap] malloc() [User Side]


	// Getting Ready
	uint32 Address = 0, bestSize=(USER_HEAP_MAX-USER_HEAP_START+PAGE_SIZE),currntSize =0,top = USER_HEAP_MAX - startAddress;
	size = ROUNDUP(size,PAGE_SIZE);

	// Searching in fragmentation
	gettingBestFrag(&Address,size,&bestSize);


	// Next Fit or fragmentation
	if(canAllocateAtTop(size))
	{
		if(top<bestSize)
		{
			Address = startAddress;
			startAddress+=size;
		}
	}

	// Update my paggy
	if(Address == 0) return NULL;
	else
	{
		sys_allocateMem(Address,size/PAGE_SIZE);
		allocateAtPaggy(Address,size);

	}

	return (void*)Address;
}


// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void optimize(int index)
{
	if( pages[index].Size == startAddress - pages[index].Address)
	{
		int decer = index;
		while(pages[decer].map == 0 && startAddress != USER_HEAP_START)
		{
			startAddress -= PAGE_SIZE;
			decer--;
		}

	}
}
//Free
void freeFromPaggy(int index)
{
	//Save the Pages cuz we gonna change it
	int z = pages[index].Size/PAGE_SIZE;

	// Update my paggy
	for(int i = 0; i < z; i++)
		pages[index+i].map = 0;

	/* OPTIMIZATION */
	optimize(index);
	/* End of Optimization */

	pages[index].Address = 0;
	pages[index].Size = 0;

}

void free(void* virtual_address)
{
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]

	for(int i = 0;i<(startAddress - USER_HEAP_START)/PAGE_SIZE;i++)
	{
		if(pages[i].Address ==(uint32) virtual_address)
		{
			sys_freeMem((uint32)virtual_address,pages[i].Size/PAGE_SIZE);
			freeFromPaggy(i);
			return;
		}
	}

}

//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address)
{
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size)
{
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize)
{
	panic("this function is not required...!!");
}
void shrink(uint32 newSize)
{
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address)
{
	panic("this function is not required...!!");
}
