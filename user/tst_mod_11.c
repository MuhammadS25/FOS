///MAKE SURE PAGE_WS_MAX_SIZE = 10000
#include <inc/lib.h>

extern void shrink(uint32 newSize) ;

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
	int expectedNumOfFrames1 = 0;
	int expectedNumOfFrames2 = 0;
	int expectedNumOfFrames3 = 0;
	uint32 lastAddr = 0;
	uint32 curAddr = 0;

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

		cprintf("1/9\n");

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

		lastAddr = 0;
		for (i = 0; i <= lastIndices[6]; i+=PAGE_SIZE)
		{
			arr[i] = i ;
			curAddr = ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) ;
			if (curAddr != lastAddr)
			{
				if (curAddr >= (uint32)ptr_allocations[6] + 5*Mega)
					expectedNumOfFrames1++ ;
				lastAddr = curAddr;
			}
		}

		cprintf("3/9\n");

	}


	//Shrink last allocated variable to 5 MB instead of 6 MB
	{
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		shrink(5*Mega) ;

		assert(usedDiskPages - sys_pf_calculate_allocated_pages() == 256) ;
		assert(sys_calculate_free_frames() - freeFrames == expectedNumOfFrames1) ;
	}
	cprintf("4/9\n");

	//Access elements after shrink
	int newLastIndex = (5*Mega)/sizeof(uint32) - 1;
	{
		lastAddr = 0;
		for (i = 0; i <= newLastIndex ; i+=PAGE_SIZE)
		{
			assert(arr[i] == i);
			curAddr = ROUNDDOWN((uint32)(&(arr[i])), PAGE_SIZE) ;

			if (curAddr != lastAddr)
			{
				if (curAddr >= (uint32)ptr_allocations[6] + 2*Mega)
					expectedNumOfFrames2++ ;
				else
					expectedNumOfFrames3++ ;
				lastAddr = curAddr;
			}
		}

		cprintf("5/9\n");

		//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
		//and continue executing the remaining code
		sys_bypassPageFault(3);

		ptr = (char *) ptr_allocations[6];
		ptr[5*Mega] = 10;
		assert(sys_rcr2() == (uint32)&(ptr[5*Mega])) ;

		ptr[5*Mega+4*kilo] = 10;
		assert(sys_rcr2() == (uint32)&(ptr[5*Mega+4*kilo])) ;

		ptr[6*Mega - kilo] = 10;
		assert(sys_rcr2() == (uint32)&(ptr[6*Mega - kilo])) ;

		//set it to 0 again to cancel the bypassing option
		sys_bypassPageFault(0);
	}

	cprintf("6/9\n");

	//Shrink it again to 2 MB instead of 5 MB
	{
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		shrink(2*Mega) ;

		assert(usedDiskPages - sys_pf_calculate_allocated_pages() == 768) ;
		assert(sys_calculate_free_frames() - freeFrames == expectedNumOfFrames2 + 1 /*table*/) ;
	}

	cprintf("7/9\n");

	//Allocate after shrinking last var
	{
		//4 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[7] = malloc(4*Mega);
		if ((uint32) ptr_allocations[7] != (USER_HEAP_START + 7*Mega + 16*kilo + 2*Mega)) panic("Wrong start address after shrink()... ");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 4*Mega/PAGE_SIZE ) panic("Extra or less pages are allocated in PageFile");
		if ((freeFrames - sys_calculate_free_frames()) != 0) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
	}

	cprintf("8/9\n");

	//free the shrunk variable
	{
		//kfree 2 MB (shrunk)
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		free(ptr_allocations[6]);
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 2*Mega/PAGE_SIZE ) panic("Extra or less pages are allocated in PageFile");
		if ((sys_calculate_free_frames() - freeFrames) != expectedNumOfFrames3 + 1 /*table*/) panic("Wrong kfree: attempt to kfree a non-existing ptr. It should do nothing");
	}

//	//Shrink last allocated variable to 4 MB - 20 kilo instead of 4 MB
//	{
//		freeFrames = sys_calculate_free_frames() ;
//		usedDiskPages = sys_pf_calculate_allocated_pages() ;
//
//		shrink(4*Mega - 20*kilo) ;
//
//		assert(usedDiskPages  - sys_pf_calculate_allocated_pages() == 5) ;
//		assert(sys_calculate_free_frames() - freeFrames == 0) ;
//	}
	cprintf("9/9\n");

	cprintf("\nCongratulations!! your modification is run successfully.\n");
}
