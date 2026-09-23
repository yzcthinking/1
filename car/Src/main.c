#include "car_control.h"

static void Delay_Ms(uint32_t ms)
{
    volatile uint32_t count = 0U;
    const uint32_t limit = (uint32_t)ms * 20000U;

    while (count < limit) {
        ++count;
    }
}

static void Run_For_Ms(float linear_x, float linear_y, float angular_z, uint32_t ms)
{
    uint32_t i;

    Set_Vel(linear_x, linear_y, angular_z);
    for (i = 0U; i < ms; ++i) {
        Motor_Task(1000U);
        Delay_Ms(1U);
    }
}

static void Run_Distance_M(float linear_x, float linear_y, float angular_z, float distance_m)
{
    const float target_speed = linear_x;
    uint32_t duration_ms;

    if (target_speed <= 0.0f) {
        Run_For_Ms(linear_x, linear_y, angular_z, 0U);
        return;
    }

    duration_ms = (uint32_t)((distance_m / target_speed) * 1000.0f + 0.5f);
    Run_For_Ms(linear_x, linear_y, angular_z, duration_ms);
}

int main(void)
{
    Car_Hardware_Init();

    while (1) {
        Run_Distance_M(0.30f, 0.0f, 0.0f, 0.95f);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 500U);
        Run_For_Ms(0.0f, 0.0f, -0.60f, 2620U);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 500U);
        Run_For_Ms(-0.5f, 0.0f, 0.0f, 1000U);
        Run_For_Ms(0.0f, 0.0f, 0.0f,5000U);
        Run_For_Ms(0.5f, 0.0f, 0.0f, 1000U);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 200U);
        Run_For_Ms(0.0f, 0.0f, 0.60f, 2620U);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 200U);
        Run_Distance_M(0.0f, 0.0f, 0.0f, 0.75);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 500U);
        Run_For_Ms(0.0f, 0.0f, 0.60f, 2620U);
        Run_For_Ms(0.0f, 0.0f, 0.0f, 200U);
        Run_Distance_M(0.3f,0.0f,0.0f,0.9f );
        Run_For_Ms(0.0f,0.0f,0.0f,200U);
        Run_For_Ms(0.0f,0.1f,0.0f,200U);
        Run_For_Ms(0.0f,0.0f,0.0f,200U);
        Run_For_Ms(0.0f,0.0f,0.6f,2620U);
        Run_For_Ms(0.0f,0.0f,0.0f,200U);
     
    }
}
