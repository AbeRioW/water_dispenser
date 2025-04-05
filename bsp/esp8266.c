#include "esp8266.h"
#include "tim.h"


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
	char *timeset2 = "\r\n+IPD,0,5:set02";
	char *timeset3 = "\r\n+IPD,0,5:set03";
	char *timeset4 = "\r\n+IPD,0,5:set04";
	char *timeset5 = "\r\n+IPD,0,5:set05";
	char *timeset6 = "\r\n+IPD,0,5:set06";
	char *timeset7 = "\r\n+IPD,0,5:set07";
	char *timeset8 = "\r\n+IPD,0,5:set08";
	char *timeset9 = "\r\n+IPD,0,5:set09";
	char *timeset10 = "\r\n+IPD,0,5:set10";
	if(rx1_end_flag)
	{
		 //printf("HANDLE %s\r\n",uart2_rx);
			rx1_end_flag = false;
		
		  if(memcmp(uart1_rx,wifi_connect,9)==0)  //wifi已连接
			{
						//printf("yes\r\n");
						device_connect=true;
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


