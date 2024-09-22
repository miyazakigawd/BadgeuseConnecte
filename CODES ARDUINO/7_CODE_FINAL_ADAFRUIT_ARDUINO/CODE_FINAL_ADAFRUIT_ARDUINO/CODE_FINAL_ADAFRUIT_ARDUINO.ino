#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <SoftwareSerial.h>

// Initialisation des objets
RFID rfid(10, 9);
unsigned char status;
unsigned char str[MAX_LEN];
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial espSerial(3, 2);

// UID valides
unsigned char validUID[3][4] = {
    {0xA3, 0xCA, 0x21, 0x11},
    {0x83, 0xED, 0x60, 0xAC},
    {0x7C, 0x19, 0xC4, 0x38}
};

// Broches des LEDs
const int pinRed = 7;
const int pinBlue = 6;
const int pinGreen = 5;

//========================
// Fonction setup()  

// Objectif:    
//    Cette fonction initialise le LCD, le module RFID, 
//    configure les broches des LEDs, et établit 
//    la connexion série avec l'ESP8266.
void setup() {
    // Vérifie l'adresse I2C du LCD
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2); // Adresse alternative si la première échoue
    }

    // Configuration des broches des LEDs
    pinMode(pinRed, OUTPUT);
    pinMode(pinBlue, OUTPUT);
    pinMode(pinGreen, OUTPUT);
    
    ledOFF(); // Éteindre toutes les LEDs

    // Initialisation du LCD
    lcd.init();
    lcd.backlight(); // Activer le rétroéclairage
    lcd.print("Scanner la carte");

    Serial.begin(9600); // Initialiser la communication série
    espSerial.begin(9600); // Initialiser la communication avec l'ESP8266
    SPI.begin(); // Initialiser le bus SPI
    rfid.init(); // Initialiser le module RFID
    Serial.println("Veuillez présenter la carte...");
}

//========================
// Fonction loop()  

// Objectif:    
//    Cette fonction lit les cartes RFID et vérifie si l'UID 
//    est valide. Elle gère également l'affichage sur le LCD 
//    et l'état des LEDs.
void loop() {
    // Vérifie si une carte RFID est présente
    if (rfid.findCard(PICC_REQIDL, str) == MI_OK) {
        Serial.println("Carte détectée !");
        
        // Lire l'UID de la carte
        if (rfid.anticoll(str) == MI_OK) {
            Serial.print("Numéro de la carte : ");
            unsigned char scannedUID[4];
            String uidString = "";

            // Construction de la chaîne d'UID pour l'affichage
            for (int i = 0; i < 4; i++) {
                Serial.print(0x0F & (str[i] >> 4), HEX);
                Serial.print(0x0F & str[i], HEX);
                scannedUID[i] = str[i];
                uidString += String(0x0F & (str[i] >> 4), HEX);
                uidString += String(0x0F & str[i], HEX);
            }
            Serial.println("");
            Serial.print("Envoi de l'UID à ESP8266 : ");
            Serial.println(uidString);
            espSerial.println(uidString); // Envoi de l'UID à l'ESP8266

            // Vérification de l'UID
            if (checkValidUID(scannedUID)) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Acces accorde");
                Serial.println("Acces accorde");
                ledGreen(); // Allume la LED verte
                delay(1000);
                ledOFF(); // Éteindre toutes les LEDs
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Scanner la carte");
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Acces refuse");
                Serial.println("Acces refuse");
                ledRouge(); // Allume la LED rouge
                delay(1000);
                ledOFF(); // Éteindre toutes les LEDs
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Scanner la carte");
            }
        }
        rfid.halt(); // Arrêter la communication avec la carte
    }
    delay(100); // Pause pour éviter un trop grand nombre de lectures
}

//========================
// Fonction checkValidUID()  

// Input:
//    - tableau d'UID scanné
// Output:
//    - booléen indiquant si l'UID est valide
// Objectif:    
//    Vérifier si l'UID scanné correspond à un UID autorisé.
bool checkValidUID(unsigned char scannedUID[4]) {
    for (int i = 0; i < 3; i++) {
        if (compareUID(scannedUID, validUID[i])) {
            return true; // UID valide trouvé
        }
    }
    return false; // Aucun UID valide trouvé
}

//========================
// Fonction compareUID()  

// Input:
//    - uid1 : UID scanné
//    - uid2 : UID valide à comparer
// Output:
//    - booléen indiquant si les UID sont identiques
// Objectif:    
//    Comparer deux UID et retourner vrai s'ils sont identiques.
bool compareUID(unsigned char uid1[4], unsigned char *uid2) {
    for (int i = 0; i < 4; i++) {
        if (uid1[i] != uid2[i]) {
            return false; // Les UID ne correspondent pas
        }
    }
    return true; // Les UID correspondent
}

//========================
// Fonction ledRouge()  

// Objectif:    
//    Allumer la LED rouge pour signaler un accès refusé.
void ledRouge() {
    digitalWrite(pinBlue, HIGH); // Éteindre la LED bleue
    digitalWrite(pinGreen, HIGH); // Éteindre la LED verte
    digitalWrite(pinRed, LOW); // Allumer la LED rouge
    delay(1000);
}

//========================
// Fonction ledGreen()  

// Objectif:    
//    Allumer la LED verte pour signaler un accès accordé.
void ledGreen() {
    digitalWrite(pinRed, HIGH); // Éteindre la LED rouge
    digitalWrite(pinGreen, HIGH); // Éteindre la LED verte
    digitalWrite(pinBlue, LOW); // Allumer la LED bleue
    delay(1000);
}

//========================
// Fonction ledOFF()  

// Objectif:    
//    Éteindre toutes les LEDs.
void ledOFF() {
    digitalWrite(pinRed, HIGH); // Éteindre la LED rouge
    digitalWrite(pinGreen, HIGH); // Éteindre la LED verte
    digitalWrite(pinBlue, HIGH); // Éteindre la LED bleue
}

//========================
// Fonction i2CAddrTest()  

// Input:
//    - addr : adresse I2C à tester
// Output:
//    - booléen indiquant si l'adresse est valide
// Objectif:    
//    Tester si l'adresse I2C donnée est valide.
bool i2CAddrTest(uint8_t addr) {
    Wire.begin(); // Initialiser le bus I2C
    Wire.beginTransmission(addr); // Commencer la transmission à l'adresse
    if (Wire.endTransmission() == 0)
        return true; // Adresse valide
    return false; // Adresse invalide
}
