#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Pedro";
const char* password = "deumaoito";
const char *mqtt_server = "m14.cloudmqtt.com"; //insert server
const int mqtt_port = 16941; //insert port
const char *mqtt_user = "gncjecus"; //insert username
const char *mqtt_pass = "EU56oW3t6R4r"; //insert pass
const char *mqtt_client_name = "arduinoclient1"; //insert connection name
#define BUFFER_SIZE 100
unsigned long previousMillis = 0;
const long interval = 10000;
WiFiClient wclient;  //Declares a WifiClient Object using ESP8266WiFi
PubSubClient client(wclient);  //instanciates client object

static String resposta = "";
int r = 12; //D6 PIN
int w = 13; //D7 PIN
int g = 15; //D8 PIN
int y = 14; //D5 PIN
int entradaSom = 4;
int valor = 0;
int rele = 5;




//Function is called when, a message is recieved in the MQTT server.
void callback(char* topic, byte* payload, unsigned int length) {

  for (int i = 0; i < length; i++) {
    resposta += (char)payload[i];
  }
  Serial.println("Resposta Recebida: " + resposta);

  if(resposta == "54687"){
    Serial.println("parou");
    resposta = "";
  }
  else if(resposta == "rled") {
    char responder[] = "led vemerlho foi acionado";
    client.publish("nodep", responder);
    digitalWrite(r,!digitalRead(r));
    resposta = "";
  }else if(resposta == "yled"){
    char responder[] = "led amarelo foi acionado";
    client.publish("nodep", responder);
    digitalWrite(y,!digitalRead(y));
    resposta = "";
  }else if(resposta == "gled"){
    char responder[] = "led verde foi acionado";
    client.publish("nodep", responder);
    digitalWrite(g,!digitalRead(g));
    resposta = "";
    
  }else if(resposta == "wled"){
    
    char responder[] = "led branco foi acionado";
    client.publish("nodep", responder);
    digitalWrite(w,!digitalRead(w));
    resposta = "";
    
  }else if(resposta == "rele"){
    char responder[] = "rele foi acionado";
    client.publish("nodep", responder);
    digitalWrite(rele,!digitalRead(rele));
    resposta = "";
    
  }else if(resposta == "todos"){
    char responder[] = "todos foram acionado";
    client.publish("nodep", responder);
    digitalWrite(rele,!digitalRead(rele));
    digitalWrite(r,!digitalRead(r));
    digitalWrite(y,!digitalRead(y));
    digitalWrite(w,!digitalRead(w));
    digitalWrite(g,!digitalRead(g));
    resposta = "";
    
  }
     
  
  
  

}


WiFiServer server(80);



void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(r, OUTPUT);
  pinMode(w, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(y, OUTPUT);
  pinMode(rele, OUTPUT);
  pinMode(entradaSom, INPUT);

  Serial.begin(9600);
  randomSeed(analogRead(0));

  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("Wifi connected");

  //Start the server

  server.begin();
  Serial.println("Server started");

  //Print the IP address
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {

  if (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("stringdeid", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // ... and resubscribe
      client.subscribe("node");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }

  client.loop();


    valor = digitalRead(entradaSom);
    Serial.println(valor);
    if(valor == 1){
      char str[12];
      sprintf(str, "%d", valor);
      client.publish("nodeS", str);
      
    }
    delay(200);
}






