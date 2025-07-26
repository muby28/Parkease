  #include <WiFi.h>
  #include <WebServer.h>
  #include <ArduinoJson.h>

  const char* ssid = "Aadhi";
  const char* password = "12345678";

  WebServer server(80);
 int slots[2][2] = {{1, 1}, {1, 1}};    // Default: all slots available
  String assignedSlots[2] = {"None", "None"};
  int assignedTagsParking1 = 0;  // Tags assigned to Parking Area 1
int assignedTagsParking2 = 0;  // Tags assigned to Parking Area 2
const int totalTags = 4;       // Total parking tags available
unsigned long lastToggleTime = 0;
bool slotsAvailable = true;  // Start with all available


String generateHTML() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    html += "<meta http-equiv='refresh' content='1'><title>Parking Status</title>";

    html += "<style>";
    html += "body { font-family: Arial, sans-serif; text-align: center; background-color: #f4f4f9; margin: 0; padding: 0; }";
    html += "h1 { background: #333; color: white; padding: 15px; margin: 0; }";
    html += ".container { width: 90%; max-width: 600px; margin: 20px auto; }";
    html += ".parking-area { margin: 20px 0; padding: 15px; background: white; border-radius: 10px; box-shadow: 0 4px 8px rgba(0,0,0,0.2); }";
    html += ".slots { display: flex; justify-content: center; gap: 15px; margin-top: 10px; }";
    html += ".slot { width: 100px; height: 50px; border-radius: 10px; display: flex; align-items: center; justify-content: center; ";
    html += "color: white; font-weight: bold; transition: 0.3s; cursor: pointer; }";
    html += ".available { background-color: #4CAF50; }";
    html += ".occupied { background-color: #FF5722; }";
    html += ".slot:hover { transform: scale(1.1); }";
    html += ".info-card { background: #fff; padding: 10px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); margin: 10px 0; }";
    html += ".info-card p { margin: 5px 0; font-weight: bold; }";
    html += "table { width: 100%; border-collapse: collapse; margin-top: 20px; }";
    html += "th, td { border: 1px solid #ddd; padding: 10px; text-align: center; }";
    html += "th { background-color: #333; color: white; }";
    html += "</style>";

    html += "</head><body>";
    html += "<h1>Parking Lot Status</h1>";
    html += "<div class='container'>";

    int assignedTagsAmenity = 0, assignedTagsBlock = 0;

    // Parking Areas and Slot Status
    for (int area = 0; area < 2; area++) {
        String areaName = (area == 0) ? "Amenity" : "A-Block";
        html += "<div class='parking-area'>";
        html += "<h2>" + areaName + "</h2>";
        html += "<div class='slots'>";
        
        for (int i = 0; i < 2; i++) {
            String status = slots[area][i] ? "available" : "occupied";
            html += "<div class='slot " + status + "'>" + areaName + "-S" + String(i + 1) + "</div>";

            // Count occupied slots (assigned tags)
            if (slots[area][i] == 0) {
                if (area == 0) assignedTagsAmenity++;
                else assignedTagsBlock++;
            }
        }

        html += "</div></div>";
    }

    // Assigned Slots & Available Slots
    html += "<div class='info-card'><h2>Slot Assignments & Available Slots</h2>";
    for (int area = 0; area < 2; area++) {
        String areaName = (area == 0) ? "Amenity" : "A-Block";
        html += "<p>🚗 <strong>" + areaName + ":</strong> Assigned to " + assignedSlots[area] + "</p>";

        // Generate available slots list
        String availableSlots = "";
        for (int i = 0; i < 2; i++) {
            if (slots[area][i] == 1) {
                availableSlots += areaName + "-S" + String(i + 1) + ", ";
            }
        }

        if (availableSlots.length() > 0) {
            availableSlots.remove(availableSlots.length() - 2);
        } else {
            availableSlots = "None";
        }

        html += "<p>🅿️ Available Slots: <span style='color: green;'>" + availableSlots + "</span></p>";
    }

    html += "</div>";

    // Parking Tags Section
    int totalTags = 4;
    int tagsLeft = totalTags - (assignedTagsAmenity + assignedTagsBlock);

    html += "<div class='info-card'><h2>Parking Tags Information</h2>";
    html += "<table>";
    html += "<tr><th>Parking Area</th><th>Tags Given</th></tr>";
    html += "<tr><td>Amenity</td><td>" + String(assignedTagsAmenity) + "</td></tr>";
    html += "<tr><td>A-Block</td><td>" + String(assignedTagsBlock) + "</td></tr>";
    html += "</table>";

    html += "<h3>Total Parking Tags: " + String(totalTags) + "</h3>";
    html += "<h3>Tags Left: <span style='color:red;'>" + String(tagsLeft) + "</span></h3>";
    html += "</div>";

    html += "</div></body></html>";
    return html;
}








  void updateParkingData() {
      if (server.hasArg("plain")) {
          String body = server.arg("plain");
          Serial.println("Received update:");
          Serial.println(body);

          StaticJsonDocument<200> doc;
          DeserializationError error = deserializeJson(doc, body);

          if (error) {
              Serial.println("JSON Parsing Failed!");
              server.send(400, "text/plain", "Invalid JSON");
              return;
          }

          int area = doc["area"];
          int slot = doc["slot"];
          int status = doc["status"];
          String driver = doc["driver"];

          if (area >= 0 && area < 2 && slot >= 0 && slot < 2) {
              slots[area][slot] = status;
              assignedSlots[area] = driver;
              Serial.println("Slot Updated Successfully!");
              server.send(200, "text/plain", "Slot Updated");
          } else {
              Serial.println("Invalid slot update request");
              server.send(400, "text/plain", "Invalid Request");
          }
      } else {
          Serial.println("No Data Received");
          server.send(400, "text/plain", "No data received");
      }
  }

  void handleRoot() {
      server.send(200, "text/html", generateHTML());
  }

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("\nStarting WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connected to WiFi!");
    Serial.print("📶 IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ WiFi connection failed.");
    Serial.print("🔍 Status code: ");
    Serial.println(WiFi.status());
  }

  server.on("/", handleRoot);
  server.on("/updateSlot", HTTP_POST, updateParkingData);
  server.on("/checkSlots", HTTP_GET, handleCheckSlots); // <--- added here
  server.begin();
}   


 void loop() {
    server.handleClient();

    unsigned long currentTime = millis();
    if (currentTime - lastToggleTime >= 60000) {  // 60 seconds
        slotsAvailable = !slotsAvailable;  // Toggle state
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                slots[i][j] = slotsAvailable ? 1 : 0;
            }
        }
        Serial.println(slotsAvailable ? "✅ All slots are now AVAILABLE" : "❌ All slots are now OCCUPIED");
        lastToggleTime = currentTime;
    }
}
void handleCheckSlots() {
  String response = "";
  response += "Amenity-S1:" + String(slots[0][0]) + "\n";
  response += "Amenity-S2:" + String(slots[0][1]) + "\n";
  response += "A-Block-S1:" + String(slots[1][0]) + "\n";
  response += "A-Block-S2:" + String(slots[1][1]) + "\n";
  server.send(200, "text/plain", response);
}


