#include <WiFi.h>
#include <FirebaseESP32.h>
#include <esp_now.h>

// Cấu hình WiFi để kết nối Firebase
#define WIFI_SSID "VIETTEL"  // Thay bằng tên WiFi của bạn
#define WIFI_PASSWORD "88888888"  // Thay bằng mật khẩu WiFi của bạn

// Cấu hình Firebase
#define FIREBASE_HOST "https://fire-2b25b-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "AIzaSyARrBA1LZE9cJvHQ2p_MiF2hhb4x7Hb_4w"

// Khởi tạo Firebase
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;

// Cấu trúc dữ liệu nhận từ Node Sensor
typedef struct {
  uint8_t node_id;         // ID của Node
  float temperature;       // Nhiệt độ
  float humidity;          // Độ ẩm
  bool flame_detected;     // Trạng thái phát hiện lửa
  bool co_detected;        // Trạng thái phát hiện khí CO
} SensorData;

SensorData data;

// Callback khi nhận dữ liệu qua ESP-NOW
void onDataRecv(const esp_now_recv_info *recvInfo, const uint8_t *incomingData, int len) {
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

  // Tải dữ liệu lên Firebase
  String path = "/node_" + String(data.node_id);
  if (Firebase.RTDB.setFloat(&firebaseData, path + "/temperature", data.temperature)) {
    Serial.println("Nhiet do da duoc tai len Firebase.");
  } else {
    Serial.println("Loi tai nhiet do len Firebase: " + firebaseData.errorReason());
  }

  if (Firebase.RTDB.setFloat(&firebaseData, path + "/humidity", data.humidity)) {
    Serial.println("Do am da duoc tai len Firebase.");
  } else {
    Serial.println("Loi tai do am len Firebase: " + firebaseData.errorReason());
  }

  if (Firebase.RTDB.setBool(&firebaseData, path + "/flame_detected", data.flame_detected)) {
    Serial.println("Trang thai lua da duoc tai len Firebase.");
  } else {
    Serial.println("Loi tai lua len Firebase: " + firebaseData.errorReason());
  }

  if (Firebase.RTDB.setBool(&firebaseData, path + "/co_detected", data.co_detected)) {
    Serial.println("Trang thai khi CO da duoc tai len Firebase.");
  } else {
    Serial.println("Loi tai khi CO len Firebase: " + firebaseData.errorReason());
  }
}

void setup() {
  Serial.begin(115200);

  // Cấu hình WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nWiFi da ket noi.");

  // Cấu hình Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.api_key = FIREBASE_AUTH;
  firebaseConfig.token_status_callback = [](TokenInfo info) {
    Serial.printf("Token info: type = %s, status = %s\n", getTokenType(info.type).c_str(), getTokenStatus(info.status).c_str());
  };

  Firebase.begin(&firebaseConfig, &firebaseData);
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
  // Gateway chỉ cần nhận và tải dữ liệu lên Firebase
}
