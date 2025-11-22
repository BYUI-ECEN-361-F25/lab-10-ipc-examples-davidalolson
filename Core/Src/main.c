/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : ECEN-361-Lab-09
  * IPC examples
  *
  * BYU-Idaho
  * Fall-2023 :   Lynn Watson
  *
  * See the  STUDENTS EDITABLE places below to modify for submission
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "MultiFunctionShield.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define B1  1
#define B2  2
#define B3  4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim17;

UART_HandleTypeDef huart2;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for NotifyToggle */
osThreadId_t NotifyToggleHandle;
const osThreadAttr_t NotifyToggle_attributes = {
  .name = "NotifyToggle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for SW_Timer_Toggle */
osThreadId_t SW_Timer_ToggleHandle;
const osThreadAttr_t SW_Timer_Toggle_attributes = {
  .name = "SW_Timer_Toggle",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Mutex_CountUp */
osThreadId_t Mutex_CountUpHandle;
const osThreadAttr_t Mutex_CountUp_attributes = {
  .name = "Mutex_CountUp",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Mutex_CountDown */
osThreadId_t Mutex_CountDownHandle;
const osThreadAttr_t Mutex_CountDown_attributes = {
  .name = "Mutex_CountDown",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for UpdateGlobDisp */
osThreadId_t UpdateGlobDispHandle;
const osThreadAttr_t UpdateGlobDisp_attributes = {
  .name = "UpdateGlobDisp",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ResetGlobal */
osThreadId_t ResetGlobalHandle;
const osThreadAttr_t ResetGlobal_attributes = {
  .name = "ResetGlobal",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for DebounceTask */
osThreadId_t DebounceTaskHandle;
const osThreadAttr_t DebounceTask_attributes = {
  .name = "DebounceTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for semaphore_d3 */
osThreadId_t semaphore_d3Handle;
const osThreadAttr_t semaphore_d3_attributes = {
  .name = "semaphore_d3",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for semaphore_d4 */
osThreadId_t semaphore_d4Handle;
const osThreadAttr_t semaphore_d4_attributes = {
  .name = "semaphore_d4",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Mutex_Line */
osThreadId_t Mutex_LineHandle;
const osThreadAttr_t Mutex_Line_attributes = {
  .name = "Mutex_Line",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SW_Timer_7Seg */
osTimerId_t SW_Timer_7SegHandle;
const osTimerAttr_t SW_Timer_7Seg_attributes = {
  .name = "SW_Timer_7Seg"
};
/* Definitions for SW_Timer_Refresh */
osTimerId_t SW_Timer_RefreshHandle;
const osTimerAttr_t SW_Timer_Refresh_attributes = {
  .name = "SW_Timer_Refresh"
};
/* Definitions for UpDownMutex */
osMutexId_t UpDownMutexHandle;
const osMutexAttr_t UpDownMutex_attributes = {
  .name = "UpDownMutex"
};
/* Definitions for Button_1_Semaphore */
osSemaphoreId_t Button_1_SemaphoreHandle;
const osSemaphoreAttr_t Button_1_Semaphore_attributes = {
  .name = "Button_1_Semaphore"
};
/* Definitions for Button_2_Semaphore */
osSemaphoreId_t Button_2_SemaphoreHandle;
const osSemaphoreAttr_t Button_2_Semaphore_attributes = {
  .name = "Button_2_Semaphore"
};
/* Definitions for Button_3_Semaphore */
osSemaphoreId_t Button_3_SemaphoreHandle;
const osSemaphoreAttr_t Button_3_Semaphore_attributes = {
  .name = "Button_3_Semaphore"
};
/* Definitions for Semaphore_Counting */
osSemaphoreId_t Semaphore_CountingHandle;
const osSemaphoreAttr_t Semaphore_Counting_attributes = {
  .name = "Semaphore_Counting"
};
/* USER CODE BEGIN PV */
osSemaphoreId_t SwitchToDebounce;  // Type has to be in here because the GUI auto

uint8_t countdown_display = 9;

/*
 * This is a global variable with multiple writers
 * But should not change, because all writers must have the mutex
 *
 */
#define Protected_Count_Initial_Value 50
uint8_t mutex_protected_count = Protected_Count_Initial_Value ;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM17_Init(void);
void Default_Task(void *argument);
void NotifyToggleTask(void *argument);
void SW_Timer_Task(void *argument);
void Mutex_CountUpTask(void *argument);
void Mutex_CountDownTask(void *argument);
void UpdateGlobDisplayProcess(void *argument);
void ResetGlobalTask(void *argument);
void StartDebounce(void *argument);
void semaphore_toggle_d3_task(void *argument);
void semaphore_toggle_d4_task(void *argument);
void multex_display_line_task(void *argument);
void SW_Timer_Countdown(void *argument);
void SW_Timer_DisplayRefresh(void *argument);

/* USER CODE BEGIN PFP */

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
  MX_USART2_UART_Init();
  MX_TIM17_Init();
  /* USER CODE BEGIN 2 */
  printf("\033\143"); printf("Welcome to ECEN-361 Lab-10 \n\r");
	// Start timer
	MultiFunctionShield_Clear();							// Clear the 7-seg display
	HAL_TIM_Base_Start_IT(&htim17);							// LED SevenSeg cycle thru them
	Clear_LEDs();


  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();
  /* Create the mutex(es) */
  /* creation of UpDownMutex */
  UpDownMutexHandle = osMutexNew(&UpDownMutex_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of Button_1_Semaphore */
  Button_1_SemaphoreHandle = osSemaphoreNew(1, 0, &Button_1_Semaphore_attributes);

  /* creation of Button_2_Semaphore */
  Button_2_SemaphoreHandle = osSemaphoreNew(1, 0, &Button_2_Semaphore_attributes);

  /* creation of Button_3_Semaphore */
  Button_3_SemaphoreHandle = osSemaphoreNew(1, 0, &Button_3_Semaphore_attributes);

  /* creation of Semaphore_Counting */
  Semaphore_CountingHandle = osSemaphoreNew(31, 31, &Semaphore_Counting_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of SW_Timer_7Seg */
  SW_Timer_7SegHandle = osTimerNew(SW_Timer_Countdown, osTimerPeriodic, NULL, &SW_Timer_7Seg_attributes);

  /* creation of SW_Timer_Refresh */
  SW_Timer_RefreshHandle = osTimerNew(SW_Timer_DisplayRefresh, osTimerPeriodic, NULL, &SW_Timer_Refresh_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  osTimerStart(SW_Timer_RefreshHandle, pdMS_TO_TICKS(5));
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(Default_Task, NULL, &defaultTask_attributes);

  /* creation of NotifyToggle */
  NotifyToggleHandle = osThreadNew(NotifyToggleTask, NULL, &NotifyToggle_attributes);

  /* creation of SW_Timer_Toggle */
  SW_Timer_ToggleHandle = osThreadNew(SW_Timer_Task, NULL, &SW_Timer_Toggle_attributes);

  /* creation of Mutex_CountUp */
  Mutex_CountUpHandle = osThreadNew(Mutex_CountUpTask, NULL, &Mutex_CountUp_attributes);

  /* creation of Mutex_CountDown */
  Mutex_CountDownHandle = osThreadNew(Mutex_CountDownTask, NULL, &Mutex_CountDown_attributes);

  /* creation of UpdateGlobDisp */
  UpdateGlobDispHandle = osThreadNew(UpdateGlobDisplayProcess, NULL, &UpdateGlobDisp_attributes);

  /* creation of ResetGlobal */
  ResetGlobalHandle = osThreadNew(ResetGlobalTask, NULL, &ResetGlobal_attributes);

  /* creation of DebounceTask */
  DebounceTaskHandle = osThreadNew(StartDebounce, NULL, &DebounceTask_attributes);

  /* creation of semaphore_d3 */
  semaphore_d3Handle = osThreadNew(semaphore_toggle_d3_task, NULL, &semaphore_d3_attributes);

  /* creation of semaphore_d4 */
  semaphore_d4Handle = osThreadNew(semaphore_toggle_d4_task, NULL, &semaphore_d4_attributes);

  /* creation of Mutex_Line */
  Mutex_LineHandle = osThreadNew(multex_display_line_task, NULL, &Mutex_Line_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init(void)
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 800-1;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 100;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_D1_Pin|LED_D2_Pin|LED_D3_Pin|SevenSeg_CLK_Pin
                          |SevenSeg_DATA_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SevenSeg_LATCH_Pin|LED_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LM35_IN_Pin */
  GPIO_InitStruct.Pin = LM35_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(LM35_IN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : Button_1_Pin Button_2_Pin */
  GPIO_InitStruct.Pin = Button_1_Pin|Button_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_D1_Pin LED_D2_Pin LED_D3_Pin */
  GPIO_InitStruct.Pin = LED_D1_Pin|LED_D2_Pin|LED_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : Button_3_Pin */
  GPIO_InitStruct.Pin = Button_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button_3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : SevenSeg_CLK_Pin SevenSeg_DATA_Pin */
  GPIO_InitStruct.Pin = SevenSeg_CLK_Pin|SevenSeg_DATA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : SevenSeg_LATCH_Pin */
  GPIO_InitStruct.Pin = SevenSeg_LATCH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SevenSeg_LATCH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_D4_Pin */
  GPIO_InitStruct.Pin = LED_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_D4_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
	{
	//BaseType_t pxH ;
	uint32_t pxH = 0 ;
	// All three buttons generate GPIO  interrupts
	switch(GPIO_Pin)
		{
		case Button_1_Pin:
			pxH = 1;
			xTaskNotifyFromISR(DebounceTaskHandle, B1, eSetBits, (BaseType_t*) pxH );
			break  ;

		case Button_2_Pin:
			pxH = 2;
			xTaskNotifyFromISR(DebounceTaskHandle, B2, eSetBits, (BaseType_t*) pxH );
			break;

		case Button_3_Pin:
			pxH = 3;
			xTaskNotifyFromISR(DebounceTaskHandle, B3, eSetBits, (BaseType_t*) pxH );
			// srand((unsigned) uwTick );
			// osSemaphoreRelease(Button_3_SemaphoreHandle);
			break;
		}

	}




/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}


int random_wait(int min)
	{
	/* Return a random number between 200 - 300
	 * Meant to be mS for the count up or count down in the protected
	 * mutex demonstration routines
	 */
	int rand_millisec = min + (rand() % 99);
	return rand_millisec;

	}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_Default_Task */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Default_Task */
void Default_Task(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_NotifyToggleTask */
/**
* @brief Function implementing the NotifToggle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_NotifyToggleTask */
void NotifyToggleTask(void *argument)
{
  /* USER CODE BEGIN NotifyToggleTask */
  /* Infinite loop */
 // unsigned int *p = 0;
 uint32_t *p = 0 ;
  for(;;)
  {
	xTaskNotifyWait( 0,  0, p, osWaitForever);
	HAL_GPIO_TogglePin(LED_D2_GPIO_Port , LED_D2_Pin);
    osDelay(20);
  }
  /* USER CODE END NotifyToggleTask */
}

/* USER CODE BEGIN Header_SW_Timer_Task */
/**
* @brief Function implementing the SW_Timer_Toggle thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_SW_Timer_Task */
void SW_Timer_Task(void *argument)
{
  /* USER CODE BEGIN SW_Timer_Task */
  /* Infinite loop */
  for(;;)
  {
	osSemaphoreAcquire(Button_2_SemaphoreHandle,osWaitForever);
	  // A button push starts or stops the SW Timer
	  // Button push is indicated by the semaphore
	if (osTimerIsRunning(SW_Timer_7SegHandle))
		osTimerStop(SW_Timer_7SegHandle );
	else
		osTimerStart(SW_Timer_7SegHandle , 20);
    osDelay(1);
  }
  /* USER CODE END SW_Timer_Task */
}

/* USER CODE BEGIN Header_Mutex_CountUpTask */
/**
* @brief Function implementing the Mutex_CountUp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Mutex_CountUpTask */
void Mutex_CountUpTask(void *argument)
{
  /* USER CODE BEGIN Mutex_CountUpTask */
  /* Infinite loop */
  for(;;)
  {
	  osMutexWait(UpDownMutexHandle,osWaitForever);
	  // Once we have it, we can start counting Up
	  // THe count up will be some random between 200 - 300 mS

	if (mutex_protected_count<99)
		mutex_protected_count++;
	else
		mutex_protected_count=0;
	// Done writing, so give back the mutex
	osMutexRelease(UpDownMutexHandle);
    osDelay(random_wait(300));
  }
  /* USER CODE END Mutex_CountUpTask */
}

/* USER CODE BEGIN Header_Mutex_CountDownTask */
/**
* @brief Function implementing the Mutex_CountDown thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Mutex_CountDownTask */
void Mutex_CountDownTask(void *argument)
{
  /* USER CODE BEGIN Mutex_CountDownTask */
  /* Infinite loop */
  for(;;)
  {
	  osMutexWait(UpDownMutexHandle,osWaitForever);
	  if (mutex_protected_count<1)
	  		mutex_protected_count=99;
	  	else
	  		mutex_protected_count--;
	  	// Done writing, so give back the mutex
		osMutexRelease(UpDownMutexHandle);
		osDelay(random_wait(200));
  }
  /* USER CODE END Mutex_CountDownTask */
}

/* USER CODE BEGIN Header_UpdateGlobDisplayProcess */
/**
* @brief Function implementing the UpdateGlobDisp thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_UpdateGlobDisplayProcess */
void UpdateGlobDisplayProcess(void *argument)
{
  /* USER CODE BEGIN UpdateGlobDisplayProcess */
  /* Infinite loop
   * This just puts the value of the GlobalVariable on the
   * right-most two-digits of the 7Seg Display */

  for(;;)
	  {
	  MultiFunctionShield_Display_Two_Digits(mutex_protected_count);
	  osDelay(150);	// The competing process to inc/dec are 200 - 300mS
					// So show this a bit faster
	  }
  /* USER CODE END UpdateGlobDisplayProcess */
}

/* USER CODE BEGIN Header_ResetGlobalTask */
/**
* @brief Function implementing the ResetGlobal thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ResetGlobalTask */
void ResetGlobalTask(void *argument)
{
  /* USER CODE BEGIN ResetGlobalTask */
  /* Infinite loop */
  for(;;)
		{
		/* Pressing Button 3 sets us to ask for
		 * the MUTEX protecting the global variable
		 * When we get it, we can reset the global to the middle
		 */
		osSemaphoreAcquire(Button_3_SemaphoreHandle,osWaitForever);
		// Now we have the semaphore because the button was pressed
		osMutexWait(UpDownMutexHandle,osWaitForever);
		mutex_protected_count = Protected_Count_Initial_Value ;
		osDelay(5000);	// Wait for 5 seconds before making the resource available again
		osMutexRelease(UpDownMutexHandle);
		osDelay(1);
		 }
  /* USER CODE END ResetGlobalTask */
}

/* USER CODE BEGIN Header_StartDebounce */
/**
* @brief Function implementing the DebounceTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartDebounce */
void StartDebounce(void *argument)
{
  /* USER CODE BEGIN StartDebounce */
	uint32_t buttons_in;  // placeholder
	/* Infinite loop */
	for(;;)
	{
		/* We wait here until there's a notification
		 * that there's been a button and delay for debounce.
		 * Could have done the semaphore release directly from the
		 * ISR, BUT there wouldn't have been a debounce.  We want
		 * the debounce done in a thread-safe way -- not holding up the ISR.
		 * Arguments are:  bits_to_clear_entry, bits_to_clear_exit
		 * So clear all but the top (0xfffffff8) going in, then the bottom 3 (0x7) going out
		 *
		 * */
		 xTaskNotifyWait( 0xfffffff8, 0x7, &buttons_in, portMAX_DELAY);
	     osDelay(30);
	     if (buttons_in & B1) { osSemaphoreRelease(Button_1_SemaphoreHandle); }
	     if (buttons_in & B2) { osSemaphoreRelease(Button_2_SemaphoreHandle); }
	     if (buttons_in & B3) { osSemaphoreRelease(Button_3_SemaphoreHandle); }
	 }
  /* USER CODE END StartDebounce */
}

/* USER CODE BEGIN Header_semaphore_toggle_d3_task */
/**
* @brief Function implementing the semaphore_d3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_semaphore_toggle_d3_task */
void semaphore_toggle_d3_task(void *argument)
{
  /* USER CODE BEGIN semaphore_toggle_d3_task */
  /* Infinite loop */
	for(;;)
	  {
		  osSemaphoreAcquire(Button_1_SemaphoreHandle, osWaitForever);
		  HAL_GPIO_TogglePin(LED_D3_GPIO_Port , LED_D3_Pin);
		  osDelay(20);
	  }
  /* USER CODE END semaphore_toggle_d3_task */
}

/* USER CODE BEGIN Header_semaphore_toggle_d4_task */
/**
* @brief Function implementing the semaphore_d4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_semaphore_toggle_d4_task */
void semaphore_toggle_d4_task(void *argument)
{
  /* USER CODE BEGIN semaphore_toggle_d4_task */
  /* Infinite loop */
	for(;;)
	  {
		  osSemaphoreAcquire(Button_1_SemaphoreHandle, osWaitForever);
		  HAL_GPIO_TogglePin(LED_D4_GPIO_Port , LED_D4_Pin);
		  osDelay(20);
	  }
  /* USER CODE END semaphore_toggle_d4_task */
}

/* USER CODE BEGIN Header_multex_display_line_task */
/**
* @brief Function implementing the Mutex_Line thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_multex_display_line_task */
void multex_display_line_task(void *argument)
{
  /* USER CODE BEGIN multex_display_line_task */
  /* Infinite loop */
    for(;;)
        {
        /* This doesn't change the value, it just clears the display  */
        /* If asked to display a negative number, the function displays a "--"
        */
    	osSemaphoreAcquire(Button_3_SemaphoreHandle, osWaitForever);
        osMutexWait(UpDownMutexHandle,osWaitForever);
        MultiFunctionShield_Display_Two_Digits(-1);
        osDelay(200);
        osMutexRelease(UpDownMutexHandle);
        osDelay(2);
        }
  /* USER CODE END multex_display_line_task */
}

/* SW_Timer_Countdown function */
void SW_Timer_Countdown(void *argument)
{
  /* USER CODE BEGIN SW_Timer_Countdown */

	/*
	 * When the timer expires, decrement the Count and Display it
	 * on the 7-Seg Upper
	 */

	if (countdown_display == 0) countdown_display = 9;
		else countdown_display--;

	MultiFunctionShield_Single_Digit_Display(4, countdown_display);   //put it on the left
	// MultiFunctionShield_Single_Digit_Display(2, -1);//blank the bottom two

  /* USER CODE END SW_Timer_Countdown */
}

/* SW_Timer_DisplayRefresh function */
void SW_Timer_DisplayRefresh(void *argument)
{
  /* USER CODE BEGIN SW_Timer_DisplayRefresh */

	MultiFunctionShield__ISRFunc();
	osDelay(1);

  /* USER CODE END SW_Timer_DisplayRefresh */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM3 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM3)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if (htim == &htim17 )
  {
	  //MultiFunctionShield__ISRFunc();
  }

  /* USER CODE END Callback 1 */
}

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
#ifdef USE_FULL_ASSERT
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
