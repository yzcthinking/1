#include "freertos_tasks.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "car_control.h"
#include "tim.h"
#include "tft.h"
#include "usart.h"

typedef enum {
  APP_EVENT_QR = 1,
  APP_EVENT_VELOCITY,
  APP_EVENT_SERVO
} AppEventType;

typedef struct {
  AppEventType type;
  char text[96];
  float x;
  float y;
  float z;
  uint8_t servo;
  uint8_t angle;
} AppEvent;

static QueueHandle_t event_queue;
static TaskHandle_t demo_task;

static uint8_t IsProductCode(const char *text)
{
  for (uint8_t index = 0U; index < 15U; ++index) {
    if (index == 3U || index == 7U || index == 11U) {
      if (text[index] != '+') return 0U;
    } else if (text[index] < '0' || text[index] > '9') {
      return 0U;
    }
  }
  return text[15] == '\0';
}

static void DriveTask(void *argument)
{
  TickType_t wake = xTaskGetTickCount();
  (void)argument;
  for (;;) {
    Car_Control_Update();
    vTaskDelayUntil(&wake, pdMS_TO_TICKS(20U));
  }
}

static uint8_t DemoWait(uint32_t duration_ms)
{
  return ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(duration_ms)) != 0U;
}

static void DemoTask(void *argument)
{
  (void)argument;

  Car_Control_SetVelocity(0.05f, 0.0f, 0.0f);
  if (DemoWait(2000U) != 0U) goto done;
  Car_Control_Stop();
  if (DemoWait(500U) != 0U) goto done;

  Car_Control_SetVelocity(-0.05f, 0.0f, 0.0f);
  if (DemoWait(2000U) != 0U) goto done;
  Car_Control_Stop();
  if (DemoWait(500U) != 0U) goto done;

  Car_Control_SetVelocity(0.0f, 0.0f, 0.50f);
  if (DemoWait(1500U) != 0U) goto done;
  Car_Control_Stop();

done:
  demo_task = NULL;
  vTaskDelete(NULL);
}

static void CameraUartTask(void *argument)
{
  char line[96];
  size_t length = 0U;
  uint8_t byte;
  AppEvent event;
  (void)argument;

  for (;;) {
    if (USART1_ReadByte(&byte) == 0U) {
      vTaskDelay(pdMS_TO_TICKS(1U));
      continue;
    }
    if (byte == '\n' || byte == '\r') {
      if (length == 0U) continue;
      line[length] = '\0';
      memset(&event, 0, sizeof(event));
      if (sscanf(line, "VEL:%f,%f,%f", &event.x, &event.y, &event.z) == 3) {
        event.type = APP_EVENT_VELOCITY;
      } else if (sscanf(line, "SERVO:%hhu,%hhu", &event.servo, &event.angle) == 2 &&
                 event.servo >= 1U && event.servo <= 4U && event.angle <= 20U) {
        event.type = APP_EVENT_SERVO;
      } else if (sscanf(line, "SERVO:%hhu", &event.angle) == 1 && event.angle <= 20U) {
        event.servo = 1U;
        event.type = APP_EVENT_SERVO;
      } else if (strncmp(line, "QR:", 3U) == 0) {
        event.type = APP_EVENT_QR;
        strncpy(event.text, line + 3, sizeof(event.text) - 1U);
      }
      if (event.type != 0) xQueueSend(event_queue, &event, 0U);
      length = 0U;
    } else if (length < sizeof(line) - 1U) {
      line[length++] = (char)byte;
    } else {
      length = 0U;
    }
  }
}

static void ControlTask(void *argument)
{
  AppEvent event;
  (void)argument;
  for (;;) {
    if (xQueueReceive(event_queue, &event, portMAX_DELAY) != pdPASS) continue;
    if (event.type == APP_EVENT_QR) {
      if (IsProductCode(event.text) != 0U) {
        TFT_WriteLargeCode(event.text, 0xFFFFU, 0x0000U);
      }
    } else if (event.type == APP_EVENT_VELOCITY) {
      if (demo_task != NULL) xTaskNotifyGive(demo_task);
      Car_Control_SetVelocity(event.x, event.y, event.z);
    } else if (event.type == APP_EVENT_SERVO) {
      static TIM_HandleTypeDef *const timers[] = {
        &htim1, &htim5, &htim3, &htim4
      };
      __HAL_TIM_SET_COMPARE(timers[event.servo - 1U], TIM_CHANNEL_1,
                            1000U + ((uint32_t)event.angle * 1000U) / 20U);
    }
  }
}

void App_RTOS_Start(void)
{
  event_queue = xQueueCreate(8U, sizeof(AppEvent));
  configASSERT(event_queue != NULL);

  Car_Control_Init();
  Car_Control_Stop();

  configASSERT(xTaskCreate(DriveTask, "drive", 256U, NULL, 3U, NULL) == pdPASS);
  configASSERT(xTaskCreate(CameraUartTask, "camera-uart", 384U, NULL, 4U, NULL) == pdPASS);
  configASSERT(xTaskCreate(ControlTask, "control", 384U, NULL, 3U, NULL) == pdPASS);
  configASSERT(xTaskCreate(DemoTask, "demo", 256U, NULL, 2U, &demo_task) == pdPASS);
  vTaskStartScheduler();
  for (;;) {}
}

void vApplicationMallocFailedHook(void)
{
  taskDISABLE_INTERRUPTS();
  for (;;) {}
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *name)
{
  (void)task;
  (void)name;
  taskDISABLE_INTERRUPTS();
  for (;;) {}
}

void vAssertCalled(const char *file, int line)
{
  (void)file;
  (void)line;
  taskDISABLE_INTERRUPTS();
  for (;;) {}
}