///MAKE SURE PAGE_WS_MAX_SIZE = 4000
#include <inc/lib.h>

void _main(void)
{
	int envID = sys_getenvid();
	cprintf("envID = %d\n",envID);

	int Mega = 1024*1024;
	int kilo = 1024;

	//[0] Make sure there're available places in the WS
	int w = 0 ;
	int requiredNumOfEmptyWSLocs = 5;
	int numOfEmptyWSLocs = 0;
	for (w = 0 ; w < myEnv->page_WS_max_size ; w++)
	{
		if( myEnv->__uptr_pws[w].empty == 1)
			numOfEmptyWSLocs++;
	}
	if ((numOfEmptyWSLocs == 0))
			panic("InitialWSError1: No empty locations in WS! please increase its size to 8000 and try again!!!");
	else if (numOfEmptyWSLocs != requiredNumOfEmptyWSLocs)
		panic("InitialWSError1: Wrong number of WS empty locations! please set its size to %d OR ASK your TA!!!", myEnv->page_WS_max_size + requiredNumOfEmptyWSLocs - numOfEmptyWSLocs);


	int8 *x = (int8 *)0x80000000;
	int usedDiskPages = sys_pf_calculate_allocated_pages() ;
	{
		//allocate 9 kilo in the heap
		int freeFrames = sys_calculate_free_frames() ;
		//sys_allocateMem(0x80000000,9*kilo) ;
		assert((uint32)malloc(9*kilo) == 0x80000000);
		assert((freeFrames - sys_calculate_free_frames()) == (1+ 1 + ROUNDUP(9 * kilo, PAGE_SIZE) / PAGE_SIZE));

		//allocate 2 MB in the heap
		freeFrames = sys_calculate_free_frames() ;
		//sys_allocateMem(0x80003000,2*Mega) ;
		assert((uint32)malloc(2*Mega) == 0x80003000);
		assert((freeFrames - sys_calculate_free_frames()) == 2);

		freeFrames = sys_calculate_free_frames() ;
		//sys_allocateMem(0x80203000,3*Mega) ;
		assert((uint32)malloc(3*Mega) == 0x80203000);
		assert((freeFrames - sys_calculate_free_frames()) == 1);

	}
	//cprintf("Allocated Disk pages = %d\n",sys_pf_calculate_allocated_pages() - usedDiskPages) ;
	assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 768+512+3);

	///====================


	int freeFrames = sys_calculate_free_frames() ;
	{
		x[0] = -1 ;
		x[4*kilo] = -1 ;
		x[8*kilo] = -1 ;
		x[12*kilo] = -1 ;
		x[16*kilo] = -1 ;
	}

	assert(x[0] == -1 );
	assert(x[4*kilo] == -1 );
	assert(x[8*kilo] == -1 );
	assert(x[12*kilo] == -1 );
	assert(x[16*kilo] == -1 );

	assert((freeFrames - sys_calculate_free_frames()) == 0 );

	int found = 0;
	for (w = 0 ; w < myEnv->page_WS_max_size; w++)
	{
		if( myEnv->__uptr_pws[w].empty == 1)
			panic("Allocated pages are not placed correctly in the WS");

		if(ROUNDDOWN(myEnv->__uptr_pws[w].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[0])), PAGE_SIZE)) found++;
		if(ROUNDDOWN(myEnv->__uptr_pws[w].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[4*kilo])), PAGE_SIZE)) found++;
		if(ROUNDDOWN(myEnv->__uptr_pws[w].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[8*kilo])), PAGE_SIZE)) found++;
		if(ROUNDDOWN(myEnv->__uptr_pws[w].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[12*kilo])), PAGE_SIZE)) found++;
		if(ROUNDDOWN(myEnv->__uptr_pws[w].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[16*kilo])), PAGE_SIZE)) found++;
	}

	if (found != 5) panic("malloc: page is not added correctly to WS");

	cprintf("Congratulations!! your modification is completed successfully.\n");

	return;
}
