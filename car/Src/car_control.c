#include "car_control.h"

CarKinematics kinematics = {
    .wheels_x_distance_ = 0.12f,
    .wheels_y_distance_ = 0.16f,
    .wheel_circumference_ = 0.1884f,
    .max_rpm_ = 120.0f
};

CarPid pid[4] = {
    { .kp = 0.02f, .ki = 0.5f, .kd = 0.0f, .output_min = -1.0f, .output_max = 1.0f },
    { .kp = 0.02f, .ki = 0.5f, .kd = 0.0f, .output_min = -1.0f, .output_max = 1.0f },
    { .kp = 0.02f, .ki = 0.5f, .kd = 0.0f, .output_min = -1.0f, .output_max = 1.0f },
    { .kp = 0.02f, .ki = 0.5f, .kd = 0.0f, .output_min = -1.0f, .output_max = 1.0f }
};

CarPid pid_yaw = { .kp = 1.0f, .output_min = -6.0f, .output_max = 6.0f };
float Get_MiMx(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
    return value;
}

void PID_Controller(u32 dT_us, float target, float feedback, CarPid *controller,
                    float integral_limit, float output_limit)
{
    const float dt = (float)dT_us * 1.0e-6f;
    const float error = target - feedback;
    float derivative;

    if (dt <= 0.0f || controller == 0) return;
    controller->integral += error * dt;
    if (integral_limit > 0.0f) {
        controller->integral = Get_MiMx(controller->integral, -integral_limit, integral_limit);
    }
    derivative = (error - controller->previous_error) / dt;
    controller->previous_error = error;
    controller->out = controller->kp * error + controller->ki * controller->integral
                    + controller->kd * derivative;
    if (output_limit > 0.0f) {
        controller->out = Get_MiMx(controller->out, -output_limit, output_limit);
    } else {
        controller->out = Get_MiMx(controller->out, controller->output_min, controller->output_max);
    }
}

void Set_Vel(float linear_x, float linear_y, float angular_z)
{
    kinematics.exp_vel.linear_x = linear_x;
    kinematics.exp_vel.linear_y = linear_y;
    kinematics.exp_vel.angular_z = angular_z;
}

static void Exp_Speed_Cal(void)
{
    const float x_rpm = kinematics.exp_vel.linear_x * 60.0f / kinematics.wheel_circumference_;
    const float y_rpm = kinematics.exp_vel.linear_y * 60.0f / kinematics.wheel_circumference_;
    const float track_radius = (kinematics.wheels_x_distance_ + kinematics.wheels_y_distance_) * 0.5f;
    const float yaw_rpm = kinematics.exp_vel.angular_z * 60.0f * track_radius
                        / kinematics.wheel_circumference_;

    pid_yaw.out = yaw_rpm;
    kinematics.exp_wheel_rpm.motor_1 = x_rpm - y_rpm - yaw_rpm;
    kinematics.exp_wheel_rpm.motor_2 = x_rpm + y_rpm + yaw_rpm;
    kinematics.exp_wheel_rpm.motor_3 = x_rpm + y_rpm - yaw_rpm;
    kinematics.exp_wheel_rpm.motor_4 = x_rpm - y_rpm + yaw_rpm;
    ((float *)&kinematics.exp_wheel_rpm)[0] = Get_MiMx(kinematics.exp_wheel_rpm.motor_1, -kinematics.max_rpm_, kinematics.max_rpm_);
    ((float *)&kinematics.exp_wheel_rpm)[1] = Get_MiMx(kinematics.exp_wheel_rpm.motor_2, -kinematics.max_rpm_, kinematics.max_rpm_);
    ((float *)&kinematics.exp_wheel_rpm)[2] = Get_MiMx(kinematics.exp_wheel_rpm.motor_3, -kinematics.max_rpm_, kinematics.max_rpm_);
    ((float *)&kinematics.exp_wheel_rpm)[3] = Get_MiMx(kinematics.exp_wheel_rpm.motor_4, -kinematics.max_rpm_, kinematics.max_rpm_);
}

void Motor_Task(u32 dT_us)
{
    float *expected = (float *)&kinematics.exp_wheel_rpm;
    int i;

    (void)dT_us;
    Exp_Speed_Cal();
    for (i = 0; i < 4; ++i) {
        ((float *)&kinematics.pwm)[i] = expected[i];
    }
    Car_Write_RPM(expected);
}