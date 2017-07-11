#include<String.h>
#include <SPI.h>
#include <Ethernet.h>
#include "DHT.h"

int relaypin1 = 2;
int relaypin2 = 3;
int relaypin3 = 4;
int flag=0;
int moisturepin=A0;
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
float h,t,f;

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//MAC address

IPAddress ip(192,168,137,20);//
EthernetServer server(80);


void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  pinMode(relaypin1, OUTPUT);
  pinMode(relaypin2, OUTPUT);
  pinMode(relaypin3, OUTPUT);
  pinMode(moisturepin, INPUT);
  digitalWrite(moisturepin,LOW);
  digitalWrite(relaypin1,LOW);
  digitalWrite(relaypin2,LOW);
  digitalWrite(relaypin3,LOW);
  dht.begin();
}

void loop()
{
  temperature();
  EthernetClient client;
  int moisture=analogRead(moisturepin);
  Serial.print("Moisture level:");
  Serial.println(moisture);
  String readString;
  client = server.available();
  if (client) 
  {
    while (client.connected()) 
    {   
      if (client.available()) 
      {
         char c = client.read(); 
         //read char by char HTTP request
         if (readString.length() < 100) 
         {
          //store characters to string
          readString += c;
         }   
         //if HTTP request has ended
         if (c == '\n') 
         {              
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println();  
              
           client.println("<HTML>");
           
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
           client.println("<TITLE>Arduino Project</TITLE>");
           client.println("</HEAD>");
           
           client.println("<BODY>");  
                  
           client.println("<br/>");  
           client.println("<H2>SMART FORM</H2>");
                              
           client.println("<br />");
           
           client.println("<b>TEMPERATURE:</b>");
           client.println(t,2);
           client.println("  'C");
           client.println("<br />");
           
           client.println("<b>MOISTURE LEVEL:</b>");
           client.println(moisture);
           
           client.println("<br />");
           
           client.println("<br />");  
           client.println("<a href=\"/?valve1on\"\">Turn On Valve1</a>");
           client.println("<a href=\"/?valve1off\"\">Turn Off Valve1</a><br />");   
           client.println("<br />");    

           client.println("<br />");  
           client.println("<a href=\"/?valve2on\"\">Turn On Valve2</a>");
           client.println("<a href=\"/?valve2off\"\">Turn Off Valve2</a><br />");   
           client.println("<br />");    

           client.println("<br />");  
           client.println("<a href=\"/?valve3on\"\">Turn On Valve3</a>");
           client.println("<a href=\"/?valve3off\"\">Turn Off Valve3</a><br />");   
           client.println("<br />");    
           
           client.println("</BODY>");

           client.println("<head>");
           client.println("<meta http-equiv='refresh' content='2'>");
           client.println("</head>");
           
           client.println("</HTML>");
     
           //delay(1);
           //stopping client
           client.stop();
           //controls the Arduino if you press the buttons
           
           if (readString.indexOf("?valve1on") >0)
             digitalWrite(relaypin1,HIGH);
           if (readString.indexOf("?valve1off") >0)
             digitalWrite(relaypin1,LOW);
             
           if (readString.indexOf("?valve2on") >0)
             digitalWrite(relaypin2,HIGH);
           if (readString.indexOf("?valve2off") >0)
             digitalWrite(relaypin2,LOW);  

           if (readString.indexOf("?valve3on") >0)
             digitalWrite(relaypin3,HIGH);
           if (readString.indexOf("?valve3off") >0)
             digitalWrite(relaypin3,LOW);                     
         }  
      } 
    }     
    readString="";            
  }
}

void temperature() 
{
  delay(2000);
  h = dht.readHumidity();
  // Read temperature as Celsius
  t = dht.readTemperature();
  // Read temperature as Fahrenheit
  f = dht.readTemperature(true);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hi);
  Serial.print(" *F\t");
}

