#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//========================
// Déclaration des variables globales
//
// Objectif: Déclaration des pins utilisés pour les LED et initialisation de l'écran LCD
//========================
const int pinrouge = 7;    // Pin pour la LED rouge
const int pinvert = 6;     // Pin pour la LED verte
const int pinbleu = 5;     // Pin pour la LED bleue

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialisation du LCD à l'adresse I2C 0x27

//========================
// Fonction setup()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction initialise les pins des LED en tant que sorties et configure l'écran LCD.
//    Si l'écran LCD à l'adresse 0x27 n'est pas détecté, il passe à l'adresse 0x3F.
//========================
void setup() {
  // Configuration des pins des LED comme sorties
  pinMode(pinrouge, OUTPUT);
  pinMode(pinvert, OUTPUT);
  pinMode(pinbleu, OUTPUT);

  // Vérification de l'adresse I2C du LCD, si 0x27 n'est pas trouvée, utiliser 0x3F
  if (!i2CAddrTest(0x27)) {
    lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  }

  // Initialisation de l'écran LCD
  lcd.init();
  lcd.backlight();  // Activation du rétroéclairage du LCD
}

//========================
// Fonction loop()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction est la boucle principale qui allume les LED rouge, verte et bleue
//    successivement, et affiche le compteur de temps écoulé en secondes sur l'écran LCD.
//========================
void loop() {
  allumerRouge();  // Allumer la LED rouge
  allumerVert();   // Allumer la LED verte
  allumerBleu();   // Allumer la LED bleue
  
  // Affiche le temps écoulé (compteur en secondes) sur la deuxième ligne du LCD
  lcd.setCursor(0, 1);  
  lcd.print("Compteur :");
  lcd.print(millis() / 1000);  // Affichage du nombre de secondes depuis le démarrage du programme
}

//========================
// Fonction allumerRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED rouge pendant 1 seconde, puis l'éteint.
//========================
void allumerRouge() {
  activerRouge();  // Active la LED rouge
  delay(1000);     // Maintient la LED allumée pendant 1 seconde
  eteindreRouge(); // Éteint la LED rouge
  delay(500);      // Pause de 500 millisecondes avant de passer à la LED suivante
}

//========================
// Fonction allumerVert()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED verte pendant 1 seconde, puis l'éteint.
//========================
void allumerVert() {
  activerVert();   // Active la LED verte
  delay(1000);     // Maintient la LED allumée pendant 1 seconde
  eteindreVert();  // Éteint la LED verte
  delay(500);      // Pause de 500 millisecondes
}

//========================
// Fonction allumerBleu()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED bleue pendant 1 seconde, puis l'éteint.
//========================
void allumerBleu() {
  activerBleu();   // Active la LED bleue
  delay(1000);     // Maintient la LED allumée pendant 1 seconde
  eteindreBleu();  // Éteint la LED bleue
  delay(500);      // Pause de 500 millisecondes
}

//========================
// Fonction activerRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Active uniquement la LED rouge en désactivant les autres LED, et met à jour le LCD.
//========================
void activerRouge() {
  digitalWrite(pinrouge, LOW);   // Allume la LED rouge
  digitalWrite(pinvert, HIGH);   // Éteint la LED verte
  digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
  lcd.setCursor(0, 0);           // Positionne le curseur sur la première ligne du LCD
  lcd.print("Rouge ");           // Affiche "Rouge" sur le LCD
}

//========================
// Fonction activerVert()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Active uniquement la LED verte en désactivant les autres LED, et met à jour le LCD.
//========================
void activerVert() {
  digitalWrite(pinrouge, HIGH);  // Éteint la LED rouge
  digitalWrite(pinvert, LOW);    // Allume la LED verte
  digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
  lcd.setCursor(0, 0);           // Positionne le curseur sur la première ligne du LCD
  lcd.print("Vert  ");           // Affiche "Vert" sur le LCD
}

//========================
// Fonction activerBleu()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Active uniquement la LED bleue en désactivant les autres LED, et met à jour le LCD.
//========================
void activerBleu() {
  digitalWrite(pinrouge, HIGH);  // Éteint la LED rouge
  digitalWrite(pinvert, HIGH);   // Éteint la LED verte
  digitalWrite(pinbleu, LOW);    // Allume la LED bleue
  lcd.setCursor(0, 0);           // Positionne le curseur sur la première ligne du LCD
  lcd.print("Bleu  ");           // Affiche "Bleu" sur le LCD
}

//========================
// Fonction eteindreRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Éteint la LED rouge uniquement.
//========================
void eteindreRouge() {
  digitalWrite(pinrouge, HIGH);  // Éteint la LED rouge
}

//========================
// Fonction eteindreVert()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Éteint la LED verte uniquement.
//========================
void eteindreVert() {
  digitalWrite(pinvert, HIGH);   // Éteint la LED verte
}

//========================
// Fonction eteindreBleu()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Éteint la LED bleue uniquement.
//========================
void eteindreBleu() {
  digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
}

//========================
// Fonction i2CAddrTest()
//
// Input:
//    - uint8_t addr: l'adresse I2C à tester
// Output:
//    - bool: true si l'adresse est trouvée, false sinon
// Objectif:    
//    Cette fonction vérifie si un périphérique est présent à l'adresse I2C spécifiée.
//========================
bool i2CAddrTest(uint8_t addr) {
  Wire.begin();
  Wire.beginTransmission(addr);
  if (Wire.endTransmission() == 0)
    return true;  // Si aucune erreur de transmission, l'adresse est valide
  return false;   // Si une erreur est rencontrée, l'adresse n'est pas valide
}
