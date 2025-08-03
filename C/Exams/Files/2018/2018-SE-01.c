#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <err.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{

	if(argc != 3) errx(1, "Wrong number of arguments");

	char buffer[1024] = {0};
	int readBytes = 0;
	if((readBytes = read(0, buffer, sizeof(buffer))) < 0) err(1, "Read error");
	buffer[readBytes] = '\0';

	if(!strcmp(argv[1], "-d") || !strcmp(argv[1], "-s"))
	{

		if(!strcmp(argv[1], "-d"))
		{

			char result[1024] = {0};
			bool indexes[100] = {false};
			int index = 0;

			for (int i = 0; i < (int)strlen(buffer); i++)
			{
				
				for (int j = 0; j < (int)strlen(argv[2]); j++)
				{
					
					if(buffer[i] == argv[2][j]) indexes[i] = true;

				}

			}
			
			for (int i = 0; i < (int)strlen(buffer); i++)
			{
					
				if(indexes[i]) continue;

				result[index] =  buffer[i];
				index += 1;

			}

			result[index] = '\0';
			if(write(1, result, strlen(result)) < 0) err(1, "Write error");

		}
		else
		{

			char result[1024] = {0};
			bool indexes[100] = {false};
			int index = 0;

			for (int i = 0; i < (int)strlen(buffer); i++)
			{
				
				for (int j = 0; j < (int)strlen(argv[2]); j++)
				{
					
					if(buffer[i] == argv[2][j])
					{

						while(i + 1 != (int)strlen(buffer) && buffer[i] == buffer[i + 1])
						{

							indexes[i + 1] = true;
							i += 1;

						}

					}

				}

			}
			
			for (int i = 0; i < (int)strlen(buffer); i++)
			{
				
				if(indexes[i]) continue;

				result[index] = buffer[i];
				index += 1;

			}

			result[index] = '\0';
			if(write(1, result, strlen(result)) < 0) err(1, "Write error");

		}

	}
	else
	{

		if(strlen(argv[1]) != strlen(argv[2])) err(1, "Invalid input");

		for (int i = 0; i < (int)strlen(buffer); i++)
		{
			
			for (int j = 0; j < (int)strlen(argv[1]); j++)
			{
				
				if(buffer[i] == argv[1][j])
				{

					buffer[i] = argv[2][j];

				}

			}

		}
		
		if(write(1, buffer, strlen(buffer)) < 0) err(1, "Write error");

	}

	return 0;

}
