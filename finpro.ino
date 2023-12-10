#include <WiFi.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include <SoftwareSerial.h>
//#include <DFPlayer_Mini_Mp3.h>

//SoftwareSerial mySerial(16, 17); // RX, TX
// Library untuk buzzer
#define BUZZER_PIN 26

#define NTP_SERVER     "pool.ntp.org"
#define UTC_OFFSET     0
#define UTC_OFFSET_DST 0

// Library untuk LCD I2C
LiquidCrystal_I2C LCD(0x27, 16, 2);

// Konfigurasi WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Konfigurasi waktu alarm
const int alarmHour = 4; // Jam alarm
const int alarmMinute = 22; // Menit alarm

// Inisialisasi buzzer
void buzzerInit() {
  pinMode(BUZZER_PIN, OUTPUT);
}

// Fungsi untuk menghasilkan bunyi pada buzzer
void buzzerBeep() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(2000);
  digitalWrite(BUZZER_PIN, LOW);
}

void setup() {
  LCD.init();
  LCD.backlight();
  LCD.setCursor(0, 0);
  LCD.print("Connecting to ");
  LCD.setCursor(0, 1);
  LCD.print("WiFi ");

  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    //spinner();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LCD.clear();
  LCD.setCursor(0, 0);
  LCD.println("Online");
  LCD.setCursor(0, 1);
  LCD.println("Updating time...");

  configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

void loop() {
  // Ambil waktu sekarang
  time_t now = time(nullptr);

  // Konversi waktu sekarang ke waktu lokal
  struct tm* localTime = localtime(&now);

  // Tampilkan waktu sekarang pada LCD
  LCD.setCursor(0, 0);
  LCD.print("Waktu: ");
  LCD.print(localTime->tm_hour);
  LCD.print(":");
  if (localTime->tm_min < 10) {
    LCD.print("0");
  }
  LCD.print(localTime->tm_min);
  LCD.print(":");
  if (localTime->tm_sec < 10) {
    LCD.print("0");
  }
  LCD.print(localTime->tm_sec);

  // Tampilkan waktu alarm pada LCD
  LCD.setCursor(0, 1);
  LCD.print("Alarm: ");
  LCD.print(alarmHour);
  LCD.print(":");
  if (alarmMinute < 10) {
    LCD.print("0");
  }
  LCD.print(alarmMinute);

  // Periksa apakah sudah waktunya alarm
  if (localTime->tm_hour == alarmHour && localTime->tm_min == alarmMinute && localTime->tm_sec == 0) {
    LCD.setCursor(0, 1);
    LCD.print("Waktunya alarm!");
    tone(BUZZER_PIN, 2000);
    delay(2000);
    noTone(BUZZER_PIN); //0001
    delay(2000);
  }

  delay(1000);
}
