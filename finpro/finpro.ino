#define BLYNK_TEMPLATE_ID "TMPL6XDJA1Zzc"
#define BLYNK_TEMPLATE_NAME "FINPRO"
#define BLYNK_AUTH_TOKEN "9POqiLlSwRJ3PV6AISrOHRFk7Dp9G0UW"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
#include <ESP32Servo.h> // Library for servo control

const char* ssid = "ssid";
const char* password = "password";
const int address = 0x27; // lcd I2C address

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET 25200
#define UTC_OFFSET_DST 25200


LiquidCrystal_I2C lcd(address, 16, 2); // Initialize lcd
String timeStamp;
String formattedDate;
String dayStamp;
Servo servo; // Initialize servo
char auth[] = BLYNK_AUTH_TOKEN;
const int servoPin = 13; // Servo motor pin
const int buzzerPin = 2; // Buzzer pin
const long SPRAY_DURATION = 2000; // 2 seconds
#define VIRTUAL_PIN_HOUR V1
#define VIRTUAL_PIN_MINUTE V2
#define VIRTUAL_PIN_alarmHour V3
#define VIRTUAL_PIN_alarmMinute V4
#define VIRTUAL_PIN_alarm_On_Off V5

const long timerInterval = 1000;
int alarmState = 1;
int alarmMinute;
int alarmHour;
int hour;
int minute;
int repeat;


void setup() {
  lcd.init(); // Initialize lcd
  lcd.backlight(); // Turn on backlight
  lcd.setCursor(0, 0); // Set cursor position
  lcd.print("Alarm Water Waker"); // Display text
  lcd.setCursor(0, 1);
  lcd.print("Ready...");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0); // Set cursor position
  lcd.print("Connecting to "); // Display text
  lcd.setCursor(0, 1);
  lcd.print(ssid);
  delay(1000);
  lcd.clear();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  // Print local IP address and start web server
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("WiFi connected.");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.println("Online");
  lcd.setCursor(0, 1);
  lcd.println("Updating time...");
  Blynk.begin(auth, ssid, password);
  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
  

}

void loop() {
  time_t now = time(nullptr);

  struct tm* localTime = localtime(&now);
  // Tampilkan waktu sekarang pada lcd
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Waktu: ");
  lcd.print(localTime->tm_hour);
  hour = localTime->tm_hour;
  lcd.print(":");
  if (localTime->tm_min < 10) {
    lcd.print("0");
  }
  lcd.print(localTime->tm_min);
  minute = localTime->tm_min;
  lcd.print(":");
  if (localTime->tm_sec < 10) {
    lcd.print("0");
  }
  lcd.print(localTime->tm_sec);
  
  Blynk.run();
  lcd.setCursor(0, 1);
  lcd.print("Alarm: ");
  lcd.print(alarmHour);
  lcd.print(":");
  if (alarmMinute < 10) {
    lcd.print("0");
  }
  lcd.print(alarmMinute);
  
  Blynk.virtualWrite(VIRTUAL_PIN_HOUR, hour);
  Blynk.virtualWrite(VIRTUAL_PIN_MINUTE, minute);
  if (alarmState && hour == alarmHour && minute == alarmMinute) {
    triggerAlarm();
  }
  delay(1000);
}


BLYNK_WRITE(VIRTUAL_PIN_alarmHour){
  int pinValue = param.asInt();
  alarmHour = pinValue;
}
BLYNK_WRITE(VIRTUAL_PIN_alarmMinute){
  int pinValue = param.asInt();
  alarmMinute = pinValue;
}
BLYNK_WRITE(VIRTUAL_PIN_alarm_On_Off){
  int pinValue = param.asInt();
  if (pinValue == 1){
    alarmState = 1;
  } else {
    alarmState = 0;    
  }
}

void triggerAlarm() {
  servo.attach(servoPin);
  servo.write(160);
  delay(1000);
  servo.write(90);
  delay(1000);
  servo.write(160);
  delay(1000);
  servo.detach();
  delay(1000);

    // Play buzzer tone (1 kHz for 500 ms)
  tone(buzzerPin, 1000, 500);
  delay(500);

  // Play another buzzer tone (2 kHz for 250 ms)
  tone(buzzerPin, 2000, 250);
  delay(500);

  // Silence the buzzer
  noTone(buzzerPin);
  delay(1000);
}





