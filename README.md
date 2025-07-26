# Parkease
🚗 ParkEase: An Automated Parking System
📍 Final Year Mini Project | Electronics and Communication Engineering | MBCET
🔍 Overview
ParkEase is a smart, automated parking management system designed specifically for college campuses. It uses microcontrollers, sensors, and real-time communication to guide users to available slots, reduce congestion, and make parking simple, fast, and eco-friendly.

🎯 Objectives
Eliminate manual parking allocation

Reduce campus traffic and idling time

Provide real-time slot updates and navigation

Improve overall user experience for drivers and security staff

💡 Key Features
ESP32-based smart tag system for dynamic parking guidance

IR sensors (x4 per slot) for precise vehicle detection

Real-time server updates with availability and navigation info

LCD display for live instructions and route guidance

GPS module to navigate drivers to their assigned slots

Switch-based interaction to repeat or change navigation routes

Paperless and eco-friendly approach to parking

🔧 Technologies & Components Used
ESP32 microcontrollers

IR Proximity Sensors (LM393)

Ublox NEO-6M GPS Module

I2C LCD Display

Arduino Uno (Central Server)

WiFi (for communication between modules)

Custom PCB layout

🧠 How It Works
Entrance: Vehicle is assigned a slot via ESP32-based tag after checking real-time availability.

Navigation: LCD guides driver using GPS coordinates and route selection.

Location Locking: Four IR sensors validate vehicle presence to confirm proper parking.

Exit: Tag guides driver to the nearest exit and updates server to release the slot.

🖥 Software
Arduino IDE for firmware

C++ / Arduino code for ESP32 and sensor communication

HTTP & JSON-based server communication

Web Dashboard (HTML + CSS) for live slot monitoring

📊 Outcomes
Reduced human intervention in parking management

Smarter space utilization

Real-time updates and remote access

Improved traffic flow and user satisfaction

Scalable for large campuses or institutions

📎 Project Members
Ammu Aby

Aadarsh H S

Abhinav A S

Aimen Navas

Rishikesh J S

🏫 Institution
Department of Electronics & Communication Engineering
Mar Baselios College of Engineering and Technology (MBCET)
APJ Abdul Kalam Technological University
