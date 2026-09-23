#ifndef CAR_CONTROL_H
#define CAR_CONTROL_H

#include <stdint.h>

typedef uint32_t u32;

typedef struct { float linear_x; float linear_y; float angular_z; } CarVelocity;
typedef struct { float motor_1; float motor_2; float motor_3; float motor_4; } CarWheelValues;
typedef struct {
    CarVelocity exp_vel;
    CarVelocity fb_vel;
    CarWheelValues exp_wheel_rpm;
    CarWheelValues fb_wheel_rpm;
    CarWheelValues pwm;
    float wheels_x_distance_;
    float wheels_y_distance_;
    float wheel_circumference_;
    float max_rpm_;
} CarKinematics;
typedef struct {
    float kp;
    float ki;
    float kd;
    float integral;
    float previous_error;
    float out;
    float output_min;
    float output_max;
} CarPid;

enum { FL = 0, FR = 1, BL = 2, BR = 3 };

extern CarKinematics kinematics;
extern CarPid pid[4];
extern CarPid pid_yaw;

void Set_Vel(float linear_x, float linear_y, float angular_z);
void Motor_Task(u32 dT_us);
void PID_Controller(u32 dT_us, float target, float feedback, CarPid *controller,
                    float integral_limit, float output_limit);
float Get_MiMx(float value, float minimum, float maximum);

void Car_Hardware_Init(void);
void Car_Read_Encoder(int32_t encoder[4]);
void Car_Write_PWM(const float pwm[4]);
void Car_Write_RPM(const float rpm[4]);

#endif