#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Pasti Dadi";
const char* password = "bejo0808";

const char* telegramBotToken = "6594610699:AAHmWH2XpXgPGqoMygq-Zp7vdMEbTyurtR4";
const char* chatId = "6211031822";

const int flameSensorPin = 5;  // Pin tempat sensor kebakaran terhubung
const int buzzerPin = 4;       // Pin tempat buzzer terhubung

// Konfigurasi untuk LCD 16x2 menggunakan I2C (IC12)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  lcd.backlight();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Menghubungkan ke WiFi...");
    lcd.begin(16, 2);  // Inisialisasi LCD
    lcd.print("Mencari Wifi !");
  }

  Serial.println("Terhubung ke WiFi!");
  lcd.begin(16, 2);  // Inisialisasi LCD
  lcd.print("Terhubung Wifi");
  lcd.begin(16, 2);  // Inisialisasi LCD
  lcd.print("  Siap Menerima");
  lcd.setCursor(0, 1);
  lcd.print("    Data...");
}

void loop() {
  int flameStatus = digitalRead(flameSensorPin);

  if (flameStatus == LOW) {

    lcd.clear();
    lcd.print("Terdeteksi Api !");
    lcd.setCursor(0, 1);
    lcd.print("Mengirim Pesan..");



    playBuzzer(1000, 500);  // Contoh: bunyi 1000 Hz selama 500 ms
    delay(500);             // Jeda 500 ms
    for (int i = 0; i < 3; i++) {
      playBuzzer(1500, 200);  // Contoh: bunyi 1500 Hz selama 200 ms
      delay(200);             // Jeda 200 ms
    }


    kirimPesanTelegram("Info Penting Ada kebakaran !!!");

  } else {

    lcd.begin(16, 2);  // Inisialisasi LCD
    lcd.print(" Tidak Ada Api !!!");
  
  }

  delay(1000);
}

void kirimPesanTelegram(const char* pesan) {
  HTTPClient http;

  String url = "https://api.telegram.org/bot";
  url += telegramBotToken;
  url += "/sendMessage?chat_id=";
  url += chatId;
  url += "&text=";
  url += pesan;

  http.begin(url);

  int kodeResponHttp = http.GET();

  if (kodeResponHttp > 0) {
    Serial.printf("[HTTP] Mengirim... kode: %d\n", kodeResponHttp);
    if (kodeResponHttp == HTTP_CODE_OK) {
      String respons = http.getString();
      Serial.println(respons);
    }
  } else {
    Serial.printf("[HTTP] GAGAL, kesalahan: %s\n", http.errorToString(kodeResponHttp).c_str());
  }

  http.end();
}

void playBuzzer(int frequency, int duration) {
  tone(buzzerPin, frequency);  // Menghasilkan bunyi dengan frekuensi tertentu
  delay(duration);             // Berbunyi selama durasi yang ditentukan
  noTone(buzzerPin);           // Matikan bunyi
  delay(50);                   // Jeda singkat antara bunyi
}
