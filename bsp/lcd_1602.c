#include "lcd_1602.h"
int time_set = 5;


/**
  * @brief  Reads the specified GPIO output data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @retval GPIO output data port value.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @brief  Writes data to the specified GPIO data port.
  * @param  GPIOx: where x can be (A..G) to select the GPIO peripheral.
  * @param  PortVal: specifies the value to be written to the port output data register.
  * @retval None
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}


void LCD_WRITE_CMD(uint8_t cmd)
{
	
	 HAL_GPIO_WritePin(GPIOC, LCD_RS_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_RW_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);
	  HAL_Delay(5);
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // ��������
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);

}

void LCD_WRITE_DATA(uint8_t cmd)
{
	
	 HAL_GPIO_WritePin(GPIOC, LCD_RS_Pin, GPIO_PIN_SET);
	 HAL_GPIO_WritePin(GPIOC, LCD_RW_Pin, GPIO_PIN_RESET);
	 HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);
	  HAL_Delay(5);
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // ��������
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_SET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(GPIOC, LCD_EC_Pin, GPIO_PIN_RESET);

}


void LCD_INIT(void)
{

	
	LCD_WRITE_CMD(0x38);
	LCD_WRITE_CMD(0x0c);
	LCD_WRITE_CMD(0x06);
	LCD_WRITE_CMD(0x01);

}

void LCD_CLR(void)
{
	LCD_WRITE_CMD(0x01);
}


void lcd1602_show_string(uint8_t x,uint8_t y,char *str)
{
	uint8_t i=0;

	if(y>1||x>15)return;//���в���������ǿ���˳�

	if(y<1)	//��1����ʾ
	{	
		while(*str!='\0')//�ַ�������'\0'��β��ֻҪǰ�������ݾ���ʾ
		{
			if(i<16-x)//����ַ����ȳ�����һ����ʾ��Χ�����ڵڶ��м�����ʾ
			{
				LCD_WRITE_CMD(0x80+i+x);//��һ����ʾ��ַ����	
			}
			else
			{
				LCD_WRITE_CMD(0x40+0x80+i+x-16);//�ڶ�����ʾ��ַ����	
			}
			LCD_WRITE_DATA(*str);//��ʾ����
			str++;//ָ�����
			i++;	
		}	
	}
	else	//��2����ʾ
	{
		while(*str!='\0')
		{
			if(i<16-x) //����ַ����ȳ����ڶ�����ʾ��Χ�����ڵ�һ�м�����ʾ
			{
				LCD_WRITE_CMD(0x80+0x40+i+x);	
			}
			else
			{
				LCD_WRITE_CMD(0x80+i+x-16);	
			}
			LCD_WRITE_DATA(*str);
			str++;
			i++;	
		}	
	}				
}
