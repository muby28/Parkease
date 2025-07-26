#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Aadhi";  // Wi-Fi SSID
const char* password = "12345678";  // Wi-Fi Password
const char* serverUrl = "http://192.168.59.247/updateSlot";  // Replace <SERVER_IP> with actual IP

#define IR_SENSOR_1 5  // GPIO 5
#define IR_SENSOR_2 18 // GPIO 18
#define IR_SENSOR_3 19 // GPIO 19
#define IR_SENSOR_4 21 // GPIO 21

int area = 1;  // Area 1 (index starts from 0)
int slot = 0;  // Slot 1 (index starts from 0)
String driver = "Driver1";  // Driver ID
bool lastStatus = -1;

void setup() {
    Serial.begin(115200);
    pinMode(IR_SENSOR_1, INPUT);
    pinMode(IR_SENSOR_2, INPUT);
    pinMode(IR_SENSOR_3, INPUT);
    pinMode(IR_SENSOR_4, INPUT);

    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void updateServer(int status) {
    if (status == lastStatus) return; // Avoid redundant updates
    lastStatus = status;
    
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        Serial.print("Connecting to: ");
        Serial.println(serverUrl);

        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");
        http.setTimeout(3000); // Reduce timeout to 3 seconds

        StaticJsonDocument<200> doc;
        doc["area"] = area;
        doc["slot"] = slot;
        doc["status"] = status;  // 0 = occupied, 1 = available

        String requestBody;
        serializeJson(doc, requestBody);

        Serial.print("Sending JSON: ");
        Serial.println(requestBody);

        int httpResponseCode = http.POST(requestBody);

        if (httpResponseCode > 0) {
            Serial.print("Server Response Code: ");
            Serial.println(httpResponseCode);
            Serial.print("Response: ");
            Serial.println(http.getString());
        } else {
            Serial.print("HTTP POST failed! Error Code: ");
            Serial.println(httpResponseCode);
            Serial.print("Error Message: ");
            Serial.println(http.errorToString(httpResponseCode).c_str());
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected! Cannot update server.");
    }
}

void loop() {
    int ir1 = digitalRead(IR_SENSOR_1);
    int ir2 = digitalRead(IR_SENSOR_2);
    int ir3 = digitalRead(IR_SENSOR_3);
    int ir4 = digitalRead(IR_SENSOR_4);

    Serial.print("IR Sensor States: ");
    Serial.print(ir1); Serial.print(" ");
    Serial.print(ir2); Serial.print(" ");
    Serial.print(ir3); Serial.print(" ");
    Serial.println(ir4);

    // Only update if all sensors detect the same state
    if (ir1 == LOW && ir2 == LOW && ir3 == LOW && ir4 == LOW) {
        updateServer(0);  // Slot is occupied
    } else if (ir1 == HIGH && ir2 == HIGH && ir3 == HIGH && ir4 == HIGH) {
        updateServer(1);  // Slot is available
    }
}
