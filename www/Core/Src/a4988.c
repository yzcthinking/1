#include "a4988.h"

static uint32_t step_delay_us = 1000U;

static void delay_us(uint32_t delay)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = delay * (HAL_RCC_GetHCLKFreq() / 1000000U);

    while ((DWT->CYCCNT - start) < cycles)
    {
    }
}

void A4988_Init(A4988_HandleTypeDef *driver)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    DWT->CYCCNT = 0U;

    HAL_GPIO_WritePin(driver->ms1_port, driver->ms1_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->ms2_port, driver->ms2_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->ms3_port, driver->ms3_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->dir_port, driver->dir_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->step_port, driver->step_pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin, GPIO_PIN_SET);
}

void A4988_SetDelayUs(uint32_t delay_us)
{
    step_delay_us = delay_us;
}

void A4988_Enable(A4988_HandleTypeDef *driver, uint8_t enable)
{
    HAL_GPIO_WritePin(driver->enable_port, driver->enable_pin,
                      enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void A4988_SetDirection(A4988_HandleTypeDef *driver, uint8_t direction)
{
    HAL_GPIO_WritePin(driver->dir_port, driver->dir_pin,
                      direction ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

HAL_StatusTypeDef A4988_SetStepMode(A4988_HandleTypeDef *driver, uint8_t step_mode)
{
    GPIO_PinState ms1 = GPIO_PIN_RESET;
    GPIO_PinState ms2 = GPIO_PIN_RESET;
    GPIO_PinState ms3 = GPIO_PIN_RESET;

    switch (step_mode)
    {
        case 1U:
            break;
        case 2U:
            ms1 = GPIO_PIN_SET;
            break;
        case 4U:
            ms2 = GPIO_PIN_SET;
            break;
        case 8U:
            ms1 = GPIO_PIN_SET;
            ms2 = GPIO_PIN_SET;
            break;
        case 16U:
            ms1 = GPIO_PIN_SET;
            ms2 = GPIO_PIN_SET;
            ms3 = GPIO_PIN_SET;
            break;
        default:
            return HAL_ERROR;
    }

    HAL_GPIO_WritePin(driver->ms1_port, driver->ms1_pin, ms1);
    HAL_GPIO_WritePin(driver->ms2_port, driver->ms2_pin, ms2);
    HAL_GPIO_WritePin(driver->ms3_port, driver->ms3_pin, ms3);
    return HAL_OK;
}

void A4988_StepOnce(A4988_HandleTypeDef *driver)
{
    HAL_GPIO_WritePin(driver->step_port, driver->step_pin, GPIO_PIN_SET);
    delay_us(step_delay_us);
    HAL_GPIO_WritePin(driver->step_port, driver->step_pin, GPIO_PIN_RESET);
    delay_us(step_delay_us);
}

void A4988_Step(A4988_HandleTypeDef *driver, uint32_t steps)
{
    while (steps-- > 0U)
    {
        A4988_StepOnce(driver);
    }
}
