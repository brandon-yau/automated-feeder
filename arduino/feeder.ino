#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

RTC_DS3231 rtc;
Servo myServo;

const int servoPin = 9;
const int triggerHour = 18;   
const int triggerMinute = 0;

bool hasActivatedToday = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  myServo.attach(servoPin);

  myServo.write(0);  // Force servo to start at 0°
  delay(500);        

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("Setup complete.");
}

void loop() {
  DateTime now = rtc.now();

  Serial.print("Time: ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.println(now.second());

  // Check for 6:00 PM
  if (now.hour() == triggerHour && now.minute() == triggerMinute && !hasActivatedToday) {
    runServoOnce();
    hasActivatedToday = true;
  }

  // Reset after time window passes
  if (now.hour() == (triggerHour + 1) % 24 && hasActivatedToday) {
    hasActivatedToday = false;
  }

  delay(1000); // Check every second
}

void runServoOnce() {
  Serial.println("Running servo...");

  myServo.write(60);
  delay(300);  // Wait for it to reach 60 degrees

  myServo.write(0);
  delay(300);  // Return servo to 0 degrees
  Serial.println("Servo cycle complete.");
}
