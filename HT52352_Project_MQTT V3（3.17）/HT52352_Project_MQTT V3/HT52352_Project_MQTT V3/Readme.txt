注意：路径一定要英文

2021.3.29  代码框架修订，初版。时间空闲情况下,还可进一步完善,拟添加消息处理机制（可参考作者B站HAL库视频）

2021.4.2
添加ringbuffer.c  环形队列
bsp_systick.h 添加Uart1RecTimer  Usart0RecTimer宏定义
config.h中添加system_message类型
system.c中添加初始化消息环形队列
修订bsp_uart1.c  bsp_usart0 ，添加串口数据接收处理
task.c添加消息处理机制、uart1_task_timer_callback、uart1_task_timer_callback

还可进一步完善......



Printf串口输出重定向（ PA4:TX, PA5:RX ）
Uart1( PB4:TX, PB5:RX )

wifi
Usart0( PA2:TX, PA3:RX )
WiFi_ResetIO---PA0

LED1----PC14
LED2----PC15
EXTI CH8-------PA8

SPI:
SPI1_MOSI----PC3
SPI1_SCLK----PC2
SPI1_MISO----PB6

//可调电源
I2C_SCL-----PC14
I2C_SDA-----PC15

DS18B20----PA1


SCTM0 CH0 产生PWM(1Hz)----PD0

GPTM  CH1V输入捕获  ---PC5



串口屏 UART0 
TX:PC4,RX:PC5

蓝牙
USART1  PA4TX PA5RX





