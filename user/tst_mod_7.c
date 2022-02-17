/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 5000 */
/* *********************************************************** */

#include <inc/lib.h>
extern void freeHeap();

void _main(void)
{
	int envID = sys_getenvid();
	//	cprintf("envID = %d\n",envID);

	int kilo = 1024;
	int Mega = 1024*1024;
	int freeFrames, origFreeFrames, usedDiskPages, origDiskPages;
	uint32 size ;
	/// testing freeHeap()
	{
		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		origFreeFrames = freeFrames ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		origDiskPages = usedDiskPages ;

		size = 1*Mega;
		unsigned char *x = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) x == USER_HEAP_START);
		assert((freeFrames - sys_calculate_free_frames()) == 1);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1*Mega/PAGE_SIZE);

		//Allocate 1 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		size = 1*Mega;
		unsigned char *t1 = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) t1 == USER_HEAP_START + 1*Mega);
		assert((freeFrames - sys_calculate_free_frames()) == 0);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1*Mega/PAGE_SIZE);

		//Allocate 2 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		size = 2*Mega;
		unsigned char *t2 = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) t2 == USER_HEAP_START + 2*Mega);
		assert((freeFrames - sys_calculate_free_frames()) == 0);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 2*Mega/PAGE_SIZE);

		//Allocate 4 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		size = 4*Mega;
		unsigned char *t3 = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) t3 == USER_HEAP_START + 4*Mega);
		assert((freeFrames - sys_calculate_free_frames()) == (1));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 4*Mega/PAGE_SIZE);

		//Allocate 4 MB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		size = 4*Mega;
		unsigned char *t4 = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) t4 == USER_HEAP_START + 8*Mega);
		assert((freeFrames - sys_calculate_free_frames()) == (1));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 4*Mega/PAGE_SIZE);

		//Allocate 2 KB
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		size = 2*kilo;
		unsigned char *y = malloc(sizeof(unsigned char)*size) ;

		assert((uint32) y == USER_HEAP_START + 12*Mega);
		assert((freeFrames - sys_calculate_free_frames()) == 1);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1);

		//Memory access
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;

		x[1]='A';
		x[512*kilo]='B';
		x[1*Mega] = 'C' ;
		x[8*Mega] = 'D';
		y[0] = 'E';

		assert(x[1]='A');
		assert(x[512*kilo]='B');
		assert(x[1*Mega] == 'C' );
		assert(x[8*Mega] == 'D');
		assert(y[0] == 'E');

		assert((freeFrames - sys_calculate_free_frames()) == 3 + 5);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 0);

		//Free 2nd 1 MB
		int freeFrames = sys_calculate_free_frames() ;
		int usedDiskPages = sys_pf_calculate_allocated_pages() ;
		free(t1);
		if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 256) panic("Wrong free: Extra or less pages are removed from PageFile");
		if ((sys_calculate_free_frames() - freeFrames) != 1 ) panic("Wrong free: WS pages in memory and/or page tables are not freed correctly");
		int var;
		for (var = 0; var < (myEnv->page_WS_max_size); ++var)
		{
			if(ROUNDDOWN(myEnv->__uptr_pws[var].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(x[1*Mega])), PAGE_SIZE))
				panic("free: page is not removed from WS");
		}


		//Free the entire Heap

		{
			freeHeap();

			//cprintf("diff = %d\n", origFreeFrames - sys_calculate_free_frames());

			assert((origFreeFrames - sys_calculate_free_frames()) == 4);
			assert((sys_pf_calculate_allocated_pages() - origDiskPages) == 0);
		}

		//Check memory access after kfreeall
		{
			//Bypass the PAGE FAULT on <MOVB immediate, reg> instruction by setting its length
			//and continue executing the remaining code
			sys_bypassPageFault(3);

			x[1]=-1;
			assert(sys_rcr2() == (uint32)&(x[1]));

			x[8*Mega] = -1;
			assert(sys_rcr2() == (uint32)&(x[8*Mega]));

			x[512*kilo]=-1;
			assert(sys_rcr2() == (uint32)&(x[512*kilo]));

			y[0] = -1;
			assert(sys_rcr2() == (uint32)&(y[0]));

			//set it to 0 again to cancel the bypassing option
			sys_bypassPageFault(0);
		}

		//Checking if freeHeap RESET the HEAP POINTER or not
		{

			//1 KB
			freeFrames = sys_calculate_free_frames() ;
			usedDiskPages = sys_pf_calculate_allocated_pages() ;

			unsigned char *w = malloc(sizeof(unsigned char)*kilo) ;

			assert((uint32)w == USER_HEAP_START);
			assert((freeFrames - sys_calculate_free_frames()) == 0);
			assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1);

			//1 B
			freeFrames = sys_calculate_free_frames() ;
			usedDiskPages = sys_pf_calculate_allocated_pages() ;

			unsigned char *f = malloc(sizeof(unsigned char)*1) ;

			assert((uint32)f == USER_HEAP_START + PAGE_SIZE);
			assert((freeFrames - sys_calculate_free_frames()) == 0);
			assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 1);

			f[0] = -1;

			//1 MB
			freeFrames = sys_calculate_free_frames() ;
			usedDiskPages = sys_pf_calculate_allocated_pages() ;

			unsigned char *z = malloc(sizeof(unsigned char)*Mega) ;

			assert((uint32)z == USER_HEAP_START + 2*PAGE_SIZE);
			assert((freeFrames - sys_calculate_free_frames()) == 0);
			assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == Mega/PAGE_SIZE);

			//Free 1 KB
			int freeFrames = sys_calculate_free_frames() ;
			int usedDiskPages = sys_pf_calculate_allocated_pages() ;
			free(w);
			if ((usedDiskPages - sys_pf_calculate_allocated_pages()) != 1) panic("Wrong free: Extra or less pages are removed from PageFile");
			if ((sys_calculate_free_frames() - freeFrames) != 0 ) panic("Wrong free: WS pages in memory and/or page tables are not freed correctly");
			int var;
			int found = 0;
			for (var = 0; var < (myEnv->page_WS_max_size); ++var)
			{
				if(ROUNDDOWN(myEnv->__uptr_pws[var].virtual_address,PAGE_SIZE) == ROUNDDOWN((uint32)(&(f[0])), PAGE_SIZE))
					found = 1;
			}

			if (!found)
				panic("free: variables are not removed correctly");

		}



		cprintf("Congratulations!! your modification is completed successfully.\n");

	}

	return;
}

