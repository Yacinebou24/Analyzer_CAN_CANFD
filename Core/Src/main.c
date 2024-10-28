/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
#include "ILI9488.h"
#include "besma.h"
#include "obd.h"
#include "ND.h"
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
/* USER CODE BEGIN PV */
FDCAN_HandleTypeDef hfdcan1;
extern SPI_HandleTypeDef hspi4;
extern DMA_HandleTypeDef hdma_spi4_tx;
extern TIM_HandleTypeDef htim1;
FDCAN_FilterTypeDef sFilterConfig;
uint8_t R[16],a[8];
uint8_t buffer[64],s,ss,bs,b[90],buffer2[16],buffer3[16],buffer4[16],buf[64],text[40],text2[40],bufferr_receive[8];
int PID020,PID_2140,PID4160,h1=0,h2=0,m=0,l=0,w=0,j=0,c=0,cc=0,fin=0;
uint8_t d[64];
uint16_t col;
uint16_t x=380,y=60;
FDCAN_RxHeaderTypeDef RxHeader;
FDCAN_TxHeaderTypeDef filter,Ms;
uint16_t _width = DISPL_WIDTH;
uint16_t _height = DISPL_HEIGHT;
uint8_t send[] = {0x10, 0x32, 0x54, 0x76, 0x98, 0x00, 0x11, 0x22};
float e_data;
extern float engine_data;
//uint8_t data[2];
uint8_t fit[] ={0x0, 0x0,0x0,0x0, 0x0, 0x0, 0x00, 0x0};
int test2=0,test1=1,test3=0,test4=0,test5=0,test6=0,stop=0,mk=0,bol=1,ww=0;
uint8_t *data = "HI !! I am from CAN 500Kbps\n";
uint8_t *data3 = "----Start---\n";
uint8_t *data2 = "HI !! I am from CAN 125Kbps\n";
uint8_t *data4 = "HI !! I am from FDCAN 125Kbps\n";
uint8_t *data5 = "HI !! I am from FDCAN 500Kbps";
uint8_t *data6 = "HI !! I am from FDCAN 1Mbps";
uint8_t *data7 = "HI !! I am from FDCAN 2Mbps";
int tftfd=0;
int speed=0,fd_speed=0;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_FDCAN1_Init(void);
/* USER CODE BEGIN PFP */
void haut(uint16_t col);
void bas(uint16_t col);
void lath(uint16_t col);
void lat(uint16_t col);
uint16_t rand_color();
void dinam(uint16_t col);
void horizanimD();
void horizanim();
void CaptureAndDisplayScreen();
void chronogramme();
void front_display();
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
  MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_SPI4_Init();
  MX_TIM1_Init();
  MX_FDCAN1_Init();
  /* USER CODE BEGIN 2 */
  if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
  {
	  Error_Handler();
  }
  if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }
	 ILI9488_Init();
	 setRotation(Displ_Orientat_90);
	 fillScreen(ILI9488_BLACK);
	 for(w=0;w<100;w++)
	 {
			x = rand() % (_width - 11);
			y = rand() % (_height - 18);
			uint16_t taille=rand()%60;
			DrawCircle(x, y,taille,rand_color());
	 }
	 HAL_Delay(500);
	 dinam(ILI9488_BLACK);
	 HAL_Delay(500);
	 Displ_CString(120,120,180,180,"CANalyzer",Font24,3,ILI9488_WHITE,ILI9488_BLACK);
	 HAL_Delay(1000);
	 horizanim();
	 sprintf(text,"CAN : 125Kbps");
	 Displ_WString(330, 10, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
	 Displ_CString(0,60,20,10,"0x0D",Font16,1,ILI9488_WHITE,ILI9488_OLIVE);
	 y=60;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_OLIVE);
	 }
	 Displ_CString(0,100,20,10,"0x46",Font16,1,ILI9488_WHITE,ILI9488_BLUE);
	 y=100;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_BLUE);
	 }
	 Displ_CString(0,140,20,10,"0x33",Font16,1,ILI9488_WHITE,ILI9488_DARKCYAN);
	 y=140;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_DARKCYAN);
	 }
	 Displ_CString(0,180,20,10,"0x1F",Font16,1,ILI9488_WHITE,ILI9488_ORANGE);
	 y=180;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_ORANGE);
	 }
	 Displ_CString(0,220,20,10,"0x21",Font16,1,ILI9488_WHITE,ILI9488_RED);
	 y=220;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_RED);
	 }
	 Displ_CString(0,260,20,10,"0x4D",Font16,1,ILI9488_WHITE,ILI9488_PINK);
	 y=260;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_PINK);
	 }
	 Displ_CString(0,300,20,10,"0x0A",Font16,1,ILI9488_WHITE,ILI9488_PURPLE);
	 y=300;
	 x=380;
	 for(int p=0;p<64;p++)
	 {
		 bas(ILI9488_PURPLE);
	 }
	  filter.Identifier = 0xb0;
	  filter.IdType = FDCAN_STANDARD_ID;
	  filter.TxFrameType = FDCAN_DATA_FRAME;
	  filter.DataLength = FDCAN_DLC_BYTES_8;
	  filter.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	  filter.BitRateSwitch = FDCAN_BRS_OFF;
	  filter.FDFormat = FDCAN_CLASSIC_CAN;
	  filter.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	  filter.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS

	  Ms.Identifier = 0xb1;
	  Ms.IdType = FDCAN_STANDARD_ID;
	  Ms.TxFrameType = FDCAN_DATA_FRAME;
	  Ms.DataLength = FDCAN_DLC_BYTES_8;
	  Ms.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
	  Ms.BitRateSwitch = FDCAN_BRS_OFF;
	  Ms.FDFormat = FDCAN_CLASSIC_CAN;
	  Ms.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	  Ms.MessageMarker = 0x0; // Ignore because FDCAN_NO_TX_EVENTS
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  switch(bufferr_receive[0])
	  {
	  case 0x0d:
		  fit[0]=0x0d;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Vitesse",Font24,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20,30,text,Font16,1,ILI9488_WHITE,ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90,30,text,Font16,1,ILI9488_BLACK,ILI9488_OLIVE);
		  Displ_CString(0,60,20,10,"0x0D",Font16,1,ILI9488_WHITE,ILI9488_OLIVE);
		  col=ILI9488_OLIVE;
		  chronogramme();
		  for(int u=0;u<80;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"vitesse :%dKm/h",(int)engine_data);
		  Displ_CString(0,100,40,10,text2,Font16,1,ILI9488_WHITE,ILI9488_OLIVE);
		  break;
	  case 0x46:
		  fit[0]=0x46;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Temperature",Font24,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_WHITE);
			  fillRect(0, 200, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_BLUE);
		  Displ_CString(0,60,20,10,"0x46",Font16,1,ILI9488_WHITE,ILI9488_BLUE);
		  col=ILI9488_BLUE;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Temperature:%dDegC",(int)engine_data);
		  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_BLUE);
		  break;
	  case 0x33:
		  fit[0]=0x33;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Pression",Font24,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_WHITE);
			  fillRect(0, 200, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_DARKCYAN);
		  Displ_CString(0,60,20,10,"0x33",Font16,1,ILI9488_WHITE,ILI9488_DARKCYAN);
		  col=ILI9488_DARKCYAN;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Pression :%dKPa",(int)engine_data);
		  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_DARKCYAN);
		  break;
	  case 0x03:
		  fit[0]=0x03;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Temps en Sec",Font16,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_ORANGE);
		  Displ_CString(0,60,20,10,"0x1F",Font16,1,ILI9488_WHITE,ILI9488_ORANGE);
		  col=ILI9488_ORANGE;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Temps :%dSec",(int)engine_data);
		  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_ORANGE);
		  break;
	  case 0x0a:
		  fit[0]=0x0a;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Distance",Font24,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_RED);
		  Displ_CString(0,60,20,10,"0x21",Font16,1,ILI9488_WHITE,ILI9488_RED);
		  col=ILI9488_RED;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Distance :%dKm",(int)engine_data);
		  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_RED);
		  break;
	  case 0x1f:
		  fit[0]=0x1f;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Temps en Min",Font16,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_PINK);
		  Displ_CString(0,60,20,10,"0x4D",Font16,1,ILI9488_WHITE,ILI9488_PINK);
		  col=ILI9488_PINK;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Temps  :%dMin",(int)engine_data);
		  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_PINK);
		  break;
	  case 0x21:
		  fit[0]=0x21;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"Pression carburant",Font16,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_PURPLE);
		  Displ_CString(0,60,20,10,"0x0A",Font16,1,ILI9488_WHITE,ILI9488_PURPLE);
		  col=ILI9488_PURPLE;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  sprintf(text2,"Pression carburant :%dKPa",(int)engine_data);
		  Displ_CString(0,100,100,10,text2,Font16,1,ILI9488_WHITE,ILI9488_PURPLE);
		  break;
	  case 0x4d:
		  fit[0]=0x4d;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"état du carburant",Font16,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_MAROON);
		  Displ_CString(0,60,20,10,"0x03",Font16,1,ILI9488_WHITE,ILI9488_MAROON);
		  col=ILI9488_MAROON;
		  chronogramme();
		  break;
	  case 0x51:
		  fit[0]=0x51;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  x=380;
		  y=60;
		  j=1;
		  lcdDrawRect(80, 60, 479, 25, ILI9488_BLACK);
		  fillRect(80,60, 479, 25, ILI9488_BLACK);
		  if(bol==1)
		  {
		  dinam(ILI9488_WHITE);
		  Displ_CString(120,120,180,180,"type carburant",Font16,3,ILI9488_BLACK,ILI9488_WHITE);
		  for(int u=0;u<20;u++)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_WHITE);
			  fillRect(0, 100, 479, 25, ILI9488_WHITE);
		  }
		  horizanim();
		  bol=0;
		  }
		  sprintf(text,"Data :");
		  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
		  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_WHITE);
		  e_data=engine_data;
		  Displ_CString(0,60,20,10,"0x51",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
		  col=ILI9488_WHITE;
		  chronogramme();
		  for(int u=0;u<40;u++)
		  {
			  lcdDrawRect(0, 200, 479, 25, ILI9488_BLACK);
			  fillRect(0, 200, 479, 25, ILI9488_BLACK);
		  }
		  Displ_CString(0,100,100,20,"Type de carburant:",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
          if (((int) engine_data) & 0x01)
          {
			  Displ_CString(200,100,250,20,"Diesel",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
          }
          if (((int) engine_data) & 0x02) {
   		  Displ_CString(190,100,230,10,"Essence",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
          }
		  break;
	  case 0x11:
		  fit[0]=0x11;
		  HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &filter, fit);
		  j=0;
		  break;
	  case 0xC1:
			  HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
			  HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 4;
			  hfdcan1.Init.NominalSyncJumpWidth = 1;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 1;
			  hfdcan1.Init.DataSyncJumpWidth = 1;
			  hfdcan1.Init.DataTimeSeg1 = 1;
			  hfdcan1.Init.DataTimeSeg2 = 1;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	          bufferr_receive[0]=0xff;
	          speed=0;
	          fd_speed=0;
	          j=0;
		  break;
	      case 0xc2:
		      HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    		  HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 2;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 1;
			  hfdcan1.Init.DataSyncJumpWidth = 1;
			  hfdcan1.Init.DataTimeSeg1 = 1;
			  hfdcan1.Init.DataTimeSeg2 = 1;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	          bufferr_receive[0]=0xff;
	          speed=1;
	          fd_speed=0;
	          j=0;
	      break;
	      case 0xc3:
		      HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
    		  HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 1;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 45;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 1;
			  hfdcan1.Init.DataSyncJumpWidth = 1;
			  hfdcan1.Init.DataTimeSeg1 = 1;
			  hfdcan1.Init.DataTimeSeg2 = 1;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	          bufferr_receive[0]=0xff;
	          speed=2;
	          fd_speed=0;
	          j=0;
	      break;
	      case 0xf1:
		      HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
			  HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 1;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 8;
			  hfdcan1.Init.DataSyncJumpWidth = 13;
			  hfdcan1.Init.DataTimeSeg1 = 21;
			  hfdcan1.Init.DataTimeSeg2 = 13;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_16;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
	          bufferr_receive[0]=0xff;
	          tftfd=0;
	          fd_speed=0;
	          speed=0;
	    break;
	    case 0xf2:
		      HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
		      HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 1;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 2;
			  hfdcan1.Init.DataSyncJumpWidth = 13;
			  hfdcan1.Init.DataTimeSeg1 = 21;
			  hfdcan1.Init.DataTimeSeg2 = 13;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_16;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
			  bufferr_receive[0]=0xff;
			  tftfd=0;
	          fd_speed=1;
	          speed=0;
	  break;
	    case 0xf3:
		      HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
		      HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 1;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 1;
			  hfdcan1.Init.DataSyncJumpWidth = 13;
			  hfdcan1.Init.DataTimeSeg1 = 21;
			  hfdcan1.Init.DataTimeSeg2 = 13;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_16;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  tftfd=0;
			  bufferr_receive[0]=0xff;
	          fd_speed=2;
	          speed=0;
	  break;
	    case 0xf4:
			  HAL_FDCAN_TT_DeactivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE);
	    	  HAL_FDCAN_Stop(&hfdcan1);
			  hfdcan1.Instance = FDCAN1;
			  hfdcan1.Init.FrameFormat = FDCAN_FRAME_FD_BRS;
			  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
			  hfdcan1.Init.AutoRetransmission = ENABLE;
			  hfdcan1.Init.TransmitPause = DISABLE;
			  hfdcan1.Init.ProtocolException = DISABLE;
			  hfdcan1.Init.NominalPrescaler = 1;
			  hfdcan1.Init.NominalSyncJumpWidth = 2;
			  hfdcan1.Init.NominalTimeSeg1 = 93;
			  hfdcan1.Init.NominalTimeSeg2 = 2;
			  hfdcan1.Init.DataPrescaler = 1;
			  hfdcan1.Init.DataSyncJumpWidth = 13;
			  hfdcan1.Init.DataTimeSeg1 = 5;
			  hfdcan1.Init.DataTimeSeg2 = 5;
			  hfdcan1.Init.MessageRAMOffset = 0;
			  hfdcan1.Init.StdFiltersNbr = 1;
			  hfdcan1.Init.ExtFiltersNbr = 0;
			  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
			  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_16;
			  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
			  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.RxBuffersNbr = 0;
			  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
			  hfdcan1.Init.TxEventsNbr = 0;
			  hfdcan1.Init.TxBuffersNbr = 0;
			  hfdcan1.Init.TxFifoQueueElmtsNbr = 1;
			  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
			  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_16;
			  HAL_FDCAN_Init(&hfdcan1);
			  HAL_FDCAN_Start(&hfdcan1);
			  HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
			  tftfd=0;
			  bufferr_receive[0]=0xff;
	          fd_speed=3;
	          speed=0;
	  break;
	  default :
		  break;
	  }
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
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
  hfdcan1.Init.AutoRetransmission = ENABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 4;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 93;
  hfdcan1.Init.NominalTimeSeg2 = 2;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.MessageRAMOffset = 0;
  hfdcan1.Init.StdFiltersNbr = 1;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.RxFifo0ElmtsNbr = 1;
  hfdcan1.Init.RxFifo0ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxFifo1ElmtsNbr = 0;
  hfdcan1.Init.RxFifo1ElmtSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.RxBuffersNbr = 0;
  hfdcan1.Init.RxBufferSize = FDCAN_DATA_BYTES_8;
  hfdcan1.Init.TxEventsNbr = 0;
  hfdcan1.Init.TxBuffersNbr = 0;
  hfdcan1.Init.TxFifoQueueElmtsNbr = 2;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  hfdcan1.Init.TxElmtSize = FDCAN_DATA_BYTES_8;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */
  sFilterConfig.IdType = FDCAN_STANDARD_ID;
  sFilterConfig.FilterIndex = 0;
  sFilterConfig.FilterType = FDCAN_FILTER_MASK;
  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
  sFilterConfig.FilterID1 = 0x0;
  sFilterConfig.FilterID2 = 0x0;
  sFilterConfig.RxBufferIndex = 0;
  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
	  Error_Handler();
  }
  /* USER CODE END FDCAN1_Init 2 */

}
/* USER CODE BEGIN 4 */
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
    {
    HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, R);
    if(RxHeader.FDFormat == FDCAN_CLASSIC_CAN )
    {
    ss= snprintf(b,30,"%02x %02x %02x %02x %02x %02x %02x %02x %02x",RxHeader.Identifier,R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
		  CDC_Transmit_FS(b, ss);
		  obd();
    }
    if(RxHeader.FDFormat == FDCAN_FD_CAN )
    {
    	j=1;
    if (RxHeader.Identifier == 0x46)
   	    {
		  	  lcdDrawRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  fillRect(0, 120, 479, 60, ILI9488_BLACK);
    	      s= snprintf(b,90,"--temperature--\n%02x %02x %02x %02x %02x %02x %02x %02x\n %02x %02x %02x %02x %02x %02x %02x %02x\n--end-temperature\n",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
   	   		  CDC_Transmit_FS(b, s);
			  col=ILI9488_DARKCYAN;
	    	  if(tftfd==0)
	    	  {
			  dinam(ILI9488_BLACK);
			  Displ_CString(80,120,180,180,"FDCAN Transmission",Font24,3,ILI9488_WHITE,ILI9488_BLACK);
			  horizanim();
			  tftfd++;
			  if(fd_speed==0)
			  {
			  sprintf(text,"FD CAN 125Kbps:");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  }
			  if(fd_speed==1)
			  {
			  sprintf(text,"FD CAN 500Kbps:");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  }
			  if(fd_speed==2)
			  {
			  sprintf(text,"FD CAN 1Mbps:");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  }
			  if(fd_speed==3)
			  {
			  sprintf(text,"FD CAN 2Mbps:");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  }
	    	  }
	    	  if(tftfd>0)
	    	  {
			  lcdDrawRect(140, 30, 479, 4, ILI9488_BLACK);
			  fillRect(140, 30, 479, 40, ILI9488_BLACK);
			  sprintf(text,"Data :");
			  Displ_WString(20, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"Temperature :");
			  Displ_WString(160, 30, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
			  Displ_WString(90, 90, text, Font16, 1, ILI9488_WHITE, col);
			  x=380;
			  y=120;
			  chronogramme();
			  x=380;
			  y=160;
			  for(int z=0;z<8;z++)
			  {
				  R[z]=R[z+8];
	    	  }
			  chronogramme();
    	  }
   	    }
        if (RxHeader.Identifier == 0x33 )
   	    {
		  	  lcdDrawRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  fillRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  lcdDrawRect(140, 30, 479, 4, ILI9488_BLACK);
		  	  fillRect(140, 30, 479, 40, ILI9488_BLACK);
   		   	  s= snprintf(b,80,"---Pression--\n%02x %02x %02x %02x %02x %02x %02x %02x\n %02x %02x %02x %02x %02x %02x %02x %02x\n--end-Pression--\n",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
   		   	  CDC_Transmit_FS(b, s);
			  col=ILI9488_GREEN;
			  sprintf(text,"Data :");
			  Displ_WString(20, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"Pression :");
			  Displ_WString(160, 30, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
			  Displ_WString(90, 90, text, Font16, 1, ILI9488_WHITE, col);
			  x=380;
			  y=120;
			  chronogramme();
			  x=380;
			  y=160;
			  for(int z=0;z<8;z++)
			  {
				  R[z]=R[z+8];
	    	  }
			  chronogramme();
   	    }
        if (RxHeader.Identifier == 0x13)
   	    {
		  	  lcdDrawRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  fillRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  lcdDrawRect(140, 30, 479, 4, ILI9488_BLACK);
		  	  fillRect(140, 30, 479, 40, ILI9488_BLACK);
			  s= snprintf(b,80,"---lumiere--\n%02x %02x %02x %02x %02x %02x %02x %02x\n %02x %02x %02x %02x %02x %02x %02x %02x\n--end-lumiere\n",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
			  CDC_Transmit_FS(b, s);
			  col=ILI9488_MAROON;
			  sprintf(text,"Data :");
			  Displ_WString(20, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"lumiere :");
			  Displ_WString(160, 30, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
			  Displ_WString(90, 90, text, Font16, 1, ILI9488_WHITE, col);
			  x=380;
			  y=120;
			  chronogramme();
			  x=380;
			  y=160;
			  for(int z=0;z<8;z++)
			  {
				  R[z]=R[z+8];
	    	  }
			  chronogramme();
   	    }
        if (RxHeader.Identifier == 0x53)
   	    {
		  	  lcdDrawRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  fillRect(0, 120, 479, 60, ILI9488_BLACK);
		  	  lcdDrawRect(140, 30, 479, 4, ILI9488_BLACK);
		  	  fillRect(140, 30, 479, 40, ILI9488_BLACK);
   		   	  s= snprintf(b,80,"---ABS--\n%02x %02x %02x %02x %02x %02x %02x %02x\n %02x %02x %02x %02x %02x %02x %02x %02x\n--end-ABS\n",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
   		   	  CDC_Transmit_FS(b, s);
			  col=ILI9488_PURPLE;
			  sprintf(text,"Data :");
			  Displ_WString(20, 60, text, Font16, 1, ILI9488_WHITE, col);
			  fin=1;
			  sprintf(text,"ABS :");
			  Displ_WString(160, 30, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 60, text, Font16, 1, ILI9488_WHITE, col);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[8],R[9],R[10],R[11],R[12],R[13],R[14],R[15]);
			  Displ_WString(90, 90, text, Font16, 1, ILI9488_WHITE, col);
			  x=380;
			  y=120;
			  chronogramme();
			  x=380;
			  y=160;
			  for(int z=0;z<8;z++)
			  {
				  R[z]=R[z+8];
	    	  }
			  chronogramme();
   	    }
    }
		  if(j==0)
		  {
		  switch (RxHeader.Identifier)
		  {
		  case 0x0d:
			  m++;
			  col=ILI9488_OLIVE;
			  x=380;
			  y=60;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_OLIVE);
			  e_data=engine_data;
			  if(m>1)
			  {
			  x=380;y=60;c=1;
			  col=ILI9488_OLIVE;
			  e_data=engine_data;
			  }
			  break;
		  case 0x46:
			  y=100;
			  x=380;
			  col=ILI9488_BLUE;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_BLUE);
			  lcdDrawRect(0, 60,479,25, ILI9488_BLACK);
			  fillRect(0, 60, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"vitesse :%dKm/h",(int)e_data);
			  Displ_CString(0,60,40,10,text2,Font16,1,ILI9488_WHITE,ILI9488_OLIVE);
			  e_data=engine_data;
			  break;
		  case 0x33:
			  y=140;
			  x=380;
			  col=ILI9488_DARKCYAN;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_DARKCYAN);
			  lcdDrawRect(0, 100, 479, 25, ILI9488_BLACK);
			  fillRect(0, 100, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Temperature:%dDegC",(int)e_data);
			  Displ_CString(0,100,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_BLUE);
			  e_data=engine_data;
			  break;
		  case 0x1f:
			  y=180;
			  x=380;
			  col=ILI9488_ORANGE;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_ORANGE);
			  lcdDrawRect(0, 140, 479, 25, ILI9488_BLACK);
			  fillRect(0, 140, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Pression :%dKPa",(int)e_data);
			  Displ_CString(0,140,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_DARKCYAN);
			  e_data=engine_data;
			  break;
		  case 0x21:
			  y=220;
			  x=380;
			  col=ILI9488_RED;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_RED);
			  lcdDrawRect(0, 180, 479, 25, ILI9488_BLACK);
			  fillRect(0, 180, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Temps :%dSec",(int)e_data);
			  Displ_CString(0,180,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_ORANGE);
			  e_data=engine_data;
			  break;
		  case 0x4d:
			  y=260;
			  x=380;
			  col=ILI9488_PINK;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_PINK);
			  lcdDrawRect(0, 220, 479, 25, ILI9488_BLACK);
			  fillRect(0, 220, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Distance :%dKm",(int)e_data);
			  Displ_CString(0,220,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_RED);
			  e_data=engine_data;
			  break;
		  case 0x0a:
			  y=300;
			  x=380;
			  col=ILI9488_PURPLE;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_PURPLE);
			  lcdDrawRect(0, 260, 479, 25, ILI9488_BLACK);
			  fillRect(0, 260, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Temps  :%dMin",(int)e_data);
			  Displ_CString(0,260,80,10,text2,Font16,1,ILI9488_WHITE,ILI9488_PINK);
			  e_data=engine_data;
			  break;
		  case 0x03:
			  x=380;
			  y=60;
			  col=ILI9488_MAROON;
			  lcdDrawRect(0, 300, 479, 25, ILI9488_BLACK);
			  fillRect(0, 300, 479, 25, ILI9488_BLACK);
			  sprintf(text2,"Pression carburant :%dKPa",(int)e_data);
			  Displ_CString(0,300,100,10,text2,Font16,1,ILI9488_WHITE,ILI9488_PURPLE);
			  e_data=engine_data;
			  break;
		  case 0x51:
			  y=100;
			  x=380;
			  fin=1;
//			  j=0;
//			  CaptureAndDisplayScreen();
			  col=ILI9488_WHITE;
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_WHITE);
			  break;
		  default:
			  break;
		  }
		  if(RxHeader.Identifier==0x03)
		  {
			  for(int u=0;u<5;u++)
			  {
				  lcdDrawRect(450, 10, 20, 10, ILI9488_BLACK);
				  fillRect(450, 10, 20, 10, ILI9488_BLACK);
			  }
			  horizanimD();
			  front_display();
			  Displ_CString(0,60,20,10,"0x03",Font16,1,ILI9488_WHITE,ILI9488_MAROON);
			  sprintf(text,"Data :");
			  Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			  sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
			  Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_MAROON);
			 for(int p=0;p<64;p++)
			 {
				 bas(ILI9488_MAROON);
			 }
			  Displ_CString(0,100,20,10,"0x51",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
			  y=100;
			  x=380;
			 for(int p=0;p<64;p++)
			 {
				 bas(ILI9488_WHITE);
			 }
			  x=380;
			  y=60;
		  }
		  if(c==1)
		  {
			     hautanim();
			     setRotation(Displ_Orientat_90);
			     if(speed==0)
			     {
				 sprintf(text,"CAN : 125Kbps");
				 Displ_WString(330, 10, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			     }
			     if(speed==1)
			     {
				 sprintf(text,"CAN : 500Kbps");
				 Displ_WString(330, 10, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			     }
			     if(speed==2)
			     {
				 sprintf(text,"CAN : 1Mbps");
				 Displ_WString(330, 10, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
			     }
				 Displ_CString(0,60,20,10,"0x0D",Font16,1,ILI9488_WHITE,ILI9488_OLIVE);
				 y=60;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_OLIVE);
				 }
				 Displ_CString(0,100,20,10,"0x46",Font16,1,ILI9488_WHITE,ILI9488_BLUE);
				 y=100;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_BLUE);
				 }
				 Displ_CString(0,140,20,10,"0x33",Font16,1,ILI9488_WHITE,ILI9488_DARKCYAN);
				 y=140;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_DARKCYAN);
				 }
				 Displ_CString(0,180,20,10,"0x1F",Font16,1,ILI9488_WHITE,ILI9488_ORANGE);
				 y=180;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_ORANGE);
				 }
				 Displ_CString(0,220,20,10,"0x21",Font16,1,ILI9488_WHITE,ILI9488_RED);
				 y=220;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_RED);
				 }
				 Displ_CString(0,260,20,10,"0x4D",Font16,1,ILI9488_WHITE,ILI9488_PINK);
				 y=260;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_PINK);
				 }
				 Displ_CString(0,300,20,10,"0x0A",Font16,1,ILI9488_WHITE,ILI9488_PURPLE);
				 y=300;
				 x=380;
				 for(int p=0;p<64;p++)
				 {
					 bas(ILI9488_PURPLE);
				 }
				 c=0;
				 y=60;
				 x=380;
				 sprintf(text,"Data :");
				 Displ_WString(20, 30, text, Font16, 1, ILI9488_WHITE, ILI9488_BLACK);
				 sprintf(text,"%02x %02x %02x %02x %02x %02x %02x %02x",R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7]);
				 Displ_WString(90, 30, text, Font16, 1, ILI9488_BLACK, ILI9488_OLIVE);
		  }
		  chronogramme();
		  if(RxHeader.Identifier==0x51)
		  {
			  lcdDrawRect(0, 100, 479, 25, ILI9488_BLACK);
			  fillRect(0, 100, 479, 25, ILI9488_BLACK);
			  lcdDrawRect(0, 100, 479, 25, ILI9488_BLACK);
			  fillRect(0, 100, 479, 25, ILI9488_BLACK);
			  lcdDrawRect(0, 100, 479, 25, ILI9488_BLACK);
			  fillRect(0, 100, 479, 25, ILI9488_BLACK);
			  lcdDrawRect(0, 100, 479, 25, ILI9488_BLACK);
			  fillRect(0, 100, 479, 25, ILI9488_BLACK);
			  Displ_CString(0,100,120,20,"Type de carburant :",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
              if (((int) engine_data) & 0x01)
              {
   			  Displ_CString(190,100,250,20,"Diesel",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
              }
              if (((int) engine_data) & 0x02) {
       		  Displ_CString(190,100,230,10,"Essence",Font16,1,ILI9488_BLACK,ILI9488_WHITE);
              }
		  }
		  }

		  }

    if (HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK)
    {
      /* Notification Error */
      Error_Handler();
    }
}
void haut(uint16_t col)
{
	 drawLine(x, y, x-5,y, col);//line hori
	 drawLine(x-5, y, x-5,y+2, col);//line hori
	 x=x-5;

}
void bas(uint16_t col)
{

	 drawLine(x, y+10, x-5,y+10, col);
	 drawLine(x-5, y+10, x-5,y+8, col);
	 x=x-5;
}
void lath(uint16_t col)
{

	 drawLine(x+5, y, x+5,y+10, col);//line vert + de px vers le bas
}
void lat(uint16_t col)
{

	 drawLine(x+5, y, x+5,y+10, col);
}
uint16_t rand_color()
{
    	        int num = (rand() % (11));
    	        switch(num)
    	        {
    	        case 0:
    	        	return ILI9488_OLIVE;
    	        	break;
    	        case 1:
    	        	return ILI9488_ORANGE;
    	        	break;
    	        case 2:
    	        	return ILI9488_RED;
    	        	break;
    	        case 3:
    	        	return ILI9488_BLUE;
    	        	break;
    	        case 4:
    	        	return ILI9488_DARKGREY;
    	        	break;
    	        case 5:
    	        	return ILI9488_PURPLE;
    	        	break;
    	        case 6:
    	        	return ILI9488_DARKCYAN;
    	        	break;
    	        case 7:
    	        	return ILI9488_NAVY;
    	        	break;
    	        case 8:
    	        	return ILI9488_GREENYELLOW;
    	        	break;
    	        case 9:
    	        	return ILI9488_WHITE;
    	        	break;
    	        case 10:
    	        	return ILI9488_YELLOW;
    	        	break;
    	        default:
    	        	break;
    	        }
}
void dinam(uint16_t col)
{
	for(int p=1;p<322;p=p+20)
	 {
		if(p==321)
		{
			p=320;
		}
		 lcdDrawRect(0,0,p+161,p,col);
		 fillRect(0,0,p+161,p,col);
	 }
}
void horizanim()
{
	for(int p=1;p<481;p=p+40)
	 {
		if(p==480)
		{
			p--;
		}
		 lcdDrawRect(0,0,p,320,ILI9488_BLACK);
		 fillRect(0,0,p,320,ILI9488_BLACK);
		 if(p==1)
		 {
		 p--;
		 }
	  }
}
void hautanim()
{
	for(int p=1;p<322;p=p+40)
	 {
		if(p==321)
		{
			p=320;
		}
		 lcdDrawRect(0,0,479,p,ILI9488_BLACK);
		 fillRect(0,0,479,p,ILI9488_BLACK);
	 }
}
void horizanimD()
{
	for(int p=1;p<481;p=p+40)
	 {
		if(p==480)
		{
			p--;
		}
		 lcdDrawRect(479-p,0,p,320,ILI9488_BLACK);
		 fillRect(479-p,0,p,320,ILI9488_BLACK);
		 if(p==1)
		 {
		 p--;
		 }
	  }
}
// Capture d'écran et affichage sur le LCD
//void CaptureAndDisplayScreen() {
//  // Allouer une mémoire tampon pour la capture d'écran
//  uint16_t screenBuffer[_width * _height];
//  ILI9488_SendCommand(ILI9488_CASET); // Commande pour définir la zone horizontale
//  ILI9488_SendCommand(ILI9488_PASET); // Commande pour définir la zone verticale
//  ILI9488_SendCommand(ILI9488_RAMWR); // Commande pour écrire les données en mémoire
//  // Envoyer les données de la mémoire tampon vers le LCD
//  for (int ij = 0; ij < _width * _height; ij++) {
//    // Envoyer les données RVB du pixel via SPI
//    data[0] = screenBuffer[ij] >> 8; // Octet de données MSB
//    data[1] = screenBuffer[ij] & 0xFF; // Octet de données LSB
//    HAL_SPI_Transmit_IT(&hspi4, data, sizeof(data));
//  }
//}
void chronogramme()
{
	  for(int z=0;z<8;z++)
	  {
		  a[0] = (R[z] & (0x01));
		  a[1] = (R[z] & (0x02));
		  a[2] = (R[z] & (0x04));
		  a[3] = (R[z] & (0x08));
		  a[4] = (R[z] & (0x10));
		  a[5] = (R[z] & (0x20));
		  a[6] = (R[z] & (0x40));
		  a[7] = (R[z] & (0x80));
		  for(int i=0;i<8;i++)
		  {
			  if(a[i]==0x00)
			  {
				  bas(col);
				  if(l==2)
				  {
					lath(col);
				  }
				  if((i==3) | (i==7))
				  {
					drawLine(x, y+10, x,y+8, ILI9488_RED);
				  }
				    l=1;
				  }
			  else
		     {
			       haut(col);
			       x=x+5;
			       bas(ILI9488_BLACK);
			       if(l==1)
			       {
				     lat(col);
			       }
			       l=2;
			       if((i==3) | (i==7))
			       {
					 drawLine(x, y, x,y+2, ILI9488_RED);
			       }
		     }
		  }
		  }
}
void front_display()
{
	switch(bufferr_receive[0])
	{
	case 0xc1:
	    Displ_CString(20,20,80,80,"Mode : CAN",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
	    Displ_CString(200,20,80,280,"500Kbps",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
    break;

	case 0xc2:
	    Displ_CString(20,20,80,80,"Mode : CAN",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
	    Displ_CString(200,20,80,280,"125Kbps",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
    break;

	case 0xf1:
	    Displ_CString(20,20,120,50,"Mode : FDCAN  125Kbps",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
    break;

	case 0xf2:
	    Displ_CString(20,20,80,80,"Mode : FDCAN",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
	    Displ_CString(200,20,80,280,"500Kbps",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
    break;

	case 0xf3:
	    Displ_CString(20,20,80,80,"Mode : FDCAN",Font16,3,ILI9488_WHITE,ILI9488_BLACK);
	    Displ_CString(200,20,80,280,"1Mbps",Font16,3,ILI9488_WHITE,ILI9488_BLACK);
    break;

	case 0xf4:
	    Displ_CString(20,20,80,80,"Mode : FDCAN",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
	    Displ_CString(200,20,80,280,"2Mbps",Font16,1,ILI9488_WHITE,ILI9488_BLACK);
    break;
default :
	break;

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
