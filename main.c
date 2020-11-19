/**
  ******************************************************************************
  * @Author         [foruzan fallah]
  * @StudentNumber  [9323054]
  * @ProjectName    Smart Home
  * @Date           18-January-2018
  * @brief          Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void init_gpio(void);
void delay(uint64_t);

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
	/* Add your variables here */
	uint32_t temp;
	float tempp;
	float lightt;
	float humidd;
	float day;
	float night;
	float fan;
	float fan_speed;
	float heater;
	float pc0;
	float pc1;
	float pc2;
	float pc3;
	float pc6;
	float pc7;
	float lamp;
	float curtain;
	float steam;
	float speed;
	int i;
	float sum_humidd;
	float sum_lightt;
	float sum_tempp;
	float array_tempp[10];
  float array_lightt[10];
  float array_humidd[10];
	/* Add your application code here */
	init_gpio();

	
  /* Infinite loop */
  while (1)
  {	
		
		for (i=0;i<=9;i++)  // tashkil halghe be manzor araye 10 taei baraye soal 3 , 4  
		{
		//GPIOA ->ODR &= 0X000000FF;
			tempp=(temp/2)-38;   //formol baraye dama
			tempp = GPIOA->ODR  & 0X000000FF;  
			GPIOA->IDR = (uint16_t)tempp;
			
			lightt =temp*32; //formol baraye noor
			lightt =GPIOA->ODR & 0X0000FF00;
			GPIOA->IDR = (uint16_t)lightt;
			 
			if(temp <= 200) //formol baraye rotubat
				humidd=temp/2;
			else humidd=0;
			
			humidd=GPIOB->ODR & 0X000000FF;
			GPIOB->IDR = (uint16_t)humidd;
			
			
			night =!day;  // taein rooz va shab
			day=GPIOB->ODR & 0X00000100;
			GPIOB->IDR = (uint16_t)day;
			
	/////sarma va garma 
	pc0=temp & 0X00000001;
	pc1=temp & 0X00000002;
			
	if((pc0==0 & pc1==0 )||(pc0==1 & pc1==0))
	fan_speed=0;
	else if(pc0==0 & pc1==1 )
	fan_speed=100;
	else fan_speed=200;
	fan=GPIOB->IDR & 0X00000003;
	GPIOB->ODR = (uint16_t)fan;
	
	heater=!(pc0 && pc1);
	heater=GPIOB->IDR & 0X00000003;
	GPIOB->ODR = (uint16_t)heater;
	
	//////lamps
		pc2=temp & 0X00000004;
		pc3=temp & 0X00000008;
	lamp= (pc2 || pc3);
	lamp=GPIOB->IDR & 0X0000000C;
	GPIOB->ODR = (uint16_t)lamp;
	
	//////curtain

	curtain= 1-(temp/3);
	curtain=GPIOB->IDR & 0X00000030;
	GPIOB->ODR = (uint16_t)curtain;
	
	
  //////steam
	pc6=temp & 0X00000040;
	pc7=temp & 0X00000080;
	if(pc7==0 & pc6==0)
		speed=0;
	else if(pc7==0 & pc6==1)
		speed=100;
	else if(pc7==1 & pc6==0)
		speed=200;
	else speed=300;
	steam=GPIOB->IDR & 0X000000C0;
	GPIOB->ODR = (uint16_t)steam;
	
	// jam kardan maghadir araye ha baraye miangin
	array_tempp[i]= tempp; 
	 sum_tempp+=array_tempp[i];
	
		array_lightt[i]= lightt;
	sum_lightt+=array_lightt[i];
	
		array_humidd[i]= humidd;
	sum_humidd+=array_humidd[i];
	
	
	// halghe bozorg baraye i=0 ya mokhalef 0 ast 
	//halghe badi agar dar soal shab va rooz bekhahad ast agar na tebgh jadval ast
	
	// command temprature sensor Q3
	
	if(i==0)
	{
										if(array_tempp[0]>15 & array_tempp[9]<15) //3 . a 
										heater=0;
									else if(array_tempp[0]>30 & array_tempp[9]<30) //3 . a 
										fan_speed=100;
									else if(array_tempp[0]>45 & array_tempp[9]<45) //3 . a 
										fan_speed=200;
									else if(array_tempp[0]<40 & array_tempp[9]>40) //3 . a 
										fan_speed=100;
									else if(array_tempp[0]<25 & array_tempp[9]>25) //3 . a
										heater=0;
									else if(array_tempp[0]<10 & array_tempp[9]>10) //3 . a 
										heater=1;
	}
		else
		{
										if(array_tempp[i]>15 & array_tempp[i-1]<15) //3 . a 
									heater=0;
								else if(array_tempp[i]>30 & array_tempp[i-1]<30) //3 . a 
									fan_speed=100;
								else if(array_tempp[i]>45 & array_tempp[i-1]<45) //3 . a 
									fan_speed=200;
								else if(array_tempp[i]<40 & array_tempp[i-1]>40) //3 . a 
									fan_speed=100;
								else if(array_tempp[i]<25 & array_tempp[i-1]>25) //3 . a
									heater=0;
								else if(array_tempp[i]<10 & array_tempp[i-1]>10) //3 . a 
									heater=1;
		}
	
		

// command LIGHT sensor Q3

		if(i==0)
			{
				if (night==1)
					{
										if ( array_lightt[0] > 2000 & array_lightt[9]<2000 )
											lamp=2;
										else if ( array_lightt[0] > 4000 & array_lightt[9]<4000 )
											lamp=1;
										else if ( array_lightt[0] > 6000 & array_lightt[9]<6000 )
											lamp=0;
										else if ( array_lightt[0] >5000 & array_lightt[9]>5000 )
											lamp=1;
										else if ( array_lightt[0] >3000 & array_lightt[9]>3000 )
											lamp=2;
										else if ( array_lightt[0] >1000 & array_lightt[9]>1000 )
											lamp=3;
					}
				else if(day==1) 
				 {
										if ( array_lightt[0] > 2000 & array_lightt[9]<2000 )
											curtain=1/3;
										else if ( array_lightt[0] > 4000 & array_lightt[9]<4000 )
											curtain=2/3;
										else if ( array_lightt[0] > 6000 & array_lightt[9]<6000 )
											curtain=1;
										else if ( array_lightt[0] >5000 & array_lightt[9]>5000 )
											curtain=2/3;
										else if ( array_lightt[0] >3000 & array_lightt[9]>3000 )
											curtain=1/3;
										else if ( array_lightt[0] >1000 & array_lightt[9]>1000 )
											curtain=0;
			   }
				
				
				
			}
			else          
			{
				if (day==1)
			 {
										if ( array_lightt[i] > 2000 & array_lightt[i-1]<2000 )
											curtain=1/3;
										else if ( array_lightt[i] > 4000 & array_lightt[i-1]<4000 )
											curtain=2/3;
										else if ( array_lightt[i] > 6000 & array_lightt[i-1]<6000 )
											curtain=1;
										else if ( array_lightt[i] >5000 & array_lightt[i-1]>5000 )
											curtain=2/3;
										else if ( array_lightt[i] >3000 & array_lightt[i-1]>3000 )
										curtain=1/3;
										else if ( array_lightt[i] >1000 & array_lightt[i-1]>1000 )
											curtain=0;
				}
			else if(night==1) 
				 {
										if ( array_lightt[i] > 2000 & array_lightt[i-1]<2000 )
											lamp=2;
										else if ( array_lightt[i] > 4000 & array_lightt[i-1]<4000 )
											lamp=1;
										else if ( array_lightt[i] > 6000 & array_lightt[i-1]<6000 )
											lamp=0;
										else if ( array_lightt[i] >5000 & array_lightt[i-1]>5000 )
											lamp=1;
										else if ( array_lightt[i] >3000 & array_lightt[i-1]>3000 )
											lamp=2;
										else if ( array_lightt[i] >1000 & array_lightt[i-1]>1000 )
											lamp=3;
			   }
				
				
			}
			
		// command humidd sensor Q3
			
			if(i==0)
	{
		if(array_humidd[0]>25 & array_humidd[9]<25) //3 . c 
		speed=200;
	else if(array_humidd[0]>50 & array_humidd[9]<50) //3 . c 
		speed=100;
	else if(array_humidd[0]>75 & array_humidd[9]<75) //3 . c
		speed=0;
	else if(array_humidd[0]<65& array_humidd[9]>65) //3 . c 
		speed=100;
	else if(array_humidd[0]<40 & array_humidd[9]>40) //3 . c
		speed=200;
	else if(array_humidd[0]<15 & array_humidd[9]>15) //3 . c 
		speed=300;
	}
		else
		{
		if(array_humidd[i]>25 & array_humidd[i-1]<25) //3 . c 
		speed=200;
	else if(array_humidd[i]>50 & array_humidd[i-1]<50) //3 . c 
		speed=100;
	else if(array_humidd[i]>75 & array_humidd[i-1]<75) //3 . c
		speed=0;
	else if(array_humidd[i]<65& array_humidd[i-1]>65) //3 . c 
		speed=100;
	else if(array_humidd[i]<40 & array_humidd[i-1]>40) //3 . c
		speed=200;
	else if(array_humidd[i]<15 & array_humidd[i-1]>15) //3 . c 
		speed=300;
		}
	
		

			// takhir baraye in ast ke ma har 1 daghighe nemone bekhanim
		delay(60);
	
}
// miangin gereftan baraye soal 4
sum_tempp=sum_tempp/10;
sum_lightt=sum_lightt/10;
sum_humidd=sum_humidd/10;

		delay(5);
  }	
}

void init_gpio(void)
{
	// gam aval 
	RCC->AHB1ENR |= 0x0000001F;     //Enable GPIOA, GPIOB, GPIOC, GPIOD and GPIOE clock in AHB1
	//a2
	GPIOA->MODER = 0x00000000;
	GPIOA->OSPEEDR = 0x55555555;
	GPIOA->PUPDR = 0x00000000;
	//a3
	GPIOB->MODER = 0x00000000;
	GPIOB->OSPEEDR = 0xAAAAAAAA;
	GPIOB->PUPDR = 0x00000000;
	//a4
	GPIOC->MODER = 0x55555555;
	GPIOC->OSPEEDR = 0xFFFFFFFF;
	GPIOC->PUPDR = 0x55555555;
	GPIOC->OTYPER &=0xFFFF0000;
	//a5
	GPIOD->MODER = 0x55555555;
	GPIOD->OSPEEDR = 0x00000000;
	GPIOD->PUPDR = 0xAAAAAAAA;
	GPIOD->OTYPER &=0xFFFF0000;
	//a6-7
	GPIOE->MODER = 0xFFFFAAAA;
	GPIOE->OSPEEDR = 0xAAAAFFFF;
	GPIOE->PUPDR = 0x00000000;
	
	/* example for initial IO port
	GPIOA->MODER = 0x00000000;
	GPIOA->OSPEEDR = 0x00000000;
	GPIOA->PUPDR = 0x00000000;

	GPIOC->MODER = 0x55555555;
	GPIOC->OTYPER &= 0xFFFF0000;
	GPIOC->OSPEEDR = 0x00000000;
	GPIOC->PUPDR = 0x00000000;
	*/
}

void delay(uint64_t n)
{
	n *= 1000000; 
	while(n != 0){n -= 1;}
}
