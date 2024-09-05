/* MIPSfpga: Example userspace application for custom_axi_gpio */

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

char SYSFS_GPIO_RST_DIR[16][50]={
	"/sys/class/gpio505/direction",
	"/sys/class/gpio504/direction",
	"/sys/class/gpio503/direction",
	"/sys/class/gpio502/direction",
	"/sys/class/gpio501/direction",
	"/sys/class/gpio500/direction",
	"/sys/class/gpio499/direction",
	"/sys/class/gpio498/direction",
	"/sys/class/gpio497/direction",
	"/sys/class/gpio496/direction",
	"/sys/class/gpio495/direction",
	"/sys/class/gpio494/direction",
	"/sys/class/gpio493/direction",
	"/sys/class/gpio492/direction",
	"/sys/class/gpio491/direction",
	"/sys/class/gpio490/direction"
};

char SYSFS_GPIO_RST_VAL[16][50]={
	"/sys/class/gpio505/value",
	"/sys/class/gpio504/value",
	"/sys/class/gpio503/value",
	"/sys/class/gpio502/value",
	"/sys/class/gpio501/value",
	"/sys/class/gpio500/value",
	"/sys/class/gpio499/value",
	"/sys/class/gpio498/value",
	"/sys/class/gpio497/value",
	"/sys/class/gpio496/value",
	"/sys/class/gpio495/value",
	"/sys/class/gpio494/value",
	"/sys/class/gpio493/value",
	"/sys/class/gpio492/value",
	"/sys/class/gpio491/value",
	"/sys/class/gpio490/value"
};


int out[17];	//二进制数字存储数组
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
	gpio[1] = 0xFFFF;
	int result = gpio[0]; //开关的值

	//关闭设备
	close(fd);

	printf("successfully open!\n");

	return result;
}

//灯全部初始化为熄灭
int offled(){
	for(int i = 0; i < 16; i++){
		//打开端口/sys/class/gpio# echo 48 > export
		int fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
         
		if(fd == -1)
         
		{
                   
			printf("ERR: Radio hard reset pin open error.\n");
  
			return EXIT_FAILURE;        
            
		}

		
		printf("Radio hard reset pin %d successfully open!\n",i);    

		char *temp_str;
		temp_str = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_PIN_VAL[i])+1));
		strncpy(temp_str,SYSFS_GPIO_RST_PIN_VAL[i],sizeof(SYSFS_GPIO_RST_PIN_VAL[i])); 
		write(fd, temp_str, sizeof(temp_str));
		close(fd);

		printf("Radio hard reset pin %d successfully finish!\n",i);    

		//设置端口方向/sys/class/gpio/gpio48# echo out > direction
	//	char *temp_str1;
	//	temp_str1 = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_DIR[i])+1));
	//	strncpy(temp_str1,SYSFS_GPIO_RST_DIR[i],sizeof(SYSFS_GPIO_RST_DIR[i])); ///sys/class/gpio/gpio505/direction
	//	fd = open(temp_str1, O_WRONLY);
		fd = open(SYSFS_GPIO_RST_DIR[i], O_WRONLY);
		if(fd == -1)
		{
			printf("ERR: Radio hard %d reset direction pin open error.\n",i);
  
			return EXIT_FAILURE;
		}

		printf("Radio hard reset direction pin %d successfully open!\n",i);    

		write(fd, SYSFS_GPIO_RST_DIR_VAL, sizeof(SYSFS_GPIO_RST_DIR_VAL));
		close(fd);

		printf("Radio hard reset direction pin %d successfully finish!\n",i);    

		//灯全部初始熄灭
		char *temp_str2;
		temp_str2 = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
		strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i])); ///sys/class/gpio/gpio505/value
		fd = open(temp_str2, O_RDWR);
		if(fd == -1)
		{
			printf("ERR: Radio hard reset %d value pin open error.\n",i);
  
			return EXIT_FAILURE;
		}

		printf("Radio hard reset pin value %d successfully open!\n",i);    

		write(fd, SYSFS_GPIO_RST_VAL_L, sizeof(SYSFS_GPIO_RST_VAL_L));
		close(fd);

		printf("Radio hard reset pin value %d successfully finish!\n",i);    
	}
}	


int lighton(char* number)
{	//亮灯
	int i=0;
	char *temp_str2;
	for(i = 0; i<16; i++)
	{
		int fd;
		if(number[i] == '1'){
			temp_str2 = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
			strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i])); 
			fd = open(temp_str2, O_RDWR);
			if(fd == -1)
			{
				printf("ERR: Radio hard reset pin value %d open error.\n",i);
  
				return EXIT_FAILURE;
			}
			write(fd, SYSFS_GPIO_RST_VAL_H, sizeof(SYSFS_GPIO_RST_VAL_H));
			close(fd);
		}

		else{
			temp_str2 = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
			strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i])); ///sys/class/gpio/gpio505/value
			fd = open(temp_str2, O_RDWR);
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
	int k = 0;
	while(k <= 99){
		lighton(number);	//点亮灯
		//数字走马，左边第一位等于最后一位，其余取左值
		char temp = number[15];
		for(int i = 15; i > 0; i--){
			number[i]=number[i-1];
		}
		number[0]=temp;
		usleep(1000000); //亮灯维持1秒，单位是微妙
		k++;
	}
}


int main(void)
{
	int sw = 0;
	char number[16] = "0000000000010000";	//组号
	sw = init();	//打开设备
	offled();	//初始灭灯
	for(int i=0;i<2;i++){
		lighton(number);
		usleep(50000);
		lighton("0000000000000000");
		usleep(50000);
	}
	usleep(2000000);
	running(number);
	return 0;
}
