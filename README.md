# Automatic Door System using FreeRTOS and Arduino

This project is an **automated door system** built with **Arduino Uno R3** and **FreeRTOS**, using ultrasonic sensors to detect human presence and trigger servo motor, lights, buzzer, and LCD messages.

## 🚀 Features
- Automatic door opening and closing
- Dual ultrasonic sensor detection (arrival and departure)
- LCD message display ("Welcome" / "Goodbye")
- Light and buzzer feedback
- Multi-tasking using FreeRTOS

## 🧠 System Overview
- **Sensor 1 (HC-SR04):** Detects approaching person
- **Sensor 2 (HC-SR04):** Detects person leaving
- **vTaskSensor1 & vTaskSensor2:** Handle detection and responses
- **vTaskUpdateLCD:** Updates default LCD text
- **Semaphore:** Manages LCD access across tasks

## 🛠 Technologies
- C++
- Arduino Uno R3
- FreeRTOS
- LCD I2C 16x2
- Servo motor
- HC-SR04 ultrasonic sensors
- Buzzer
- LEDs
## 📝 Tác giả
- **Họ tên:** Huỳnh Đặng Phương Âu  
- **Vai trò:** Thiết kế – Lập trình – Tích hợp hệ thống  
- **Sinh viên Công nghệ Kỹ thuật máy tính**
