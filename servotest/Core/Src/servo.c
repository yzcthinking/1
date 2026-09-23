#include "servo.h"
#include "math.h"
#define PI 3.1415f
#define L1 122.5f
#define L2 130.0f
#define alpha1_0 108.0f
#define alpha2_0 7.75f
extern TIM_HandleTypeDef htim1;


void Servo_SetAngle(TIM_HandleTypeDef *htim, uint32_t Channel, float angle)
{
    if(angle < 0.0f)
        angle = 0.0f;
    if(angle > 180.0f)
        angle = 180.0f;
    uint16_t pulse = (uint16_t)(500.0f + (angle / 180.0f) * 2000.0f);

    __HAL_TIM_SET_COMPARE(htim, Channel, pulse);
}
void Servo_test1(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
  HAL_Delay(1000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(1000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
  HAL_Delay(1000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(10000);
}
void Servo_test2(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,120.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,60.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,120.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(5000);
}
void Servo_test3(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,30.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,70.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,30.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,50.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,30.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,40.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,30.0f);
  HAL_Delay(500);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
  HAL_Delay(500);
}
void Servo_test4(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(3000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,150.0f);
  HAL_Delay(3000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(3000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,120.0f);
  HAL_Delay(3000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
  HAL_Delay(3000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,100.0f);
  HAL_Delay(5000);
}
void Servo_Init(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
}
void Servo_Init2(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
}
void Servo_Init_tremble(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,10.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,10.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,10.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,10.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,10.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,0.0f);
}
void Servo_Init_tremble2(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,170.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,170.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,170.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,170.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,170.0f);
	HAL_Delay(300);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
}
void hand(void)
{
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,130.0f);
	HAL_Delay(5000);
	Servo_SetAngle(&htim1,TIM_CHANNEL_1,180.0f);
	HAL_Delay(5000);
	
}
float xyset(float x, float y)
{
    float dist = sqrtf(x * x + y * y);
    if (dist < 1.0e-6f)
    {
        Servo_SetAngle(&htim1, TIM_CHANNEL_1, 0.0f);
        return 0.0f;
    }

    float r2 = x * x + y * y;
    float max_r = (L1 + L2) * (L1 + L2);
    float min_r = (L1 - L2) * (L1 - L2);
    if (r2 > max_r || r2 < min_r)
    {
        Servo_SetAngle(&htim1, TIM_CHANNEL_1, 0.0f);
        return 0.0f;
    }

    float theta0 = atan2f(y, x);
    float theta = theta0 * 180.0f / PI;

    float cos_beta1 = (L1 * L1 + r2 - L2 * L2) / (2.0f * L1 * dist);
    float cos_beta2 = (L2 * L2 + r2 - L1 * L1) / (2.0f * L2 * dist);

    if (cos_beta1 > 1.0f) cos_beta1 = 1.0f;
    if (cos_beta1 < -1.0f) cos_beta1 = -1.0f;
    if (cos_beta2 > 1.0f) cos_beta2 = 1.0f;
    if (cos_beta2 < -1.0f) cos_beta2 = -1.0f;

    float beta1 = acosf(cos_beta1) * 180.0f / PI;
    float beta2 = acosf(cos_beta2) * 180.0f / PI;

    float alpha1 = theta + beta1;
    float alpha2 = theta - beta2;
    float theta1 = alpha1_0 - alpha1;
    float theta2 = 180.0f - (alpha2_0 - alpha2);

    float target = theta1;
    if (target < 0.0f) target = 0.0f;
    if (target > 180.0f) target = 180.0f;

    Servo_SetAngle(&htim1, TIM_CHANNEL_1, target);
    return target;
}
void perform(void)
{
Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,60.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,175.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,170.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,165.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,161.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,158.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,155.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,151.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,148.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,145.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,142.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,140.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,52.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,139.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,49.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,136.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,47.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,134.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,45.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,130.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,43.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,127.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,41.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,125.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,39.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,123.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,36.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,120.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,34.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,116.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,33.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,112.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,32.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,109.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,106.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,103.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,30.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,100.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim4,TIM_CHANNEL_1,0.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,60.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,175.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,170.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,165.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,161.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,158.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,155.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,151.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,148.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,145.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,142.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,140.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,52.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,139.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,49.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,136.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,47.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,134.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,45.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,130.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,43.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,127.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,41.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,125.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,39.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,123.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,36.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,120.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,34.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,116.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,33.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,112.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,32.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,109.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,106.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,103.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,30.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,100.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim4,TIM_CHANNEL_1,90.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,60.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,175.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,170.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,165.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,161.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,158.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,155.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,151.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,148.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,145.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,142.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,140.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,52.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,139.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,49.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,136.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,47.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,134.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,45.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,130.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,43.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,127.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,41.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,125.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,39.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,123.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,36.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,120.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,34.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,116.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,33.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,112.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,32.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,109.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,106.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,103.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,30.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,100.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim4,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,60.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,175.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,170.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,165.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,161.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,158.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,155.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,151.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,148.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,145.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,142.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,140.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,52.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,139.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,49.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,136.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,47.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,134.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,45.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,130.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,43.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,127.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,41.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,125.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,39.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,123.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,36.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,120.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,34.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,116.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,33.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,112.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,32.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,109.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,106.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,31.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,103.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,30.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,100.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,0.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,90.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,180.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,0.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,90.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,60.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,175.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,170.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,59.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,165.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,161.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,58.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,158.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,155.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,57.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,151.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,148.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,56.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,145.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.5f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,142.0f);
HAL_Delay(300);
Servo_SetAngle(&htim2,TIM_CHANNEL_1,55.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,140.0f);
HAL_Delay(1000);

Servo_SetAngle(&htim4,TIM_CHANNEL_1,0.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,90.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,180.0f);
HAL_Delay(500);

Servo_SetAngle(&htim4,TIM_CHANNEL_1,90.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,180.0f);
HAL_Delay(500);
Servo_SetAngle(&htim4,TIM_CHANNEL_1,0.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim2,TIM_CHANNEL_1,0.0f);
HAL_Delay(300);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(2000);

Servo_SetAngle(&htim3,TIM_CHANNEL_1,150.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,150.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,150.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,150.0f);
HAL_Delay(100);
Servo_SetAngle(&htim3,TIM_CHANNEL_1,180.0f);
HAL_Delay(100);
}
