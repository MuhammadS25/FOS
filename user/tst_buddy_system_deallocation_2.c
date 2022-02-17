#include <inc/lib.h>

int GetPowOf2(int size);

void
_main(void)
{
	int freeFrames1 = sys_calculate_free_frames() ;
	int usedDiskPages1 = sys_pf_calculate_allocated_pages() ;

	char line[100];
	int N = 100;
	assert(N * sizeof(int) <= BUDDY_LIMIT);
	int M = 1000;
	assert(M * sizeof(uint8) <= BUDDY_LIMIT);

	uint8 ** arr = malloc(N * sizeof(int)) ;
	int expectedNumOfAllocatedFrames = GetPowOf2(N * sizeof(int));

	for (int i = 0; i < N; ++i)
	{
		arr[i] = malloc(M) ;
		expectedNumOfAllocatedFrames += GetPowOf2(M);
		for (int j = 0; j < M; ++j)
		{
			arr[i][j] = i % 255;
		}
	}
	expectedNumOfAllocatedFrames = ROUNDUP(expectedNumOfAllocatedFrames, PAGE_SIZE) / PAGE_SIZE;
	int freeFrames2 = sys_calculate_free_frames() ;
	int usedDiskPages2 = sys_pf_calculate_allocated_pages() ;
	if(freeFrames1 - freeFrames2 != 1 + 1 + expectedNumOfAllocatedFrames) panic("Less or more frames are allocated in MEMORY."); //1 for page table + 1 for disk table
	if(usedDiskPages2 - usedDiskPages1 != expectedNumOfAllocatedFrames) panic("Less or more frames are allocated in PAGE FILE.");

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			assert(arr[i][j] == i % 255);
		}
	}

	//[1] Freeing the allocated arrays + checking the BuddyLevels content + free frames after free
	for (int i = 0; i < N; ++i)
	{
		free(arr[i]);
	}
	free(arr);
	int i;
	for(i = BUDDY_LOWER_LEVEL; i < BUDDY_UPPER_LEVEL; i++)
	{
		if(LIST_SIZE(&BuddyLevels[i]) != 0)
		{
			cprintf("Level # = %d - # of nodes = %d\n", i, LIST_SIZE(&BuddyLevels[i]));
			panic("The BuddyLevels at level <<%d>> is not freed ... !!", i);
		}
	}
	int freeFrames3 = sys_calculate_free_frames() ;
	int usedDiskPages3 = sys_pf_calculate_allocated_pages() ;
	if(freeFrames1 - freeFrames3 != 1) panic("Extra or less frames are freed from the MEMORY.");
	if(usedDiskPages3 - usedDiskPages1 != 0) panic("Extra or less frames are freed from PAGE FILE.");

	//[2] Creating new arrays after FREEing the created ones + checking no extra frames are taken + checking content + BuddyLevels
	uint8 ** arr2 = malloc(N * sizeof(int)) ;
	for (int i = 0; i < N; ++i)
	{
		arr2[i] = malloc(M) ;
		for (int j = 0; j < M; ++j)
		{
			arr2[i][j] = (i + 1)%255;
		}
	}
	int freeFrames4 = sys_calculate_free_frames() ;
	int usedDiskPages4 = sys_pf_calculate_allocated_pages() ;

	//Check that no extra frames are taken
	if(freeFrames4 - freeFrames2 != 0) panic("Creating new arrays after FREE is failed.");
	if(usedDiskPages4 - usedDiskPages2 != 0) panic("Creating new arrays after FREE is failed.");

	//Check the array content
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			if(arr2[i][j] != (i + 1)%255) panic("Wrong content in the created arrays.");
		}
	}

	//Check the lists content of the BuddyLevels array
	{
	int L = BUDDY_LOWER_LEVEL;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	}

	//[3] Freeing the allocated arrays + checking the frames
	for (int i = 0; i < N; ++i)
	{
		free(arr2[i]);
	}
	free(arr2);

	int freeFrames5 = sys_calculate_free_frames() ;
	int usedDiskPages5 = sys_pf_calculate_allocated_pages() ;
	if(freeFrames5 - freeFrames3 != 0) panic("Extra or less frames are freed from the MEMORY.");
	if(usedDiskPages5 - usedDiskPages3 != 0) panic("Extra or less frames are freed from the DISK.");

	//[5] Creating new arrays with DIFFERENT sizes than the old ones + checking BuddyLevels + checking free frames + checking content
	N = 70;
	M = 1;
	uint8 ** arr3 = malloc(N * sizeof(int)) ;
	expectedNumOfAllocatedFrames = GetPowOf2(N * sizeof(int));
	for (int i = 0; i < N; ++i)
	{
		arr3[i] = malloc(M+1) ;
		expectedNumOfAllocatedFrames += GetPowOf2(M+1);
		for (int j = 0; j < M; ++j)
		{
			arr3[i][j] = (i + 2)%255;
		}
	}
	//Check the lists content of the BuddyLevels array
	{
	int L = BUDDY_LOWER_LEVEL;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=0)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	if(LIST_SIZE(&BuddyLevels[L])!=1)	{panic("WRONG number of nodes at Level # %d - # of nodes = %d.\n", L, LIST_SIZE(&BuddyLevels[L])); } L = L + 1;
	}

	int freeFrames6 = sys_calculate_free_frames() ;
	int usedDiskPages6 = sys_pf_calculate_allocated_pages() ;
	expectedNumOfAllocatedFrames = ROUNDUP(expectedNumOfAllocatedFrames, PAGE_SIZE) / PAGE_SIZE;
	//Check that no extra frames are taken
	if(freeFrames5 - freeFrames6 != expectedNumOfAllocatedFrames + 1);
	if(usedDiskPages6 - usedDiskPages5 != expectedNumOfAllocatedFrames);
	//Check the array content
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			assert(arr3[i][j] == (i + 2)%255);
		}
	}

	cprintf("Congratulations!! test BUDDY SYSTEM deallocation (2) completed successfully.\n");

	return;
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
