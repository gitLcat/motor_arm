#include "servo.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_adc.h"

extern TIM_HandleTypeDef htim3;
extern ADC_HandleTypeDef hadc1;

void update_servo_from_pot(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_5; // PA5
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, 10);
    uint32_t adc_value = HAL_ADC_GetValue(&hadc1);

    uint32_t pulse_width = 1000 + (adc_value * 1000) / 4095;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pulse_width);
}