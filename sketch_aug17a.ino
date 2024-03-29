/*
Webserver zum Steuern einer Zentralheizung mit Wasserkreislauf
 */

#include <SPI.h>
#include <Ethernet.h>
#include <MemoryFree.h>
const int led1 = 2;          //relay1
const int led2 = 3;          //relay2
const int led3 = 4;          //relay3
const int led4 = 5;          //relay4

void(* resetFunc) (void) = 0; //declare reset function @ address 0

unsigned long ZeitWasser=0;  //some global variables available anywhere in the program
unsigned long ZeitHeizung=0;
unsigned long ZeitPumpe=0;
bool WasserAn = false;
bool HeizungAn = false;
bool PumpeAn = false;

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEA };   // mac address
byte ip[] = { 192, 168, 0, 71 };                      // configurate il vostro IP a vostra scelta("192.168.1.89")
byte gateway[] = { 192, 168, 0, 253 };                   // internet access router
byte subnet[] = { 255, 255, 255, 0 };                  //subnet mask
EthernetServer server(80);                             //server port     
//String readString;

void setup() {
 // Apro la comunicazione seriale:
  //Serial.begin(9600);
   //while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
 // }
  //Imposto i pin come OUTPUT
  pinMode(led1, OUTPUT);
  digitalWrite(led1, HIGH);
  pinMode(led2, OUTPUT);
  digitalWrite(led2, HIGH);
  pinMode(led3, OUTPUT);
  digitalWrite(led3, HIGH);
  pinMode(led4, OUTPUT);

  

  // Inizio la comunicazione Ethernet con il server:
  Ethernet.begin(mac, ip, gateway, subnet);
  server.begin();
 // Serial.print("server is at ");
 // Serial.println(Ethernet.localIP());
}


void loop() {
  String readString;
  unsigned long AktuelleZeit=0;
  int AnFurWasser = 0;
  int AnFurHeizung = 0;
  int AnFurPumpe = 0;
  int AnFurWasserSek = 0;
  int AnFurHeizungSek = 0;
  int AnFurPumpeSek = 0;    
  AktuelleZeit = millis();
  // Creo una connessione al client
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {   
      if (client.available()) {
        char c = client.read();
     
        //Leggo i caratteri da HTTP
        if (readString.length() < 100) {
          //Inserisco i caratteri nella stringa 
          readString += c;
          //Serial.print(c);
         }

         //Se la richiesta HTTP è andata a buon fine
         if (c == '\n') {          
           Serial.println(readString); //scrivi sul monitor seriale per debugging

           if(WasserAn){
            AnFurWasser = (ZeitWasser-AktuelleZeit)/60000;
            AnFurWasserSek = ((ZeitWasser-AktuelleZeit)-(AnFurWasser * 60000))/1000;
           }
           if(HeizungAn){
            AnFurHeizung = (ZeitHeizung-AktuelleZeit)/60000;
            AnFurHeizungSek = ((ZeitHeizung-AktuelleZeit)-(AnFurHeizung * 60000))/1000;
           }
           if(PumpeAn){
            AnFurWasser = (ZeitWasser-AktuelleZeit)/60000;
            AnFurWasserSek = ((ZeitWasser-AktuelleZeit)-(AnFurWasser * 60000))/1000;
           }
           client.println("HTTP/1.1 200 OK"); //Invio nuova pagina
           client.println("Content-Type: text/html");
           client.println();     
           client.println("<HTML>");
           client.println("<HEAD>");
           client.println("<meta name='apple-mobile-web-app-capable' content='yes' />");
           client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
           client.println("<link rel='stylesheet' type='text/css' href='http://www.progettiarduino.com/uploads/8/1/0/8/81088074/style3.css' />");
           client.println("<TITLE>Heizung & Warmwasser</TITLE>");
           client.println("</HEAD>");
           client.println("<BODY>");
           client.println("<H1>Regelung von Heizung & Warmwasser</H1>");
           client.println("<hr />");
           client.println("<br />");  
           // Anfang: Knöpfe auf Webserver für Wasser
           client.println("<a href=\"/?button1on\"\">Wasser einschalten</a>");          //Modifica a tuo piacimento:"Accendi LED 1"
           client.println("<a href=\"/?button1off\"\">Wasser ausschalten</a><br />");    //Modifica a tuo piacimento:"Spegni LED 1" 
           if(WasserAn){
            client.print("<p>Wasser noch an fur ");
            client.print(AnFurWasser);
            client.print(" Minuten, ");
            client.print(AnFurWasserSek);
            client.print(" Sekunden.");
            client.print("</p>");
            }
           else{client.println("<br />"); client.println("<br />"); client.println("<br />");}
           client.println("<br />");
           // Ende: Knöpfe auf Webserver für Wasser

           // Anfang: Knöpfe auf Webserver für Heizung
           client.println("<a href=\"/?button2on\"\">Heizung einschalten</a>");          //Modifica a tuo piacimento:"Accendi LED 2"
           client.println("<a href=\"/?button2off\"\">Heizung ausschalten</a><br />");    //Modifica a tuo piacimento:"Spegni LED 2"
           if(HeizungAn){
            client.print("<p>Heizung noch an fur ");
            client.print(AnFurHeizung);
            client.print(" Minuten, ");
            client.print(AnFurHeizungSek);
            client.print(" Sekunden.");
            client.print("</p>");
            }
           else{client.println("<br />"); client.println("<br />"); client.println("<br />");}
           client.println("<br />");
            // Ende: Knöpfe auf Webserver für Heizung

            // Anfang: Knöpfe auf Webserver für Pumpe
           client.println("<a href=\"/?button3on\"\">Pumpe einschalten</a>");          //Modifica a tuo piacimento:"Accendi LED 3"
           client.println("<a href=\"/?button3off\"\">Pumpe ausschalten</a><br />");    //Modifica a tuo piacimento:"Spegni LED 3"
           if(PumpeAn){
            client.print("<p>Pumpe noch an fur ");
            client.print(AnFurPumpe);
            client.print(" Minuten, ");
            client.print(AnFurPumpeSek);
            client.print(" Sekunden.");
            client.print("</p>");
            }
           else{client.println("<br />"); client.println("<br />"); client.println("<br />");}
           // Ende: Knöpfe auf Webserver für Pumpe
           //client.println("<a href=\"/?button3on\"\">Accendi LED 3</a>");          //Modifica a tuo piacimento:"Accendi LED 3"
           //client.println("<a href=\"/?button3off\"\">Spegni LED 3</a><br />");    //Modifica a tuo piacimento:"Spegni LED 3"
           //client.println("<br />");   
           //client.println("<br />");
           //client.println("<a href=\"/?button4on\"\">Accendi LED 4</a>");          //Modifica a tuo piacimento:"Accendi LED 4"
           //client.println("<a href=\"/?button4off\"\">Spegni LED 4</a><br />");    //Modifica a tuo piacimento:"Spegni LED 4"
           client.println("<br />");
           client.println("<a href=\"/\"\">F5</a>");      
           //client.println("<p>Creato da Salvatore Fancello. Visita http://progettiarduino.com per altri progetti!</p>");  
                                            //Sostieni il blog visita: http://www.progettiarduino.com/sostieni-il-blog.html
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("<br />"); 
           client.println("Freier Speicher:"); 
           client.println(freeMemory());
           client.println("<br />"); 
           client.println("An seit:");  
           client.println(AktuelleZeit/1000);
           client.println("</BODY>");
           client.println("</HTML>");
     
           delay(1);
           client.stop();
           //Controlli su Arduino: Se è stato premuto il pulsante sul webserver
           if (readString.indexOf("?button1on") >0){
               ZeitWasser = AktuelleZeit+900000;
           }
           if (readString.indexOf("?button1off") >0){
               ZeitWasser = AktuelleZeit;
           }
           if (readString.indexOf("?button2on") >0){
               ZeitHeizung = AktuelleZeit+900000;
           }
           if (readString.indexOf("?button2off") >0){
               ZeitHeizung = AktuelleZeit;
           }
           if (readString.indexOf("?button3on") >0){
               ZeitPumpe = AktuelleZeit+300000;
           }
           if (readString.indexOf("?button3off") >0){
               ZeitPumpe = AktuelleZeit;
           }
           if (readString.indexOf("?button4on") >0){
               digitalWrite(led4, HIGH);  
           }
           if (readString.indexOf("?button4off") >0){
               digitalWrite(led4, LOW);
           }


           if (ZeitWasser > AktuelleZeit){
              digitalWrite(led1, LOW);//Relai ist LOW-Active
              WasserAn = true;
           }
           else{
              digitalWrite(led1, HIGH);//Relai ist LOW-Active
              WasserAn = false;
           }
           if (ZeitHeizung > AktuelleZeit){
              digitalWrite(led2, LOW);//Relai ist LOW-Active
              HeizungAn = true;
           }
           else{
              digitalWrite(led2, HIGH);//Relai ist LOW-Active
              HeizungAn = false;
           }
           if (ZeitPumpe > AktuelleZeit){
              digitalWrite(led3, LOW);//Relai ist LOW-Active
              PumpeAn = true;
           }
           else{
              digitalWrite(led3, HIGH);//Relai ist LOW-Active
              PumpeAn = false;
           }
            //Cancella la stringa una volta letta
            readString="";  
           
         }
       }
    }
}
if (ZeitWasser <= AktuelleZeit){
  digitalWrite(led1, HIGH);//Relai ist LOW-Active
  WasserAn = false;
  }
if (ZeitHeizung <= AktuelleZeit){
  digitalWrite(led2, HIGH);//Relai ist LOW-Active
  HeizungAn = false;
  }
if (ZeitPumpe <= AktuelleZeit){
  digitalWrite(led3, HIGH);//Relai ist LOW-Active
  PumpeAn = false;
  }
if ((AktuelleZeit > 600000)&&(!WasserAn)&&(!HeizungAn)&&(!PumpeAn)){
    resetFunc();  //call reset
}

}
