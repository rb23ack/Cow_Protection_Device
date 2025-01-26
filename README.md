# Cow Protection Device

An IoT-based device designed to monitor and protect cows by utilizing ESP32. This system features motion detection, temperature and humidity monitoring, automatic gate control, an OLED display for real-time updates, and a manual override function for emergencies. I built it using "Wokwi" the online simulator. It is a great tool for prototyping. We need to be careful with wirings when building the physical device since it's a bit different when implementing the "actual" work. 

## Features   
- **Motion Detection:** Detects motion using a PIR sensor and triggers an alert.
- **Temperature & Humidity Monitoring:** Monitors environmental conditions using a DHT22 sensor.
- **Automatic Gate Control:** Controls a servo motor to open/close gates based on motion detection.
- **OLED Display:** Displays real-time system information and status updates.
- **Manual Override:** Allows the system to be manually stopped or restarted using a button.

## Components Used
- ESP32 microcontroller   
- DHT22 temperature and humidity sensor   
- PIR motion sensor
- Potentiometer for sensitivity adjustment
- Servo motor
- OLED display (128x64)
- Buzzer
- Push buttons
- LED


## Setup Instructions
1. **Hardware:**
   - Connect all components as per the provided schematic.
   - Ensure the servo motor is powered using a reliable power source to avoid voltage drops.
2. **Software:**
   - Install the necessary libraries:
     - [DHT Library](https://github.com/adafruit/DHT-sensor-library)
     - [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)
     - [Adafruit SSD1306 Library](https://github.com/adafruit/Adafruit_SSD1306)
     - [ESP32Servo Library](https://github.com/madhephaestus/ESP32Servo)
   - Upload the provided code to the ESP32 using the Arduino IDE.
3. **Power Up:**
   - Power the ESP32 and ensure all components are working as intended.
   - Check the OLED for real-time status updates.

## How It Works
1. The device monitors motion using the PIR sensor and environmental conditions with the DHT22 sensor.
2. If motion is detected above a set threshold (adjustable via potentiometer), the servo motor opens the gate, and the buzzer/LED is activated.
3. The OLED displays relevant information, including system status, temperature, humidity, and motion alerts.
4. A manual mode button can be used to disable automatic functionality and stop the system.

## Example Usage
- Livestock management: Protect cows from unauthorized access or predators.
- Environmental monitoring: Observe temperature and humidity in the cowshed.

## Code
The complete source code is available in the repository. 

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgments
- Adafruit for their excellent sensor libraries.
- ESP32 community for the extensive resources and support. 

