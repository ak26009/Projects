#include <Preferences.h>

#define IN1 33
#define IN2 25

#define PWM_FREQ 1000
#define PWM_RESOLUTION 8

// ~20% PWM
#define MOTOR_PWM 51

// Scratch rate
#define SCRATCHES_PER_MINUTE 17

Preferences preferences;

unsigned long previousMinute = 0;
unsigned long minuteCounter = 0;

unsigned long totalScratches = 0;

void setup()
{
    Serial.begin(115200);

    pinMode(IN2, OUTPUT);

    // Open ESP32 non-volatile memory

    preferences.begin("scratch", false);

    // Read previously stored count
    totalScratches = preferences.getULong("count", 0);

    Serial.println();
    Serial.println("================================");
    Serial.println(" Scratch Counter Started");
    Serial.println("================================");

    Serial.print("Previous stored count: ");
    Serial.println(totalScratches);

    // -----------------------------
    // Motor PWM
    // -----------------------------
    ledcAttach(IN1, PWM_FREQ, PWM_RESOLUTION);

    ledcWrite(IN1, 0);
    digitalWrite(IN2, LOW);

    // Start motor
    digitalWrite(IN2, LOW);
    ledcWrite(IN1, MOTOR_PWM);

    // Start 1-minute timer
    previousMinute = millis();
}


void loop()
{
    // Motor running
    digitalWrite(IN2, LOW);
    ledcWrite(IN1, MOTOR_PWM);

    // ----------------------------------
    // Check if one minute has passed
    // ----------------------------------
    if (millis() - previousMinute >= 60000)
    {
        previousMinute += 60000;

        minuteCounter++;

        // Add 17 scratches
        totalScratches += SCRATCHES_PER_MINUTE;

        // Store in ESP32 flash/NVS
        preferences.putULong("count", totalScratches);

        // Display information
        Serial.println();
        Serial.println("-----------------------------");

        Serial.print("Minute: ");
        Serial.println(minuteCounter);

        Serial.print("Scratches this minute: ");
        Serial.println(SCRATCHES_PER_MINUTE);

        Serial.print("Total scratches: ");
        Serial.println(totalScratches);

        Serial.println("-----------------------------");
    }
}
