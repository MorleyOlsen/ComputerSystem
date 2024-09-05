#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>
#include <unistd.h>
#include <fcntl.h>   //define O_WRONLY and O_RDONLY  
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/mman.h>

volatile unsigned int * gpio;
//定义芯片引脚:
#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export"  
#define SYSFS_GPIO_RST_DIR_VAL      "out"  
#define SYSFS_GPIO_RST_VAL_H        "1"
#define SYSFS_GPIO_RST_VAL_L        "0"



char SYSFS_GPIO_RST_PIN_VAL[16][25] = { //端口号
	"505", "504", "503", "502", "501", 
	"500", "499", "498", "497", "496",
	"495", "494", "493", "492", "491", "490"};  
char SYSFS_GPIO_RST_DIR[16][50]={       //端口方向（开关输入IN，灯输出OUT）
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
	"/sys/class/gpio/gpio490/direction"}; 
	
char SYSFS_GPIO_RST_VAL[16][50] = {  //端口值文件
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
	"/sys/class/gpio/gpio490/value"}; 

//函数功能：十进制转换成二进制数存入字符串
void int_to_chars(char* str) {			
	int num = gpio[0];
	for (int i = 15; i >= 0; i--) {
		str[i] = num % 2 + '0';
		num /= 2;
	}
}

//打开开关设备，并设置控制方向
int init() {							
	int fd = open("/dev/uio0", O_RDWR);
	if (fd < 1) {
		printf("could not open /dev/uio0\n");
		exit(EXIT_FAILURE);
	} 
	
	//虚拟地址到物理地址的映射
	gpio = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	//Controller，控制所有开关，4位16进制数对应16个二进制位（16个开关）
	gpio[1]  = 0xFFFF; 	
	int result = gpio[0]; //开关的值
	
	//关闭设备
	close(fd); 
	
	return result;
}

//初始化灯全灭 
int offled() {
	for(int i = 0; i < 16; i++)	{		
	    //打开端口/sys/class/gpio# echo 48 > export
        int fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);  
        if(fd == -1) {
            printf("ERR: Radio hard reset pin open error.\n");
            return EXIT_FAILURE;
        }
        char *temp_str;
	    temp_str = (char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_PIN_VAL[i])+1));
	    strncpy(temp_str, SYSFS_GPIO_RST_PIN_VAL[i], sizeof(SYSFS_GPIO_RST_PIN_VAL[i]));
        write(fd, temp_str ,sizeof(temp_str)); 
        close(fd); 
        
	    //设置端口方向/sys/class/gpio/gpio48# echo out > direction
	    char *temp_str1;
	    temp_str1=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_DIR[i])+1));
	    strncpy(temp_str1,SYSFS_GPIO_RST_DIR[i],sizeof(SYSFS_GPIO_RST_DIR[i]));	///sys/class/gpio/gpio505/direction
        fd = open(temp_str1, O_WRONLY);
        if(fd == -1) {
            printf("ERR: Radio hard reset pin direction open error.\n");
            return EXIT_FAILURE;
        }
        write(fd, SYSFS_GPIO_RST_DIR_VAL, sizeof(SYSFS_GPIO_RST_DIR_VAL)); 	//out 
        close(fd); 

        //灯全部初始化为熄灭 
	    char *temp_str2;
	    temp_str2=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
	    strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i]));	///sys/class/gpio/gpio505/value
	    fd = open(temp_str2, O_RDWR);
        if(fd == -1) {
           	printf("ERR: Radio hard reset pin value open error.\n");
         	return EXIT_FAILURE;
        }   
        write(fd, SYSFS_GPIO_RST_VAL_L, sizeof(SYSFS_GPIO_RST_VAL_L));		//0低电平 
        close(fd);
	}
} 

//点亮灯
int onled(char* number){							 
	int i=0;
	char *temp_str2;
	for(i = 0; i < 16; i++) {
		if(number[i] == '1'){	//如果该位数字为1，则对应的灯需要点亮 
			int fd;
			temp_str2=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
			strncpy(temp_str2, SYSFS_GPIO_RST_VAL[i], sizeof(SYSFS_GPIO_RST_VAL[i])); 
	        fd = open(temp_str2, O_RDWR);
	        if(fd == -1){
                   printf("ERR: Radio hard reset pin value open error.\n");
                   return EXIT_FAILURE;
	        }
	        write(fd, SYSFS_GPIO_RST_VAL_H, sizeof(SYSFS_GPIO_RST_VAL_H));  //高电平
	        close(fd);
		}

		else{				//为0则该灯灭 
			int fd; 
			temp_str2=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
			strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i])); 
	        fd = open(temp_str2, O_RDWR);
	        if(fd == -1){
                   printf("ERR: Radio hard reset pin value open error.\n");
                   return EXIT_FAILURE;
	        }       
	        write(fd, SYSFS_GPIO_RST_VAL_L, sizeof(SYSFS_GPIO_RST_VAL_L));
	        close(fd);
		}
	} 
} 

void light(char* number) {					//跑马灯代码
	int k = 0;
	while(k <= 99) {
		onled();				//点亮灯 		
        //数字走马，左边第一位等于最后一位，其余取其左值 
		char temp = number[15];
		for(int i = 1; i <16; i++) {
			number[i] = number[i-1];
		}
		number[0] = temp;
		usleep(1000000); //亮灯维持1秒，单位是微秒
		k++;
	}
}  

int main() {
    int sw = 0;	
	char number[16] = "0001011100000101"; 	//1705的16进制转2进制 
	sw = init();  //打开开关设备，并设置控制方向
	offled();     //初始化灯全灭 
//	while(gpio[0] != 0x7000) {
//		int_to_chars(number);   //进制转换，将gpio结果放到number
//		onled(number);		    //点亮灯
//	}
//    int_to_chars(number);   //进制转换，将gpio结果放到number
    for (int i=0;i<2;i++)
	{
	 onled(number);  //点亮灯
	 usleep(500000); //延迟0.5秒，单位是微秒
	 onled("0000000000000000");//灭灯 
	 usleep(500000); //延迟0.5秒，单位是微秒
	} 
	
	
	
    usleep(2000000); //延迟2秒，单位是微秒
	
	while(1)
	{
		char str1[30];
		scanf("%s",str1);
		if( strcmp(str1,"LED")==0)
		{
			volatile int *IO_iotr1= (int*)0xb0600004;
  			volatile int *IO_iotr2= (int*)0xb0c00004;
  			volatile int *IO_SWITCHES = (int*)0xb0c00000;
 			 volatile int *IO_LEDR = (int*)0xb0600000;
  			volatile unsigned int switches;
  
			*IO_iotr1=0x00000000;
			*IO_iotr2=0xffffffff;
  			while (1) {
    			switches =*IO_SWITCHES;
    			*IO_LEDR = switches;	  
   				 delay();
    			*IO_LEDR = 0;  // turn off LEDs
    			delay();
  		}


		}
		else if( strcmp(str1,"KEY")==0)
		{
			number[16] = "0001011100000000"; 
			light(number);
		}
		
		else break;
	}
	return 0;
}  
