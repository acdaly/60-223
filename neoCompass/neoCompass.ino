/* Author = helscream (Omer Ikram ul Haq)
Last edit date = 2014-06-22
Website: http://hobbylogs.me.pn/?p=17
Location: Pakistan
Ver: 0.1 beta --- Start
Ver: 0.2 beta --- Debug feature included
*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            0

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      24

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGBW + NEO_KHZ800);

#include <Wire.h>
#include "compass.h"
#include <math.h>
# define PI 3.14159
#define Task_t 10          // Task Time in milli seconds

int dt=0;
unsigned long t;

int pixel;


int servoPos = 0;    // variable to store the servo position

// Test Code

// Main code -----------------------------------------------------------------


void setup(){
  Serial.begin(9600);
  // Serial.print("Setting up I2C ........\n");
  Wire.begin();
  compass_x_offset = 122.17;
  compass_y_offset = 230.08;
  compass_z_offset = 389.85;
  compass_x_gainError = 1.12;
  compass_y_gainError = 1.13;
  compass_z_gainError = 1.03;
  
  
  
  compass_init(2);
  compass_debug = 1;
  compass_offset_calibration(3);

/*
  String nomz = "LightBlueBean";
  Bean.setBeanName(nomz);
  Bean.enableWakeOnConnect(true);
*/

 // Servo
  pixels.begin();

}

// Test F(x)

  float tests[][5] = {

    // Unit Tests
    {  0 ,  0 ,  0 ,  1 ,  0  },
    
    {  0 ,  0 ,  -1 ,  1 ,  360 - 45  },

    {  0 ,  0 ,   1 ,  1 ,        45  },

    {  0 ,  0 ,   1 , -1 ,  90  + 45  },

    {  0 ,  0 ,  -1 , -1 ,  180 + 45  },

    
    // Case A
    {  0 ,  0 ,  -4 , 2 ,  270 + 26.57  },

    {  0 ,  0 ,  -2 , 4 ,  270 + 63.43  },

    // Case B
    {  0 ,  0 ,  2 , 4 ,   26.57  },

    {  0 ,  0 ,  4 , 2 ,   63.43  },

    // Case C
    {  0 ,  0 ,  4 , -2 ,  90 + 26.57 },

    {  0 ,  0 ,  2 , -4 ,  90 + 63.43 },

    // Case D
    {  0 ,  0 ,  -2 , -4 , 180 + 26.57  },

    {  0 ,  0 ,  -4 , -2 , 180 + 63.43  }
    

  };

 
// Main loop -----------------------------------------------------------------
void loop(){
  
  t = millis();
 
  float load;
 
  compass_scalled_reading();
  /*
  Serial.print("x = ");
  Serial.println(compass_x_scalled);
  Serial.print("y = ");
  Serial.println(compass_y_scalled);
  Serial.print("z = ");
  Serial.println(compass_z_scalled);
  */

  compass_heading();
  Serial.print ("Heading angle = ");
  Serial.print (bearing);
  Serial.println(" Degree");
  
  dt = millis()-t;
  load = (float)dt/(Task_t/100);
  //Serial.print ("Load on processor = ");
  //Serial.print(load);
  //Serial.println("%");

// END COPIED CODE


  // Run Connection Dependent Code (Scratch Tx)
  
//  bool connected = Bean.getConnectionState();

  //if(connected) {
    // Write current temperature to a scratch data area.
    //uint8_t temperatureBuffer[1];
    //temperatureBuffer[0] = Bean.getTemperature();
    //Bean.setScratchData(temperatureScratch, temperatureBuffer, 1);

    // Update LEDs
    //ScratchData receivedData = Bean.readScratchData(ledScratch);

    //uint8_t redLed = receivedData.data[0];
    //uint8_t greenLed = receivedData.data[1];
    //uint8_t blueLed = receivedData.data[2];

    //Bean.setLed(redLed, greenLed, blueLed);


    // BEGIN Scratch Tx
    
    
    // Read GPS From Scratch
  //float lat = Bean.readScratchData(3);
  //float lon = Bean.readScratchData(4);

  // serial print GPS

  // Read Mag Field
//  Bean.setScratchNumber(2, bearing);



  // Begin Compass Algo

  float GPS[5] =     {  2 ,  3 ,  1 ,  4 ,  0  };
  //load test data
  float deltaY = (GPS[1]-GPS[3]);
  float deltaX = (GPS[0]-GPS[2]);

  float theta1 = atan2(deltaY,deltaX)*180/PI;
  float theta2 = bearing;
  
  float phi = theta2-theta1;
  phi = int(phi) % 360;
  

  pixel = (24 - (phi/15));
  for (int i = 0; i < 24; i++) {
     if (i == pixel) {pixels.setPixelColor(i, pixels.Color(50,0,0));
     } // Moderately bright green color.
     else {
      pixels.setPixelColor(i, pixels.Color(0,0,0));
     }
     pixels.show();
      
  }
  
  Serial.println(pixel);
/*
  if (( (phi > -180) && (phi < -90) ) | ( (phi > 180) && (phi < 270) )){servoPos = 0;Serial.print("Case D - Deg: ");Serial.println(phi);}
  // Case D
  else if ( ( (phi > -270) && (phi < -180) ) | ( (phi > 90) && (phi < 180) )){servoPos = 180;Serial.print("Case C - Deg: ");Serial.println(phi);}
  // Case C

  else if ( (phi > -90) && (phi < 90) ){servoPos = int(phi)+90;Serial.print("Live Case - Deg: ");Serial.println(phi);}
  else if ( (phi > 270) && (phi < 450) ){servoPos = int(phi)-270;Serial.print("Live Case - Deg: ");Serial.println(phi);}
  // Cases A/B
*/
  
  // servoPos = 2*((phi > -90)&&(phi < 90)) ? phi : ((phi>0)?90:-90);
  Serial.print ("Phi = ");
  Serial.println(phi);
  

    
  //}
  /*
  else {
    // Turn LED off and put to sleep.
    Bean.setLed(0, 0, 0);
    Bean.sleep(0xFFFFFFFF);
  }
  */
  delay(15);
  
  
}

