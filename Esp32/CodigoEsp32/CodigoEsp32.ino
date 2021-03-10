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

// this int will hold the current count for our sketch
uint8_t Hora;
String Time;
AdafruitIO_Feed *HoraFeed = io.feed("HoraPrueba");
AdafruitIO_Feed *RojaFeed = io.feed("LedR");
AdafruitIO_Feed *VerdeFeed = io.feed("LedV");
void setup() {

  // start the serial connection
  Serial.begin(115200);
  Serial2.begin(9600,SERIAL_8N1,16,17);

  // wait for serial monitor to open
  while(! Serial);
  //while(! Serial2);
  
  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  RojaFeed->onMessage(handleMessage);
  VerdeFeed->onMessage(handleMessage);
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
  //while(Serial2.read()!=22);
  /*while (1){
    if(Serial2.available()>0){
  Serial.print(Serial2.read());
  Serial.print("\n");
    }
  }*/
  //if (RojaFeed == "ON")
//  Serial2.write(1);
  /*if(Serial2.available()>0){
    while(Serial2.read()!=10){
      Serial2.read();
      }
    Time = "Reloj ";
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
    Time = Time + char(Serial2.read());
//    Serial2.write(0);
  }*/
  
  
  
  
  // save count to the 'counter' feed on Adafruit IO
  Serial.print("sending -> ");
 // Serial.println("holis");
  HoraFeed->save(Time);

  // increment the count by 1
  Hora++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000);

}
// this function is called whenever a 'counter' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {

  Serial.print("received <- ");
  Serial.println(data->value());

}
