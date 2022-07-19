#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "RingBuffer.h"

int main()
{
	RingBuffer ringBuffer;
	char str[] = "1234567890 abcdefghijklmnopqrstuvwxyz 1234567890 abcdefghijklmnopqrstuvwxyz 12345";
	char buffer[100];
	char testBuffer[100];
	int deqCnt = 0;
	int enqCnt = 0;

	srand(7879);
	while (1)
	{
		int enqSize = 0;
		int strSize = sizeof(str);
		while (strSize)
		{
			int size = rand() % strSize + 1;
			ringBuffer.Enqueue(&str[enqSize], size);
			enqSize += size;
			strSize -= size;
			enqCnt++;
		}

		int deqSize = 0;
		while (enqSize)
		{
			int size = rand() % enqSize + 1;
			ringBuffer.Peek(&testBuffer[deqSize], size);
			ringBuffer.Dequeue(&buffer[deqSize], size);
			if (memcmp(testBuffer, buffer, size) != 0)
			{
				printf("memcmp error\n");
				return -1;
			}

			deqSize += size;
			enqSize -= size;
			deqCnt++;
		}

		printf(buffer);
		if (strcmp(buffer, str) != 0)
		{
			printf("strcmp error\n");
			printf_s("enqCnt : %d, deqCnt : %d\n", enqCnt, deqCnt);
			return -1;
		}
		buffer[0] = '\0';
	}

	return 0;
}