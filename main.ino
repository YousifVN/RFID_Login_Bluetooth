#include <SPI.h>
#include <MFRC522.h>

// RFID pins
#define RST_PIN 9
#define SS_PIN 10

// LED pins
int led1 = 6; // Access granted
int led2 = 7; // Access denied

// Predefined card data
struct Employee {
  String name;
  String department;
  bool authorized;
};

// Employee database
Employee employees[] = {
  {"Ali", "Marketing", true},   // Card 1
  {"Yousif", "Marketing", true}, // Card 2
  {"Jasim", "Sales", false},    // Card 3
  {"Unknown", "Unknown", false} // Card 4 (unregistered card)
};

// Corresponding card IDs
String cardIDs[] = {
  "6BA51B2F",  // Card 1
  "7C72A3E4",  // Card 2
  "8D83B4F5",  // Card 3
  "00000000"   // Card 4 (unknown card)
};

String tagID = ""; // Scanned card ID

// Create RFID instance
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  SPI.begin();       // Initialize SPI bus
  mfrc522.PCD_Init(); // Initialize RFID reader
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);

  Serial.begin(9600); // Initialize serial communication
  Serial.println("Marketing Team Login System");
  Serial.println("Scan Your Card...");
}

void loop() {
  if (getID()) { // If a card is scanned
    int employeeIndex = -1;
    for (int i = 0; i < sizeof(cardIDs) / sizeof(cardIDs[0]); i++) {
      if (tagID == cardIDs[i]) {
        employeeIndex = i;
        break;
      }
    }

    if (employeeIndex != -1) {
      // Valid card found
      Employee emp = employees[employeeIndex];
      if (emp.authorized && emp.department == "Marketing") {
        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        Serial.println("---------------");
        Serial.println("Access Granted!");
        Serial.print("Name: ");
        Serial.println(emp.name);
        Serial.print("Department: ");
        Serial.println(emp.department);
        Serial.println("---------------");
      } else {
        digitalWrite(led1, LOW);
        digitalWrite(led2, HIGH);
        Serial.println("---------------");
        Serial.println("Access Denied!");
        Serial.print("Name: ");
        Serial.println(emp.name);
        Serial.print("Department: ");
        Serial.println(emp.department);
        if (emp.department != "Marketing") {
          Serial.println("Reason: Unauthorized Department");
        } else {
          Serial.println("Reason: Not Authorized");
        }
        Serial.println("---------------");
      }
    } else {
      // Unknown card
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      Serial.println("---------------");
      Serial.println("Access Denied!");
      Serial.println("Unknown Card Scanned");
      Serial.println("---------------");
    }

    delay(2000); // Wait for 2 seconds before resetting
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    Serial.println("Scan Your Card...");
  }
}

// Read new tag if available
boolean getID() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return false;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return false;
  }
  tagID = "";
  for (uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
