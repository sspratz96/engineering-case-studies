// Original course code (October 2024), unchanged except for the redacted RFID tag UID.
#include <Servo.h>  // Include the Servo library
#include <SPI.h>    // SPI library for RFID
#include <MFRC522.h> // RFID library

int calibrationTime = 50; // the sensor needs 30-60 secs to stabilize

long unsigned int lowIn;         
long unsigned int pause = 5000;  
long unsigned int motionStartTime;  
long unsigned int yellowPhaseTime = 10000;  
long unsigned int redPhaseTime = 0; 

boolean lockLow = true;
boolean takeLowTime;
boolean inYellowPhase = false;  
boolean alarmSystemOn = false; 
boolean alarmDisabled = false;  
boolean intruderAlertMsg = false;

int pirPin = 7;    
int redLedPin = 5;  
int buzzerPin = 6;
int yellowLedPin = 8;  

Servo doorServo;  
int servoPin = 3;  
int lockPosition = 90;  
int unlockPosition = 0;  

// RFID reader pins
#define SS_PIN 10  // Slave Select pin for RFID reader
#define RST_PIN 9  // Reset pin for RFID reader
MFRC522 rfid(SS_PIN, RST_PIN);  // Create an MFRC522 instance

byte authorizedUID[] = {0xDE, 0xAD, 0xBE, 0xEF};  // Authorized RFID tag UID (example)
byte lastPresentedUID[4];  // Store last presented RFID UID

void setup(){
  Serial.begin(9600);
  pinMode(pirPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);  

  doorServo.attach(servoPin);  // Attach the servo motor to the defined pin
  doorServo.write(unlockPosition);  // Start with the door unlocked (servo at 0 degrees)

  SPI.begin();  // Initialize SPI bus for RFID reader
  rfid.PCD_Init();  // Initialize RFID reader

  Serial.print("calibrating sensor ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
  Serial.println(" done");
  tone(buzzerPin, 500);
  delay(20);  // Short beep duration
  noTone(buzzerPin);
  delay(20);  // Short pause between beeps  
  Serial.println("SENSOR ACTIVE");
  delay(50);
}

void loop(){
  if(digitalRead(pirPin) == HIGH && !alarmDisabled) {
    if(lockLow) {
      lockLow = false;  
      inYellowPhase = true;  
      motionStartTime = millis();  

      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis()/1000);
      Serial.println(" sec");
      delay(50);
    }

    if(inYellowPhase) {
      // YELLOW PHASE: Turn on yellow LED and beep 3 times
      digitalWrite(yellowLedPin, HIGH);
      digitalWrite(redLedPin, LOW);  
      doorServo.write(lockPosition);  

      if (!alarmSystemOn) {
        alarmSystemOn = true;
        for(int i = 0; i < 3; i++) {
          tone(buzzerPin, 200);
          delay(200);  // Short beep duration
          noTone(buzzerPin);
          delay(200);  // Short pause between beeps
        }
        // Send alarm activation to Python
        Serial.print("ALARM_ACTIVATED: ");
        Serial.println(millis()/1000);  
      }

      // Check for RFID tag to disable the alarm
      if(checkRFID()) {
        disableAlarm();
      }

      // After 10 seconds, switch to red phase
      if(millis() - motionStartTime > yellowPhaseTime) {
        inYellowPhase = false;
      }
    } else {
      // RED PHASE: Turn on red LED, turn off yellow LED, and continuous buzzer
      digitalWrite(yellowLedPin, LOW);
      delay(50);
      digitalWrite(redLedPin, HIGH);
      delay(50);
      tone(buzzerPin, 500);  // Continuous tone

      if (!intruderAlertMsg){
      Serial.print("INTRUDER_ALERT: ");
      Serial.println(millis()/1000);  // Send time of activation in seconds
      intruderAlertMsg = true;
      }
      // Track the duration of the red phase
      redPhaseTime = millis() - motionStartTime;
    }

    takeLowTime = true;
  }

  if(digitalRead(pirPin) == LOW && !alarmDisabled) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(yellowLedPin, LOW);
    doorServo.write(unlockPosition);  // Move the servo back to 0 degrees (unlock position)

    if(takeLowTime) {
      lowIn = millis(); 
      takeLowTime = false;
    }

    if(!lockLow && millis() - lowIn > pause) {
      lockLow = true;
      inYellowPhase = false;
      alarmSystemOn = false;
      delay(50);
      noTone(buzzerPin);
      delay(50);
      Serial.print("motion ended at ");
      Serial.print((millis() - pause)/1000);
      Serial.println(" sec");
      delay(50);

      // If red phase was active, send intruder alert and red phase time
      if (redPhaseTime > 0) {
        Serial.print("INTRUDER_GONE: ");
        Serial.println(redPhaseTime / 1000);  // Send red phase duration in seconds
        intruderAlertMsg = false;
      }
      redPhaseTime = 0;

    }
  }
}

// Function to check RFID tag
bool checkRFID() {

  String content = "";
  byte letter ;
  delay(80);
  
  // Look for new RFID cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return false; // No RFID card / tag present
  }

  // Store the last presented UID
  for (byte i = 0; i < 4; i++) {
    lastPresentedUID[i] = rfid.uid.uidByte[i];
  }
  // Compare the read UID with the authorized UID
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.println( rfid.uid.uidByte[ i ] == 0x10 ? " 0" : " ") ;
    content.concat( String( rfid.uid.uidByte[ i ] == 0x10 ? " 0" : " ") ) ;
    content.concat( String( rfid.uid.uidByte[ i ] , HEX ) ) ;
  }
  content.toUpperCase();
  if (content.substring(1) != "XX XX XX XX") {  // authorized tag UID, redacted for publication
    return false;  // Unauthorized tag
  }
  
  // Send authorized UID to Python
  Serial.print("RFID_AUTHORIZED: ");
  Serial.println(content.substring(1)) ;

  // Authorized tag detected
  return true;
}

// Function to disable the alarm with RFID tag
void disableAlarm() {
  alarmDisabled = true;  // Disable the alarm
  digitalWrite(yellowLedPin, LOW);  // Turn off yellow LED
  noTone(buzzerPin);  // Stop the buzzer
  doorServo.write(unlockPosition);  // Unlock the door (move servo to 0 degrees)
  
  // Send alarm disabled status to Python
  Serial.println("ALARM_DISABLED: 0");  // "0" indicates the owner

  Serial.println("Alarm disabled by RFID tag");
  delay(50);  // Small delay to debounce
}
