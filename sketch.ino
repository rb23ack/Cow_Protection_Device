#include <DHT.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>

// Pin Definitions
#define DHTPIN 14          // Pin connected to DHT22 sensor
#define PIRPIN 18          // Pin connected to PIR sensor
#define LEDPIN 26          // Pin connected to LED
#define BUTTONPIN 17       // Pin connected to main Button
#define MANUALBUTTON 16    // Pin connected to Manual Mode Button
#define POTPIN 33          // Pin connected to Potentiometer
#define SERVO_PIN 13       // Pin connected to Servo
#define BUZZER_PIN 12      // Pin connected to Buzzer
#define OLED_SDA 19        // Pin connected to OLED SDA
#define OLED_SCL 4         // Pin connected to OLED SCL (SCL typically on 23)

// OLED Display Dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize OLED Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Initialize DHT sensor
DHT dht(DHTPIN, DHT22);

// Initialize Servo
Servo myServo;

// Variables
int servoPos = 0;                   // Initial position of the servo (0°)
bool isMotionDetected = false;
bool buttonState = false;
bool previousButtonState = false;
bool manualButtonState = false;
bool manualMode = false;
unsigned long lastDebounceTime = 0; // For button debounce
const unsigned long debounceDelay = 50;

void setup() {
  // Start Serial Monitor
  Serial.begin(115200);     // Start serial communication for debugging
  pinMode(PIRPIN, INPUT);   // Set PIRPIN as input to read motion sensor
  pinMode(LEDPIN, OUTPUT);  // Set LEDPIN as output to control LED
  Serial.println("System Initialized");

  // Initialize DHT sensor
  dht.begin();

  // Initialize Servo
  myServo.attach(SERVO_PIN);

  // Initialize OLED Display
  Wire.begin(OLED_SDA, OLED_SCL);  // Connect I2C using specific pins
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Correct initialization
    Serial.println("OLED initialization failed!");
    while (true); // Halt the program if OLED is not found
  }
  display.clearDisplay();
  display.display();

  // Initialize LED, Buzzer, and Buttons
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);     // Enable internal pull-up resistor
  pinMode(MANUALBUTTON, INPUT_PULLUP);  // Manual Mode Button
  pinMode(PIRPIN, INPUT);

  // Move the servo to initial position
  myServo.write(servoPos);

  // Display Welcome Message
  displayMessage("Cow Protection Device\nInitializing...");
}

void loop() {
  // Check if manual button is pressed to activate manual mode
  int manualReading = digitalRead(MANUALBUTTON);
  if (manualReading != manualButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the manual button state has changed
    if (manualReading == LOW) {
      activateBuzzer();
      displayMessage("System Stopped");
      manualMode = true; // Set manual mode to true and exit loop
      return; // Exit the loop and stop any further motion control
    }
  }
  manualButtonState = manualReading;

  // If the system is in manual mode, do not run motion detection or servo actions
  if (manualMode) {
    displayMessage("Manual Mode\nSystem Stopped");
    return; // Exit the loop early to prevent any automatic actions
  }

  // Read temperature and humidity
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if DHT reading is valid
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Automatic Motion Control
  isMotionDetected = digitalRead(PIRPIN);

  // Read potentiometer value (sensitivity adjustment)
  int potValue = analogRead(POTPIN);
  int threshold = map(potValue, 0, 4095, 0, 100);

  // Trigger system alert if motion is detected and above threshold
  if (isMotionDetected && threshold > 50) {
    digitalWrite(LEDPIN, HIGH);  // Turn on the LED to indicate an alert
    myServo.write(90);          // Open the gate (move servo)
    displayMessage("Motion Detected!\nGate Opened");
  } else {
    digitalWrite(LEDPIN, LOW);  // Turn off LED
    myServo.write(0);           // Close the gate (move servo back)
    displayMessage("Monitoring...");
  }

  // Read main button state with debounce
  int reading = digitalRead(BUTTONPIN);
  if (reading != previousButtonState) {
    lastDebounceTime = millis(); // Reset debounce timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the button state has changed
    if (reading == LOW) {
      buttonState = !buttonState; // Toggle button state
    }
  }
  previousButtonState = reading;

  // Print data to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Motion Detected: ");
  Serial.print(isMotionDetected);
  Serial.print(", Threshold: ");
  Serial.println(threshold);

  // Delay before next loop
  delay(500);
}

// Function to display messages on OLED
void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

// Function to activate the buzzer
void activateBuzzer() {
  digitalWrite(BUZZER_PIN, HIGH); // Turn on buzzer
  delay(1000);                    // Buzzer sound duration
  digitalWrite(BUZZER_PIN, LOW);  // Turn off buzzer
}
