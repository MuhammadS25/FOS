/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 11 */
/* *********************************************************** */

#include <inc/lib.h>
extern void sys_clear_ffl() ;

void _main(void)
{
	int envID = sys_getenvid();
	//	cprintf("envID = %d\n",envID);

	uint8* ptr = (uint8* )0x0801000 ;
	uint8* ptr2 = (uint8* )0x080400A ;
	uint8* ptr3 = (uint8* )(USTACKTOP - PTSIZE) ;

	//("STEP 0: checking InitialWSError2: INITIAL WS entries ...\n");
	{
		if( ROUNDDOWN(myEnv->__uptr_pws[0].virtual_address,PAGE_SIZE) !=   0x200000)  	panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[1].virtual_address,PAGE_SIZE) !=   0x201000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[2].virtual_address,PAGE_SIZE) !=   0x202000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[3].virtual_address,PAGE_SIZE) !=   0x203000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[4].virtual_address,PAGE_SIZE) !=   0x204000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[5].virtual_address,PAGE_SIZE) !=   0x205000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[6].virtual_address,PAGE_SIZE) !=   0x800000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[7].virtual_address,PAGE_SIZE) !=   0x801000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[8].virtual_address,PAGE_SIZE) !=   0x802000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[9].virtual_address,PAGE_SIZE) !=   0x803000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( ROUNDDOWN(myEnv->__uptr_pws[10].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("InitialWSError2: INITIAL PAGE WS last index checking failed! Review size of the WS..!!");
	}

	//Reading (Not Modified)
	char garbage1 = *((char*)(0x200000)) ;
	char garbage2 = *((char*)(0x201000)) ;

	//Writing (Modified)
	char *c = ((char*)(0x202000)) ;
	*c = 'A';

	//Clear the FFL
	sys_clear_ffl();

	//Writing (Modified) (3 frames should be allocated (stack page, mem table, page file table)
	*ptr3 = 255 ;

	//Check the written values
	if (*c != 'A') panic("test failed!");
	if (*ptr3 != 255) panic("test failed!");

	//Check the WS and values
	int i;
	for (i = 0 ; i < myEnv->page_WS_max_size; i++)
	{
		//There should be two empty locations that are freed for the two tables (mem table and page file table)
		int numOfEmptyLocs = 0 ;
		for (int i = 0 ; i < myEnv->page_WS_max_size; i++)
		{
			if (myEnv->__uptr_pws[i].empty)
				numOfEmptyLocs++ ;
		}
		if (numOfEmptyLocs != 2) panic("test failed! either wrong victim or victim is not removed from WS");

		uint32 expectedAddresses[9] = {0x800000,0x801000,0x802000,0x803000,0x202000,0x204000,0x205000,0xee7fe000,0xeebfd000} ;
		int numOfFoundedAddresses = 0;
		for (int j = 0; j < 9; j++)
		{
			for (int i = 0 ; i < myEnv->page_WS_max_size; i++)
			{
				if (ROUNDDOWN(myEnv->__uptr_pws[i].virtual_address,PAGE_SIZE) == expectedAddresses[j])
				{
					numOfFoundedAddresses++;
					break;
				}
			}
		}
		if (numOfFoundedAddresses != 9) panic("test failed! either wrong victim or victim is not removed from WS");

	}

	cprintf("Congratulations!! test modification is completed successfully.\n");
	return;
}
