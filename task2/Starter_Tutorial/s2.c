﻿/* MIPSfpga: Example userspace application for custom_axi_gpio */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

volatile unsigned int* gpio;

#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export" 
/*
#define SYSFS_GPIO_RST_VAL    "/sys/class/gpio/gpio490/value"
#define SYSFS_GPIO_RST_DIR  "/sys/class/gpio/gpio490/direction"

*/
#define SYSFS_GPIO_RST_VAL_H        "1"

#define SYSFS_GPIO_RST_VAL_L        "0"
/*
#define SYSFS_GPIO_RST_PIN_VAL 		"490" 
*/
#define SYSFS_GPIO_RST_DIR_VAL      "OUT"  


char SYSFS_GPIO_RST_PIN_VAL[16][25] = {
	"505","504","503","502",
	"501","500","499","498",
	"497","496","495","494",
	"493","492","491","490"
};

char SYSFS_GPIO_RST_DIR[16][100]={
	"/sys/class/gpio/gpio505/direction",
	"/sys/class/gpio/gpio504/direction",
	"/sys/class/gpio/gpio503/direction",
	"/sys/class/gpio/gpio502/direction",
	"/sys/class/gpio/gpio501/direction",
	"/sys/class/gpio/gpio500/direction",
	"/sys/class/gpio/gpio499/direction",
	"/sys/class/gpio/gpio498/direction",
	"/sys/class/gpio/gpio497/direction",
	"/sys/class/gpio/gpio496/direction",
	"/sys/class/gpio/gpio495/direction",
	"/sys/class/gpio/gpio494/direction",
	"/sys/class/gpio/gpio493/direction",
	"/sys/class/gpio/gpio492/direction",
	"/sys/class/gpio/gpio491/direction",
	"/sys/class/gpio/gpio490/direction"
};

char SYSFS_GPIO_RST_VAL[16][100]={
	"/sys/class/gpio/gpio505/value",
	"/sys/class/gpio/gpio504/value",
	"/sys/class/gpio/gpio503/value",
	"/sys/class/gpio/gpio502/value",
	"/sys/class/gpio/gpio501/value",
	"/sys/class/gpio/gpio500/value",
	"/sys/class/gpio/gpio499/value",
	"/sys/class/gpio/gpio498/value",
	"/sys/class/gpio/gpio497/value",
	"/sys/class/gpio/gpio496/value",
	"/sys/class/gpio/gpio495/value",
	"/sys/class/gpio/gpio494/value",
	"/sys/class/gpio/gpio493/value",
	"/sys/class/gpio/gpio492/value",
	"/sys/class/gpio/gpio491/value",
	"/sys/class/gpio/gpio490/value"
};


int out[17];	//二进制数字存储数组
/*
void delay(){
	volatile unsigned int j;
	for(j=0;j<100000;j++)
		;
}
void trans(char * str){
	//输入的数字转换成二进制
	int num = gpio[0];
	for(int i=15; i>=0; i--){
		str[i] = num % 2 + '0';
		num /= 2;
	}
}

void show(){
	//打印二进制数
	int i;
	for(i=0;i<16;i++)
		printf("%d",out[i]);
	printf("\n");
}
*/

//开关设备
int init(){
	int fd = open("/dev/uio0",O_RDWR);
	if(fd < 1){
		printf("could not open /dev/uio0\n");
		exit(EXIT_FAILURE);
	}

	//虚拟地址到物理地址的映射
	gpio = (unsigned int*)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	//Controller,控制所有开关，4位16进制对应16个开关
	gpio[1] = 0x0000;
	int result = gpio[0]; //开关的值

	//关闭设备
	close(fd);

	printf("successfully open!\n");

	return result;
}

//灯全部初始化为熄灭
int offled(){
	int index = 0;
	for(int i = 0; i < 16; i++){
		int fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
    		if (fd ==-1) {
        		printf("could not open ERROR\n");
        		exit(EXIT_FAILURE);
    		}
			write(fd,SYSFS_GPIO_RST_PIN_VAL[ (abs(i-index)) %16],sizeof(SYSFS_GPIO_RST_PIN_VAL[(abs(i-index))%16])); //"echo SYSFS_GPIO_RST_PIN_VAL > export"写入编号 
    		close(fd);
    
    		fd = open(SYSFS_GPIO_RST_DIR[(i+index)%16], O_WRONLY);
    		if (fd==-1) {
        		printf("could not open ERROR1\n");
        		exit(EXIT_FAILURE);
    		}
    		write(fd,SYSFS_GPIO_RST_DIR_VAL,sizeof(SYSFS_GPIO_RST_DIR_VAL)); //"echo out > RST_DIR"定义方向
    		close(fd);
 
    		fd = open(SYSFS_GPIO_RST_VAL[(i+index)%16], O_RDWR);
    		if (fd ==-1) {
        		printf("could not open ERROR2\n");
        		exit(EXIT_FAILURE);
    		}
        	write(fd,"1",sizeof(SYSFS_GPIO_RST_VAL_L));                 //"echo 0 > SYSFS_GPIO_RST_VAL"设置关灯
    		close(fd);
	}
}	


int lighton(char* number)
{	//亮灯
	int i=0;
	for(i = 0; i<16; i++)
	{
		int fd;
		if(number[i] == '1'){
			fd = open(SYSFS_GPIO_RST_VAL[i], O_RDWR);
			if(fd == -1)
			{
				printf("ERR: Radio hard reset pin value %d open error.\n",i);
  
				return EXIT_FAILURE;
			}
			write(fd, SYSFS_GPIO_RST_VAL_H, sizeof(SYSFS_GPIO_RST_VAL_H));
			close(fd);
		}

		else{			///sys/class/gpio/gpio505/value
			fd = open(SYSFS_GPIO_RST_VAL[i], O_RDWR);
			if(fd == -1)
			{
				printf("ERR: Radio hard reset value pin %d open error.\n",i);
  
				return EXIT_FAILURE;
			}
			write(fd, SYSFS_GPIO_RST_VAL_L, sizeof(SYSFS_GPIO_RST_VAL_L));
			close(fd);
		}
	}      
}

void running(char* number){
	while(1){
		lighton(number);	//点亮灯
		//数字走马，左边第一位等于最后一位，其余取左值
		char temp = number[15];
		for(int i = 15; i > 0; i--){
			number[i]=number[i-1];
		}
		number[0]=temp;
		usleep(1000000);
	}
}


int main(void)
{
	int sw = 0;
	char number[16] = "0000000000001000";
	sw = init();
	offled();
	running(number);
	return 0;
}
