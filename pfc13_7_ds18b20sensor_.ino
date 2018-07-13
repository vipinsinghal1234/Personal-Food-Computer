#include <ESP8266WiFi.h>   //library for nodemcu
#include <OneWire.h>      //library for ds18b20
#include <DallasTemperature.h>   ////library for ds18b20
 
const char* ssid = "hello";
const char* password = "1selekr8";

#define ONE_WIRE_BUS 2 // Data wire is plugged into pin 2 on the nodemcu
OneWire oneWire(ONE_WIRE_BUS);// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire);// Pass our oneWire reference to Dallas Temperature. 

unsigned long prev_time = 0;

WiFiServer server(80);

int fanval = LOW;
int pumpval = LOW;
int lightval = LOW;
int tmp = 24;
float watertmp[50];
int i=0;

void setup() {
  sensors.begin();
  Serial.begin(9600);
  delay(10);

 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}
 
void loop() {
  // Check if a client has connected
 WiFiClient client = server.available();

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  water_tmp(client);
  
  client.println("</html>");
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}

void water_tmp(WiFiClient client)
   {
    unsigned long current_time=millis();
    if(current_time-prev_time>2000)
  {
    sensors.requestTemperatures();      // Send the command to get temperatures
    watertmp[i]=sensors.getTempCByIndex(0); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
    i=i+1;
    prev_time=current_time;
    Serial.println(watertmp[i-1]);
  }
    client.print("<p>");
    client.print("Water Temprature: ");
    client.print("<br>");
    int j=0;
    for(j=0;j<i;j++)
  {
    client.print(watertmp[j]);
    client.print("<br>");
  }
    client.print("</p>"); 
  }

