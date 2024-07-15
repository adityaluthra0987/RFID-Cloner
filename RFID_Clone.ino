#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4); // Adjust the address if needed

MFRC522::MIFARE_Key key;

byte buffer[18];

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  lcd.init();
  lcd.backlight();

  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  displayMessage("System Initialized", "Ready to read card", "", "");
}

void loop() {
  // Look for new RFID cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    if (readCard()) {
      cloneCard();
    } else {
      displayErrorMessage("Reading failed");
    }
  }
}

// Function to display a message on the LCD
void displayMessage(String line1, String line2, String line3, String line4) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  lcd.setCursor(0, 2);
  lcd.print(line3);
  lcd.setCursor(0, 3);
  lcd.print(line4);
  Serial.println(line1 + " " + line2 + " " + line3 + " " + line4); // Log the message
}

bool readCard() {
  displayMessage("Reading card...", "Please wait", "", "");

  byte block = 4; // You can change this to read different blocks
  MFRC522::StatusCode status;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  status = mfrc522.MIFARE_Read(block, buffer, &buffer[18]);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Reading failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  Serial.print(F("Data in block "));
  Serial.print(block);
  Serial.print(F(": "));
  for (byte i = 0; i < 16; i++) {
    Serial.print(buffer[i], HEX);
    Serial.print(F(" "));
  }
  Serial.println();

  displayMessage("Card read", "Successfully", "", "");
  delay(2000);

  displayMessage("Ready to imprint", "Tap card again", "", "");
  return true;
}

// Function to clone card data
void cloneCard() {
  // Wait for the same card to be tapped again
  while (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(100);
  }

  displayMessage("Cloning card...", "Please wait", "", "");

  byte block = 4; // You can change this to write to different blocks
  MFRC522::StatusCode status;

  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Authentication failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    displayErrorMessage("Authentication failed");
    return;
  }

  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Writing failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    displayErrorMessage("Writing failed");
    return;
  }

  displayMessage("Card cloned", "Operation successful", "", "");
  delay(2000);

  displayMessage("Cloned the fosfii", "", "", "");
  delay(2000);

  displayMessage("System Ready", "Tap a card", "to read", "");
}

void displayErrorMessage(String message) {
  displayMessage("Error:", message, "Please try again", "");
  delay(3000);
  displayMessage("System Ready", "Tap a card", "to read", "");
}
