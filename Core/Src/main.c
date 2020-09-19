/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "rtthread.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define THREAD_PRIORITY         25
#define THREAD_STACK_SIZE       512
#define THREAD_TIMESLICE        5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

static void thread1_entry(void *parameter);
static void thread2_entry(void *parameter);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint8_t num = 0;
uint8_t press_num = 0;
static rt_thread_t receive_thread1 = RT_NULL;
static rt_thread_t receive_thread2 = RT_NULL;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */
    /* 创建线程 1，名称是 thread1，入口是 thread1_entry*/
    receive_thread1 = rt_thread_create("thread1",
                            thread1_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (receive_thread1 != RT_NULL) {
        rt_thread_startup(receive_thread1);
    }
    /* 创建线程 2，名称是 thread2，入口是 thread2_entry*/
    receive_thread2 = rt_thread_create("thread2",
                            thread2_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* 如果获得线程控制块，启动这个线程 */
    if (receive_thread2 != RT_NULL) {
        rt_thread_startup(receive_thread2);
    }
    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    /* USER CODE END 3 */
}

/* 线程 1 的入口函数 */
static void thread1_entry(void *parameter)
{
    GPIO_PinState temp;
    while (1) {
        temp = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
        if (temp) {
            if (press_num == 2) {
                num = 0;
                press_num = 0;
            } else {
                num++;
                press_num++;
            }
            // 按键消抖
            rt_thread_delay(1000);
        } else {
            if (press_num == 1) {
                press_num--;
            }
        }
    }
}
/* 线程 2 的入口函数 */
static void thread2_entry(void *parameter)
{
    while (1) {
        rt_kprintf("num:%d\r\n",num);
        rt_kprintf("press_num:%d\r\n",press_num);
        if (press_num == 2) {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
            rt_thread_yield();
        } else {
            switch (num % 3) {
                case 0:
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
                    rt_thread_yield();
                    break;
                case 1:
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
                    rt_thread_delay(1000);
                    break;
                case 2:
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_2);
                    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_3);
                    rt_thread_delay(500);
                    break;
            }
        }
    }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
