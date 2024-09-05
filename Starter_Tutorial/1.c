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


#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export" 
#define SYSFS_GPIO_RST_VAL    "/sys/class/gpio/gpio499/value"
#define SYSFS_GPIO_RST_DIR  "/sys/class/gpio/gpio499/direction"

#define SYSFS_GPIO_RST_VAL_H        "1"

#define SYSFS_GPIO_RST_VAL_L        "0"
#define SYSFS_GPIO_RST_PIN_VAL 		"499"
#define SYSFS_GPIO_RST_DIR_VAL      "OUT"  



int out[17];
void delay(){
	volatile unsigned int j;
	for(j=0;j<100000;j++)
		;
}
void trans(int in){
	int i,c=0;
	for(i=0;i<17;i++)
		out[i]=0;
	while(in!=0){
		i=in%2;
		out[c]=i;
		c++;
		in/=2;
	}
	
}

void show(){
	int i;
	for(i=0;i<16;i++)
		printf("%d",out[i]);
	printf("\n");
}

void lighton()
{
	int fd;
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
         
	if(fd == -1)
         
	{
                   
		printf("ERR: Radio hard reset pin open error.\n");
                          
	}
         
	write(fd, SYSFS_GPIO_RST_PIN_VAL ,sizeof(SYSFS_GPIO_RST_PIN_VAL));     
	close(fd);  	        
	printf("!!!\n");
	//设置端口方向/sys/class/gpio/gpio48# echo out > direction
         
	fd = open(SYSFS_GPIO_RST_DIR, O_WRONLY);
         
	if(fd == -1)
         
	{
                   
		printf("ERR: Radio hard reset pin direction open error.\n");
                         
	}
         
	write(fd, SYSFS_GPIO_RST_DIR_VAL, sizeof(SYSFS_GPIO_RST_DIR_VAL)); 
 
	close(fd);          


	printf("!!!\n");         

	//输出复位信号: 拉高>100ns
         
	fd = open(SYSFS_GPIO_RST_VAL, O_RDWR);
         
	if(fd == -1)
         
	{
                   
		printf("ERR: Radio hard reset pin value open error.\n");
                       
	}       
         
	printf("!!!\n");
	write(fd, SYSFS_GPIO_RST_VAL_H, sizeof(SYSFS_GPIO_RST_VAL_H));
 
	close(fd);  
	for(int k=0;k<20;k++)
		delay();
      
}

int main(void)
{
    volatile unsigned int * gpio;
	int fd = open("/dev/uio0", O_RDWR);
	printf("fd:%d\n",fd);
    if (fd < 1) {
        printf("could not open /dev/uio0\n");
        exit(EXIT_FAILURE);
    }

    // memory map physical address 0x10C0_0000-0x10C0_000B to a virtual address to access the GPIO
	gpio = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    	printf("gpio d:%d\n",gpio);
	printf("gpio p:%p\n",gpio);
    	gpio[1]  = 0xFFFF; // write 0x00FF to memory at location 0x10C0_0004 
					   // to set 8 least significant bits to inputs (Switches)
					   // and next 8 to outputs (LEDs)

	printf("sw:%d\n",gpio[0]); 
	lighton();

    	while(1)
	{
		printf("sw:%d\n",gpio[0]); 
		trans(gpio[0]);
		show();
		delay();
	}

	printf("Reading 0x10C0_0000 for reading Switches[15:8] %08x \n", gpio[0]);
}
