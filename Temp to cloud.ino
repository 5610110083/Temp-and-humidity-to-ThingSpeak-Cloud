
// Author by : siczones

// **Your need to setup  <DHT.h>   (header) before use ! 
// 		Paste: folder "DHT-sensor-library" to Directory of ArduinoIDE    Ex: "C:\Program Files (x86)\Arduino\libraries"

// ** Please fill :	- ssid (Wi-Fi) name 
// 		- your_Wi-Fi_password
// 		- your_api-key
// 		- set Pin D1-D7 to LED output


#include "DHT.h"
#include <ESP8266WiFi.h>

#define DHTPIN 2        // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11  (AM2302)

const char* ssid     = "<ssid name>";
const char* password = "<your_Wi-Fi_password>";
/*String white = "D5";
String green = "D6";
String yellow = "D7";
String red = "D8";*/
DHT *dht;

void connectWifi();
void reconnectWifiIfLinkDown();
void initDht(DHT **dht, uint8_t pin, uint8_t dht_type);
void readDht(DHT *dht, float *temp, float *humid);
void uploadThingsSpeak(float t, float );

void setup() {
	// Set for LED status
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    pinMode(D8, OUTPUT);

    Serial.begin(115200);
    delay(10);
    connectWifi();
    initDht(&dht, DHTPIN, DHTTYPE);

}

void loop() {
    static float t_ds;
    static float t_dht;
    static float h_dht;

    readDht(dht, &t_dht, &h_dht);
    uploadThingsSpeak(t_dht, h_dht);
    //Serial.println(t_ds);
    delay(10 * 1000);

    reconnectWifiIfLinkDown();
}

void reconnectWifiIfLinkDown() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WIFI DISCONNECTED");
        connectWifi();
    }
}

void connectWifi() {
    Serial.println();
    Serial.println();
    Serial.println("Connecting to ");
    Serial.print(ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(D1, 0);
        digitalWrite(D2, 0); 
        digitalWrite(D3, 0); 
        delay(500);
        Serial.print(".");
        digitalWrite(D2, 1); 
        delay(500);
    }
    if (WiFi.status() == WL_CONNECTED) {
        digitalWrite(D2, 1); 
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void initDht(DHT **dht, uint8_t pin, uint8_t dht_type) {
    
    *dht = new DHT(pin, dht_type, 30);
    (*dht)->begin();
    
}




void uploadThingsSpeak(float t, float h) {
    static const char* host = "api.thingspeak.com";
    static const char* apiKey = "<your_api-key>";

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        digitalWrite(D3, LOW);
        digitalWrite(D1, HIGH); 
        return;
    }
    else{
        digitalWrite(D3, HIGH);
        digitalWrite(D1, LOW);
    }

    // We now create a URI for the request
    String url = "/update/";
    //  url += streamId;
    url += "?key=";
    url += apiKey;
    url += "&field1=";
    url += t;
    url += "&field2=";
    url += h;

    Serial.print("Requesting URL: ");  Serial.println(url);  Serial.println();
    // This will send the request to the serv
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +"Host: " + host + "\r\n" +"Connection: close\r\n\r\n");
    
}



void readDht(DHT *dht, float *temp, float *humid) {

    if (dht == NULL) {
       Serial.println("[dht11] is not initialised. please call initDht() first.");
        return;
    }

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht->readHumidity();

    // Read temperature as Celsius
    float t = dht->readTemperature();
    // Read temperature as Fahrenheit
    float f = dht->readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }


    
    Serial.print("Temperature: ");  Serial.print(t);  Serial.print(" *C \t");
    Serial.print("Humidity: ");     Serial.print(h);  Serial.print(" %  \t");
    if(t <= 10){
      digitalWrite(D8, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(D5, HIGH);
    }
    else if(t > 10 && t <= 20){
      digitalWrite(D8, LOW);
      digitalWrite(D7, LOW);
      digitalWrite(D6, HIGH);
      digitalWrite(D5, LOW);
    }
    else if(t > 20 && t <= 30){
      digitalWrite(D8, LOW);
      digitalWrite(D7, HIGH);
      digitalWrite(D6, LOW);
      digitalWrite(D5, LOW);
    }
    else if(t > 30){
      digitalWrite(D8, HIGH);
      digitalWrite(D7, LOW);
      digitalWrite(D6, LOW);
      digitalWrite(D5, LOW);
    }

    *temp = t;
    *humid = h;
    

}


