# RFID-Based Login System Using Arduino and Bluetooth

This repository contains the code and documentation for an RFID-based login system developed as part of the Digital System Design (DSD) course at the University of Baghdad. The system leverages RFID technology and Bluetooth communication to authenticate users and provide feedback via a mobile phone interface.

## Features
- RFID card-based authentication
- Bluetooth-enabled control and feedback
- Modular and easy-to-extend design
- Replaces traditional LCD screens with mobile interfaces
- Real-world application example: Marketing team access control

## Components
- **Arduino UNO**: Central controller
- **RFID Reader (RC522)**: Reads RFID tags
- **Bluetooth Module (HC-05)**: Enables mobile communication
- **LED Indicators**: Visual feedback for access status
- **Breadboard**: Prototype assembly

## How It Works
1. **System Activation**: Controlled via a mobile phone.
2. **Card Scanning**: Reads RFID cards and checks credentials.
3. **Access Control**: Grants or denies access based on predefined permissions.
4. **Feedback**: Displays the result and user information on the mobile phone.

## Setup Instructions
1. Clone this repository:
   ```bash
   git clone https://github.com/yousifvn/RFID_Login_Bluetooth.git
2.	Open the project in Arduino IDE.
3.	Upload the code to your Arduino board.
4.	Connect the hardware as described in the circuit diagram.
5.	Test the system using an RFID card and your mobile phone.

## Contributions

Contributions are welcome! Feel free to fork the repo and submit pull requests.

## Acknowledgments
	•	University of Baghdad, Computer Engineering Department
	•	Arduino open-source community
	•	Online resources for Arduino and RFID tutorials
