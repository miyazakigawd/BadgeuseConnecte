#include <SPI.h>
#include <MFRC522.h>

//========================
// Déclaration des constantes et variables
//
// Objectif:
//    - Initialisation des pins du module RFID et des LED
//    - Création de l'instance pour le module MFRC522
//========================
#define SS_PIN 10     // Pin SDA pour le module RFID
#define RST_PIN 9     // Pin RST pour le module RFID
#define LED_VERTE 7   // Pin pour la LED verte
#define LED_ROUGE 6   // Pin pour la LED rouge

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Création de l'instance MFRC522

// Donnée autorisée pour le bloc 6
const char secteurAutorise[] = "CYBER   ";  // Autorisation basée sur "CYBER" avec des espaces

//========================
// Fonction setup()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:
//    - Initialiser la communication SPI et le module MFRC522
//    - Configurer les LED comme sorties et les éteindre au démarrage
//    - Afficher un message dans la console série pour indiquer que le lecteur est prêt
//========================
void setup() {
  Serial.begin(9600);   // Initialisation de la communication série
  SPI.begin();          // Initialisation du bus SPI pour le module RFID
  mfrc522.PCD_Init();   // Initialisation du module MFRC522
  
  // Initialisation des LED
  pinMode(LED_VERTE, OUTPUT);  // Configurer la LED verte comme sortie
  pinMode(LED_ROUGE, OUTPUT);  // Configurer la LED rouge comme sortie
  
  // Éteindre les LED au démarrage
  digitalWrite(LED_VERTE, LOW);
  digitalWrite(LED_ROUGE, LOW);

  Serial.println("Placez un badge RFID à proximité...");  // Indication à l'utilisateur
}

//========================
// Fonction loop()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:
//    - Scanner les badges RFID à proximité
//    - Authentifier et lire le bloc 6 du badge RFID
//    - Comparer les données lues avec celles autorisées pour accorder ou refuser l'accès
//========================
void loop() {
  // Vérifier si un nouveau badge RFID est détecté
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;  // Aucun badge détecté, on retourne
  }

  // Lecture du badge RFID
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;  // Erreur de lecture, on retourne
  }

  // Lecture des données du bloc 6 du badge
  char secteur[16 + 1];  // Tampon de 16 octets pour stocker les données lues
  if (!readBlockData(6, secteur)) {
    Serial.println("Erreur de lecture du bloc 6.");  // En cas d'erreur
  } else {
    secteur[16] = '\0';  // Terminer correctement la chaîne de caractères
    Serial.print("Données du bloc 6: ");
    Serial.println(secteur);  // Afficher les données lues
  }

  // Comparaison des données lues avec la donnée autorisée
  if (strcmp(secteur, secteurAutorise) == 0) {
    // Si les données correspondent, accès autorisé
    Serial.println("Access Granted!");
    digitalWrite(LED_VERTE, HIGH);  // Allumer la LED verte
    digitalWrite(LED_ROUGE, LOW);   // Éteindre la LED rouge
  } else {
    // Si les données ne correspondent pas, accès refusé
    Serial.println("Access Denied!");
    digitalWrite(LED_ROUGE, HIGH);  // Allumer la LED rouge
    digitalWrite(LED_VERTE, LOW);   // Éteindre la LED verte
  }

  // Délai avant de réinitialiser les LEDs
  delay(2000);
  digitalWrite(LED_VERTE, LOW);
  digitalWrite(LED_ROUGE, LOW);

  // Arrêter la communication avec le badge RFID
  mfrc522.PICC_HaltA();         // Arrêter la lecture de la carte
  mfrc522.PCD_StopCrypto1();    // Désactiver la cryptographie
}

//========================
// Fonction readBlockData()
// 
// Input:
//    - byte bloc: numéro du bloc à lire
//    - char* dataOut: tampon pour stocker les données lues
// Output:
//    - bool: retourne true si la lecture est réussie, false en cas d'erreur
// Objectif:
//    - Authentifier et lire les données du bloc spécifié du badge RFID
//    - Stocker les données lues dans le tampon fourni
//========================
bool readBlockData(byte bloc, char *dataOut) {
  MFRC522::MIFARE_Key key;  // Clé pour l'authentification du bloc
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  // Clé par défaut : 0xFF

  // Authentification du bloc spécifié
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloc, &key, &(mfrc522.uid)
  );
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Erreur d'authentification du bloc ");
    Serial.print(bloc);
    Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;  // Retourne false si l'authentification échoue
  }

  // Lecture des données du bloc
  byte buffer[16];  // Tampon pour stocker les données du bloc
  byte size = sizeof(buffer);
  status = mfrc522.MIFARE_Read(bloc, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Erreur de lecture du bloc ");
    Serial.print(bloc);
    Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;  // Retourne false si la lecture échoue
  }

  // Afficher les données en hexadécimal pour débogage
  Serial.print("Données en hexadécimal du bloc ");
  Serial.print(bloc);
  Serial.print(": ");
  for (byte i = 0; i < 16; i++) {
    Serial.print(" ");
    if (buffer[i] < 0x10) Serial.print("0");  // Ajouter un '0' pour les valeurs inférieures à 0x10
    Serial.print(buffer[i], HEX);
  }
  Serial.println();

  // Copier les données lues dans dataOut
  for (byte i = 0; i < 16; i++) {
    dataOut[i] = buffer[i];  // Copier les données du tampon dans dataOut
  }
  dataOut[16] = '\0';  // Terminer la chaîne de caractères

  return true;  // Retourne true si la lecture réussit
}
