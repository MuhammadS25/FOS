///MAKE SURE PAGE_WS_MAX_SIZE = 10000
#include <inc/lib.h>

extern void expand(uint32 newSize) ;

void _main(void)
{
	int Mega = 1024*1024;
	int kilo = 1024;


	char minByte = 1<<7;
	char maxByte = 0x7F;
	short minShort = 1<<15 ;
	short maxShort = 0x7FFF;
	int minInt = 1<<31 ;
	int maxInt = 0x7FFFFFFF;

	char *byteArr, *byteArr2 ;
	short *shortArr, *shortArr2 ;
	int *intArr;
	struct MyStruct *structArr ;
	int lastIndexOfByte, lastIndexOfByte2, lastIndexOfShort, lastIndexOfShort2, lastIndexOfInt, lastIndexOfStruct;
	int start_freeFrames = sys_calculate_free_frames() ;

	//malloc some spaces
	int i, freeFrames, usedDiskPages ;
	char* ptr;
	int lastIndices[20] = {0};

	uint32 *arr;
	int expectedNumOfFrames = 0;
	uint32 lastAddr = 0;
	void* ptr_allocations[20] = {0};
	{
		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages();
		ptr_allocations[0] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] !=  (USER_HEAP_START)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[0] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[1] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] != (USER_HEAP_START + 2*Mega)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[1] = (2*Mega-kilo)/sizeof(char) - 1;

		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[2] = malloc(2*kilo);
		if ((uint32) ptr_allocations[2] != (USER_HEAP_START + 4*Mega)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 1 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[2] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[2];
		for (i = 0; i < lastIndices[2]; ++i)
		{
			ptr[i] = 2 ;
		}

		cprintf("1/9\n");
		//2 KB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[3] = malloc(2*kilo);
		if ((uint32) ptr_allocations[3] != (USER_HEAP_START + 4*Mega + 4*kilo)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 1 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[3] = (2*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[3];
		for (i = 0; i < lastIndices[3]; ++i)
		{
			ptr[i] = 3 ;
		}

		//7 KB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[4] = malloc(7*kilo);
		if ((uint32) ptr_allocations[4] != (USER_HEAP_START + 4*Mega + 8*kilo)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 2 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[4] = (7*kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[4];
		for (i = 0; i < lastIndices[4]; ++i)
		{
			ptr[i] = 4 ;
		}

		//3 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[5] = malloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] != (USER_HEAP_START + 4*Mega + 16*kilo) ) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 768 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[5] = (3*Mega-kilo)/sizeof(char) - 1;
		ptr = (char*)ptr_allocations[5];
		for (i = 0; i < lastIndices[5]; i+=PAGE_SIZE)
		{
			ptr[i] = 5 ;
		}
		cprintf("2/9\n");
		//6 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[6] = malloc(6*Mega);
		if ((uint32) ptr_allocations[6] != (USER_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... check return address of malloc & updating of heap ptr");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 1536 ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 2) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		lastIndices[6] = (6*Mega)/sizeof(uint32) - 1;
		arr = (uint32*)ptr_allocations[6];
		uint32 lastAddr = 0 ;
		freeFrames = sys_calculate_free_frames() ;
		lastAddr = 0;
		for (i = 0; i <= lastIndices[6]; i+=PAGE_SIZE)
		{
			arr[i] = i ;
			if (ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) != lastAddr)
			{
				expectedNumOfFrames++ ;
				lastAddr = ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) ;
			}
		}
		cprintf("3/9\n");
	}

	//Expand last allocated variable to 7 MB instead of 6 MB
	int newLastIndex = (7*Mega)/sizeof(uint32) - 1;
	{
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		expand(7*Mega) ;

		assert(sys_pf_calculate_allocated_pages() - usedDiskPages == 256) ;
		assert(freeFrames - sys_calculate_free_frames() == 0) ;

		lastAddr = 0;
		for (i = lastIndices[6]; i < newLastIndex ; i+=PAGE_SIZE)
		{
			arr[i] = i ;
			if (ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) != lastAddr)
			{
				expectedNumOfFrames++ ;
				lastAddr = ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) ;
			}
		}
	}
	cprintf("4/9\n");
	//Access elements after expansion
	{
		for (i = 0; i < lastIndices[6] ; i+=PAGE_SIZE)
		{
			assert(arr[i] ==i);
		}
		for (i = lastIndices[6]; i < newLastIndex ; i+=PAGE_SIZE)
		{
			arr[i] = i ;
		}

	}
	cprintf("5/9\n");
	//Expand it again to 10 MB instead of 7 MB
	int newLastIndex2 = (10*Mega)/sizeof(uint32) - 1;
	{
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		expand(10*Mega) ;

		assert(sys_pf_calculate_allocated_pages() - usedDiskPages == 768) ;
		assert(freeFrames - sys_calculate_free_frames() == 1) ;

		freeFrames = sys_calculate_free_frames() ;
		lastAddr = 0;
		for (i = newLastIndex; i < newLastIndex2 ; i+=PAGE_SIZE)
		{
			arr[i] = i ;
			if (ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) != lastAddr)
			{
				expectedNumOfFrames++ ;
				lastAddr = ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) ;
			}
		}
	}
	cprintf("6/9\n");
	//Access elements after expansion
	{
		for (i = 0; i < lastIndices[6] ; i+=PAGE_SIZE)
		{
			assert(arr[i] ==i);
		}
		for (i = lastIndices[6]; i < newLastIndex ; i+=PAGE_SIZE)
		{
			assert(arr[i] ==i);
		}
		for (i = newLastIndex ; i < newLastIndex2 ; i+=PAGE_SIZE)
		{
			assert(arr[i] ==i);
		}
	}
	cprintf("7/9\n");
	//Allocate after expanding last var
	{
		//4 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[7] = malloc(4*Mega);
		if ((uint32) ptr_allocations[7] != (USER_HEAP_START + 7*Mega + 16*kilo + 10*Mega)) panic("Wrong start address after expand()... ");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 4*Mega/PAGE_SIZE ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 1) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
	}
	cprintf("8/9\n");
	//free the expanded variable
	{
		//kfree 10 MB (expanded)
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		free(ptr_allocations[6]);
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 10*Mega/PAGE_SIZE) panic("Page file is changed while it's not expected to. (pages are wrongly allocated/de-allocated in PageFile)");
		if ((sys_calculate_free_frames() - freeFrames) != expectedNumOfFrames + 3 /*tables*/) panic("Wrong kfree");
	}
	cprintf("9/9\n");
	//	//Expand last allocated variable to 4 MB + 20 kilo instead of 4 MB
	//	{
	//		freeFrames = sys_calculate_free_frames() ;
	//		usedDiskPages = sys_pf_calculate_allocated_pages() ;
	//
	//		expand(4*Mega + 20*kilo) ;
	//
	//		assert(sys_pf_calculate_allocated_pages() - usedDiskPages == 5) ;
	//		assert(freeFrames - sys_calculate_free_frames()  == 0) ;
	//	}

	cprintf("\nCongratulations!! your modification is run successfully.\n");

	return;
}
