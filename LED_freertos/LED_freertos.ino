// Needed for atoi()
#include <stdlib.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t buf_len = 20;
#define LED_BUILTIN 2
unsigned int uFreq=1000;

void toggleLED (void *parameter){
  while(1){
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(uFreq/portTICK_PERIOD_MS);
    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(uFreq/portTICK_PERIOD_MS);
  }
}

void getFreq(void *parameter){

  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, buf_len);

  // Loop forever
  while (1) {

    // Read characters from serial
    if (Serial.available() > 0) {
      c = Serial.read();

      // Update delay variable and reset buffer if we get a newline character
      if (c == '\n') {
        uFreq = atoi(buf);
        Serial.print("Updated LED delay to: ");
        Serial.println(uFreq);
        memset(buf, 0, buf_len);
        idx = 0;
      } else {
        
        // Only append if index is not over message limit
        if (idx < buf_len - 1) {
          buf[idx] = c;
          idx++;
        }
      }
    }
  }
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  vTaskDelay(1000 / portTICK_PERIOD_MS);

  // Create a task
  xTaskCreatePinnedToCore(		// Use xTaskCreate() in Vanilla FreeRTOS (ESP official)
  					toggleLED,		// Function to be called
  					"Toggle LED",	// Name of task
  					1024,				// Stack size (bytes in ESP32, words in FreeRTOS)
  					NULL,				// Parameter to pass to function
  					1,					// Priority (the highest the number the highest the priority, 0 to config_MAX_PRIORITIES - 1)
  					NULL,				// Task handle
  					app_cpu);		// Run on one core for this demo (ESP32 only)
  
  xTaskCreatePinnedToCore(    // Use xTaskCreate() in Vanilla FreeRTOS (ESP official)
            getFreq,    // Function to be called
            "Get frequency", // Name of task
            1024,       // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,       // Parameter to pass to function
            1,          // Priority (the highest the number the highest the priority, 0 to config_MAX_PRIORITIES - 1)
            NULL,       // Task handle
            app_cpu);   // Run on one core for this demo (ESP32 only)
  //In vanilla RTOS you'd want to call vTaskStartScheduler() in main after setting up your tasks
  vTaskDelete(NULL);
}

void loop() {
  // put your main code here, to run repeatedly:

}
