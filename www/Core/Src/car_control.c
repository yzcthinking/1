#include "car_control.h"

#include "main.h"
#include "tim.h"

#define CAR_WHEEL_CIRCUMFERENCE_M 0.1884f
#define CAR_HALF_TRACK_M          0.14f
#define CAR_MAX_RPM               120.0f
#define CAR_STEPS_PER_REV         200.0f
#define CAR_STEP_TICK_HZ          50000.0f

#define CAR_STEP_MASK (GPIO_PIN_2 | GPIO_PIN_4)
#define CAR_DIR_MASK  (GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_7)
#define CAR_FRONT_LEFT_STEP_PIN GPIO_PIN_6
#define CAR_REAR_RIGHT_STEP_PIN GPIO_PIN_7

static volatile uint32_t step_phase[4];
static volatile uint32_t step_increment[4];
static volatile float velocity_x;
static volatile float velocity_y;
static volatile float velocity_z;

static float Clamp(float value, float minimum, float maximum)
{
  if (value < minimum) return minimum;
  if (value > maximum) return maximum;
  return value;
}

static void WriteRpm(const float rpm[4])
{
  uint32_t direction_set = 0U;

  for (uint32_t index = 0U; index < 4U; ++index) {
    float frequency = Clamp(rpm[index], -CAR_MAX_RPM, CAR_MAX_RPM)
                    * CAR_STEPS_PER_REV / 60.0f;
    if (frequency < 0.0f) {
      frequency = -frequency;
    } else {
      direction_set |= GPIO_PIN_1 << (index * 2U);
    }
    step_increment[index] = (uint32_t)(frequency * 4294967296.0f / CAR_STEP_TICK_HZ);
  }

  GPIOA->BSRR = ((uint32_t)CAR_DIR_MASK << 16U) | direction_set;
}

void Car_Control_Init(void)
{
  GPIO_InitTypeDef gpio = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  gpio.Pin = CAR_STEP_MASK | CAR_DIR_MASK;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Pull = GPIO_NOPULL;
  gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &gpio);

  gpio.Pin = GPIO_PIN_13 | CAR_FRONT_LEFT_STEP_PIN | CAR_REAR_RIGHT_STEP_PIN;
  gpio.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &gpio);

  GPIOA->BSRR = (uint32_t)CAR_STEP_MASK << 16U;
  GPIOC->BSRR = (uint32_t)(GPIO_PIN_13 | CAR_FRONT_LEFT_STEP_PIN |
                           CAR_REAR_RIGHT_STEP_PIN) << 16U;

  __HAL_RCC_TIM2_CLK_ENABLE();
  TIM2->CR1 = 0U;
  TIM2->PSC = 83U;
  TIM2->ARR = 19U;
  TIM2->EGR = TIM_EGR_UG;
  TIM2->SR = 0U;
  TIM2->DIER = TIM_DIER_UIE;
  HAL_NVIC_SetPriority(TIM2_IRQn, 5U, 0U);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  TIM2->CR1 = TIM_CR1_CEN;

  (void)htim2;
}

void Car_Control_SetVelocity(float linear_x, float linear_y, float angular_z)
{
  velocity_x = linear_x;
  velocity_y = linear_y;
  velocity_z = angular_z;
}

void Car_Control_Update(void)
{
  const float x_rpm = velocity_x * 60.0f / CAR_WHEEL_CIRCUMFERENCE_M;
  const float y_rpm = velocity_y * 60.0f / CAR_WHEEL_CIRCUMFERENCE_M;
  const float yaw_rpm = velocity_z * 60.0f * CAR_HALF_TRACK_M / CAR_WHEEL_CIRCUMFERENCE_M;
  const float rpm[4] = {
    x_rpm - y_rpm - yaw_rpm,
    x_rpm + y_rpm + yaw_rpm,
    x_rpm + y_rpm - yaw_rpm,
    x_rpm - y_rpm + yaw_rpm
  };

  WriteRpm(rpm);
}

void Car_Control_Stop(void)
{
  Car_Control_SetVelocity(0.0f, 0.0f, 0.0f);
  WriteRpm((const float[4]){0.0f, 0.0f, 0.0f, 0.0f});
}

void Car_Control_TimerIrq(void)
{
  uint32_t step_set = 0U;

  TIM2->SR = (uint16_t)~TIM_SR_UIF;
  GPIOA->BSRR = (uint32_t)CAR_STEP_MASK << 16U;
  GPIOC->BSRR = (uint32_t)(CAR_FRONT_LEFT_STEP_PIN | CAR_REAR_RIGHT_STEP_PIN) << 16U;
  for (uint32_t index = 0U; index < 4U; ++index) {
    uint32_t previous = step_phase[index];
    step_phase[index] += step_increment[index];
    if (step_phase[index] < previous) {
      if (index == 0U) {
        GPIOC->BSRR = CAR_FRONT_LEFT_STEP_PIN;
      } else if (index == 3U) {
        GPIOC->BSRR = CAR_REAR_RIGHT_STEP_PIN;
      } else {
        step_set |= GPIO_PIN_2 << ((index - 1U) * 2U);
      }
    }
  }
  GPIOA->BSRR = step_set;
}