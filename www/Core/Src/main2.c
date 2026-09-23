#include "main.h"
#include "tim.h"
#include "gpio.h"

STM32_Programmer_CLI.exe -c port=USB1 -w build/Release/www.hex -vvoid SystemClock_Config(void);
static void Servo_SetAngle(uint8_t angle);

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM1_Init();

	if (HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	while (1)
	{
		for (uint16_t angle = 0U; angle <= 180U; angle += 5U)
		{
			Servo_SetAngle((uint8_t)angle);
			HAL_Delay(50U);
		}

		for (int16_t angle = 175; angle >= 0; angle -= 5)
		{
			Servo_SetAngle((uint8_t)angle);
			HAL_Delay(50U);
		}
	}
}

static void Servo_SetAngle(uint8_t angle)
{
	uint32_t pulse_us;

	if (angle > 180U)
	{
		angle = 180U;
	}

	/* MG996R: 50 Hz PWM, approximately 500 to 2500 us pulse width. */
	pulse_us = 500U + ((uint32_t)angle * 2000U) / 180U;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, pulse_us);
}

void SystemClock_Config(void)
{
	RCC_OscInitTypeDef osc_config = {0};
	RCC_ClkInitTypeDef clock_config = {0};

	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_config.HSIState = RCC_HSI_ON;
	osc_config.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	osc_config.PLL.PLLState = RCC_PLL_ON;
	osc_config.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_config.PLL.PLLM = 8;
	osc_config.PLL.PLLN = 168;
	osc_config.PLL.PLLP = RCC_PLLP_DIV2;
	osc_config.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&osc_config) != HAL_OK)
	{
		Error_Handler();
	}

	clock_config.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
													 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clock_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clock_config.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clock_config.APB1CLKDivider = RCC_HCLK_DIV4;
	clock_config.APB2CLKDivider = RCC_HCLK_DIV2;
	if (HAL_RCC_ClockConfig(&clock_config, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}
}

void Error_Handler(void)
{
	__disable_irq();
	while (1)
	{
	}
}
