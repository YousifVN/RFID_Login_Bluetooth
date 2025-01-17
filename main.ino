#include <SPI.h>        // SPI library for communication with RFID reader
#include <MFRC522.h>    // MFRC522 library for RFID functionality

// RFID module pins
#define RST_PIN 9       // Reset pin for the RFID module
#define SS_PIN 10       // Slave Select (SS) pin for the RFID module

// LED pins to indicate access
int led1 = 6;           // Green LED for access granted
int led2 = 7;           // Red LED for access denied

// System control flag to manage system activation by HR
bool systemActive = false; 

// Structure to represent employee information
struct Employee {
  String name;          // Employee's name
  String role;          // Employee's role
  String department;    // Employee's department
  bool authorized;      // Whether the employee is authorized
};

// Array of employees (database)
Employee employees[] = {
  {"Ali Hassan", "Team Lead", "Marketing", true},   // Card 1 - Authorized
  {"Yousif Ahmed", "Designer", "Marketing", true},  // Card 2 - Authorized
  {"Jasim Karim", "Salesman", "Sales", false},      // Card 3 - Unauthorized
  {"Unknown", "Unknown", "Unknown", false}          // Card 4 - Unregistered
};

// Corresponding RFID card IDs for employees
String cardIDs[] = {
  "8382203",  // Card 1 - Ali Hassan
  "DCE02B3",  // Card 2 - Yousif Ahmed
  "A3EA14DA", // Card 3 - Jasim Karim
  "00000000"  // Card 4 - Unknown card
};

// Variable to store the scanned RFID tag ID
String tagID = ""; 

// Create an instance of the MFRC522 RFID reader
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  // Initialize SPI communication for RFID reader
  SPI.begin();       
  // Initialize the RFID reader
  mfrc522.PCD_Init(); 

  // Set LED pins as output
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  // Initialize serial communication for system messages
  Serial.begin(9600); 
  Serial.println("Marketing Team Login System");
  Serial.println("Waiting for HR to activate the system...");
}

void loop() {
  // Check for HR's command input via serial communication
  if (Serial.available() > 0) {
    char command = Serial.read(); // Read the incoming command
    if (command == '1') {
      // Activate the system
      systemActive = true;
      Serial.println("System activated by HR.");
      Serial.println("Scan your card...");
    } else if (command == '0') {
      // Deactivate the system
      systemActive = false;
      Serial.println("System deactivated by HR.");
    }
  }

  // Only process card scans if the system is active
  if (systemActive && getID()) {
    int employeeIndex = -1;

    // Check if the scanned card ID matches any in the database
    for (int i = 0; i < sizeof(cardIDs) / sizeof(cardIDs[0]); i++) {
      if (tagID == cardIDs[i]) {
        employeeIndex = i; // Store the matched employee index
        break;
      }
    }

    if (employeeIndex != -1) {
      // A valid card ID was found in the database
      Employee emp = employees[employeeIndex];
      if (emp.authorized && emp.department == "Marketing") {
        // Access granted for authorized employees in Marketing
        digitalWrite(led1, HIGH); // Turn on green LED
        digitalWrite(led2, LOW);  // Turn off red LED
        Serial.print("Access Granted | ");
        Serial.print("Name: " + emp.name + " | ");
        Serial.print("Role: " + emp.role + " | ");
        Serial.print("Department: " + emp.department);
        Serial.println();
      } else {
        // Access denied for unauthorized employees or non-Marketing staff
        digitalWrite(led1, LOW);  // Turn off green LED
        digitalWrite(led2, HIGH); // Turn on red LED
        Serial.print("Access Denied | ");
        Serial.print("Name: " + emp.name + " | ");
        Serial.print("Role: " + emp.role + " | ");
        Serial.print("Department: " + emp.department + " | ");
        if (emp.department != "Marketing") {
          // Denied due to being in the wrong department
          Serial.print("Reason: Unauthorized Department");
        } else {
          // Denied due to lack of authorization
          Serial.print("Reason: Not Authorized");
        }
        Serial.println();
      }
    } else {
      // Unknown card scanned (not in the database)
      digitalWrite(led1, LOW);  // Turn off green LED
      digitalWrite(led2, HIGH); // Turn on red LED
      Serial.print("Access Denied | Unknown Card Scanned");
      Serial.println();
    }

    // Wait for 2 seconds before resetting LEDs and readying for the next scan
    delay(2000); 
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    Serial.println("Scan your card...");
  }
}

// Function to read the RFID tag ID if a new card is present
boolean getID() {
  // Check if a new RFID card is present
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false; // No new card found
  }
  // Attempt to read the card's serial number (ID)
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false; // Failed to read the card
  }

  // Convert the card's serial number (byte array) to a string
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Concatenate each byte as a hex string
  }
  // Convert tag ID to uppercase for consistency
  tagID.toUpperCase();

  // Halt communication with the current card
  mfrc522.PICC_HaltA();
  return true; // Successfully read the card ID
}
