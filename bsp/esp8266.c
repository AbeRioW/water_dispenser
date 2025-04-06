#include "esp8266.h"
#include "tim.h"
#include "stdlib.h"
#include "lcd_1602.h"


bool device_connect = false;


static  void handle_wifi_data(void);
bool start_esp8266(void)
{ 
	
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_MODE2),13,0xffff);
			HAL_Delay(1000);
      HAL_UART_Transmit(&huart1,(uint8_t*)(AT_SET),38,0xffff);
				HAL_Delay(1000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_RST),8,0xffff);
			HAL_Delay(4000);
			
			HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPMUX),13,0xffff);
			HAL_Delay(1000);
			
		  HAL_UART_Transmit(&huart1,(uint8_t*)(AT_CIPSERVER),21,0xffff);
			HAL_Delay(1000);
			
				__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);  
			HAL_UART_Receive_DMA(&huart1,uart1_rx,1000);   
			
			 return true;
			 
}

void handle_esp8266(void)
{
//	char *send = "hello\r\n";
	char *wifi_connect = "0,CONNECT";
	char *wifi_rec = "\r\n+IPD,";
	char *layon = "\r\n+IPD,0,5:layon";
	char *timeset2 = "\r\n+IPD,0,7:water01";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *timeset3 = "\r\n+IPD,0,7:water02";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *timeset4 = "\r\n+IPD,0,7:water03";  //\r\n+IPD,0,5:water02这里的02表示阈值
	
	
	char *tmp1 = "\r\n+IPD,0,5:tmp10";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *tmp2 = "\r\n+IPD,0,5:tmp20";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *tmp3 = "\r\n+IPD,0,5:tmp40";  //\r\n+IPD,0,5:water02这里的02表示阈值
		char *tmp4 = "\r\n+IPD,0,5:tmp40";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *tmp5 = "\r\n+IPD,0,5:tmp50";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *tmp6 = "\r\n+IPD,0,5:tmp60";  //\r\n+IPD,0,5:water02这里的02表示阈值
		char *tmp7 = "\r\n+IPD,0,5:tmp70";  //\r\n+IPD,0,5:water02这里的02表示阈值
	char *tmp8 = "\r\n+IPD,0,5:tmp80";  //\r\n+IPD,0,5:water02这里的02表示阈值
		char *tmp9 = "\r\n+IPD,0,5:tmp90";  //\r\n+IPD,0,5:water02这里的02表示阈值
	
	
	
//	char *timeset3 = "\r\n+IPD,0,5:tmp";    //\r\n+IPD,0,5:tmp03，这里的03表示阈值

	if(rx1_end_flag)
	{
		 //printf("HANDLE %s\r\n",uart2_rx);
			rx1_end_flag = false;
		
		  if(memcmp(uart1_rx,wifi_connect,9)==0)  //wifi已连接
			{
						//printf("yes\r\n");
						device_connect=true;
			}
			
		  if(memcmp(uart1_rx,timeset2,18)==0)  //水深的阈值
			{
						water_ban= 1;
			}
			
		  if(memcmp(uart1_rx,timeset3,18)==0)  //温度的阈值
			{
						water_ban= 2;
			}
			
			if(memcmp(uart1_rx,timeset4,18)==0)  //温度的阈值
			{
						water_ban= 3;
			}
			
			
			if(memcmp(uart1_rx,tmp1,16)==0)  //温度的阈值
			{
						temp_ban= 10;
			}
			
						if(memcmp(uart1_rx,tmp2,16)==0)  //温度的阈值
			{
						temp_ban= 20;
			}
			
						if(memcmp(uart1_rx,tmp3,16)==0)  //温度的阈值
			{
						temp_ban= 30;
			}
			
						if(memcmp(uart1_rx,tmp4,16)==0)  //温度的阈值
			{
						temp_ban= 40;
			}
			
						if(memcmp(uart1_rx,tmp5,16)==0)  //温度的阈值
			{
						temp_ban= 50;
			}
			
									if(memcmp(uart1_rx,tmp6,16)==0)  //温度的阈值
			{
						temp_ban= 60;
			}
			
			
									if(memcmp(uart1_rx,tmp7,16)==0)  //温度的阈值
			{
						temp_ban= 70;
			}
			
			
									if(memcmp(uart1_rx,tmp8,16)==0)  //温度的阈值
			{
						temp_ban= 80;
			}

												if(memcmp(uart1_rx,tmp9,16)==0)  //温度的阈值
			{
						temp_ban= 90;
			}
			
			rx1_count=0;
			memset(uart1_rx,0,UART1_SIZE);
			HAL_UART_Receive_DMA(&huart1,uart1_rx,UART1_SIZE);  //需要重新启动DMA
	} 
				
}

void send_wifi(char *data,int size)
{
	  int send_size=0;
	  if(size<10)
		{
			 send_size=1; 
		}
		
		if(size>10&&size<100)
		{
				send_size=2;
		}
		
			
	  if(device_connect)
		{
			char send_data[50]={0};
			char send_data1[50]={0};
			sprintf(send_data,"AT+CIPSEND=0,%d\r\n",size);
			HAL_UART_Transmit(&huart1,(uint8_t*)(send_data),15+send_size,0xffff);

			HAL_Delay(100);
//		  HAL_UART_Transmit(&huart2,(uint8_t*)(data),size,0xffff);
			HAL_UART_Transmit(&huart1,(uint8_t*)data,size,0xffff);
		}


}


