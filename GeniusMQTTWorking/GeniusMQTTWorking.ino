#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "XXXXX";  //set a ssid
const char* password = "XXXX"; //set a wifi password
const char *mqtt_server = "XXXXXxx"; //insert server
const int mqtt_port = xxxx; //insert port
const char *mqtt_user = "xxxxxx"; //insert username
const char *mqtt_pass = "xxxxxxxxx"; //insert pass
const char *mqtt_client_name = "xxxxxxxxx"; //insert connection name
#define BUFFER_SIZE 100
unsigned long previousMillis = 0;
const long interval = 10000;
WiFiClient wclient;  //Declares a WifiClient Object using ESP8266WiFi
PubSubClient client(wclient);  //instanciates client object

static String letras = "";
static String resposta = "";

//Function is called when, a message is recieved in the MQTT server.
void callback(char* topic, byte* payload, unsigned int length) {

  for (int i = 0; i < length; i++) {
    resposta += (char)payload[i];
  }
  Serial.println("Resposta Recebida: " + resposta);

  if(resposta == "54687") {
    randomizarLetra();
    resposta = "";
    
  }else {
    Jogar();
    }

}


WiFiServer server(80);

long randNumber;

int r = 12; //D6 PIN
int w = 13; //D7 PIN
int g = 15; //D8 PIN
int y = 14; //D5 PIN
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(r, OUTPUT);
  pinMode(w, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(y, OUTPUT);

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
}


void Jogar () {
  if(lerString(resposta)) {
    randomizarLetra();
  }else {
    fimErrado();
    letras = "";
  }
  resposta = "";
}


boolean lerString(String entrada){ //desmanchar string em letras e tocar segundo elas

    if(entrada == letras){
      return true;
      //fimCerto();
    }
    else{
      return false;
      //fimErrado();
     
    }
}

void piscar(){

   //char verificar[0] = letras;
   int tamanho = letras.length();

   if(tamanho >= 10) {
    fimCerto();
   } else {
    
   Serial.println(letras + " " + tamanho);
   for(int i =0; i <= tamanho; i++){
    Serial.println("letra:" + letras[i]);
    if(letras[i] == 'Y'){
      digitalWrite(y, HIGH);
      Serial.println("Y");
      delay(500);
      digitalWrite(y, LOW);
      delay(500);
      
    }else if(letras[i] == 'R'){
      digitalWrite(r, HIGH);
      Serial.println("R");
      delay(500);
      digitalWrite(r, LOW);
      delay(500);
      
    } else if(letras[i] == 'W'){
      digitalWrite(w, HIGH);
      Serial.println("W");
      delay(500);
      digitalWrite(w, LOW);
      delay(500);
      
    } else if(letras[i] == 'G'){
      digitalWrite(g, HIGH);
      Serial.println("G");
      delay(500);
      digitalWrite(g, LOW);
      delay(500);
      
    }
   }
  
  }
}

void randomizarLetra(){ //OK

      //char sequencia[15]; 
      
      randNumber = random(0, 4);
      Serial.println("randNumber:");
      Serial.println(randNumber);

      if(randNumber == 0) {
         letras += 'Y';
      } else if(randNumber == 1) {
        letras += 'R';
      } else if (randNumber == 2) {
        letras += 'W';
      } else if (randNumber == 3) {
        letras += 'G';
      }
      piscar();
}
void fimCerto(){ //OK

   digitalWrite(r, HIGH);
   digitalWrite(w, HIGH);
   digitalWrite(y, HIGH);  
   digitalWrite(g, HIGH);
   delay(1000);                      
   digitalWrite(r, LOW);
   digitalWrite(w, LOW);
   digitalWrite(y, LOW);  
   digitalWrite(g, LOW);    
   delay(1000);

//Zerar string
letras = "";

   Serial.println("Parabens, voce acertou");

}
void fimErrado(){ //OK

   digitalWrite(y, HIGH);
   delay(500);
   digitalWrite(y, LOW);
   delay(500);
   digitalWrite(r, HIGH);
   delay(500);
   digitalWrite(r, LOW);
   delay(500);
   digitalWrite(w, HIGH);
   delay(500);
   digitalWrite(w, LOW);
   delay(500);
   digitalWrite(g, HIGH);
   delay(500);
   digitalWrite(g, LOW);
   delay(500);
  
//Zerar string
   letras = "";
   
   Serial.println("Que pena, voce errou");

}




