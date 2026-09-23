#include "car_control.h"

/* STM32F407 running from the default 16 MHz HSI clock. */
#define RCC_BASE                0x40023800UL
#define GPIOA_BASE              0x40020000UL
#define GPIOC_BASE              0x40020800UL
#define TIM2_BASE               0x40000000UL
#define NVIC_ISER0              (*(volatile uint32_t *)0xE000E100UL)

#define RCC_AHB1ENR             (*(volatile uint32_t *)(RCC_BASE + 0x30UL))
#define RCC_APB1ENR             (*(volatile uint32_t *)(RCC_BASE + 0x40UL))

#define GPIO_MODER(base)        (*(volatile uint32_t *)((base) + 0x00UL))
#define GPIO_OTYPER(base)       (*(volatile uint32_t *)((base) + 0x04UL))
#define GPIO_OSPEEDR(base)      (*(volatile uint32_t *)((base) + 0x08UL))
#define GPIO_PUPDR(base)        (*(volatile uint32_t *)((base) + 0x0CUL))
#define GPIO_BSRR(base)         (*(volatile uint32_t *)((base) + 0x18UL))

#define TIM2_CR1                (*(volatile uint32_t *)(TIM2_BASE + 0x00UL))
#define TIM2_DIER               (*(volatile uint32_t *)(TIM2_BASE + 0x0CUL))
#define TIM2_SR                 (*(volatile uint32_t *)(TIM2_BASE + 0x10UL))
#define TIM2_EGR                (*(volatile uint32_t *)(TIM2_BASE + 0x14UL))
#define TIM2_PSC                (*(volatile uint32_t *)(TIM2_BASE + 0x28UL))
#define TIM2_ARR                (*(volatile uint32_t *)(TIM2_BASE + 0x2CUL))

#define STEP_MASK               ((1UL << 0) | (1UL << 2) | (1UL << 4) | (1UL << 6))
#define ENABLE_MASK             (1UL << 13)
#define STEPS_PER_REV           200.0f
#define STEP_TICK_HZ            50000.0f

static volatile uint32_t step_phase[4];
static volatile uint32_t step_increment[4];
static volatile uint8_t step_direction[4];

__attribute__((weak)) void SystemInit(void) {}

static void Configure_Output_Pin(uint32_t port, uint32_t pin)
{
    GPIO_MODER(port) &= ~(3UL << (pin * 2U));
    GPIO_MODER(port) |=  (1UL << (pin * 2U));
    GPIO_OTYPER(port) &= ~(1UL << pin);
    GPIO_OSPEEDR(port) |= (3UL << (pin * 2U));
    GPIO_PUPDR(port) &= ~(3UL << (pin * 2U));
}

void Car_Hardware_Init(void)
{
    uint32_t pin;

    RCC_AHB1ENR |= (1UL << 0) | (1UL << 2);
    RCC_APB1ENR |= (1UL << 0);

    for (pin = 0U; pin < 8U; ++pin) Configure_Output_Pin(GPIOA_BASE, pin);
    Configure_Output_Pin(GPIOC_BASE, 13U);

    GPIO_BSRR(GPIOA_BASE) = STEP_MASK << 16U;
    GPIO_BSRR(GPIOC_BASE) = ENABLE_MASK << 16U;

    /* 16 MHz / (15 + 1) / (19 + 1) = 50 kHz. */
    TIM2_PSC = 15U;
    TIM2_ARR = 19U;
    TIM2_EGR = 1U;
    TIM2_DIER = 1U;
    TIM2_CR1 = 1U;
    NVIC_ISER0 = (1UL << 28);
}

__attribute__((weak)) void Car_Read_Encoder(int32_t encoder[4])
{
    int i;
    for (i = 0; i < 4; ++i) encoder[i] = 0;
}

void Car_Write_RPM(const float rpm[4])
{
    int i;

    for (i = 0; i < 4; ++i) {
        float frequency = rpm[i] * STEPS_PER_REV / 60.0f;
        if (frequency < 0.0f) {
            step_direction[i] = 0U;
            frequency = -frequency;
        } else {
            step_direction[i] = 1U;
        }
        if (frequency > 120.0f * STEPS_PER_REV / 60.0f) {
            frequency = 120.0f * STEPS_PER_REV / 60.0f;
        }
        step_increment[i] = (uint32_t)(frequency * 4294967296.0f / STEP_TICK_HZ);
        if (step_direction[i] != 0U) {
            GPIO_BSRR(GPIOA_BASE) = (1UL << (1U + (uint32_t)(i * 2)));
        } else {
            GPIO_BSRR(GPIOA_BASE) = (1UL << (17U + (uint32_t)(i * 2)));
        }
    }
}

void Car_Write_PWM(const float pwm[4])
{
    (void)pwm;
}

void TIM2_IRQHandler(void)
{
    int i;

    TIM2_SR &= ~1UL;
    GPIO_BSRR(GPIOA_BASE) = STEP_MASK << 16U;
    for (i = 0; i < 4; ++i) {
        uint32_t previous = step_phase[i];
        step_phase[i] += step_increment[i];
        if (step_phase[i] < previous) GPIO_BSRR(GPIOA_BASE) = 1UL << (uint32_t)(i * 2);
    }
}