#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10    ; Définition de la pin SDA du module RFID
#define RST_PIN 9    ; Définition de la pin RST du module RFID

MFRC522 mfrc522(SS_PIN, RST_PIN);   ; Création d'une instance de la classe MFRC522

;========================
; Fonction setup()

; Objectif:
;    - Initialise la communication série et le bus SPI.
;    - Prépare le module RFID pour la détection de cartes.

void setup() {
  Serial.begin(9600);   ; Initialisation de la communication série
  SPI.begin();          ; Initialisation du bus SPI
  mfrc522.PCD_Init();   ; Initialisation du module MFRC522
  Serial.println("Placez un badge RFID à proximité..."); ; Message pour indiquer l'attente d'un badge
}

;========================
; Fonction loop()

; Objectif:
;    - Boucle principale qui vérifie la présence d'une carte RFID, puis lit et écrit des informations
;      sur les blocs désignés (nom, prénom, secteur).

void loop() {
  ; Chercher un badge RFID
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;   ; Si aucune nouvelle carte n'est détectée, on quitte la boucle
  }

  ; Lire le badge RFID
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;   ; Si l'échec de la lecture de la carte, quitter la boucle
  }

  ;========================
  ; Afficher l'UID de la carte

  Serial.print("UID du badge : ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);   ; Affichage de l'UID en hexadécimal
  }
  Serial.println();

  ;========================
  ; Définition des blocs de données

  byte blocNom = 4;      ; Bloc pour le nom
  byte blocPrenom = 5;   ; Bloc pour le prénom
  byte blocSecteur = 6;  ; Bloc pour le secteur

  ;========================
  ; Authentification des blocs (clé par défaut)

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  ; Clé par défaut pour MIFARE Classic

  ;========================
  ; Authentifier chaque bloc

  if (!authenticateBlock(blocNom, key)) return;   ; Authentification du bloc 4 (nom)
  if (!authenticateBlock(blocPrenom, key)) return;   ; Authentification du bloc 5 (prénom)
  if (!authenticateBlock(blocSecteur, key)) return;   ; Authentification du bloc 6 (secteur)

  ;========================
  ; Écriture des informations sur les blocs

  if (!writeDataToBlock(blocNom, "OMAR")) return;      ; Écrire "OMAR" dans le bloc 4 (nom)
  if (!writeDataToBlock(blocPrenom, "IDA")) return;   ; Écrire "IDA" dans le bloc 5 (prénom)
  if (!writeDataToBlock(blocSecteur, "CYBER")) return;  ; Écrire "CYBER" dans le bloc 6 (secteur)

  Serial.println("Informations écrites avec succès !"); ; Confirmation de l'écriture réussie

  ;========================
  ; Arrêter la communication avec la carte

  mfrc522.PICC_HaltA();   ; Fin de communication avec le badge
  mfrc522.PCD_StopCrypto1();   ; Arrêt de la cryptographie
}

;========================
; Fonction authenticateBlock()

; Input:
;    - bloc : numéro du bloc à authentifier
;    - key : clé de sécurité pour l'authentification
; Output:
;    - Retourne 'true' si l'authentification est réussie, sinon 'false'.
; Objectif:
;    - Authentifie un bloc RFID avec la clé donnée.
  
bool authenticateBlock(byte bloc, MFRC522::MIFARE_Key key) {
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A, bloc, &key, &(mfrc522.uid)   ; Authentification du bloc
  );
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Échec de l'authentification du bloc ");   ; Afficher le message d'erreur
    Serial.print(bloc);
    Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  return true;
}

;========================
; Fonction writeDataToBlock()

; Input:
;    - bloc : numéro du bloc où écrire les données
;    - data : chaîne de caractères à écrire
; Output:
;    - Retourne 'true' si l'écriture est réussie, sinon 'false'.
; Objectif:
;    - Écrit les données fournies dans le bloc RFID spécifié.

bool writeDataToBlock(byte bloc, String data) {
  byte dataBlock[16];  ; Chaque bloc a une taille de 16 octets

  ; Copier les données dans dataBlock (16 octets max)
  for (int i = 0; i < 16; i++) {
    if (i < data.length()) {
      dataBlock[i] = data[i];   ; Copier les caractères de la chaîne
    } else {
      dataBlock[i] = 0x00;   ; Compléter avec des zéros si la chaîne est plus courte
    }
  }

  ; Écrire les données sur le bloc
  MFRC522::StatusCode status = mfrc522.MIFARE_Write(bloc, dataBlock, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print("Échec de l'écriture sur le bloc ");   ; Message d'erreur
    Serial.print(bloc);
    Serial.print(": ");
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  Serial.print("Écriture réussie sur le bloc ");   ; Confirmation de l'écriture réussie
  Serial.println(bloc);
  return true;
}
