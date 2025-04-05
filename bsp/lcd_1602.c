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
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // 发送数据
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
	
	
	GPIO_Write(GPIOA, (GPIO_ReadOutputData(GPIOA) & 0xFF00) | cmd); // 发送数据
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

	if(y>1||x>15)return;//行列参数不对则强制退出

	if(y<1)	//第1行显示
	{	
		while(*str!='\0')//字符串是以'\0'结尾，只要前面有内容就显示
		{
			if(i<16-x)//如果字符长度超过第一行显示范围，则在第二行继续显示
			{
				LCD_WRITE_CMD(0x80+i+x);//第一行显示地址设置	
			}
			else
			{
				LCD_WRITE_CMD(0x40+0x80+i+x-16);//第二行显示地址设置	
			}
			LCD_WRITE_DATA(*str);//显示内容
			str++;//指针递增
			i++;	
		}	
	}
	else	//第2行显示
	{
		while(*str!='\0')
		{
			if(i<16-x) //如果字符长度超过第二行显示范围，则在第一行继续显示
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
