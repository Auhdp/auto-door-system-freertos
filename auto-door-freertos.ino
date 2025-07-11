#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

// Định nghĩa các chân kết nối
#define TRIG1 3
#define ECHO1 2
#define TRIG2 5
#define ECHO2 4
#define SERVO_PIN 9
#define BUZZER_PIN 7
#define LIGHT_PIN 6 

// Địa chỉ I2C của màn hình LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo servo;
SemaphoreHandle_t xMutex;

bool personDetected = false; // Cờ để theo dõi phát hiện người
const int detectionDistance = 3; // Khoảng cách phát hiện tính bằng cm
const int servoOpenAngle = 90; // Góc servo khi phát hiện người
const int servoCloseAngle = 0; // Góc servo khi không phát hiện người
const int openDelay = 3000; // Thời gian giữ cổng mở tính bằng mili giây
const int detectionDelay = 200; // Thời gian trễ trước khi cho phép phát hiện tiếp theo tính bằng mili giây

// Hàm khởi tạo cảm biến SRF04
long readUltrasonicDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration / 29 / 2;
}

// Hàm điều khiển servo
void controlServo(int angle) {
  servo.write(angle);
}

// Hàm phát âm thanh trên loa
void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

// Hàm bật và tắt đèn
void controlLight(bool state) {
  digitalWrite(LIGHT_PIN, state ? HIGH : LOW);
}

// Hàm xử lý khi phát hiện người
void handlePersonDetected(const char* message) {
  controlServo(servoOpenAngle); // Mở cửa
  controlLight(true); // Bật đèn
  playTone(1000, 100); // Phát âm thanh beep trong 100 mili giây
  if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
    lcd.setCursor((16 - strlen(message)) / 2, 1); // Căn giữa thông điệp
    lcd.print(message);
    xSemaphoreGive(xMutex);
  }
  vTaskDelay(openDelay / portTICK_PERIOD_MS); // Giữ cửa mở trong 3 giây
  controlServo(servoCloseAngle); // Đóng cửa
  controlLight(false); // Tắt đèn
  if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
    lcd.setCursor(0, 1);
    lcd.print("                "); // Xóa thông điệp
    xSemaphoreGive(xMutex);
  }
}

// Task cập nhật LCD với thông điệp tùy chỉnh
void vTaskUpdateLCD(void *pvParameters) {
  (void) pvParameters;
  while (true) {
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
      lcd.setCursor(0, 0);
      lcd.print("  Doan Cao Danh"); // Thông điệp tùy chỉnh
      lcd.setCursor(0, 1);
      lcd.print("                "); // Xóa dòng thứ hai
      xSemaphoreGive(xMutex);
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS); // Cập nhật mỗi giây
  }
}

// Task xử lý cảm biến SRF04 1
void vTaskSensor1(void *pvParameters) {
  (void) pvParameters;
  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);
  while (true) {
    long distance = readUltrasonicDistance(TRIG1, ECHO1);
    if (distance < detectionDistance && !personDetected) {
      personDetected = true;
      handlePersonDetected("   Xin chao    "); // Thông điệp căn giữa
      personDetected = false;
    }
    vTaskDelay(detectionDelay / portTICK_PERIOD_MS); // Kiểm tra mỗi 200ms
  }
}

// Task xử lý cảm biến SRF04 2
void vTaskSensor2(void *pvParameters) {
  (void) pvParameters;
  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);
  while (true) {
    long distance = readUltrasonicDistance(TRIG2, ECHO2);
    if (distance < detectionDistance && !personDetected) {
      personDetected = true;
      handlePersonDetected(" Hen gap lai   "); // Thông điệp căn giữa
      personDetected = false;
    }
    vTaskDelay(detectionDelay / portTICK_PERIOD_MS); // Kiểm tra mỗi 200ms
  }
}

void setup() {
  // Khởi tạo Serial để debug
  Serial.begin(9600);
  // Khởi tạo LCD
  lcd.begin(16, 2); // 16 cột, 2 hàng
  lcd.backlight();
  // Khởi tạo Servo
  servo.attach(SERVO_PIN);
  servo.write(servoCloseAngle); // Vị trí ban đầu: cửa đóng
  // Khởi tạo Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  // Khởi tạo đèn
  pinMode(LIGHT_PIN, OUTPUT);
  digitalWrite(LIGHT_PIN, LOW); // Trạng thái ban đầu: đèn tắt
  // Tạo mutex cho LCD
  xMutex = xSemaphoreCreateMutex();
  // Tạo các task FreeRTOS với các mức ưu tiên khác nhau
  xTaskCreate(vTaskUpdateLCD, "UpdateLCD", 128, NULL, 1, NULL);   // Ưu tiên 1
  xTaskCreate(vTaskSensor1, "Sensor1", 128, NULL, 2, NULL);       // Ưu tiên 2
  xTaskCreate(vTaskSensor2, "Sensor2", 128, NULL, 2, NULL);       // Ưu tiên 2
  // Bắt đầu trình lập lịch FreeRTOS
  vTaskStartScheduler();
}

void loop() {
  // Hàm loop không được sử dụng với FreeRTOS.
}
