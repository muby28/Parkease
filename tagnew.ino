#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define BUTTON_NEXT 18  // Button to toggle steps
#define BUTTON_ROUTE 19 // Button to switch route

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD address (Change to 0x3F if needed)

// Define two routes with 5 steps each
const char* route1[] = {
    "1. Move Straight 50m",
    "2. Turn Left & Go 100m",
    "3. Turn Right & Go 80m",
    "4. Move Ahead for 200m",
    "5. Exit Reached!"
};

const char* route2[] = {
    "1. Go Right 40m",
    "2. Turn Left & Move 120m",
    "3. Turn Right after 70m",
    "4. Straight ahead 150m",
    "5. Destination Arrived!"
};

const int totalSteps = 5;
int stepIndex = 0;
bool currentRoute = false; // false = Route 1, true = Route 2
volatile bool nextStepPressed = false;
volatile bool switchRoutePressed = false;
unsigned long lastPressTime1 = 0;
unsigned long lastPressTime2 = 0;

// Interrupt service routine for next step button
void IRAM_ATTR handleNextStep() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime1 > 300) { // Debounce (300ms)
        nextStepPressed = true;
        lastPressTime1 = currentTime;
    }
}

// Interrupt service routine for route switch button
void IRAM_ATTR handleRouteSwitch() {
    unsigned long currentTime = millis();
    if (currentTime - lastPressTime2 > 300) { // Debounce (300ms)
        switchRoutePressed = true;
        lastPressTime2 = currentTime;
    }
}

void setup() {
    Wire.begin();
    lcd.init();
    lcd.backlight();

    pinMode(BUTTON_NEXT, INPUT_PULLUP);
    pinMode(BUTTON_ROUTE, INPUT_PULLUP);
    
    attachInterrupt(BUTTON_NEXT, handleNextStep, FALLING);
    attachInterrupt(BUTTON_ROUTE, handleRouteSwitch, FALLING);

    lcd.setCursor(0, 0);
    lcd.print("Navigation Start");
    delay(2000);
    lcd.clear();
    displayInstruction();
}

void loop() {
    if (nextStepPressed) {
        nextStepPressed = false;
        
        // Check if last step is reached
        if (stepIndex == totalSteps - 1) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Route Completed!");
            delay(3000); // Show message for 3 seconds
            stepIndex = 0; // Reset to first step
        } else {
            stepIndex++; // Move to the next step
        }

        displayInstruction();
    }

    if (switchRoutePressed) {
        switchRoutePressed = false;
        currentRoute = !currentRoute; // Toggle between Route 1 and Route 2
        stepIndex = 0; // Reset step to start of new route
        displayInstruction();
    }
}

void displayInstruction() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(currentRoute ? "Route 2" : "Route 1");
    lcd.print(" - Step ");
    lcd.print(stepIndex + 1);
    
    lcd.setCursor(0, 1);
    lcd.print(currentRoute ? route2[stepIndex] : route1[stepIndex]);
}
