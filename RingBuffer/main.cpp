#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <process.h>
#include "RingBuffer.h"

RingBuffer g_ringBuffer(100);
char g_str[] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";
int g_enqCnt = 0;
int g_deqCnt = 0;

unsigned WINAPI EnqueueProc(LPVOID lpParam)
{
	srand(7879);
	
	char testBuffer[100];
	int enqSize = 0;
	int strSize = sizeof(g_str);

	while (1)
	{
		if (strSize == 0)
		{
			enqSize = 0;
			strSize = sizeof(g_str);

			if (strcmp(testBuffer, g_str) != 0)
			{
				printf("strcmp error\n");
				return -1;
			}
		}

		int size = rand() % strSize + 1;
		if (!g_ringBuffer.Enqueue(&g_str[enqSize], size)) continue;
		memcpy_s(&testBuffer[enqSize], size, &g_str[enqSize], size);

		enqSize += size;
		strSize -= size;
		g_enqCnt++;
	}

	return 0;
}

unsigned WINAPI DequeueProc(LPVOID lpParam)
{
	srand(100);

	char buffer[100];
	char testBuffer[100];
	memset(buffer, 0, 100);
	memset(testBuffer, 0, 100);

	int deqSize = 0;
	int strSize = sizeof(g_str);

	while (1)
	{
		if (strSize == 0)
		{
			printf(buffer);
			if (strcmp(buffer, g_str) != 0)
			{
				printf("strcmp error\n");
				printf_s("enqCnt : %d, deqCnt : %d\n", g_enqCnt, g_deqCnt);
				return -1;
			}

			deqSize = 0;
			strSize = sizeof(g_str);
			memset(buffer, 0, 100);
			memset(testBuffer, 0, 100);
		}

		int size = rand() % strSize + 1;
		if (!g_ringBuffer.Peek(&testBuffer[deqSize], size)) continue;
		if (!g_ringBuffer.Dequeue(&buffer[deqSize], size)) continue;
		if (memcmp(testBuffer, buffer, size) != 0)
		{
			printf("memcmp error\n");
			return -1;
		}

		deqSize += size;
		strSize -= size;
		g_deqCnt++;
	}

	return 0;
}

int main()
{
	HANDLE hThread[2];

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, EnqueueProc, (LPVOID)0, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, DequeueProc, (LPVOID)1, 0, NULL);

	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	return 0;
}