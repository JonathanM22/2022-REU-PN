#include <Arduino.h>

int count1 = 0;
int count2 = 0;

TaskHandle_t Task1;
TaskHandle_t Task2;

//Task 1 Function
void task1(void *parameters) {
  for (;;) {
    Serial.print("Task 1: ");
    Serial.println(count1++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

//Task 2 Function
void task2(void *parameters) {
  for (;;) {
    Serial.print("Task 2: ");
    Serial.println(count2++);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);

  //Create Task 1
  xTaskCreate(
    task1,    //function name
    "Task 1", //task name
    1000,     //stack size (bytes)
    NULL,     //Parameters
    1,        //Priority
    &Task1   //Handler
  );

  //Creat Task 2
    xTaskCreate(
    task2,    //function name
    "Task 2", //task name
    1000,     //stack size (bytes)
    NULL,     //Parameters
    1,        //Priority
    &Task2   //Handler
  ); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
