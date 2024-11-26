#include <esp_now.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Cấu hình Wi-Fi và Firebase
#define WIFI_SSID "Nguyen Tien Hoa"
#define WIFI_PASSWORD "Nguyentienhoa"
#define API_KEY "AIzaSyARrBA1LZE9cJvHQ2p_MiF2hhb4x7Hb_4w"
#define DATABASE_URL "https://fire-2b25b-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Cấu trúc dữ liệu nhận từ Node Sensor
typedef struct {
  uint8_t node_id;         // ID của Node
  float temperature;       // Nhiệt độ
  float humidity;          // Độ ẩm
  bool flame_detected;     // Trạng thái phát hiện lửa
  bool co_detected;        // Trạng thái phát hiện khí CO
} SensorData;

SensorData data; // Biến chứa dữ liệu nhận được

// Firebase và Wi-Fi
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Gửi dữ liệu lên Firebase
void sendToFirebase(SensorData sensorData) {
  // Tạo đường dẫn Firebase dựa trên node_id
  String path = "/node_" + String(sensorData.node_id);

  // Gửi dữ liệu lên Firebase
  if (Firebase.RTDB.setFloat(&fbdo, path + "/temperature", sensorData.temperature)) {
    Serial.println("Temperature sent successfully!");
  } else {
    Serial.print("Failed to send temperature: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.RTDB.setFloat(&fbdo, path + "/humidity", sensorData.humidity)) {
    Serial.println("Humidity sent successfully!");
  } else {
    Serial.print("Failed to send humidity: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.RTDB.setBool(&fbdo, path + "/flame_detected", sensorData.flame_detected)) {
    Serial.println("Flame status sent successfully!");
  } else {
    Serial.print("Failed to send flame status: ");
    Serial.println(fbdo.errorReason());
  }

  if (Firebase.RTDB.setBool(&fbdo, path + "/co_detected", sensorData.co_detected)) {
    Serial.println("CO status sent successfully!");
  } else {
    Serial.print("Failed to send CO status: ");
    Serial.println(fbdo.errorReason());
  }
}

// Callback khi nhận dữ liệu qua ESP-NOW
void onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
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

  // Gửi dữ liệu nhận được lên Firebase
  sendToFirebase(data);
}

void setup() {
  Serial.begin(115200);

  // Kết nối Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Da ket noi WiFi");

  // Cấu hình Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; // Để theo dõi trạng thái token
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Cấu hình ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }
  esp_now_register_recv_cb(onDataRecv);
}

void loop() {
  // Gateway chỉ nhận và xử lý dữ liệu khi có từ ESP-NOW
}
