#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>



// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
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
}

void loop(){
  if (Firebase.ready() && dataReady){
    // sendDataPrevMillis = millis();

    // Send readings to database:
    sendFloat(tempPath, data.temperature);
    sendFloat(humPath, data.humidity);
    sendFloat(flamePath, data.flame_detected);
    sendFloat(COPath, data.co_detected);
    dataReady = false;
  }
}