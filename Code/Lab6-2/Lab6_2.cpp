#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <string>
#include <sys/ioctl.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;


void setGPIO(int gpio, char* status)
{

	int io = open("/dev/demo", O_RDWR);
	if(io < 0)
	{
		perror("gpio error");
		return;	
	}
	char buf[10];
	if(strcmp(status, "on") == 0)
	{
		strcpy(buf, (to_string(gpio) + "1").c_str());
	}
	else
	{
		strcpy(buf, (to_string(gpio) + "0").c_str());
	}
	ioctl(io, gpio, gpio);
	write(io, buf, strlen(buf));
	close(io);
	return;
}

string readGPIO(int gpio)
{
	int io = open("/dev/demo", O_RDWR);
	if(io < 0)
	{
		perror("gpio error");
		return "error";	
	}
	char buf[10];
	strcpy(buf, to_string(gpio).c_str());
	ioctl(io, 0, gpio);
	read(io, buf, 2);
	close(io);
	buf[1] = '\0';
	return buf;
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

int toInt(char *p)
{
    int result = 0;
    while(*p)
    {
        result = (result << 3) + (result << 1) + ('0' ^ *p++);
    } 
    return result;
}

int state;
int port[4] = {466, 255, 429, 427};
int count;
pthread_mutex_t mutex;
sem_t semaphore;


void * semaphore_child(void * arg) {
    int *input = (int *) arg;
    for (int i = 0; i < count*2; ++i) {
        sem_wait(&semaphore);
	if(input[1])
	{
		printf("num = %d, %d \n", input[0], 1);
		setGPIO(input[0], "on");
	}
	else
	{
		printf("num = %d, %d \n", input[0], 0);
		setGPIO(input[0], "off");
	}
	input[1] ^= 1;
        sleep(1);
    }
    pthread_exit(NULL);
}

void * mutex_child(void *arg) {
    int input = *(int*)arg;
    for (int i = 0; i < count*2; ++i) {
        pthread_mutex_lock(&mutex);
	if(input == 1){
		int tmp = state;
		char tmpc[3] = "00";
		char *p = tmpc + 1;
		while(tmp)
		{
			*p-- = '0' ^ (tmp&1);
			tmp>>=1;
		}
		p++;
		printf("status: %s\n", p);
    	}
	printf("num = %d, %d \n", port[input], (state>>input)&1);
	if((state>>input)&1)
		setGPIO(port[input], "on");
	else
		setGPIO(port[input], "off");
	state^= (1 << input);
        pthread_mutex_unlock(&mutex);
	sleep(1);
    }
    pthread_exit(NULL);
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
				cout << "input: " << port << endl;
				cout << argv[1] << "(GPIO=" << port << ")" << "Status: " << readGPIO(port) << endl;
			}
			break;
		}			
		case 3:
		{
			int toport = transPort(argv[1]);
			if(toport > 0)
			{
				setGPIO(toport, argv[2]);
			}
			else if(strcmp(argv[1], "mutex") == 0)
			{
				pthread_t led1, led2;
				count = toInt(argv[2]);
				pthread_mutex_init(&mutex, 0);
				
				state = 0;
				for(int i = 0; i < 2; i++)
				{
					state<<=1;
					state ^= '1' ^ '0';
				}

				int input[2] = {1, 0}; 
				pthread_create(&led1, NULL, mutex_child, (void*) input);
				pthread_create(&led2, NULL, mutex_child, (void*) (input + 1));

				pthread_join(led1, NULL);
				pthread_join(led2, NULL);
				pthread_mutex_destroy(&mutex);

			}
			else if(strcmp(argv[1], "semaphore") == 0)
			{
				pthread_t led1, led2;
				count = toInt(argv[2]);
				sem_init(&semaphore, 0, 0);

				int input1[2] = {0, 1};
				input1[0] = 466;
				int input2[2] = {0, 1};
				input2[0] = 255;
				pthread_create(&led1, NULL, semaphore_child, (void*) input1);
				pthread_create(&led2, NULL, semaphore_child, (void*) input2);

				for(int i = 0; i < count*8; i++) {
				    sem_post(&semaphore);
				}
				pthread_join(led1, NULL);
				pthread_join(led2, NULL);
			}
			break;
		}
		default:
			cout << "do nothing" << endl;
			break;
	}
	return 0;
}

