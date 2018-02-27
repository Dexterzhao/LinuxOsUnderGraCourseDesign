#include <unistd.h>

int main(int argc, char *argv[])
{
	int p1, p2, p3;
	while ((p1 = fork()) == -1);
	if (p1)
	{
		while ((p2 = fork()) == -1);
		if (p2)
		{
			while ((p3 = fork()) == -1);
		}
	}
	if (!p1)
	{
		execv("clock", argv);
	}
	else if (!p2)
	{
		execv("monitor", argv);
	}
	else if (!p3)
	{
		execv("add", argv);
	}
	else
	{
		wait();
		wait();
		wait();
	}

	return 0;
}