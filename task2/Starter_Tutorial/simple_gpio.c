/* MIPSfpga: Example userspace application for custom_axi_gpio */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include<unistd.h>


int main(void)
{

    volatile unsigned int * gpio;
    volatile unsigned int * gpioled;
    // ���뿪��
    int fd = open("/dev/uio0", O_RDWR);
    if (fd < 1) {
        printf("could not open /dev/uio0\n");
        exit(EXIT_FAILURE);
    }
    //���뿪�صĵ�ַ
    gpio = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    // ���뿪��д��
    gpio[1]  = 0xFFFF; 
    // LED 
    int fd2=open("/dev/mem",O_RDWR);
    if (fd2 < 1) {
        printf("could not open /dev/mem\n");
        exit(EXIT_FAILURE);
    }
    //led��ַ
    gpioled = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd2,0x10600000);
    // led��ȡ
    gpioled[1]=0x0000;
    // ���Ƶ��߼�(C ���Ա��)
    int ctr=1;
    unsigned int nums;
    scanf("%d",&ctr);
    switch (ctr)
    {
    case 1:
        while (1)
        {
            gpioled[0]=gpio[0];
        }
        break;
    case 2:
	nums=0x0004;
        gpioled[0]=nums;
        sleep(5);
        // ѭ������
        while (1)
        {
            if((nums&1)==1){
                nums=((nums>>1)&0xffff)+0x8000;
            }else{
                nums=(nums>>1)&0xffff;
            }
		gpioled[0]=nums;
            sleep(2);
        }
        break;
    	default:
        break;
    }
}
