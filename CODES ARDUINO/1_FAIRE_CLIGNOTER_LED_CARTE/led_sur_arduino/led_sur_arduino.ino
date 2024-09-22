#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RFID.h>
#include <SoftwareSerial.h>

//========================
// Fonction setup()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction initialise les composants (LCD, RFID, LED, ESP8266) 
//    et prépare l'appareil pour scanner une carte RFID.
//========================
void setup() {
    // Teste si l'adresse I2C 0x27 est valide pour le LCD, sinon utilise l'adresse 0x3F
    if (!i2CAddrTest(0x27)) {
        lcd = LiquidCrystal_I2C(0x3F, 16, 2);
    }
    
    // Initialisation des broches pour les LED
    pinMode(pinRed, OUTPUT);
    pinMode(pinBlue, OUTPUT);
    pinMode(pinGreen, OUTPUT);

    // Éteint toutes les LED
    ledOFF();

    // Initialisation de l'écran LCD et activation du rétroéclairage
    lcd.init();
    lcd.backlight();
    lcd.print("Scanner la carte");

    // Initialisation de la communication série (PC) et ESP8266
    Serial.begin(9600);
    espSerial.begin(9600);

    // Démarre la communication SPI pour le lecteur RFID
    SPI.begin();
    rfid.init();

    // Message initial dans le moniteur série
    Serial.println("Veuillez présenter la carte...");
}

//========================
// Fonction loop()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction se répète en boucle et gère la lecture des cartes RFID. 
//    Elle vérifie l'UID et accorde ou refuse l'accès en fonction.
//========================
void loop() {
    // Cherche une carte RFID à proximité
    if (rfid.findCard(PICC_REQIDL, str) == MI_OK) {
        Serial.println("Carte détectée !");

        // Si une carte est détectée, on récupère son UID pour le traitement
        if (rfid.anticoll(str) == MI_OK) {
            Serial.print("Numéro de la carte : ");
            unsigned char scannedUID[4];
            String uidString = "";

            // Boucle pour extraire et afficher l'UID de la carte
            for (int i = 0; i < 4; i++) {
                Serial.print(0x0F & (str[i] >> 4), HEX);
                Serial.print(0x0F & str[i], HEX);
                scannedUID[i] = str[i];

                // Construction d'une chaîne représentant l'UID
                uidString += String(0x0F & (str[i] >> 4), HEX);
                uidString += String(0x0F & str[i], HEX);
            }
            Serial.println("");
            Serial.print("Envoi de l'UID à ESP8266 : ");
            Serial.println(uidString);

            // Envoi de l'UID au module ESP8266 via Serial
            espSerial.println(uidString);

            // Vérifie si l'UID est valide
            if (checkValidUID(scannedUID)) {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Acces accorde");
                Serial.println("Acces accorde");

                // Allume la LED verte pour indiquer l'accès accordé
                ledGreen();
                delay(1000);
                ledOFF();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Scanner la carte");
            } else {
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Acces refuse");
                Serial.println("Acces refuse");

                // Allume la LED rouge pour indiquer l'accès refusé
                ledRouge();
                delay(1000);
                ledOFF();
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Scanner la carte");
            }
        }
        rfid.halt();  // Arrête la communication avec la carte
    }
    delay(100);  // Pause avant de scanner à nouveau
}

//========================
// Fonction checkValidUID()
//
// Input:
//    - unsigned char scannedUID[]: tableau contenant l'UID de la carte scannée
// Output:
//    - bool: retourne true si l'UID est valide, false sinon
// Objectif:    
//    Cette fonction compare l'UID scanné à une liste d'UIDs valides.
//========================
bool checkValidUID(unsigned char scannedUID[]) {
    // Vérifie l'UID scanné contre les 3 UID autorisés
    for (int i = 0; i < 3; i++) {
        if (compareUID(scannedUID, validUID[i])) {
            return true;  // UID trouvé dans la liste
        }
    }
    return false;  // UID non trouvé
}

//========================
// Fonction compareUID()
//
// Input:
//    - unsigned char uid1[]: tableau contenant l'UID à comparer
//    - unsigned char *uid2: tableau d'un UID autorisé à comparer
// Output:
//    - bool: retourne true si les deux UID sont identiques, false sinon
// Objectif:    
//    Cette fonction compare deux UID octet par octet.
//========================
bool compareUID(unsigned char uid1[], unsigned char *uid2) {
    // Compare les 4 octets de l'UID
    for (int i = 0; i < 4; i++) {
        if (uid1[i] != uid2[i]) {
            return false;  // Si un octet est différent, retourne false
        }
    }
    return true;  // UIDs identiques
}

//========================
// Fonction ledRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED rouge pour indiquer un accès refusé.
//========================
void ledRouge() {
    digitalWrite(pinBlue, HIGH);  // Éteint la LED bleue
    digitalWrite(pinGreen, HIGH);  // Éteint la LED verte
    digitalWrite(pinRed, LOW);  // Allume la LED rouge
    delay(1000);
}

//========================
// Fonction ledGreen()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED verte pour indiquer un accès accordé.
//========================
void ledGreen() {
    digitalWrite(pinRed, HIGH);  // Éteint la LED rouge
    digitalWrite(pinGreen, HIGH);  // Allume la LED verte
    digitalWrite(pinBlue, LOW);  // Éteint la LED bleue
    delay(1000);
}

//========================
// Fonction ledOFF()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction éteint toutes les LEDs.
//========================
void ledOFF() {
    digitalWrite(pinRed, HIGH);  // Éteint la LED rouge
    digitalWrite(pinGreen, HIGH);  // Éteint la LED verte
    digitalWrite(pinBlue, HIGH);  // Éteint la LED bleue
}

//========================
// Fonction i2CAddrTest()
//
// Input:
//    - uint8_t addr: adresse I2C à tester
// Output:
//    - bool: retourne true si l'adresse est valide, false sinon
// Objectif:    
//    Cette fonction teste la communication avec une adresse I2C donnée.
//========================
bool i2CAddrTest(uint8_t addr) {
    Wire.begin();  // Initialisation du bus I2C
    Wire.beginTransmission(addr);  // Démarre la transmission à l'adresse donnée
    if (Wire.endTransmission() == 0)  // Si aucun problème de transmission
        return true;
    return false;  // Si la transmission échoue
}
