#define BLYNK_TEMPLATE_ID "TMPL6XDJA1Zzc"
#define BLYNK_TEMPLATE_NAME "FINPRO"
#define BLYNK_AUTH_TOKEN "9POqiLlSwRJ3PV6AISrOHRFk7Dp9G0UW"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "ssi";
const char* password = "password";

// I2C address for the LCD
const int address = 0x27;

// NTP server settings
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 25200
#define UTC_OFFSET_DST 25200

// Initialize the LCD (16x2 characters)
LiquidCrystal_I2C lcd(address, 16, 2);

// Initialize the servo
Servo servo;

// Define your Blynk Auth Token
char auth[] = BLYNK_AUTH_TOKEN;

// Pin definitions
const int servoPin = 13; // Servo motor pin
const int buzzerPin = 2; // Buzzer pin

// Duration for the spray action
const long SPRAY_DURATION = 2000; // 2 seconds

// Virtual pins for Blynk
#define VIRTUAL_PIN_HOUR V1
#define VIRTUAL_PIN_MINUTE V2
#define VIRTUAL_PIN_alarmHour V3
#define VIRTUAL_PIN_alarmMinute V4
#define VIRTUAL_PIN_alarm_On_Off V5

// Timer interval for the loop
const long timerInterval = 1000;

// Variables for the alarm state and time
int alarmState = 1;
int alarmMinute;
int alarmHour;
int hour;
int minute;
int repeat;

// Mutex for LCD access to prevent conflicts
SemaphoreHandle_t xMutex;

// Forward declarations of functions
void updateTimeTask(void *pvParameters);
void checkAlarmTask(void *pvParameters);
void handleBlynkTask(void *pvParameters);
void triggerAlarm();

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Initialize Blynk
  Blynk.begin(auth, ssid, password);

  // Set up NTP server for time synchronization
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);

  // Initialize the mutex
  xMutex = xSemaphoreCreateMutex();

  // Create tasks for updating time, checking alarm, and handling Blynk
  xTaskCreate(updateTimeTask, "Update Time Task", 4096, NULL, 1, NULL);
  xTaskCreate(checkAlarmTask, "Check Alarm Task", 4096, NULL, 1, NULL);
  xTaskCreate(handleBlynkTask, "Handle Blynk Task", 4096, NULL, 1, NULL);

  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty loop since FreeRTOS is handling tasks
}

// Task for updating the time on the LCD
void updateTimeTask(void *pvParameters) {
 while (1) {
 time_t now = time(nullptr);
 struct tm localTime;
 localtime_r(&now, &localTime);

 // Use mutex to access the shared resource (LCD)
 if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Waktu: ");
   lcd.print(localTime.tm_hour);
   hour = localTime.tm_hour;
   lcd.print(":");
   lcd.print(localTime.tm_min < 10 ? "0" : "");
   lcd.print(localTime.tm_min);
   minute = localTime.tm_min;
   lcd.print(":");
   lcd.print(localTime.tm_sec < 10 ? "0" : "");
   lcd.print(localTime.tm_sec);
   xSemaphoreGive(xMutex);
 }
 vTaskDelay(pdMS_TO_TICKS(1000));
 }
}




// Task for checking the alarm and triggering events
void checkAlarmTask(void *pvParameters) {
  while (1) {
    if (alarmState && hour == alarmHour && minute == alarmMinute) {
      triggerAlarm();
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Task for handling Blynk communication
void handleBlynkTask(void *pvParameters) {
  while (1) {
    Blynk.run();
    if (xSemaphoreTake(xMutex, portMAX_DELAY)) {
      lcd.setCursor(0, 1);
      lcd.print("Alarm: ");
      lcd.print(alarmHour);
      lcd.print(":");
      lcd.print(alarmMinute < 10 ? "0" : "");
      lcd.print(alarmMinute);
      xSemaphoreGive(xMutex);
    }
    Blynk.virtualWrite(VIRTUAL_PIN_HOUR, hour);
    Blynk.virtualWrite(VIRTUAL_PIN_MINUTE, minute);
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Blynk functions to handle virtual pin writes
BLYNK_WRITE(VIRTUAL_PIN_alarmHour) {
  alarmHour = param.asInt();
}

BLYNK_WRITE(VIRTUAL_PIN_alarmMinute) {
  alarmMinute = param.asInt();
}

BLYNK_WRITE(VIRTUAL_PIN_alarm_On_Off) {
  alarmState = param.asInt();
}

// Function to trigger the alarm
void triggerAlarm() {
  servo.attach(servoPin);
  servo.write(160);
  delay(SPRAY_DURATION);
  servo.write(90);
  delay(SPRAY_DURATION);
  servo.detach();

  // Play buzzer tone (1 kHz for 500 ms)
  tone(buzzerPin, 1000, 500);
  delay(500);

  // Play another buzzer tone (2 kHz for 250 ms)
  tone(buzzerPin, 2000, 250);
  delay(500);

  // Silence the buzzer
  noTone(buzzerPin);
}