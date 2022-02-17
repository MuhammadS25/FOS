#include <inc/lib.h>

void _main(void)
{
	// Create & run the slave environments
	int ID;
	for(int i = 0; i < 3; ++i)
	{
		ID = sys_create_env("dummy_process", (myEnv->page_WS_max_size), (myEnv->SecondListSize), (myEnv->percentage_of_WS_pages_to_be_removed));
		if (ID == E_ENV_CREATION_ERROR)
			panic("RUNNING OUT OF ENV!! terminating...");
		sys_run_env(ID);
	}

	env_sleep(100000);
	// To ensure that the slave environments completed successfully
	cprintf("Congratulations!! test CPU SCHEDULING using MLFQ is completed successfully.\n");

	return;
}


