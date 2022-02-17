/* *********************************************************** */
/* MAKE SURE PAGE_WS_MAX_SIZE = 9 */
/* *********************************************************** */

#include <inc/lib.h>
extern void sys_clear_ffl() ;

void _main(void)
{
	
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
		if( ROUNDDOWN(myEnv->__uptr_pws[8].virtual_address,PAGE_SIZE) !=   0xeebfd000)  panic("InitialWSError2: INITIAL PAGE WS entry checking failed! Review size of the WS..!!");
		if( myEnv->page_last_WS_index !=  0)  										panic("InitialWSError2: INITIAL PAGE WS last index checking failed! Review size of the WS..!!");
	}

	//Reading (Not Modified)
	char garbage1 = *((char*)(0x200000)) ;
	char garbage2 = *((char*)(0x202000)) ;
	char garbage3 = *((char*)(0x204000)) ;

	//Writing (Modified)
	char *a = ((char*)(0x201000)) ;
	*a = 'A';
	char *b = ((char*)(0x203000)) ;
	*b = 'B';
	char *c = ((char*)(0x205000)) ;
	*c = 'C';

	//Clear the FFL
	sys_clear_ffl();

	//Writing (Modified) (3 frames should be allocated (stack page, mem table, page file table)
	*ptr3 = 255 ;

	//Check the WS and values
	int i;
	for (i = 0 ; i < myEnv->page_WS_max_size; i++)
	{
		if (ROUNDDOWN(myEnv->__uptr_pws[0].virtual_address,PAGE_SIZE) ==  0x200000 ||
			ROUNDDOWN(myEnv->__uptr_pws[2].virtual_address,PAGE_SIZE) ==  0x202000 ||
			ROUNDDOWN(myEnv->__uptr_pws[4].virtual_address,PAGE_SIZE) ==  0x204000)
			panic("test failed! either wrong victim or victim is not removed from WS");

	}
	if (*a != 'A') panic("test failed!");
	if (*b != 'B') panic("test failed!");
	if (*c != 'C') panic("test failed!");
	if (*ptr3 != 255) panic("test failed!");

	cprintf("Congratulations!! test modification is completed successfully.\n");
	return;
}
