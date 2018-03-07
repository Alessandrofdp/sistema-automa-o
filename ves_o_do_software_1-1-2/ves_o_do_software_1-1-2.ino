/*
   programa controle de tv sansumg

   Autor: >> Alessandro Fernandes Pinho

   Data: >> 13/03/2016

   vesão do software: >> 1.0.1



  Data: >> 21/03/2016
  vesão do software: >> 1.0.2

  novos recursos rct como alarme
  quatro de alta performaçe ao lado da porta



*/


void funcaodelay(){
  
 delay(75); 
  }



#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include "ds3231.h"
#include <Ethernet.h>
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 10);
EthernetServer server(8090);



///relogio

#define BUFF_MAX 256

uint8_t wake_HOUR = 05;
uint8_t wake_MINUTE = 00;
uint8_t wake_SECOND = 05;

unsigned long prev = 5000, interval = 5000;



//sensor de temperatura


#define DHTPIN A15 // pino que estamos conectado
#define DHTTYPE DHT11 // DHT 11

// Conecte pino 1 do sensor (esquerda) ao +5V
// Conecte pino 2 do sensor ao pino de dados definido em seu Arduino
// Conecte pino 4 do sensor ao GND
// Conecte o resistor de 10K entre pin 2 (dados)
// e ao pino 1 (VCC) do sensor
DHT dht(DHTPIN, DHTTYPE);
float temperatura = dht.readTemperature();





int IRledPin =  18;   //ir controle
int led1 = 22;
int led2 = 24;
int led3 = 26;
int botao = 30;
int botao2 = 32;
//int botao3 = 34;
int estado = 0;
int estado2 = 0;
//int estado3=0;

//variaveis que controla estado de botoes
String readString = String(30);
String statusled;
String readString1 = String(30);
String statusTemperatura;



void setup()
{
  Serial.begin(9600);
  Ethernet.begin(mac, ip);


  pinMode(IRledPin, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(botao, INPUT);
  pinMode(botao2, INPUT);

  ///parametros relogio

  Wire.begin();
  DS3231_init(DS3231_INTCN);
  DS3231_clear_a1f();

  
  set_alarm();

  //sensor de temperatura
  dht.begin();

}

void loop()
{
  char buff[BUFF_MAX];
  unsigned long now = millis();
  struct ts t;

  // once a while show what is going on
  if ((now - prev > interval) && (Serial.available() <= 0)) {
    DS3231_get(&t);

    // display current time
    snprintf(buff, BUFF_MAX, "Data: %d.%02d.%02d\nHoras: %02d:%02d:%02d\n",
             t.mday,
             t.mon,
             t.year,

             t.hour,
             t.min,
             t.sec);
    Serial.println(buff);










    // display a1 debug info
    DS3231_get_a1(&buff[0], 59);
    Serial.println(buff);

    if (DS3231_triggered_a1()) {

      digitalWrite(led2, HIGH);
      digitalWrite(led1, HIGH);

      DS3231_clear_a1f();
    }
    prev = now;
  }






//temperatura=dht.readTemperature();

  // Serial.println(temperatura);


  char c = Serial.read();

  estado = digitalRead(botao);
  estado2 = digitalRead(botao2);
  // estado3 = digitalRead(botao3);


 funcaodelay();

  if (estado == HIGH) {
    digitalWrite(led1, !digitalRead(led1));
  }

 funcaodelay();
  if (estado2 == HIGH) {
    digitalWrite(led2, !digitalRead(led2));
  }

  /*
     trecho do codigo conexão com a internet



  */



  EthernetClient client = server.available();
  if (client)
  {
    while (client.connected())
    {
      if (client.available())
      {
        char c = client.read();

        if (readString.length() < 30) {
          readString += (c);
        }
        if (c == '\n')
        {





          if (readString.indexOf("led1") > 0 ) {
            digitalWrite(led1, !digitalRead(led1));
          }

          if (readString.indexOf("led2") > 0 ) {
            digitalWrite(led2, !digitalRead(led2));
          }

          if (readString.indexOf("led3") > 0 ) {
            digitalWrite(led3, !digitalRead(led3));
          }


          /*

             novos controle



          */

          if (readString.indexOf("liga") > 0 ) {


            SendChannelUpCode1();
            delay(500);

          }


          if (readString.indexOf("Volumemais") > 0 ) {
            SendChannelUpCode2();
            delay(500);

          }


          if (readString.indexOf("volumemenos") > 0 ) {
            SendChannelUpCode3();
            delay(500);

          }


          if (readString.indexOf("pchdmi") > 0 ) {
            SendChannelUpCode4();
            delay(500);

          }


          if (readString.indexOf("canalmais") > 0 ) {
            SendChannelUpCode5();
            delay(500);

          }


          if (readString.indexOf("canalmenos") > 0 ) {
            SendChannelUpCode6();
            delay(500);
          }





       //   if (readString.indexOf("Temperatura") > 0 ) {

           

         //  statusTemperatura=temperatura;

   //       }







          Serial.println(readString);
        


          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Refresh: 5");
          client.println();

           temperatura=dht.readTemperature();
          statusTemperatura=temperatura;
                

          client.println("<form action=\"Temperatura\" method=\"get\">");
          client.println("<button" +statusTemperatura+"</button>");
          client.println("</form> <br />");

             
            readString1 = "";    


        
          
          client.println("<!doctype html>");
          client.println("<html>");
          client.println("<head>");
          client.println("<title>Tutorial</title>");
          client.println("<meta name=\"viewport\" content=\"width=320\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width\">");
          client.println("<meta charset=\"utf-8\">");
          client.println("<meta name=\"viewport\" content=\"initial-scale=1.0, user-scalable=no\">");
          client.println("</head>");
          client.println("<body>");
          client.println("<center>");
          client.println("<font size=\"5\" face=\"verdana\" color=\"green\">Android</font>");
          client.println("<font size=\"3\" face=\"verdana\" color=\"red\"> & </font>");
          client.println("<font size=\"5\" face=\"verdana\" color=\"blue\">Arduino</font><br />");




          if (digitalRead(led1)) {
            statusled = "ligado";
          } else {
            statusled = "Desligado";
          }

          client.println("<form action=\"led1\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Led 1 - " + statusled + "</button>");
          client.println("</form> <br />");

          if (digitalRead(led2)) {
            statusled = "ligado";
          } else {
            statusled = "Desligado";
          }

          client.println("<form action=\"led2\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Led 2 - " + statusled + "</button>");
          client.println("</form> <br />");

          if (digitalRead(led3)) {
            statusled = "ligado";
          } else {
            statusled = "Desligado";
          }

          client.println("<form action=\"led3\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Led 3 - " + statusled + "</button>");
          client.println("</form> <br />");


          /*

             novos recursos



          */
          //          if (digitalRead(IRledPin)) {
          //         SendChannelUpCode1();
          //      }


          client.println("<form action=\"liga\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">liga  TV""</button>");
          client.println("</form> <br />");


          client.println("<form action=\"Volumemais\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Volume +""</button>");
          client.println("</form> <br />");


          client.println("<form action=\"volumemenos\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Volume -""</button>");
          client.println("</form> <br />");

          client.println("<form action=\"pchdmi\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">PC-HDMI""</button>");
          client.println("</form> <br />");


          client.println("<form action=\"canalmais\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Canal + ""</button>");
          client.println("</form> <br />");

          client.println("<form action=\"canalmenos\" method=\"get\">");
          client.println("<button type=submit style=\"width:200px;\">Canal -" "</button>");
          client.println("</form> <br />");


      //    if (statusled){
                
        //        statusled=temperatura;
                
      //          }

          

         // client.println("<form action=\"Temperatura\" method=\"get\">");
          //client.println("<button  type=submit style=\"width:200px;\">Temperatura: "+ statusled+"°C</button>");
          //client.println("</form> <br />");


           // client.println("<P>");

           
          //   client.println("<form action=\"Hora\" method=\"get\">");
          //  client.println("<button type=submit style=\"width:200px;\">Hora " + statusRelogio + "</button>");
          // client.println("</form> <br />");






          client.println("</center>");
          client.println("</body>");
          client.println("</html>");
          readString = "";

          client.stop();

        }



      }

    }
  }






}



