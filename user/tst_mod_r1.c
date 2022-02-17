/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 2000 */
/* *********************************************************** */
#include <inc/lib.h>

void _main(void)
{


	int Mega = 1024*1024;
	int kilo = 1024;


	int8 *x ;
	{
		int usedDiskPages = sys_pf_calculate_allocated_pages() ;
		int freeFrames = sys_calculate_free_frames() ;

		//allocate 2 MB in the heap

		x = malloc(2*Mega) ;
		assert((uint32) x == USER_HEAP_START);
		//		cprintf("Allocated frames = %d\n", (freeFrames - sys_calculate_free_frames())) ;
		assert((freeFrames - sys_calculate_free_frames()) == (1 + 1 + 2 * Mega / PAGE_SIZE));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 2 * Mega / PAGE_SIZE);

		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		assert((uint32)malloc(2*Mega) == USER_HEAP_START + 2*Mega) ;
		assert((freeFrames - sys_calculate_free_frames()) == (2 * Mega / PAGE_SIZE));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 2 * Mega / PAGE_SIZE);

		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		assert((uint32)malloc(3*Mega) == USER_HEAP_START + 4*Mega) ;
		assert((freeFrames - sys_calculate_free_frames()) == (1 + 1 + 3 * Mega / PAGE_SIZE));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 3 * Mega / PAGE_SIZE);

		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		assert((uint32)malloc(2*kilo) == USER_HEAP_START + 7*Mega) ;
		assert((freeFrames - sys_calculate_free_frames()) == (1));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1);

		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		assert((uint32)malloc(2*kilo) == USER_HEAP_START + 7*Mega + 4*kilo) ;
		assert((freeFrames - sys_calculate_free_frames()) == (1));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1);

		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		assert((uint32)malloc(7*kilo) == USER_HEAP_START + 7*Mega + 8*kilo) ;
		assert((freeFrames - sys_calculate_free_frames()) == (2));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 2);
	}

	///====================


	int freeFrames = sys_calculate_free_frames() ;
	int usedDiskPages = sys_pf_calculate_allocated_pages() ;
	{
		x[0] = -1 ;
		x[2*Mega] = -1 ;
		x[3*Mega] = -1 ;
		x[4*Mega] = -1 ;
		x[5*Mega] = -1 ;
		x[6*Mega] = -1 ;
		x[7*Mega-1] = -1 ;
		x[7*Mega+1*kilo] = -1 ;
		x[7*Mega+5*kilo] = -1 ;
		x[7*Mega+10*kilo] = -1 ;
	}

	assert(x[0] == -1);
	assert(x[2*Mega] == -1 );
	assert(x[3*Mega] == -1 );
	assert(x[4*Mega] == -1 );
	assert(x[5*Mega] == -1 );
	assert(x[6*Mega] == -1 );
	assert(x[7*Mega-1] == -1 );
	assert(x[7*Mega+1*kilo] == -1 );
	assert(x[7*Mega+5*kilo] == -1 );
	assert(x[7*Mega+10*kilo] == -1 );
	assert((freeFrames - sys_calculate_free_frames()) == 0 );
	assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 0);

	cprintf("Congratulations!! your modification is completed successfully.\n");

	return;
}
