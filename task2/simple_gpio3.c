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
//����оƬ����:
#define SYSFS_GPIO_EXPORT           "/sys/class/gpio/export"  
#define SYSFS_GPIO_RST_DIR_VAL      "out"  
#define SYSFS_GPIO_RST_VAL_H        "1"
#define SYSFS_GPIO_RST_VAL_L        "0"



char SYSFS_GPIO_RST_PIN_VAL[16][25] = { //�˿ں�
	"505", "504", "503", "502", "501", 
	"500", "499", "498", "497", "496",
	"495", "494", "493", "492", "491", "490"};  
char SYSFS_GPIO_RST_DIR[16][50]={       //�˿ڷ��򣨿�������IN�������OUT��
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
	
char SYSFS_GPIO_RST_VAL[16][50] = {  //�˿�ֵ�ļ�
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

//�������ܣ�ʮ����ת���ɶ������������ַ���
void int_to_chars(char* str) {			
	int num = gpio[0];
	for (int i = 15; i >= 0; i--) {
		str[i] = num % 2 + '0';
		num /= 2;
	}
}

//�򿪿����豸�������ÿ��Ʒ���
int init() {							
	int fd = open("/dev/uio0", O_RDWR);
	if (fd < 1) {
		printf("could not open /dev/uio0\n");
		exit(EXIT_FAILURE);
	} 
	
	//�����ַ�������ַ��ӳ��
	gpio = (unsigned int *)mmap(NULL, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	//Controller���������п��أ�4λ16��������Ӧ16��������λ��16�����أ�
	gpio[1]  = 0xFFFF; 	
	int result = gpio[0]; //���ص�ֵ
	
	//�ر��豸
	close(fd); 
	
	return result;
}

//��ʼ����ȫ�� 
int offled() {
	for(int i = 0; i < 16; i++)	{		
	    //�򿪶˿�/sys/class/gpio# echo 48 > export
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
        
	    //���ö˿ڷ���/sys/class/gpio/gpio48# echo out > direction
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

        //��ȫ����ʼ��ΪϨ�� 
	    char *temp_str2;
	    temp_str2=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
	    strncpy(temp_str2,SYSFS_GPIO_RST_VAL[i],sizeof(SYSFS_GPIO_RST_VAL[i]));	///sys/class/gpio/gpio505/value
	    fd = open(temp_str2, O_RDWR);
        if(fd == -1) {
           	printf("ERR: Radio hard reset pin value open error.\n");
         	return EXIT_FAILURE;
        }   
        write(fd, SYSFS_GPIO_RST_VAL_L, sizeof(SYSFS_GPIO_RST_VAL_L));		//0�͵�ƽ 
        close(fd);
	}
} 

//������
int onled(char* number){							 
	int i=0;
	char *temp_str2;
	for(i = 0; i < 16; i++) {
		if(number[i] == '1'){	//�����λ����Ϊ1�����Ӧ�ĵ���Ҫ���� 
			int fd;
			temp_str2=(char*)malloc(sizeof(char)*(sizeof(SYSFS_GPIO_RST_VAL[i])+1));
			strncpy(temp_str2, SYSFS_GPIO_RST_VAL[i], sizeof(SYSFS_GPIO_RST_VAL[i])); 
	        fd = open(temp_str2, O_RDWR);
	        if(fd == -1){
                   printf("ERR: Radio hard reset pin value open error.\n");
                   return EXIT_FAILURE;
	        }
	        write(fd, SYSFS_GPIO_RST_VAL_H, sizeof(SYSFS_GPIO_RST_VAL_H));  //�ߵ�ƽ
	        close(fd);
		}

		else{				//Ϊ0��õ��� 
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

void light(char* number) {					//����ƴ���
	int k = 0;
	while(k <= 99) {
		onled(number);				//������ 		
        //����������ߵ�һλ�������һλ������ȡ����ֵ 
		char temp = number[15];
		for(int i = 15; i > 0; i--) {
			number[i] = number[i-1];
		}
		number[0] = temp;
		usleep(2000000); //����ά��2�룬��λ��΢��
		k++;
	}
}  

int main() {
    int sw = 0;	
	char number[16] = "0000000000000111"; 	//���(7)
	sw = init();  //�򿪿����豸�������ÿ��Ʒ���
//    int_to_chars(number);   //����ת������gpio����ŵ�number
    onled(number);  //������ 
	light(number);  //�����
	return 0;
}  
