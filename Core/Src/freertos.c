/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#if 0
#include "wifi.h"
#else
#include "iot_wifi.h"
#endif
#include "aws_clientcredential.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Update SSID and PASSWORD with own Access point settings */
#if 0
#define AP_SSID     "U+NetD0C1"
#define AP_PASSWORD "5C81216DM$"
#endif

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void Wifi_conTest(void);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 384);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	Wifi_conTest();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
#if 0
void Wifi_conTest(void)
{
	uint8_t  IP_Addr[4];

	/*Initialize  WIFI module */
	if(WIFI_Init() ==  WIFI_STATUS_OK)
	{
		printf("> WIFI Module Initialized.\n");

		if( WIFI_Connect(AP_SSID, AP_PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
		{
			if(WIFI_GetIP_Address(IP_Addr) == WIFI_STATUS_OK)
			{
				printf("> es-wifi module got IP Address : %d.%d.%d.%d\n",
						IP_Addr[0], IP_Addr[1], IP_Addr[2], IP_Addr[3]);
			}
			else
			{
				printf("> ERROR : es-wifi module CANNOT get IP address\n");
			}
		}
		else
		{
			printf("> ERROR : es-wifi module NOT connected\n");
		}
	}
}
#else
static void prvWifiConnect( void )
{
    WIFINetworkParams_t xNetworkParams;
    WIFIReturnCode_t xWifiStatus;
    uint8_t ucIPAddr[ 4 ];

    /* Setup WiFi parameters to connect to access point. */
    xNetworkParams.pcSSID = clientcredentialWIFI_SSID;
    xNetworkParams.ucSSIDLength = sizeof( clientcredentialWIFI_SSID );
    xNetworkParams.pcPassword = clientcredentialWIFI_PASSWORD;
    xNetworkParams.ucPasswordLength = sizeof( clientcredentialWIFI_PASSWORD );
    xNetworkParams.xSecurity = clientcredentialWIFI_SECURITY;
    xNetworkParams.cChannel = 0;

    /* Try connecting using provided wifi credentials. */
    xWifiStatus = WIFI_ConnectAP( &( xNetworkParams ) );

    if( xWifiStatus == eWiFiSuccess )
    {
        printf("WiFi connected to AP %s.\r\n", xNetworkParams.pcSSID);

        /* Get IP address of the device. */
        WIFI_GetIP( &ucIPAddr[ 0 ] );

        printf("IP Address acquired %d.%d.%d.%d\r\n",
                        ucIPAddr[ 0 ], ucIPAddr[ 1 ], ucIPAddr[ 2 ], ucIPAddr[ 3 ]);
    }
    else
    {
        /* Connection failed configure softAP to allow user to set wifi credentials. */
        printf("WiFi failed to connect to AP %s.\r\n", xNetworkParams.pcSSID);

        /*
        xNetworkParams.pcSSID = wificonfigACCESS_POINT_SSID_PREFIX;
        xNetworkParams.pcPassword = wificonfigACCESS_POINT_PASSKEY;
        xNetworkParams.xSecurity = wificonfigACCESS_POINT_SECURITY;
        xNetworkParams.cChannel = wificonfigACCESS_POINT_CHANNEL;

        printf( ( "Connect to softAP %s using password %s. \r\n",
                        xNetworkParams.pcSSID, xNetworkParams.pcPassword ) );
        while( WIFI_ConfigureAP( &xNetworkParams ) != eWiFiSuccess )
        {
            printf( ( "Connect to softAP %s using password %s and configure WiFi. \r\n",
                            xNetworkParams.pcSSID, xNetworkParams.pcPassword ) );
        }
        printf( ( "WiFi configuration successful. \r\n", xNetworkParams.pcSSID ) );
        */
    }
}

void Wifi_conTest(void)
{
    WIFIReturnCode_t xWifiStatus;

    xWifiStatus = WIFI_On();

    if( xWifiStatus == eWiFiSuccess )
    {
    	printf("WiFi module initialized.\r\n");

    	prvWifiConnect();
    }
}
#endif
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
