/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 1000 */
/* *********************************************************** */
//test allocation of small sizes with large sizes
#include <inc/lib.h>

void _main(void)
{
	//Initial test to ensure it works on "PLACEMENT" not "REPLACEMENT"
	{
		uint8 fullWS = 1;//pointer to page working set array
		for (int i = 0; i < myEnv->page_WS_max_size; ++i)
		{
			if (myEnv->__uptr_pws[i].empty)
			{
				fullWS = 0;
				break;
			}
		}
		if (fullWS) panic("Please increase the WS size");
	}

	int Mega = 1024*1024;
	int kilo = 1024;

	void* ptr_allocations[20] = {0};
	{
		//2 Mega
		int freeFrames = sys_calculate_free_frames() ;
		int usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[0] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[0] <  (USER_HEAP_START) || (uint32) ptr_allocations[0] > (USER_HEAP_START + PAGE_SIZE))
		{
			panic("Wrong start address for the allocated space... ");
		}
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//2 Mega
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[1] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[1] < (USER_HEAP_START + 2*Mega) || (uint32) ptr_allocations[1] > (USER_HEAP_START + 2*Mega + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//1 KB
		//round down the addresses to the nearest page to allow for right node or left node allocation in buddy system
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[2] = malloc(1*kilo);
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 1) panic("Extra or less pages are allocated in PageFile");
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		//1 KB
		ptr_allocations[3] = malloc(1*kilo);
		//swap the two addresses if the left node address is not in allocation 2
		if(ptr_allocations[2]>ptr_allocations[3])
		{
			uint32* temp =ptr_allocations[3];
			ptr_allocations[3]=ptr_allocations[2];
			ptr_allocations[2]=temp;
		}
		uint32 start_of_page = ROUNDDOWN((uint32)ptr_allocations[2], PAGE_SIZE);
		//test the address of the first 1 kilo
		if ((uint32)ptr_allocations[2] < (USER_HEAP_START + 4*Mega) || (uint32) ptr_allocations[2] > (USER_HEAP_START + 4*Mega + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		//test the address of the second 1 kilo
		if ((uint32) ptr_allocations[3] < (USER_HEAP_START + 4*Mega+ 1*kilo) || (uint32) ptr_allocations[3] > (USER_HEAP_START + 4*Mega + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");

		//2 Bytes
		ptr_allocations[2] = malloc(2);
		if ((uint32) ptr_allocations[2] < (USER_HEAP_START + 4*Mega) || (uint32) ptr_allocations[2] > (USER_HEAP_START + 4*Mega + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");

		//1023 Bytes
		ptr_allocations[3] = malloc(1023);
		if ((uint32) ptr_allocations[3] < (USER_HEAP_START + 4*Mega) || (uint32) ptr_allocations[3] > (USER_HEAP_START + 4*Mega + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 0) panic("Extra or less pages are allocated in PageFile");

		//NEW PAGE => 2000 Bytes
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[2] = malloc(2000);
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile %d", (freeFrames - sys_calculate_free_frames()));
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 1) panic("Extra or less pages are allocated in PageFile");

		//2048 Bytes
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[3] = malloc(2048);
		//swap the two addresses if the left node address is not in allocation 2
		if(ptr_allocations[2]>ptr_allocations[3])
		{
			uint32* temp =ptr_allocations[3];
			ptr_allocations[3]=ptr_allocations[2];
			ptr_allocations[2]=temp;
		}
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 0) panic("Extra or less pages are allocated in PageFile");
		if ((uint32) ptr_allocations[2] < (USER_HEAP_START + 4*Mega+ PAGE_SIZE) || (uint32) ptr_allocations[2] > (USER_HEAP_START + 4*Mega + PAGE_SIZE + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		if ((uint32) ptr_allocations[3] < (USER_HEAP_START + 4*Mega+ PAGE_SIZE + 2*kilo) || (uint32) ptr_allocations[3] > (USER_HEAP_START + 4*Mega + PAGE_SIZE + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");

		//7 Kilo
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[4] = malloc(7*kilo);
		if ((uint32) ptr_allocations[4] < (USER_HEAP_START + 4*Mega + 2*PAGE_SIZE) || (uint32) ptr_allocations[4] > (USER_HEAP_START + 4*Mega + 2*PAGE_SIZE + 2*PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 2) panic("Extra or less pages are allocated in PageFile");

		//3 Mega
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[5] = malloc(3*Mega-kilo);
		if ((uint32) ptr_allocations[5] < (USER_HEAP_START + 4*Mega + 16*kilo) || (uint32) ptr_allocations[5] > (USER_HEAP_START + 7*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 3*Mega/4096) panic("Extra or less pages are allocated in PageFile");

		//2 Mega
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[6] = malloc(2*Mega-kilo);
		if ((uint32) ptr_allocations[6] < (USER_HEAP_START + 7*Mega + 16*kilo) || (uint32) ptr_allocations[6] > (USER_HEAP_START + 9*Mega + 16*kilo)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 1 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 512) panic("Extra or less pages are allocated in PageFile");

		//257 Bytes
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		ptr_allocations[7] = malloc(510);
		if ((uint32) ptr_allocations[7] < (start_of_page) || (uint32) ptr_allocations[7] > (start_of_page + PAGE_SIZE)) panic("Wrong start address for the allocated space... ");
		if ((freeFrames - sys_calculate_free_frames()) != 0 ) panic("Wrong allocation: either extra pages are allocated in memory or pages not allocated correctly on PageFile");
		if ((sys_pf_calculate_allocated_pages() - usedDiskPages) != 0) panic("Extra or less pages are allocated in PageFile");
	}
	cprintf("Congratulations!! test malloc (4) completed successfully.\n");

	return;
}
