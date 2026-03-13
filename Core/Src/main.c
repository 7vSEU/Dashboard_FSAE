/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "st7796.h"
#include "GUI.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

FDCAN_HandleTypeDef hfdcan1;

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
uint8_t Wheel_Speed_v01 = 0;
uint16_t BatVoltage_v02 = 0;
uint16_t BatCurrent_v03 = 0; //offset = -1000
uint16_t MaxCellVolt_v04 = 0;
uint16_t MinCellVolt_v05 = 0;
uint8_t MaxTemp_v06 = 0;	//offset = -40
int16_t Motor_Actual_Velocity_LF_v07 = 0;
int16_t Motor_Actual_Velocity_RF_v08 = 0;
int16_t Motor_Actual_Velocity_LR_v07 = 0;
int16_t Motor_Actual_Velocity_RR_v08 = 0;
int16_t Temp_IGBT_v09 = 0;
uint16_t Currentmeter_v10 = 0;

uint16_t InverterErr[4];
uint16_t SysReady[4];

uint16_t LV_volt = 0;

uint16_t Oil_ADC1 = 0;
uint16_t Oil_ADC2 = 0;

uint16_t Pedal_ADC_1_v11 = 2000;
uint16_t Pedal_1_up = 0xd2;			//2.10
uint16_t Pedal_1_down = 0xf1;		//2，41
uint16_t Pedal_ADC_2_v12 = 50;
uint16_t Pedal_2_up = 0x79;			//1.21
uint16_t Pedal_2_down = 0x5a;		//0.9

uint16_t SC_ADC = 0x00;//SC_ADC = 0;

uint16_t SAS_ADC = 0;
uint16_t SAS_offset = 0;

uint8_t Unreliable_Status1 = 0;
uint8_t Unreliable_Status2 = 0;

// CAN 接收缓存
uint8_t can_rx_data[12];       // 接收数据
uint32_t can_rx_id = 0;       // 接收ID
uint8_t can_rx_len = 0;       // 接收数据长度
uint8_t can_rx_flag = 0;      // 接收完成标志（1=有新数据）
uint8_t _CAN_unit_to_show[8];
uint8_t _CAN_unit_to_show2[8];
uint8_t _CAN_unit_to_show3[8];
uint8_t _CAN_unit_to_show4[8];
uint8_t _CAN_unit_to_show5[8];

uint8_t key = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_FDCAN1_Init(void);
/* USER CODE BEGIN PFP */
void Read_Can(uint32_t, uint8_t*);
void can1_transmit(uint8_t, uint8_t*, FDCAN_HandleTypeDef*);
void CAN_Unit_Duplicate(uint8_t*, uint8_t*, uint32_t);
void Readkeys(uint8_t*);
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

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
  HAL_FDCAN_Start(&hfdcan1);
  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
  LCD_Init();
  GUI_Refresh(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  GUI_Refresh(1);
	  Readkeys(&key);
	  //HAL_Delay(1000);
	  //LCD_FillScreen(WHITE);LCD_FillScreen(BLACK);
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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_CKPER;
  PeriphClkInitStruct.CkperClockSelection = RCC_CLKPSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief FDCAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_FDCAN1_Init(void)
{

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 10;
  hfdcan1.Init.NominalSyncJumpWidth = 4;
  hfdcan1.Init.NominalTimeSeg1 = 4;
  hfdcan1.Init.NominalTimeSeg2 = 5;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 4;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 10;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 10;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  // 过滤器配置（接收所有标准帧和扩展帧）
  	  FDCAN_FilterTypeDef  sFilterConfig;
  	  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  	  sFilterConfig.FilterIndex = 0;
  	  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  	  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  	  sFilterConfig.FilterID1 = 0x000;
  	  sFilterConfig.FilterID2 = 0x000;

    if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig)!= HAL_OK) {
      Error_Handler();
    }
  /* USER CODE END FDCAN1_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0x0;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  hspi1.Init.NSSPolarity = SPI_NSS_POLARITY_LOW;
  hspi1.Init.FifoThreshold = SPI_FIFO_THRESHOLD_01DATA;
  hspi1.Init.TxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.RxCRCInitializationPattern = SPI_CRC_INITIALIZATION_ALL_ZERO_PATTERN;
  hspi1.Init.MasterSSIdleness = SPI_MASTER_SS_IDLENESS_00CYCLE;
  hspi1.Init.MasterInterDataIdleness = SPI_MASTER_INTERDATA_IDLENESS_00CYCLE;
  hspi1.Init.MasterReceiverAutoSusp = SPI_MASTER_RX_AUTOSUSP_DISABLE;
  hspi1.Init.MasterKeepIOState = SPI_MASTER_KEEP_IO_STATE_DISABLE;
  hspi1.Init.IOSwap = SPI_IO_SWAP_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(BLK_GPIO_Port, BLK_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CS_Pin|DC_Pin|RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED1_Pin */
  GPIO_InitStruct.Pin = LED1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BLK_Pin CS_Pin DC_Pin RES_Pin */
  GPIO_InitStruct.Pin = BLK_Pin|CS_Pin|DC_Pin|RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : K1_Pin K2_Pin K3_Pin K4_Pin */
  GPIO_InitStruct.Pin = K1_Pin|K2_Pin|K3_Pin|K4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : K5_Pin */
  GPIO_InitStruct.Pin = K5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(K5_GPIO_Port, &GPIO_InitStruct);

  /*AnalogSwitch Config */
  HAL_SYSCFG_AnalogSwitchConfig(SYSCFG_SWITCH_PA1, SYSCFG_SWITCH_PA1_CLOSE);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void can1_transmit(uint8_t can1_tx_id, uint8_t* can1_tx_data, FDCAN_HandleTypeDef* can1_handle){
	FDCAN_TxHeaderTypeDef TxHeader;
	TxHeader.Identifier =can1_tx_id;
	TxHeader.IdType = FDCAN_STANDARD_ID;
	TxHeader.TxFrameType = FDCAN_DATA_FRAME;
	TxHeader.DataLength = FDCAN_DLC_BYTES_8;
	TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
	TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
	TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	TxHeader.MessageMarker = 0;

	if (HAL_FDCAN_AddMessageToTxFifoQ(can1_handle, &TxHeader, can1_tx_data) != HAL_OK)
		{
			Error_Handler();
		}
}

/*
void HAL_FDCAN_RxFifo0MsgPendingCallback(FDCAN_HandleTypeDef *hcan) {
  FDCAN_RxHeaderTypeDef rx_header;

  // 读取接收数据（CAN FD 需用 HAL_CAN_GetRxMessage 兼容）
  if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &rx_header, canfd_rx_data) == HAL_OK) {
    canfd_rx_id = rx_header.StdId;                  // 保存接收 ID
    canfd_rx_len = rx_header.DLC;                   // 数据长度
    canfd_rx_flag = 1;                              // 置位接收标志
    printf("Received %s Frame - ID:0x%X, Len:%d\r\n",
           rx_header.CanFDFormat ? "CAN FD" : "CAN",
           canfd_rx_id, canfd_rx_len);
  }
}
*/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs){
	FDCAN_RxHeaderTypeDef RxHeader;
	if (hfdcan->Instance == hfdcan1.Instance) {
		  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET){
			 if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, can_rx_data) != HAL_OK){Error_Handler();}
			 //can1_transmit(0x001, data, &hfdcan1);
			    can_rx_id = RxHeader.Identifier;  // 保存接收ID
			    can_rx_len = RxHeader.DataLength;   // 保存数据长度
			    can_rx_flag = 1;              // 置位接收标志
			    Read_Can(can_rx_id, can_rx_data);

		  }
	}
}

void Read_Can(uint32_t can_rx_id, uint8_t* can_rx_data){
	switch(can_rx_id){
	//case 0x123: Wheel_Speed_v01 = can_rx_data[0];	break;
	case 0x124:{
		BatVoltage_v02 = can_rx_data[3] + (can_rx_data[2])*0x100;
		//CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);//27 10 13 3D 00 00 00 00
		break;
	}
	case 0x130:{
		Oil_ADC1 = can_rx_data[1] + (can_rx_data[0])*0x100;
		Oil_ADC2 = can_rx_data[3] + (can_rx_data[2])*0x100;
		break;
	}
	case 0x131:	{
		Pedal_ADC_1_v11 = can_rx_data[1] + (can_rx_data[0])*0x100;
		Pedal_1_up = can_rx_data[3];
		Pedal_1_down = can_rx_data[5];
		//CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);//07 D9 00 D2 00 F1 00 00 VOLT UP DOWN
		break;
	}
	case 0x132:	{
		Pedal_ADC_2_v12 = can_rx_data[1] + (can_rx_data[0])*0x100;
		Pedal_2_up = can_rx_data[3];
		Pedal_2_down = can_rx_data[5];
		break;
		}
	case 0x133:{
		SC_ADC = can_rx_data[1] + (can_rx_data[0])*0x100;
		MinCellVolt_v05 = can_rx_data[3] + (can_rx_data[2])*0x100;
		LV_volt = can_rx_data[5] + (can_rx_data[4])*0x100;
		//CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);//08 16 0E A4 00 00 00 00
		break;
	}
	case 0x134:{
		Wheel_Speed_v01 = (can_rx_data[1] + (can_rx_data[0])*0x100)/(double)159;
		//CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);
		break;
	}
	case 0x135:{
		InverterErr[0] = can_rx_data[1] + (can_rx_data[0])*0x100;	//FR
		InverterErr[1] = can_rx_data[3] + (can_rx_data[2])*0x100;	//FL
		InverterErr[2] = can_rx_data[5] + (can_rx_data[4])*0x100;	//RR
		InverterErr[3] = can_rx_data[7] + (can_rx_data[6])*0x100;	//RL
		//CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);
		break;
	}
	case 0x136:{
		/*
		InverterErr[0] = can_rx_data[1] + (can_rx_data[0])*0x100;	//FR
		InverterErr[1] = can_rx_data[3] + (can_rx_data[2])*0x100;	//FL
		InverterErr[2] = can_rx_data[5] + (can_rx_data[4])*0x100;	//RR
		InverterErr[3] = can_rx_data[7] + (can_rx_data[6])*0x100;	//RL
		*/
		CAN_Unit_Duplicate(can_rx_data, _CAN_unit_to_show, can_rx_id);
		break;
	}
	case 0x188:{//134
		Unreliable_Status1 = can_rx_data[1];
		Unreliable_Status2 = can_rx_data[3];
		break;
	}
	return;
	}
}

void CAN_Unit_Duplicate(uint8_t* _source, uint8_t* _target, uint32_t _id){
	_target[0] = _source[0];
	_target[1] = _source[1];
	_target[2] = _source[2];
	_target[3] = _source[3];
	_target[4] = _source[4];
	_target[5] = _source[5];
	_target[6] = _source[6];
	_target[7] = _source[7];
}

void Readkeys(uint8_t* _key){
	uint8_t _key_cache;
	_key_cache = _key[0];
	_key[0] = HAL_GPIO_ReadPin(K1_GPIO_Port, K1_Pin); _key[0] *=2;
	_key[0] += HAL_GPIO_ReadPin(K2_GPIO_Port, K2_Pin); _key[0] *=2;
	_key[0] += HAL_GPIO_ReadPin(K3_GPIO_Port, K3_Pin); _key[0] *=2;
	_key[0] += HAL_GPIO_ReadPin(K4_GPIO_Port, K4_Pin); _key[0] *=2;
	_key[0] += HAL_GPIO_ReadPin(K5_GPIO_Port, K5_Pin);
	if(_key_cache == 0b11111){
		if(_key[0] == 0b11110)	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if(_key[0] == 0b11101)	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if(_key[0] == 0b11011)	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if(_key[0] == 0b10111)	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		if(_key[0] == 0b01111)	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
}
/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

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
