#include "stdafx.h"

timespec diffTime(timespec start, timespec end);
void Usage();

int main(int argc, char *argv[])
{
	timespec timeStart, timeEnd, timeResult;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeStart);

	CAnagramFinder cAnagramApp;

	if (argc != 3)
		Usage();
	else
		cAnagramApp.Initialize(argv);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &timeEnd);
	timeResult = diffTime(timeStart, timeEnd);

	cAnagramApp.PrintResults(&timeResult);
	return 0;
}

//http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
timespec diffTime(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec - start.tv_nsec) < 0) {
		temp.tv_sec = end.tv_sec - start.tv_sec - 1;
		temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
	}
	else
	{
		temp.tv_sec = end.tv_sec - start.tv_sec;
		temp.tv_nsec = end.tv_nsec - start.tv_nsec;
	}
	return temp;
}

void Usage()
{
	printf("AnagramFinder requires 2 arguments:\n");
	printf("\t1) Path to dictionary file (Windows 1257 codepage file with ANSI encoding)\n");
	printf("\t2) A word to find the anagrams for (Maximal length: %d)\n", MAX_WORD_LENGTH);
}