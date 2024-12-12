#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "config.h"



// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
// #define WIFI_SSID "WICOM"
// #define WIFI_PASSWORD "Wicom123"
#define WIFI_SSID "VIETTEL"
#define WIFI_PASSWORD "88888888"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAoCKv3BaqZz_mNPUJEgpAzcGT6HbL_r08"

// Insert Authorized Email and Corresponding Password
#define USER_EMAIL "loc1852002@gmail.com"
#define USER_PASSWORD "Martini1905"

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://fire-warning-ca3ca-default-rtdb.asia-southeast1.firebasedatabase.app/"

bool dataReady = false;

// define the display
Adafruit_SSD1306 display1(-1);
Adafruit_SSD1306 display2(-1);
// this is the default address of the display(0x78) on back of display
#define OLED1 0x3C
// this is after switching over the jumper.
#define OLED2 0x3D

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

String databasePath;
String tempPath;
String humPath;
String flamePath;
String COPath;

// Cấu trúc dữ liệu nhận từ Node Sensor
typedef struct {
  uint8_t node_id;         // ID của Node
  float temperature;       // Nhiệt độ
  float humidity;          // Độ ẩm
  bool flame_detected;     // Trạng thái phát hiện lửa
  bool co_detected;        // Trạng thái phát hiện khí CO
} SensorData;

SensorData data;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 10000;


// Initialize WiFi
void initWiFi() 
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void initESPNow() 
{
  WiFi.mode(WIFI_STA);
    // Initialize ESP-NOW

  // Tắt chế độ tiết kiệm năng lượng
  if (esp_wifi_set_ps(WIFI_PS_NONE) == ESP_OK) {
    Serial.println("Power Save Mode: NONE (Max Performance)");
  } else {
    Serial.println("Failed to disable Power Save mode");
  }

  // Đặt công suất phát tối đa
  if (esp_wifi_set_max_tx_power(78) == ESP_OK) {
    Serial.println("TX Power: Maximum (19.5 dBm)");
  } else {
    Serial.println("Failed to set TX Power");
  }

  if (esp_now_init() == ESP_OK) 
    Serial.println("ESP-NOW initialized"); 
  else {
    Serial.println("ESP-NOW initialization failed");
    return;
  }
  // esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));
}

void initFirebase() 
{
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;
  Firebase.begin(&config, &auth);
  Serial.println("Getting User UID");

  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
}

void sendFloat(String path, float value){
  if (Firebase.RTDB.setFloat(&fbdo, path.c_str(), value)){
    Serial.println("PASSED");
    Serial.print("TYPE: ");Serial.println(fbdo.dataType());
  }
  else {
    Serial.println("FAILED");
    Serial.print("REASON: ");Serial.println (fbdo.errorReason());
  }
}

void onDataRecv(const uint8_t *recvInfo, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));

  // Hiển thị dữ liệu trên Serial Monitor
  Serial.println("=============== Thong Tin Sensor ===============");
  Serial.print("Node ID: ");
  Serial.println(data.node_id);

  Serial.print("Nhiet do: ");
  Serial.print(data.temperature);
  Serial.println(" C");

  Serial.print("Do am: ");
  Serial.print(data.humidity);
  Serial.println(" %");

  Serial.print("Lua: ");
  Serial.println(data.flame_detected ? "Co Lua" : "Khong");

  Serial.print("Khi CO: ");
  Serial.println(data.co_detected ? "Nguy hiem" : "An toan");

  Serial.println("===============================================");
  dataReady = true;

  databasePath = "/Node_/";
  databasePath += String(data.node_id);
  // databasePath += String(data.node_id);

  // Update database path for sensor readings
  tempPath = databasePath;
  tempPath += "/temperature"; // --> UsersData/<user_uid>/temperature
  humPath = databasePath;
  humPath += "/humidity"; // --> UsersData/<user_uid>/humidity
  flamePath = databasePath;
  flamePath += "/flame"; // --> UsersData/<user_uid>/pressure
  COPath = databasePath;
  COPath += "/CO"; // --> UsersData/<user_uid>/CO
}


void setup(){
  Serial.begin(115200);
  initWiFi();
  initESPNow();
  initFirebase();
  esp_now_register_recv_cb(esp_now_recv_cb_t(onDataRecv));


  display1.begin(SSD1306_SWITCHCAPVCC, OLED1);
  display1.clearDisplay();
  display1.display();
  display2.begin(SSD1306_SWITCHCAPVCC, OLED2);
  display2.clearDisplay();
  display2.display();
  pinMode(BUZZ1, OUTPUT);
  pinMode(BUZZ2, OUTPUT);
  pinMode(LG1, OUTPUT);
  pinMode(LY1, OUTPUT);
  pinMode(LR1, OUTPUT);
  pinMode(LG2, OUTPUT);
  pinMode(LY2, OUTPUT);
  pinMode(LR2, OUTPUT);

  // digitalWrite(BUZZ1, LOW);
  // digitalWrite(BUZZ2, LOW);
  digitalWrite(LG1, HIGH);
  digitalWrite(LY1, HIGH);
  digitalWrite(LR1, HIGH);
  digitalWrite(LG2, HIGH);
  digitalWrite(LY2, HIGH);
  digitalWrite(LR2, HIGH);

}

void loop(){
  if (Firebase.ready() && dataReady){
    // sendDataPrevMillis = millis();

    // Send readings to database:
    sendFloat(tempPath, data.temperature);
    sendFloat(humPath, data.humidity);
    sendFloat(flamePath, data.flame_detected);
    sendFloat(COPath, data.co_detected);

    // Send data to display
    if (data.node_id == 1){
      display1.clearDisplay();
      display1.setTextSize(1);
      display1.setTextColor(WHITE);
      display1.setCursor(0, 0 );
      display1.print("Temp 1: ");
      display1.print(data.temperature);
      display1.print((char)247);
      display1.println("C");
      display1.setCursor(0, 8); 
      display1.print("Humidity 1:");
      display1.print(data.humidity);
      display1.println("%");
      display1.setCursor(0, 16);
      display1.print("Flame 1:");
      display1.println(data.flame_detected ? "Flame detect" : "No detect");
      display1.setCursor(0, 24);
      display1.print("CO 1: ");
      display1.println(data.co_detected ? "Nguy hiem" : "An toan");
      display1.display();
      if (data.temperature <30 && data.flame_detected == false && data.co_detected == false) {
        digitalWrite(LG1, LOW);
        digitalWrite(LR1, HIGH);
        digitalWrite(LY1, HIGH);
      }
      else if (data.temperature >30 || data.flame_detected == true || data.co_detected == true) {
        digitalWrite(LG1, LOW);
        digitalWrite(LR1, HIGH);
        digitalWrite(LY1, LOW);
      }
      else if (data.temperature >30 && data.flame_detected == true && data.co_detected == true) {
        digitalWrite(LG1, LOW);
        digitalWrite(LR1, LOW);
        digitalWrite(LY1, HIGH);
      }
    }

    else if (data.node_id == 2){
      display2.clearDisplay();
      display2.setTextSize(1);
      display2.setTextColor(WHITE);
      display2.setCursor(0, 0 );
      display2.print("Temp 2: ");
      display2.print(data.temperature);
      display2.print((char)247);
      display2.println("C");
      display2.setCursor(0, 8); 
      display2.print("Humidity 2:");
      display2.print(data.humidity);
      display2.println("%");
      display2.setCursor(0, 16);
      display2.print("Flame 2:");
      display2.println(data.flame_detected ? "Flame detect" : "No detect");
      display2.setCursor(0, 24);
      display2.print("CO 2: ");
      display2.println(data.co_detected ? "Nguy hiem" : "An toan");
      display2.display();

      if (data.temperature <30 && data.flame_detected == false && data.co_detected == false) {
        digitalWrite(LG2, LOW);
        digitalWrite(LR2, HIGH);
        digitalWrite(LY2, HIGH);
      }
      else if (data.temperature >30 || data.flame_detected == true || data.co_detected == true) {
        digitalWrite(LG2, LOW);
        digitalWrite(LR2, HIGH);
        digitalWrite(LY2, LOW);
      }
      else if (data.temperature >30 && data.flame_detected == true && data.co_detected == true) {
        digitalWrite(LG2, LOW);
        digitalWrite(LR2, LOW);
        digitalWrite(LY2, HIGH);
      }
    }

    dataReady = false;
  }
}