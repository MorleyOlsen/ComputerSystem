/*
 * main.c for the MIPSfpga core running on Nexys4 DDR board.
 *
 * This program:
 *   (1) reads the switches on the Nexys4 DDR board and 
 *   (2) flashes the value of the switches on the LEDs
 */

void delay();

int main() {
  // volatile：防止编译器优化变量
  volatile int *IO_iotr1= (int*)0xb0600004;
  volatile int *IO_iotr2= (int*)0xb0c00004;
  /*
    [virtual address for tris register]

    input is 1(f...f), output is 0(0...0), which are set later

    IO_iotr1 is 0 => output => LEDR
    IO_iotr2 is 1 => input => SWITCHES

    offset = 0x04
    [IO_iotr1] = [LEDR] + offset
    [IO_iotr2] = [SWITCHES] + offset
  */

  volatile int *IO_SWITCHES = (int*)0xb0c00000;
  volatile int *IO_LEDR = (int*)0xb0600000;
  /*
    [virtual addresses for data register]

    io_switches：拨码开关的存储映射IO地址
    io_ledr：LED RED灯的存储映射IO地址
    只是给定了GPIO的输入&&输出地址，控制地址需要确定

    返回低16位：拨码开关的值
  */
  volatile unsigned int switches;
  
  *IO_iotr1=0x00000000;
  *IO_iotr2=0xffffffff;
  /*
    32bit
    IO_iotr1 is input
    IO_iotr2 is output
  */

  while (1) {
    switches =*IO_SWITCHES; // 读取拨码开关地址，确定开关状态
    delay();                // 延时
    *IO_LEDR = switches;    // 显示值到LED灯上
    *IO_LEDR = 0;           // turn off LEDs
    delay();                // 延时
  }
  return 0;
}

void delay() {
  volatile unsigned int j;
  // delay for 100,0000 times
  for (j = 0; j < (1000000); j++) ;	// delay 
}