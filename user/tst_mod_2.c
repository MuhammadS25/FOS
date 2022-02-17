/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 2000 */
/* *********************************************************** */
#include <inc/lib.h>

int CheckWSEntries(volatile struct Env* e, uint32 startVA, uint32 endVA) ;
void _main(void)
{
	int envID = sys_getenvid();
	cprintf("envID = %d\n",envID);

	int Mega = 1024*1024;
	int kilo = 1024;

	int numOfFreeWSEntries = 0;
	//("STEP 0: checking Initial WS free size...\n");
	{
		int i;
		for (i = 0; i < myEnv->page_WS_max_size; ++i)
		{
			if( myEnv->__uptr_pws[i].empty == 1)
				numOfFreeWSEntries++ ;
		}

		assert(numOfFreeWSEntries == (myEnv->page_WS_max_size - myEnv->page_last_WS_index));
	}


	{
		//allocate Half WS free size in the heap [Should alloc in RAM and Page File]
		int usedDiskPages = sys_pf_calculate_allocated_pages() ;
		int freeFrames = sys_calculate_free_frames() ;

		int numOfReqPages1 = numOfFreeWSEntries/2 ;
		int numOfReqTables1 = ROUNDUP(numOfReqPages1, 1024) / 1024;
		int size1 = numOfReqPages1 * PAGE_SIZE;
		int8 *x1 = malloc(size1) ;
		assert((uint32) x1 == USER_HEAP_START);
		assert((freeFrames - sys_calculate_free_frames()) == (2*numOfReqTables1 + numOfReqPages1));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == numOfReqPages1);
		assert(CheckWSEntries(myEnv, (uint32)x1, (uint32)x1+size1) == numOfReqPages1);

		//allocate original WS free size in the heap [should not be all. in RAM]
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		freeFrames = sys_calculate_free_frames() ;
		int numOfReqPages2 = numOfFreeWSEntries ;
		int numOfReqTables2 = (ROUNDUP(numOfReqPages1+numOfReqPages2, 1024) - ROUNDUP(numOfReqPages1, 1024)) / 1024;
		int size2 = numOfReqPages2 * PAGE_SIZE;
		int8 *x2 = malloc(size2);
		assert((uint32) x2 == USER_HEAP_START + size1);
		assert((freeFrames - sys_calculate_free_frames()) == (1*numOfReqTables2));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == numOfReqPages2);
		assert(CheckWSEntries(myEnv, (uint32)x2, (uint32)x2+size2) == 0);

		//allocate Half the remaining WS free size in the heap [Should alloc in RAM and Page File]
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		freeFrames = sys_calculate_free_frames() ;
		int numOfReqPages3 = (numOfFreeWSEntries - numOfReqPages1) /2 ;
		int numOfReqTables3 = (ROUNDUP(numOfReqPages1+numOfReqPages2+numOfReqPages3, 1024) - ROUNDUP(numOfReqPages1+numOfReqPages2, 1024)) / 1024;
		int extraTable = 0;
		if (numOfReqTables2 > 0)
			extraTable = 1; //as numOfReqTables2 is not allocated in RAM for the prev. alloc.
		int size3 = numOfReqPages3 * PAGE_SIZE;
		int8 *x3 = malloc(size3);
		assert((uint32) x3 == USER_HEAP_START + size1 + size2);
//		cprintf("startVA = %x, size = %x, numOfReqTables = %d, numOfReqPages = %d\n", x3, size3, numOfReqTables3,numOfReqPages3);
//		cprintf("freeFrames - sys_calculate_free_frames() = %d\n", freeFrames - sys_calculate_free_frames());
		assert((freeFrames - sys_calculate_free_frames()) == (extraTable + 2*numOfReqTables3 + numOfReqPages3));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == numOfReqPages3);
		assert(CheckWSEntries(myEnv, (uint32)x3, (uint32)x3+size3) == numOfReqPages3);

		//allocate half original WS free size in the heap [should not be all. in RAM]
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		freeFrames = sys_calculate_free_frames() ;
		int numOfReqPages4 = numOfFreeWSEntries/2 ;
		int numOfReqTables4 = (ROUNDUP(numOfReqPages1+numOfReqPages2+numOfReqPages3+numOfReqPages4, 1024) - ROUNDUP(numOfReqPages1+numOfReqPages2+numOfReqPages3, 1024)) / 1024;
		int size4 = numOfReqPages4 * PAGE_SIZE;
		int8 *x4 = malloc(size4);
		assert((uint32) x4 == USER_HEAP_START + size1 + size2 + size3);
		assert((freeFrames - sys_calculate_free_frames()) == (1*numOfReqTables4));
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == numOfReqPages4);
		assert(CheckWSEntries(myEnv, (uint32)x4, (uint32)x4+size4) == 0);

		//Access already allocated in RAM and Page File
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		{
			x1[0] = -1 ;
			x3[0] = -1 ;
		}
		assert(x1[0] == -1);
		assert(x3[0] == -1);
		assert((freeFrames - sys_calculate_free_frames()) == 0 );
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 0);

		//Access allocated in Page File ONLY
		freeFrames = sys_calculate_free_frames() ;
		usedDiskPages = sys_pf_calculate_allocated_pages() ;
		{
			x2[0] = -1 ;
			x4[0] = -1 ;
		}
		assert(x2[0] == -1);
		assert(x4[0] == -1);
		
		assert((freeFrames - sys_calculate_free_frames()) >= 2);
		assert((sys_pf_calculate_allocated_pages() - usedDiskPages) == 0);

	}
	cprintf("Congratulations!! your modification is completed successfully.\n");

	return;
}



int CheckWSEntries(volatile struct Env* e, uint32 startVA, uint32 endVA)
{
	uint32 va ;
	startVA = ROUNDDOWN(startVA, PAGE_SIZE);
	endVA = ROUNDUP(endVA, PAGE_SIZE);
	int totalNumOfFound = 0 ;
	for (va = startVA; va < endVA; va+=PAGE_SIZE)
	{
		int i;
		for (i = 0; i < e->page_WS_max_size; ++i)
		{
			if( ROUNDDOWN(e->__uptr_pws[i].virtual_address,PAGE_SIZE) == va)
			{
				totalNumOfFound++ ;
				break;
			}
		}
	}
	return totalNumOfFound;
}
