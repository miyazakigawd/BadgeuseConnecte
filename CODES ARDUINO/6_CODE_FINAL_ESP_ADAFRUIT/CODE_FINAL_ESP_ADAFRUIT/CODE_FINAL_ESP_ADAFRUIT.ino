#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//========================
// Initialisation des paramètres Wi-Fi et Adafruit IO
//
// Objectif:
//    - Définir les constantes pour la connexion Wi-Fi et au serveur Adafruit IO
//========================
#define WLAN_SSID "en bas la"      // Nom du réseau Wi-Fi
#define WLAN_PASS "azerty123"      // Mot de passe Wi-Fi

#define AIO_SERVER "io.adafruit.com"   // Adresse du serveur Adafruit IO
#define AIO_SERVERPORT 1883            // Port MQTT
#define IO_USERNAME "UsX01"            // Nom d'utilisateur Adafruit IO
#define IO_KEY "aio_befC76UViow7cdgDhwmbPFltRXb0"  // Clé API Adafruit IO

int ledPin = 5;   // Pin de la LED

//========================
// Déclaration des objets Wi-Fi et MQTT
//
// Objectif:
//    - Créer les objets nécessaires pour la communication Wi-Fi et MQTT
//========================
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, IO_USERNAME, IO_KEY);
Adafruit_MQTT_Publish Presence = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/rfid-att");

String rfidUID = "";  // Stockage de l'UID RFID

//========================
// Fonction setup()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:
//    - Initialiser la communication série et Wi-Fi
//    - Connecter le module NodeMCU à un réseau Wi-Fi
//    - Initialiser la connexion avec le serveur Adafruit IO
//========================
void setup() {
  pinMode(ledPin, OUTPUT);   // Configurer la LED comme sortie
  digitalWrite(ledPin, LOW); // Éteindre la LED au démarrage
  Serial.begin(9600);        // Initialiser la communication série pour le débogage
  delay(1000);               // Délai d'une seconde pour initialiser correctement
  
  // Afficher le début de la connexion Wi-Fi
  Serial.println();
  delay(10);
  Serial.print("Connexion à ");
  Serial.println(WLAN_SSID);

  // Connexion au réseau Wi-Fi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Afficher des points jusqu'à la connexion réussie
  }

  // Lorsque la connexion est réussie, allumer la LED et afficher l'IP
  digitalWrite(ledPin, HIGH);  
  Serial.println();
  Serial.println("Connexion Wi-Fi réussie!");
  Serial.print("Adresse IP NodeMCU : ");
  Serial.println(WiFi.localIP());  // Afficher l'adresse IP de l'ESP8266

  connecter();  // Appeler la fonction pour se connecter à Adafruit IO
}

//========================
// Fonction connecter()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:
//    - Gérer la connexion au serveur Adafruit IO
//    - Traiter les erreurs possibles et les afficher
//========================
void connecter() {
  Serial.println("Connexion à Adafruit IO...");
  int8_t ret;  // Variable de retour pour suivre l'état de la connexion
  while ((ret = mqtt.connect()) != 0) {
    // Gestion des différents codes d'erreur et affichage du message correspondant
    switch (ret) {
      case 1: Serial.println("Protocole incorrect"); break;
      case 2: Serial.println("ID rejeté"); break;
      case 3: Serial.println("Serveur indisponible"); break;
      case 4: Serial.println("Mauvais identifiant/mot de passe"); break;
      case 5: Serial.println("Non autorisé"); break;
      case 6: Serial.println("Échec de l'abonnement"); break;
      default: Serial.println("Échec de la connexion"); break;
    }
    // Si la connexion échoue, déconnexion et nouvelle tentative après 5 secondes
    if (ret != 0) {
      mqtt.disconnect();
      Serial.println("Nouvel essai de connexion dans 5 secondes...");
      delay(5000);
    }
  }
  // Si la connexion est réussie, afficher le message correspondant
  Serial.println("Connecté à Adafruit IO !");
}

//========================
// Fonction loop()
//
// Input:
//    - Aucun
// Output:
//    - Aucun
// Objectif:
//    - Maintenir la connexion avec Adafruit IO
//    - Envoyer l'UID du badge RFID lu au serveur Adafruit IO
//========================
void loop() {
  // Vérifier si la connexion avec Adafruit IO est active
  if (!mqtt.ping(3)) {  // Ping pour vérifier la connexion
    if (!mqtt.connected())  // Si la connexion est perdue, reconnecter
      connecter();
  }

  // Lire les données série (RFID UID) envoyées
  if (Serial.available() > 0) {
    rfidUID = Serial.readStringUntil('\n');  // Lire l'UID RFID depuis la console série
    rfidUID.trim();  // Supprimer les espaces et les caractères de fin de ligne

    // Publier l'UID sur le flux Adafruit IO
    if (!Presence.publish(rfidUID.c_str())) {
      Serial.println("Erreur en publiant ID");  // En cas d'erreur de publication
    } else {
      Serial.println("ID publié avec succès !");  // Afficher un message si la publication est réussie
    }
  }
}
