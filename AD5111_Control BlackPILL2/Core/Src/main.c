/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fonts.h"
#include "ssd1306.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//Digital pot-----------------
int AD5111Pos = 0;
int AD5111GoalPos = 0;
int AD5111MaxPos = 127;
int AD5111MinPos = 0;
int Button_Status = 0;
int Button_UP = 0;
int Button_DOWN = 0;
int ButtonLightingTime = 0;
int BLT_Counter = 0;
int countClear = 0;
int Bussy = 0;
int EnableUP = 1 ;
int EnableDOWN = 1;
int32_t T1_PulseWide= 50; // 45us
int32_t T1_Periode = 1500; // 1.5ms
// OledDisplay -------------------------------
char Pos[5];

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */

void incValue(void);
void decValue(void);
void saveMem(void);
void AD5111_UpdateParamter(void);
void AD5111_GoToMax(void);
void AD5111_GoToMin(void);
void onceClearLCD(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  TIM1->CCR1 = T1_PulseWide;
  TIM1->ARR = T1_Periode;
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);
 // AD5111Pos = 32;
  //AD5111_GoToMin();
 // AD5111_UpdateParamter();
  AD5111Pos = 0;



  SSD1306_Init();

      SSD1306_GotoXY (0,0);
      SSD1306_Puts (" UV-SENSOR", &Font_11x18, 1);
      SSD1306_GotoXY (0, 30);
      SSD1306_Puts ("ePROGRAMMER", &Font_11x18, 1);
      SSD1306_UpdateScreen();
      HAL_Delay (1000);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//clear once display


// Button to change lighting time ----------------------
	  if (ButtonLightingTime ==1)
	  {
		  if (BLT_Counter < 3)
		  {
			  BLT_Counter++ ;
			  TIM1->CCR1 = T1_PulseWide*BLT_Counter;
		     }
		  else
		   {
			  BLT_Counter = 0;
		      }

		   ButtonLightingTime = 0;
	      }
//------------------------------------------------------
// increment by button up ------------------------------


	  if ( Button_UP == 1 && EnableUP == 1)
	  {

		  HAL_Delay(500);
		  if (HAL_GPIO_ReadPin(UP_GPIO_Port, UP_Pin) == 0) // if pressed than
		  {
			  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET);

			  AD5111_GoToMax();
			  AD5111_UpdateParamter();

			  SSD1306_GotoXY (0,0);
			  SSD1306_Puts ("Digital Resistor", &Font_7x10, 1);
			  SSD1306_GotoXY (0, 30);
			  SSD1306_Puts ("Set max.", &Font_11x18, 1);
			  SSD1306_UpdateScreen();
			  HAL_Delay (100);

		        }


		  Bussy = 1;
		  EnableDOWN = 1 ;
		  onceClearLCD();
		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET); // Toggle The Output (LED) Pin

		  //SSD1306_Clear();
          if ( AD5111Pos >= AD5111MaxPos)

          	  {

          		 SSD1306_GotoXY (0, 30);
          		 SSD1306_Puts ("Max.Reached", &Font_11x18, 1);
          		 SSD1306_UpdateScreen();
          		 HAL_Delay (100);
          		 EnableUP = 0 ;


          	      }


             else
                {
            	  AD5111Pos = AD5111Pos + 1;
            	  incValue();
            	  HAL_Delay(1);
            	  saveMem();
            	  HAL_Delay(100);
            	  itoa(AD5111Pos,Pos,10);
				  SSD1306_GotoXY (0,0);
				  SSD1306_Puts ("Digital Resistor", &Font_7x10, 1);
				  SSD1306_GotoXY (0,30);
				  SSD1306_Puts ("Step: ", &Font_11x18, 1);
				  SSD1306_GotoXY (66, 30);
				  SSD1306_Puts ("       ", &Font_11x18, 1);
				  SSD1306_UpdateScreen();
				  HAL_Delay(100);
				  SSD1306_GotoXY (66, 30);
				  SSD1306_Puts(Pos,&Font_11x18, 1);
				  SSD1306_UpdateScreen();
				  HAL_Delay(100);
                     }

		  Button_Status = 0;
		  Button_UP = 0;
		  Button_DOWN = 0;
		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);
		  Bussy = 0;


	  }
//////////////////////////-------------------------------
	  // decrement by button DOWN ------------------------------
	  	  if ( Button_DOWN == 1 && EnableDOWN == 1 )
	  	  {

	  		    HAL_Delay(500);
	  			if (HAL_GPIO_ReadPin(DOWN_GPIO_Port, DOWN_Pin) == 0) // if pressed than
	  				{
	  			      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET);

	  			      AD5111_GoToMin();
	  				  AD5111_UpdateParamter();

	  				  SSD1306_GotoXY (0,0);
	  				  SSD1306_Puts ("Digital Resistor", &Font_7x10, 1);
	  				  SSD1306_GotoXY (0, 30);
	  				  SSD1306_Puts ("Set min.", &Font_11x18, 1);
	  				  SSD1306_UpdateScreen();
	  				  HAL_Delay (100);

	  				        }


	  		  Bussy = 1;
	  		  EnableUP = 1 ;
	  		  onceClearLCD();

	  		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_RESET); // Toggle The Output (LED) Pin


	          if ( AD5111Pos <= AD5111MinPos)

	                    	  {

	                    		 SSD1306_GotoXY (0, 30);
	                    		 SSD1306_Puts ("Min.Reached", &Font_11x18, 1);
	                    		 SSD1306_UpdateScreen();
	                    		 HAL_Delay (100);
	                    	     EnableDOWN = 0 ;


	                    	 	    }
	          else
	          {
	          AD5111Pos = AD5111Pos -1;
	          decValue();
	          HAL_Delay(1);
	          saveMem();
	          HAL_Delay(100);
	          itoa(AD5111Pos,Pos,10);
	          SSD1306_GotoXY (0,0);
	          SSD1306_Puts ("Digital Resistor", &Font_7x10, 1);
	          SSD1306_GotoXY (0,30);
	          SSD1306_Puts ("Step: ", &Font_11x18, 1);
	          SSD1306_GotoXY (66, 30);
	          SSD1306_Puts ("       ", &Font_11x18, 1);
	          SSD1306_UpdateScreen();
	          HAL_Delay(100);
	          SSD1306_GotoXY (66, 30);
	          SSD1306_Puts(Pos,&Font_11x18, 1);
	  		  SSD1306_UpdateScreen();
	  		  HAL_Delay(100);
	             }

	  		 Button_Status = 0;
	  		 Button_UP = 0;
	  		 Button_DOWN = 0;
	  		 HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin,GPIO_PIN_SET);
	  		 Bussy = 0;

	  	  }
	  //////////////////////////-------------------------------

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 16;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1500;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, CLK_Pin|UP_DOWN_Pin|CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_Pin */
  GPIO_InitStruct.Pin = Button_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Button_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : CLK_Pin CS_Pin */
  GPIO_InitStruct.Pin = CLK_Pin|CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : UP_DOWN_Pin */
  GPIO_InitStruct.Pin = UP_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(UP_DOWN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : UP_Pin DOWN_Pin ButtonLightingTime_Pin */
  GPIO_InitStruct.Pin = UP_Pin|DOWN_Pin|ButtonLightingTime_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

void incValue(void)
{


	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_SET);

	HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);// here was cs to reset


	HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET);

	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);// here was cs to set
}
void decValue(void)
{

	    HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_RESET);

		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);// here was cs to reset


		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET);
		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_RESET);

		HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET);

		HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);// here was cs to set

}
void saveMem(void)
{

	HAL_GPIO_WritePin(CLK_GPIO_Port,CLK_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_RESET);

	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);

}
void shutDown(void)
{

	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(UP_DOWN_GPIO_Port,UP_DOWN_Pin,GPIO_PIN_SET);
}

void AD5111_UpdateParamter(void)
{
	int i =0;
	if (AD5111Pos <= AD5111GoalPos)
	{
		for (i = AD5111Pos; i <= AD5111GoalPos; i++)

		 {
			incValue();
			AD5111Pos = AD5111Pos +1;


		    }
	     }
	else if (AD5111Pos > AD5111GoalPos)
	 {
		for (i = AD5111Pos; i >= AD5111GoalPos; i--)

			{
				decValue();
				AD5111Pos = AD5111Pos -1;
					}
			    }


}

void AD5111_GoToMax(void)
{
	AD5111GoalPos = AD5111MaxPos;
	AD5111_UpdateParamter();
}

void AD5111_GoToMin(void)
{
	 AD5111GoalPos = AD5111MinPos;
     AD5111_UpdateParamter();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == Button_Pin && Bussy == 0) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
   // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); // Toggle The Output (LED) Pin
    Button_Status = 1 ;

   // HAL_ADC_Start_IT(&hadc1);

      }

    if(GPIO_Pin == UP_Pin && Bussy == 0) // If The INT Source Is EXTI Line9 (A9 Pin)
        {
           Button_UP = 1 ;


           //HAL_Delay(10);
          }

    if(GPIO_Pin == DOWN_Pin && Bussy == 0) // If The INT Source Is EXTI Line9 (A9 Pin)
         {
            Button_DOWN = 1 ;


           // HAL_Delay(10);

           }

    if(GPIO_Pin == ButtonLightingTime_Pin ) // If The INT Source Is EXTI Line9 (A9 Pin)
         {

    	     ButtonLightingTime = 1 ;

            // HAL_Delay(10);

           }


}


void onceClearLCD(void)
{
	if (countClear < 1 )
		  {
			  SSD1306_Clear();
			  countClear+=1;

		       }
}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
