/*
 * main.cpp
 */

#include "WProgram.h" // local, no system include
const int GATE_PIN1 = 10;
const int GATE_PIN2 = 11;
const int LED_PIN1 = 12;
const int LED_PIN2 = 13;

// unit is ms
const unsigned int BLINK_INTERVAL = 500; 
const unsigned int FORCE_TIMEOUT = 30000;
const unsigned int TRIGGER_PAUSE = 500;

boolean alarm = false;
boolean trigger = false;

int i = 0;
int lightBarrier1 = 0;
int lightBarrier2 = 0;

unsigned long previousMillis = 0;
unsigned long lastGateTriggerTime = 0;
unsigned long forcedAlarmEndTime = 0;

void setup() {

    pinMode(LED_PIN1, OUTPUT);
    pinMode(LED_PIN2, OUTPUT);
    pinMode(GATE_PIN1, INPUT);
    pinMode(GATE_PIN2, INPUT);

    lightBarrier1 = digitalRead(GATE_PIN1);
    lightBarrier2 = digitalRead(GATE_PIN2);

    digitalWrite(LED_PIN1, LOW);
    digitalWrite(LED_PIN2, LOW);
    //Serial.begin(9600);
}

void loop() {

    if (alarm) {
        digitalWrite(LED_PIN2, (i == 0) ? LOW : HIGH);
        digitalWrite(LED_PIN1, (i == 0) ? HIGH : LOW);
        if (millis() - previousMillis > BLINK_INTERVAL) {
            previousMillis = millis();
            i = (i + 1) % 2;
        }
    } else {
        // switch off after a full blink BLINK_INTERVAL
        if (millis() - previousMillis > BLINK_INTERVAL) {
            digitalWrite(LED_PIN1, LOW);
            digitalWrite(LED_PIN2, LOW);
        }
    }

    lightBarrier1 = digitalRead(GATE_PIN1);
    lightBarrier2 = digitalRead(GATE_PIN2);

    // zero pin signal means light barrier is interrupted
    trigger = (lightBarrier1 == 0 || lightBarrier2 == 0) ? true : false;

    if (trigger) {
        forcedAlarmEndTime = millis() + FORCE_TIMEOUT;
        // avoid subsequent low edges to trigger the alarm
        if ((millis() < (TRIGGER_PAUSE + lastGateTriggerTime))) {
            trigger = false;
        }
        lastGateTriggerTime = millis();
    }

    if (trigger) {
        alarm = !alarm;
    }

    if (forcedAlarmEndTime < millis()) {
        alarm = false;
    }

    //  Serial.print(lightBarrier1);
    //  Serial.print(", ");
    //  Serial.print(lightBarrier2);
    //  Serial.print(": ");
    //  Serial.print(alarm ? "true" : "false");
    //  Serial.println();
}

