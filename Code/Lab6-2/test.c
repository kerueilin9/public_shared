/*#include <stdio.h>

int main()
{
	char buf[1024] = "Data Input 123456 hello world";
	FILE *fp = fopen("/dev/demo", "w+");
	if( fp == NULL )
	{
		printf("can't open device!\n");
		return 0;
	}
	fwrite(buf, sizeof(buf), 1, fp);
	fread(buf, sizeof(buf), 1, fp);
	fclose(fp);
	return 0;
}
*/
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>




void setGPIO(int gpio, char* status)
{
	FILE *io = fopen("/dev/demo", "w+");
	//int io = open("/dev/demo", O_RDWR);
	if(io < 0)
	{
		perror("gpio error");
		return;	
	}
	char buf[10];
	if(strcmp(status, "on") == 0)
	{
		sprintf(buf, "%d1", gpio);
		//strcpy(buf, (to_string(gpio) + "1").c_str());
	}
	else
	{
		sprintf(buf, "%d0", gpio);
		//strcpy(buf, (to_string(gpio) + "0").c_str());
	}
	fwrite(buf, strlen(buf), 1, io);
	//write(io, buf, strlen(buf));
	close(io);
	return;
}

void readGPIO(int gpio)
{
	FILE *io = fopen("/dev/demo", "r");
	//int * io = open("/dev/demo", O_RDWR);
	if(io < 0)
	{
		perror("gpio error");
		return;	
	}
	char buf[10] = "";
	sprintf(buf, "%d", gpio);
	//strcpy(buf, to_string(gpio).c_str());
	ioctl(gpio, gpio, io);
	fread(buf, 2, 1, io);
	//read(io, buf, 2);
	close(io);
	buf[1] = '\0';
	printf("%s\n", buf);
	return;
}

int transPort(char *src)
{
	if(strcmp(src, "LED1") == 0)
		return 466;
	else if(strcmp(src, "LED2") == 0)
		return 255;
	else if(strcmp(src, "LED3") == 0)
		return 429;
	else if(strcmp(src, "LED4") == 0)
		return 427;
	else 
		return 0;
}

int main(int argc, char *argv[])
{
	switch(argc)
	{
		case 2:
		{
			int port = transPort(argv[1]);
			if(port > 0)
			{
				printf("input: %d\n", port);
				printf("%s (GPIO= %d) Status: ", argv[1], port);
				readGPIO(port);

			}
			break;
		}			
		case 3:
		{
			int port = transPort(argv[1]);
			if(port > 0)
			{
				setGPIO(port, argv[2]);
			}
			break;
		}
		default:
			printf("do nothing\n");
;			//cout << "do nothing" << endl;
			break;
	}
	return 0;
}

