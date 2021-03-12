// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

// Definimos las variables a utilizar
uint8_t Hora;
char datos[8];
String Time;
String ValorR;
String ValorV;

// Creamos nuestros feeds y los conectamos con Adafruit
AdafruitIO_Feed *HoraFeed = io.feed("HoraPrueba");
AdafruitIO_Feed *RojaFeed = io.feed("LedR");
AdafruitIO_Feed *VerdeFeed = io.feed("LedV");
void setup() {

  // Empezar comunicacion serial 0 y 2
  Serial.begin(115200);
  Serial2.begin(9600,SERIAL_8N1,16,17);

  // wait for serial monitor to open
  while(! Serial);
  while(! Serial2);
  
  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  RojaFeed->onMessage(handleMessage);
  VerdeFeed->onMessage(handleMessage1);
  // Because Adafruit IO doesn't support the MQTT retain flag, we can use the
  // get() function to ask IO to resend the last value for this feed to just
  // this MQTT client after the io client is connected.
  RojaFeed->get();
  VerdeFeed->get();
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  Time = "Reloj "; //Inicia el string de tiempo como Reloj
  Serial2.write('M');
  Serial.println(Serial2.available());
  if(Serial2.available()>0){ //Solo entra si hay datos en el buffer serial
    
    
    
   /*
   Serial2.readBytesUntil(10, datos, 8); //Se toman en orden los valores del pic
   Time = Time + datos; //Se une en Time el string Reloj con los valores recibidos del pic
   */
   
   if (Serial2.read()==10);{
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   Time = Time + char(Serial2.read());
   }
  }
  
  //Codigo para enviar estados de piloto dependiendo de los valores de estados recibidos del adafruit
  if ((ValorR=="ON") & (ValorV=="ON")){
    Serial2.write('A');
   // Serial.println("A"); // Se imprimen en la terminal para verificar que si esta entrando al if
  }

  else if ((ValorR=="ON") & (ValorV=="OFF")){
     Serial2.write('B');
     //Serial.println("B");
  }

  else if ((ValorR=="OFF") & (ValorV=="ON")){
    Serial2.write('C');
    //Serial.println("C");
  }

  else if ((ValorR=="OFF") & (ValorV=="OFF")){
    Serial2.write('D');
    //Serial.println("D");
  }
  
  
  
  // save count to the 'counter' feed on Adafruit IO
 // Serial.print("sending -> ");
  
  HoraFeed->save(Time);

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  //Serial.print("received <- ");
 // Serial.println(data->value());
  ValorR = data->value();
}
void handleMessage1(AdafruitIO_Data *data) {
  //Serial.print("received <- ");
  //Serial.println(data->value());
  ValorV = data->value();
}
