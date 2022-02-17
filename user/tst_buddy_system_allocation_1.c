#include <inc/lib.h>

int GetPowOf2(int size);

void
_main(void)
{
	int freeFrames1 = sys_calculate_free_frames() ;
	int usedDiskPages1 = sys_pf_calculate_allocated_pages() ;

	char line[100];
	int N = 500;
	assert(N * sizeof(int) <= BUDDY_LIMIT);
	int M = 1;
	assert(M * sizeof(uint8) <= BUDDY_LIMIT);

	uint8 ** arr = malloc(N * sizeof(int)) ;
	int expectedNumOfAllocatedFrames = GetPowOf2(N * sizeof(int));

	for (int i = 0; i < N; ++i)
	{
		arr[i] = malloc(M + i) ;
		expectedNumOfAllocatedFrames += GetPowOf2(M + i);
		for (int j = 0; j < M; ++j)
		{
			arr[i][j] = i % 255;
		}
	}

	//[1] Check the lists content of the BuddyLevels array
	{
	int L = BUDDY_LOWER_LEVEL;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{cprintf("WRONG number of nodes at Level # %d - # of nodes = %d\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	}

	//[2] Check the frames taken after the allocation
	expectedNumOfAllocatedFrames = ROUNDUP(expectedNumOfAllocatedFrames, PAGE_SIZE) / PAGE_SIZE;
	int freeFrames2 = sys_calculate_free_frames() ;
	int usedDiskPages2 = sys_pf_calculate_allocated_pages() ;
	assert(freeFrames1 - freeFrames2 == 1 + 1 + expectedNumOfAllocatedFrames); //2 for page table + 1 for disk table
	assert(usedDiskPages2 - usedDiskPages1 == expectedNumOfAllocatedFrames);

	//[3] Check the created arrays content
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			assert(arr[i][j] == i % 255);
		}
	}

	cprintf("Congratulations!! test BUDDY SYSTEM allocation (1) completed successfully. Evaluation = 100%\n");
}

int GetPowOf2(int size)
{
	int i;
	for(i = BUDDY_LOWER_LEVEL; i <= BUDDY_UPPER_LEVEL; i++)
	{
		if(BUDDY_NODE_SIZE(i) >= size)
			return 1<<i;
	}
	return 0;
}
