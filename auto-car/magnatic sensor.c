#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "camera.h"

void SystemClock_Config(void);

extern volatile uint16_t ADC_Values[ADC_HISTORY_LEN][NUM_ADC_CHANNELS];

uint16_t max_f_m_ms;
uint16_t max_f_l_ms;
uint16_t max_f_r_ms;
uint16_t max_b_ms;

bool button_pressed = false;
bool button_trigger = false;

uint16_t tune_m_diff_m = 0;
uint16_t tune_t_diff_m = 0;

//reading button 1
void read_1(){
  static uint32_t last_time = 0;
  uint32_t now, button;

  now = HAL_GetTick();
  if (now - last_time >= 50) {
    button = HAL_GPIO_ReadPin(BUTTON1_GPIO_Port, BUTTON1_Pin);
    if (button == 0) {
      if (button_pressed == false) {
        button_trigger = true;
      }
      button_pressed = true;
    } else {
      button_pressed = false;
      button_trigger = false;
    }
    last_time = now;
  }
}

void setup_mode(){
  //front middle sensor = channel 0
  //front left sensor = channel 1
  //front right sensor = channel 2
  //back middle sensor = channel 4
  max_f_m_ms = ADC_Values[0][0];
  max_f_l_ms = ADC_Values[0][1];
  max_f_r_ms = ADC_Values[0][2];
  max_b_ms = ADC_Values[0][3];

  uint32_t int_fin = HAL_GetTick();

  //tuning time and flux difference to confirm turning


  bool finish_setup = false;
  bool first_fin = true;

  //read button_1 till it's pressed and start setup mode
  while(finish_setup == false){
    read_1();
    if(button_trigger == true){
      do{
        bool f_max_done = false;
        bool b_max_done = false;
        uint16_t now_f_m_ms = ADC_Values[0][0];
        uint16_t now_b_ms = ADC_Values[0][3];
        uint16_t now_f_l_ms = ADC_Values[0][1];
        uint16_t now_f_r_ms = ADC_Values[0][2];
        if(now_f_m_ms > max_f_m_ms && now_b_ms == now_f_m_ms){
          HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
          max_f_m_ms = now_f_m_ms;
          max_f_l_ms = now_f_l_ms;
          max_f_r_ms = now_f_r_ms;
        }
        else{
          f_max_done = true;
        }
        if(now_b_ms > max_b_ms && now_b_ms == now_f_m_ms){
          HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_RESET);
          max_b_ms = now_b_ms;
        }
        else{
          b_max_done = true;
        }
        if(f_max_done == true && b_max_done == true){
          HAL_GPIO_WritePin(LED1_GPIO_Port,LED1_Pin,GPIO_PIN_SET);
        }
        if(now_f_m_ms == max_f_m_ms && now_b_ms == max_b_ms){
          if(first_fin == false){
            int_fin = HAL_GetTick();
            first_fin = true;
          }
          else if(HAL_GetTick() - int_fin >= 1000){
            HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);
            finish_setup = true;
          }
        }
        else{
          HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_RESET);
          first_fin = false;
        }
      }while(finish_setup == false);
    }
  }
}

int main(){
  return 0;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV8;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {}
