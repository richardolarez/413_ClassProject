// This #include statement was automatically added by the Particle IDE.
#include "spo2_algorithm.h"

// This #include statement was automatically added by the Particle IDE.
#include "MAX30105.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED); //uncomment to use w/out wifi

#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"     //courtesy of sparkfun
#include "heartRate.h"          //courtesy of sparkfun
#include <string>               //standard c library if you don't know this you don't deserve to look at my code
#include <stdint.h>

// Global variables
// Spo2 function variables
uint32_t bufferLength;
int32_t spo2; // Blood oxygen level
int8_t validO2; // Indicator if spo2 value is valid
int32_t heartRate;    // Heart rate value    
int8_t validHeartRate; // Indicator if heartRate is valid

uint32_t irBuffer[100];
uint32_t redBuffer[100];

// Time variables
float currentTime = 0;
float timeConstraintLow = 0.01; // Start of measurement window default 1 am
float timeConstraintHigh = 24.999; // End of measurement window default 10 pm
uint32_t previousRequest = 0; // Time of previous measurement request
uint32_t delayTime = 60000; // ms delay between readings ( default 1800000ms or 30 min) 

// From Salehi's code
long lastBeat = 0; // Time at which the last beat occurred
float beatsPerMinute;
MAX30105 particleSensor;

// Other global variables

char event_data[3];   // New parameters

// Storage structure
struct storage {
  String buffer[24];
  uint32_t time_stored[24];
  uint8_t amountStored = 0;
} storage;

uint8_t state = 0;

LEDStatus blinkLED(RGB_COLOR_BLUE, LED_PATTERN_BLINK, LED_SPEED_FAST, LED_PRIORITY_IMPORTANT);

// Webhook Event handlers
void handle(const char *event, const char *data){

}

int led = D7;  // The on-board LED

void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");

    // Subscriptions to Particle Webhooks
    Particle.subscribe("hook-response/hr", handle, MY_DEVICES);

    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) // Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    
    // Green is standby
    // Blue is Ready for recording
    // Yellow is saving locally because the server could not be reached

    blinkLED.setColor(RGB_COLOR_GREEN);
    blinkLED.setSpeed(LED_SPEED_SLOW);
    blinkLED.setPattern(LED_PATTERN_FADE);
    if(!blinkLED.isActive()) blinkLED.setActive(true);
    blinkLED.on();

    particleSensor.setup(); //Configure sensor with default settings
    Particle.variable("hr", (int)heartRate);
    Particle.variable("spo2", (int)spo2);
}



void loop() {
    
    digitalWrite(led, HIGH);   // Turn ON the LED

    // Synchronous state machine below

    // State 0: Start
    if (state == 0) { // Idle state
        Serial.println("Current state = " + String(state));

        // Current time
        currentTime = float(Time.hour()) + float(Time.minute()) / 60.00;
        Serial.println("CurrentTime = " + String(currentTime));
        if (currentTime < 0) {
            currentTime += 24.0;
        }  

        // Check acceptable time frame
        if (currentTime > timeConstraintLow && currentTime < timeConstraintHigh) {

            // If enough time has passed between measurements
            if (millis() - previousRequest >= delayTime) {

                // Move to state 1 : Request user for HB
                state = 1;
                previousRequest = millis();
                Serial.println("Moving to state 1");
            } else {
                Serial.println("Idle, not enough time has passed from the previous request");
            }
        }
        else {
            Serial.println("It is not within an acceptable time frame");
        }

        delay(2000);
    }

    if (state == 1) { // Request measurement
        Serial.println("Current state = " + String(state));

        Serial.println("Requesting Measurement Place your index finger on the sensor with steady pressure.");

        // LED CHANGE
        blinkLED.setColor(RGB_COLOR_BLUE);
        blinkLED.setPattern(LED_PATTERN_BLINK);
        blinkLED.setSpeed(LED_SPEED_FAST);
        if (blinkLED.isActive()) blinkLED.setActive(true);
        blinkLED.on();

        // Detect finger

        // Move to state 2 : Calculate
        state = 2;
        Serial.println("Moving to state 2");
    }

    if (state == 2) { // Get measurement
        Serial.println("Current state = " + String(state));
        
        bufferLength = 100;
        // Loop to gather 100 samples
        for (byte i = 0; i < bufferLength; ++i) {
            while (particleSensor.available() == false) // Check if sensor available
                particleSensor.check();

            redBuffer[i] = particleSensor.getRed();
            irBuffer[i] = particleSensor.getIR();
            particleSensor.nextSample();
            // Serial.println(i);
        }

        maxim_heart_rate_and_oxygen_saturation(irBuffer, bufferLength, redBuffer, &spo2, &validO2, &heartRate,  &validHeartRate);

       // Check if measurements are valid
        if ((80 < spo2) && (spo2 <= 100) && (40 < heartRate) && (heartRate <= 250)) {
            // Bounds for blood oxygenation (spo2) are set between 80 and 100,
            // and for heart rate between 40 and 250 beats per minute.
            // These ranges are chosen based on physiological norms and typical sensor limitations,
            // helping filter out potentially erroneous or unrealistic data.
            Serial.println("Moving to state 3"); // If measurements are valid, move to state 3
            state = 3;
        }


        // Check if 5 min or 300000 ms have passed and have not transitioned yet 
        else if (millis() - previousRequest > 60000) { // Check if 5 min have passed since the request for measurement
            // Reset LED and go back to idle
            blinkLED.setColor(RGB_COLOR_GREEN);
            blinkLED.setSpeed(LED_SPEED_SLOW);
            blinkLED.setPattern(LED_PATTERN_FADE);

            state = 0;
        } else { // Invalid measurement & timeout has not occurred
            Serial.println("Invalid measurement");
            Serial.println("Moving to state 1");
            state = 1; // Get new measurement 
        }
    }
    
    
    // State 3: Sending the data
    if (state == 3) { // Send data via webhook
    
        String request = "{\"HR\":" + String(heartRate) + ",\"SPO2\":" + String(spo2) + "}";
        bool success = Particle.publish("hr", request, PRIVATE);
        Particle.publish("spo2", String(spo2), PRIVATE);
     
        // Check if data sent
        if (success) {
            // Go back to idle state
            state = 0;

            // Reset LED 
            blinkLED.setColor(RGB_COLOR_GREEN);
            blinkLED.setSpeed(LED_SPEED_FAST);
            blinkLED.setPattern(LED_PATTERN_BLINK);
            delay(2000);

            blinkLED.setColor(RGB_COLOR_GREEN);
            blinkLED.setSpeed(LED_SPEED_SLOW);
            blinkLED.setPattern(LED_PATTERN_FADE);

            Serial.println("Data send Succeeded");

            // Stores Data that cannont be sent to the server for 24 hours as stated in requirements ***********************************
            // Try to send any stored data
            uint8_t temp = storage.amountStored;
            for (uint8_t i = 0; i < temp; i++) {

                // Data less than 24 hrs old
                if ((millis() - storage.time_stored[i]) < 24 * 60 * 60 * 1000) { // 24 hrs convert to ms

                    success = Particle.publish("hr", storage.buffer[i], PRIVATE);

                    if (success) {
                        Serial.println("Stored data sent");
                        Serial.println(storage.buffer[i]);
                    }
                } else {
                    Serial.println("Data out of date, not sent");
                }

            }
            if (success) {
                // Reset storage position 
                storage.amountStored = 0;
                Serial.println("All stored data sent");
            }
        }
        else { // Failed to send data

            // Store data for later
            Serial.println("No internet Data will be stored");
            storage.buffer[storage.amountStored] = request;
            storage.time_stored[storage.amountStored] = millis();
            storage.amountStored ++;
            state = 0;

            // Reset LED
            blinkLED.setColor(RGB_COLOR_YELLOW); // Flashes yellow
            blinkLED.setSpeed(LED_SPEED_FAST);
            blinkLED.setPattern(LED_PATTERN_BLINK);
            delay(2000);

            blinkLED.setColor(RGB_COLOR_GREEN); // Resets to standby
            blinkLED.setSpeed(LED_SPEED_SLOW);
            blinkLED.setPattern(LED_PATTERN_FADE);
        }
    }
}
