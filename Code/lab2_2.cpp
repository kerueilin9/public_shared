#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

using namespace std;

int gpio_export(unsigned int gpio)
{
	int fd, len;
	char buf[64];
	
	fd = open("/sys/class/gpio/export", O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/export");
		return fd;
	} 

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_unexport(unsigned int gpio)
{
	int fd, len;
	char buf[64];
	
	fd = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/unexport");
		return fd;
	} 

	len = snprintf(buf, sizeof(buf), "%d", gpio);
	write(fd, buf, len);
	close(fd);
	return 0;
}

int gpio_set_dir(unsigned int gpio, string dirStatus)
{
	int fd;
	char buf[64];
	
	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/direction", gpio);
	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/direction");
		return fd;
	} 

	if(dirStatus == "out")
		write(fd, "out", 4);
	else
		write(fd, "in", 3);
	close(fd);
	return 0;
}

int gpio_set_value(unsigned int gpio, int value)
{
	int fd;
	char buf[64];
	
	snprintf(buf, sizeof(buf), "/sys/class/gpio/gpio%d/value", gpio);
	fd = open(buf, O_WRONLY);
	if(fd < 0)
	{
		perror("gpio/set-value");
		return fd;
	} 

	if(value == 0)
		write(fd, "0", 2);
	else
		write(fd, "1", 2);
	close(fd);
	return 0;
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


int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        if(strcmp(argv[1], "Mode_Shine") == 0)
        {
			//cout << argv[1] << argv[2] <<endl;
            gpio_export(466);
            gpio_export(255);
            gpio_export(429);
            gpio_export(427);
            gpio_set_dir(466, "out");
            gpio_set_dir(255, "out");
            gpio_set_dir(429, "out");
            gpio_set_dir(427, "out");
            for(int i = 0; i < toInt(argv[2]); i++)
            {
		cout << i <<endl;
                gpio_set_value(466, 1);
                gpio_set_value(255, 1);
                gpio_set_value(429, 0);
                gpio_set_value(427, 0);
                sleep(1);
                gpio_set_value(466, 0);
                gpio_set_value(255, 0);
                gpio_set_value(429, 1);
                gpio_set_value(427, 1);
                sleep(1);
            }
        }
        else
        {
			//cout << argv[1] << argv[2] <<endl;
            int port = transPort(argv[1]);
            gpio_export(port);
            gpio_set_dir(port, "out");
            if(port > 0)
            {
                if(strcmp(argv[2], "on") == 0)
                    gpio_set_value(port, 1);
                else
                    gpio_set_value(port, 0);
            }
            //gpio_unexport(port);
        }


    }

    return 0;
}
