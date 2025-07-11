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
## 📸 Images
<img width="450" height="380" alt="image" src="https://github.com/user-attachments/assets/2f6b192f-988a-461f-a498-c91852d58a81" />
<img width="450" height="380" alt="image" src="https://github.com/user-attachments/assets/a3ad54ac-3300-4d3d-96c1-e4659d4a9569" />

## 📝 Tác giả
- **Họ tên:** Huỳnh Đặng Phương Âu  
- **Vai trò:** Thiết kế – Lập trình – Tích hợp hệ thống  
- **Sinh viên Công nghệ Kỹ thuật máy tính**
