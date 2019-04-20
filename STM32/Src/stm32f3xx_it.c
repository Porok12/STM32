/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 32
#define DATA_SIZE 2
#define IN_SIZE 32
#define DMA_SIZE 4

#define INA219_ADDRESS 						(0x40<<1)
#define INA219_REG_CONFIG                      (0x00)
#define INA219_REG_SHUNTVOLTAGE                (0x01)
#define INA219_REG_BUSVOLTAGE                  (0x02)
#define INA219_REG_POWER                       (0x03)
#define INA219_REG_CURRENT                     (0x04)
#define INA219_REG_CALIBRATION                 (0x05)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern uint8_t data[DATA_SIZE];
extern uint8_t buffer[BUFFER_SIZE];
extern uint8_t uart1[IN_SIZE];
extern uint8_t uart2[IN_SIZE];
extern uint8_t *uart_ptr;
extern uint16_t adc_val;

volatile uint16_t cnt_tim;
volatile uint8_t tx_flag;
volatile int _cnt, _freq;
uint16_t adc_buffer[DMA_SIZE];

volatile enum COMMAND {
	CMD_SHUNT_VOLTAGE = 0x01,
	CMD_BUS_VOLTAGE = 0x02,
	CMD_POWER = 0x03,
	CMD_CURRENT = 0x04,
	CMD_STOP = 0x05,
	CMD_ADC = 0x06,
	CMD_UNKNOWN = 0x07
} cmd;

volatile enum MEASUREMENT {
	MEASURE_INA219 = 0x01,
	MEASURE_CUSTOM = 0x02
} measurement = MEASURE_INA219;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern float getCurrent_mA(uint16_t value);
extern float getPower_mW(uint16_t value);
extern float getBusVoltage_V(uint16_t value);
extern float getShuntVoltage_mV(uint16_t value);
void cmdParser(uint8_t* in, uint8_t cnt);
void setupTimerDMA();
void setupTimerIRQ();
void convertToCommand(uint8_t* in);
void cmdI2C();

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles ADC1 and ADC2 interrupts.
  */
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */

  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update and TIM16 interrupts.
  */
void TIM1_UP_TIM16_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM16_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM16_IRQn 1 */
}

/**
  * @brief This function handles I2C1 event global interrupt / I2C1 wake-up interrupt through EXT line 23.
  */
void I2C1_EV_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_EV_IRQn 0 */

  /* USER CODE END I2C1_EV_IRQn 0 */
  HAL_I2C_EV_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_EV_IRQn 1 */

  /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
  /* USER CODE BEGIN I2C1_ER_IRQn 0 */

  /* USER CODE END I2C1_ER_IRQn 0 */
  HAL_I2C_ER_IRQHandler(&hi2c1);
  /* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXT line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */

  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles EXTI line[15:10] interrupts.
  */
void EXTI15_10_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI15_10_IRQn 0 */

  /* USER CODE END EXTI15_10_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
  /* USER CODE BEGIN EXTI15_10_IRQn 1 */

  /* USER CODE END EXTI15_10_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/**
 * @brief Switches way of measurements
 * @param GPIO_Pin
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == B1_Pin) {
		HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		//measurement = (measurement == MEASURE_CUSTOM) ? MEASURE_INA219 : MEASURE_CUSTOM;
		HAL_TIM_Base_Stop_IT(&htim1);
		if(measurement == MEASURE_INA219) {
			measurement = MEASURE_CUSTOM;
			HAL_ADC_Stop_IT(&hadc1);
			setupTimerDMA();
			HAL_ADC_Start_DMA(&hadc1, (void*)adc_buffer, DMA_SIZE);
		}
		else {
			measurement = MEASURE_INA219;
			HAL_ADC_Stop_DMA(&hadc1);
			setupTimerIRQ();
			cmdI2C();
		}
	}
}

/**
 * @brief Triggers measurements
 * @param TIM_HandleTypeDef
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(cnt_tim) {
		cnt_tim = (cnt_tim < 0) ? -1 : cnt_tim - 1;

		if(measurement == MEASURE_INA219){
			cmdI2C();
		}
		else { // MEASURE_CUSTOM
			HAL_ADC_Start_DMA(&hadc1, (void*)adc_buffer, DMA_SIZE);
		}
	}
	else {
		HAL_TIM_Base_Stop_IT(&htim1);
	}
}

/**
 * @brief Handles received commands
 * @param uint8_t buffer
 */
void cmdParser(uint8_t* in, uint8_t cnt)
{
	convertToCommand(in);

	if(measurement == MEASURE_CUSTOM) {
		HAL_ADC_Stop_IT(&hadc1);
		HAL_TIM_Base_Stop_IT(&htim1);

		setupTimerDMA();
		HAL_ADC_Start_DMA(&hadc1, (void*)adc_buffer, DMA_SIZE);
	}
	else { // MEASURE_INA219
		HAL_ADC_Stop_DMA(&hadc1);
		HAL_TIM_Base_Stop_IT(&htim1);

		switch (cmd) {
			case CMD_ADC:
				setupTimerIRQ();
				HAL_ADC_Start_IT(&hadc1);
				break;
			case CMD_SHUNT_VOLTAGE:
			case CMD_BUS_VOLTAGE:
			case CMD_POWER:
			case CMD_CURRENT:
				setupTimerIRQ();
				cmdI2C();
				//HAL_ADC_Start_IT(&hadc1);
				break;
			case CMD_STOP:
				HAL_TIM_Base_Stop_IT(&htim1);
				break;
			default:
				break;
		}
	}
}

/**
 * @brief Gets measurement values and send them back to PC via UART
 * @param ADC_HandleTypeDef
 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(measurement == MEASURE_CUSTOM) {
		uint32_t avg = 0;
		for(uint16_t i = 0; i < DMA_SIZE; i++) {
			avg += adc_buffer[i];
		}
		adc_val = (uint16_t)(avg / DMA_SIZE);
	}
	else {
		adc_val = HAL_ADC_GetValue(&hadc1);
	}

	float Vcc = 5.0f; // [V]
	float volt = 3.3 * adc_val / 0xFFF; // [V]
	float realVolt = volt / (1 + (97.0f / 2.18f)); // [V]
	float current = (realVolt) / 5.6f; // [A]
	float power = current * Vcc; // [W]

	switch (cmd) {
		case CMD_SHUNT_VOLTAGE:
			sprintf((char*)buffer, "%0.1f [mV]\n", realVolt * 1000.0f);
			break;
		case CMD_BUS_VOLTAGE:
			sprintf((char*)buffer, "%0.3f [V]\n", Vcc);
			break;
		case CMD_POWER:
			sprintf((char*)buffer, "%0.1f [mW]\n", power * 1000.0f);
			break;
		case CMD_CURRENT:
			sprintf((char*)buffer, "%0.1f [mA]\n", current * 1000.0f);
			break;
		case CMD_ADC:
			sprintf((char*)buffer, "%0.3f [V]\n", volt);
			break;
		case CMD_STOP:
			break;
		default:
			sprintf((char*)buffer, "?\n");
			break;
	}

	//HAL_UART_Transmit(&huart2, (uint8_t*)&buffer, strlen((char*)buffer), 20);
	tx_flag = 1;
	HAL_UART_Transmit_IT(&huart2, (uint8_t*)&buffer, strlen((char*)buffer));
	while (tx_flag) { }
}

/**
 * @brief Reset tx_flag when transmit completed
 * @param UART_HandleTypeDef
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	tx_flag = 0;
}

/**
 * @brief Handles receiving bytes on UART
 * @param UART_HandleTypeDef
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t cnt = 0;

	if (uart_ptr[cnt] == '\n') {
		cmdParser(uart_ptr, cnt);
		HAL_UART_Transmit_IT(&huart2, (uint8_t*)uart_ptr, cnt+1);
		uart_ptr = (uart_ptr == uart1) ? uart2 : uart1;
		HAL_UART_Receive_IT(&huart2, uart_ptr, 1);
		cnt = 0;
	}
	else {
		cnt++;
		HAL_UART_Receive_IT(&huart2, uart_ptr + cnt, 1);
	}
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{

}

/**
 * @brief Handles received bytes on I2C
 * @param I2C_HandleTypeDef
 */
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	uint16_t value = (data[0] << 8) | data[1];
	float result = 0.0f;
	switch (cmd) {
		case CMD_SHUNT_VOLTAGE:
			result = getShuntVoltage_mV(value); //value * 0.01f;
			sprintf((char*)&buffer, "%.1f [mV]\n", result);
			break;
		case CMD_BUS_VOLTAGE:
			result = getBusVoltage_V(value); //((value >> 3) * 4) * 0.001f;
			sprintf((char*)&buffer, "%.3f [V]\n", result);
			break;
		case CMD_POWER:
			result = getPower_mW(value); //value * getPowerMultiplier_mW();
			sprintf((char*)&buffer, "%.1f [mW]\n", result);
			break;
		case CMD_CURRENT:
			result = getCurrent_mA(value); //value / getCurrentDivider_mA();
			sprintf((char*)&buffer, "%.1f [mA]\n", result);
			break;
		case CMD_STOP:
			break;
		default:
			sprintf((char*)&buffer, "?\n");
			break;
	}

	HAL_UART_Transmit(&huart2, (uint8_t*) &buffer, strlen((char*)buffer), 50);
}

/**
 * @brief Triggers INA219 actions depending on cmd variable
 */
void cmdI2C()
{
	switch (cmd) {
		case CMD_SHUNT_VOLTAGE:
		case CMD_BUS_VOLTAGE:
		case CMD_POWER:
		case CMD_CURRENT:
			data[0] = cmd;
			HAL_I2C_Master_Transmit(&hi2c1, INA219_ADDRESS, (uint8_t*)&data, 1, 100);
			HAL_I2C_Master_Receive_IT(&hi2c1, INA219_ADDRESS, (uint8_t*)&data, 2);
			break;
		case CMD_ADC:
			HAL_ADC_Start_IT(&hadc1);
			break;
		default:
			break;
	}
}

/**
 * @brief Sets up timer counter and frequency for DMA
 */
void setupTimerDMA()
{
	cnt_tim = _cnt * DMA_SIZE;
	htim1.Instance->CNT = 0;
	htim1.Instance->ARR = _freq / DMA_SIZE - 1;
	HAL_TIM_Base_Start_IT(&htim1);
}

/**
 * @brief Sets up timer counter and frequency for IRQ
 */
void setupTimerIRQ()
{
	cnt_tim = _cnt;
	htim1.Instance->CNT = 0;
	htim1.Instance->ARR = _freq - 1;
	HAL_TIM_Base_Start_IT(&htim1);
}

/**
 * @brief Tries to convert received message to command and obtains data
 * @param uint8_t buffer
 */
void convertToCommand(uint8_t* in)
{
	_cnt = 1;
	_freq = 100;

	char cmd_adc[] = "adc";
	char cmd_power[] = "power";
	char cmd_current[] = "curr";
	char cmd_voltage[] = "vol";
	char cmd_voltage2[] = "vol2";
	char cmd_stop[] = "stop";

	if (memcmp(in, cmd_adc, strlen(cmd_adc)) == 0) {
		sscanf((char*)(in + strlen(cmd_adc)),"%d,%d", &_cnt, &_freq);
		cmd = CMD_ADC;
	}
	else if (memcmp(in, cmd_voltage2, strlen(cmd_voltage2)) == 0) {
		sscanf((char*)(in + strlen(cmd_voltage2)),"%d,%d", &_cnt, &_freq);
		cmd = CMD_SHUNT_VOLTAGE;
	}
	else if (memcmp(in, cmd_voltage, strlen(cmd_voltage)) == 0) {
		sscanf((char*)(in + strlen(cmd_voltage)),"%d,%d", &_cnt, &_freq);
		cmd = CMD_BUS_VOLTAGE;
	}
	else if (memcmp(in, cmd_power, strlen(cmd_power)) == 0) {
		sscanf((char*)(in + strlen(cmd_power)),"%d,%d", &_cnt, &_freq);
		cmd = CMD_POWER;
	}
	else if (memcmp(in, cmd_current, strlen(cmd_current)) == 0) {
		sscanf((char*)(in + strlen(cmd_current)),"%d,%d", &_cnt, &_freq);
		cmd = CMD_CURRENT;
	}
	else if(memcmp(in, cmd_stop, strlen(cmd_stop)) == 0) {
		cmd = CMD_STOP;
	}
	else {
		cmd = in[0];
	}
}

/**
 * @brief Sends errors
 * @param I2C_HandleTypeDef
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	sprintf((char*)&buffer, "HAL_I2C_ErrorCallback\n");
	HAL_UART_Transmit(&huart2, (uint8_t*) &buffer, strlen((char*)buffer), 10);
}

/**
 * @brief Sends errors
 * @param UART_HandleTypeDef
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	uint32_t error = HAL_UART_GetError(&huart2);
	uint32_t state = HAL_UART_GetState(&huart2);

	sprintf((char*)&buffer, "error code: %d\n", (int)error);
	HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), 50);
	sprintf((char*)&buffer, "status code: %d\n", (int)state);
	HAL_UART_Transmit(&huart2, buffer, strlen((char*)buffer), 50);
}


/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
