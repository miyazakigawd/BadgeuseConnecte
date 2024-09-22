const int pinrouge = 7;
const int pinvert = 5;
const int pinbleu = 6;

//========================
// Fonction setup()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction configure les broches des LED en mode sortie. 
//    Elle prépare le système pour pouvoir allumer et éteindre les LEDs.
//========================
void setup() {
    // Configuration des broches des LED en tant que sorties
    pinMode(pinrouge, OUTPUT);
    pinMode(pinvert, OUTPUT);
    pinMode(pinbleu, OUTPUT);
}

//========================
// Fonction loop()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction est la boucle principale qui contrôle l'allumage 
//    séquentiel des LED rouge, vert et bleu. Chaque LED s'allume, reste allumée
//    pendant une seconde, puis s'éteint avant de passer à la suivante.
//========================
void loop() {
    // Allume et éteint la LED rouge
    allumerRouge();
    
    // Allume et éteint la LED verte
    allumerVert();
    
    // Allume et éteint la LED bleue
    allumerBleu();
}

//========================
// Fonction allumerRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED rouge pendant 1 seconde,
//    puis l'éteint pendant 500 millisecondes avant de passer à la suivante.
//========================
void allumerRouge() {
    // Active la LED rouge
    activerRouge();
    delay(1000);  // Garde la LED allumée pendant 1 seconde

    // Éteint la LED rouge
    eteindreRouge();
    delay(500);   // Attend 500 millisecondes avant l'étape suivante
}

//========================
// Fonction allumerVert()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED verte pendant 1 seconde,
//    puis l'éteint pendant 500 millisecondes avant de passer à la suivante.
//========================
void allumerVert() {
    // Active la LED verte
    activerVert();
    delay(1000);  // Garde la LED allumée pendant 1 seconde

    // Éteint la LED verte
    eteindreVert();
    delay(500);   // Attend 500 millisecondes avant l'étape suivante
}

//========================
// Fonction allumerBleu()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction allume la LED bleue pendant 1 seconde,
//    puis l'éteint pendant 500 millisecondes avant de passer à la suivante.
//========================
void allumerBleu() {
    // Active la LED bleue
    activerBleu();
    delay(1000);  // Garde la LED allumée pendant 1 seconde

    // Éteint la LED bleue
    eteindreBleu();
    delay(500);   // Attend 500 millisecondes avant l'étape suivante
}

//========================
// Fonction activerRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction active la LED rouge en éteignant les autres LEDs (vert et bleu).
//========================
void activerRouge() {
    digitalWrite(pinrouge, LOW);   // Allume la LED rouge (LOW active l'LED)
    digitalWrite(pinvert, HIGH);   // Éteint la LED verte
    digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
}

//========================
// Fonction activerVert()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction active la LED verte en éteignant les autres LEDs (rouge et bleu).
//========================
void activerVert() {
    digitalWrite(pinrouge, HIGH);  // Éteint la LED rouge
    digitalWrite(pinvert, LOW);    // Allume la LED verte
    digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
}

//========================
// Fonction activerBleu()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction active la LED bleue en éteignant les autres LEDs (rouge et vert).
//========================
void activerBleu() {
    digitalWrite(pinrouge, HIGH);  // Éteint la LED rouge
    digitalWrite(pinvert, HIGH);   // Éteint la LED verte
    digitalWrite(pinbleu, LOW);    // Allume la LED bleue
}

//========================
// Fonction eteindreRouge()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:    
//    Cette fonction éteint la LED rouge.
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
//    Cette fonction éteint la LED verte.
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
//    Cette fonction éteint la LED bleue.
//========================
void eteindreBleu() {
    digitalWrite(pinbleu, HIGH);   // Éteint la LED bleue
}
