#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             //String to store app command state.
int speedA = 255;  // 0 to 255
int speedB = 255;
int speedCar = 255; //255+255
void speed(int s){
  speedA= s;
  speedB= s;
}
const char* ssid = "NodeMCU Car";
const char* pswd = "12345678";
ESP8266WebServer server(80);

void setup() {

  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(ENB, OUTPUT);

  delay(1000);
  Serial.println(">> Setup");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, pswd);  //Start HOTspot removing password will disable security

  IPAddress myIP = WiFi.softAPIP();  //Get IP address
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void forword() {  
  speed(speedCar);           //forword
  digitalWrite(IN_1, HIGH);  //Right Motor forword Pin
  digitalWrite(IN_2, LOW);   //Right Motor backword Pin
  digitalWrite(IN_3, LOW);   //Left Motor backword Pin
  digitalWrite(IN_4, HIGH);  //Left Motor forword Pin
}

void rightforword() {  
  speedB=speedCar+(speedCar*(0.5));
  speedA=510 - speedB;
  digitalWrite(IN_1, HIGH);  //Right Motor forword Pin
  digitalWrite(IN_2, LOW);   //Right Motor backword Pin
  digitalWrite(IN_3, LOW);   //Left Motor backword Pin
  digitalWrite(IN_4, HIGH);  //Left Motor forword Pin
}

void leftforword() {  
  speedA=speedCar+(speedCar*(0.5));
  speedB=510 - speedA;
  digitalWrite(IN_1, HIGH);  //Right Motor forword Pin
  digitalWrite(IN_2, LOW);   //Right Motor backword Pin
  digitalWrite(IN_3, LOW);   //Left Motor backword Pin
  digitalWrite(IN_4, HIGH);  //Left Motor forword Pin
}

void backword() {            //backword
  speed(speedCar); 
  digitalWrite(IN_1, LOW);   //Right Motor forword Pin
  digitalWrite(IN_2, HIGH);  //Right Motor backword Pin
  digitalWrite(IN_3, HIGH);  //Left Motor backword Pin
  digitalWrite(IN_4, LOW);   //Left Motor forword Pin
}

void leftbackword() {            //backword
  speedB=speedCar+(speedCar*(0.5));
  speedA=510 - speedB;
  digitalWrite(IN_1, LOW);   //Right Motor forword Pin
  digitalWrite(IN_2, HIGH);  //Right Motor backword Pin
  digitalWrite(IN_3, HIGH);  //Left Motor backword Pin
  digitalWrite(IN_4, LOW);   //Left Motor forword Pin
}

void rightbackword() {            //backword
  speedA=speedCar+(speedCar*(0.5));
  speedB=510 - speedA;
  digitalWrite(IN_1, LOW);   //Right Motor forword Pin
  digitalWrite(IN_2, HIGH);  //Right Motor backword Pin
  digitalWrite(IN_3, HIGH);  //Left Motor backword Pin
  digitalWrite(IN_4, LOW);   //Left Motor forword Pin
}

void turnRight() {           //turnRight
  speed(speedCar); 
  digitalWrite(IN_1, LOW);   //Right Motor forword Pin
  digitalWrite(IN_2, HIGH);  //Right Motor backword Pin
  digitalWrite(IN_3, LOW);   //Left Motor backword Pin
  digitalWrite(IN_4, HIGH);  //Left Motor forword Pin
}

void turnLeft() {            //turnLeft
  speed(speedCar); 
  digitalWrite(IN_1, HIGH);  //Right Motor forword Pin
  digitalWrite(IN_2, LOW);   //Right Motor backword Pin
  digitalWrite(IN_3, HIGH);  //Left Motor backword Pin
  digitalWrite(IN_4, LOW);   //Left Motor forword Pin
}

void Stop() {               //stop
  digitalWrite(IN_1, LOW);  //Right Motor forword Pin
  digitalWrite(IN_2, LOW);  //Right Motor backword Pin
  digitalWrite(IN_3, LOW);  //Left Motor backword Pin
  digitalWrite(IN_4, LOW);  //Left Motor forword Pin
}

void loop() {
    server.handleClient();

  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

      command = server.arg("State");
      if (command == "F") forword();
      else if (command == "B") backword();
      else if (command == "L") turnLeft();
      else if (command == "R") turnRight();
      else if (command == "I") rightforword();
      else if (command == "G") leftforword();
      else if (command == "J") leftbackword();
      else if (command == "H") rightbackword();
      else if (command == "9") speedCar=111; //12
      else if (command == "8") speedCar=127; //39
      else if (command == "7") speedCar=143; //66
      else if (command == "6") speedCar=159; //93
      else if (command == "5") speedCar=175; //120
      else if (command == "4") speedCar=191; //147
      else if (command == "3") speedCar=207; //175
      else if (command == "2") speedCar=223; //201
      else if (command == "1") speedCar=239; //228
      else if (command == "0") speedCar=255; //255
      else if (command == "S") Stop();
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
