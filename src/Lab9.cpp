/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "c:/Users/USER/Desktop/Lab9/src/Lab9.ino"
void setup();
void printToDisplay(String output);
void loop();
#line 1 "c:/Users/USER/Desktop/Lab9/src/Lab9.ino"
SYSTEM_THREAD(ENABLED);
#include <Wire.h>
#include "oled-wing-adafruit.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <blynk.h>

VCNL4040 sensor;
OledWingAdafruit display;

String mode = "init";
uint16_t lowCal;
uint16_t highCal;
int lowCalibrated;
int highCalibrated;
bool isLight;
bool isTemp;
uint16_t prevValue;

void setup()
{
  // Lights
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  
  // Switches and button and thermometer
  pinMode(D2, INPUT);
  pinMode(A5, INPUT);
  pinMode(A4, INPUT);

  //join i2c bus
  Wire.begin();

  //set up display
  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Turn the potentiometer to calibrate the ambient light values!");
  display.display();

  //check sensor
  if (sensor.begin() == false)
  {
    Serial.println("Device not found. Please check wiring.");
    while (1); //Freeze!
  }

  sensor.powerOffProximity();
  sensor.powerOnAmbient();

  digitalWrite(D4, LOW);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);

  //init blynk
  Blynk.begin("cnjXDn_t7vcs3I-PR-jbSobGd__j2qM9", IPAddress(167, 172, 234, 162), 9090);
  Blynk.run();

  prevValue = sensor.getAmbient();

  int lowCalibrated = 500;
  int highCalibrated = 1500;
  isLight = false;
  isTemp = false;
}

void printToDisplay(String output)
{
  //reset display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

      //output to display
  display.println(output);
  display.display();
}

BLYNK_WRITE(V1)
{   
  int pinValue = param.asInt();
  digitalWrite(D4, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  delay(2000);
  
}

void loop()
{
  //loop
  display.loop();
  if (digitalRead(A4)== HIGH) {//this means that we are pressing button 1 down}
  int highCalibrated = 1500;
Serial.println("button 1 id pressed...");


  
  
  

    //change modes mode
    if (display.pressedA())
    {
      isLight = true;
      isTemp = false;
      //Blynk.virtualWrite(V1, 0);
    }
    if (display.pressedB())
    {
      isLight = false;
      isTemp = true;
      //Blynk.virtualWrite(V1, 0);
    }


    //ambient light mode
    if (isLight)
    {
      //turn on sensor and read value
      sensor.powerOnAmbient();
      uint16_t ambiValue = sensor.getAmbient();

      //format string
      String output = ""; 
      //change light & notify if needed
      if (ambiValue < lowCal)
      {
        digitalWrite(D4, HIGH);
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);

        //check if changed from previous bounds and notify
        if (!(prevValue < lowCal))
        {
          Blynk.notify("Light value dipped below lower bound!");
          output += "Light value has dipped below lower bound!";
        }

      }
      else if (ambiValue < highCal)
      {
        digitalWrite(D4, LOW);
        digitalWrite(D5, HIGH);
        digitalWrite(D6, LOW);

        //check if changed from previous bounds and notify
        if (!((prevValue > lowCal) && (prevValue < highCal)))
        {
          Blynk.notify("Light value has reached target range!");
          output += "Light value has reached target range!";
        }
      }
      else if (ambiValue > highCal)
      {
        digitalWrite(D4, LOW);
        digitalWrite(D5, LOW);
        digitalWrite(D6, HIGH);

        //check if changed from previous bounds and notify
        if (!((prevValue > highCal)))
        {
          Blynk.notify("Light value has passed higher bound!");
          output += "Light value has passed higher bound!";
        }
      }


      output += "\nAmbient Value: ";
      output += (String) ambiValue;

      //output to display
      printToDisplay(output);

      prevValue = ambiValue;
    }

    //print proximity value when button A pressed
    if (isTemp)
    {
      digitalWrite(D4, LOW);
      digitalWrite(D5, LOW);
      digitalWrite(D6, LOW);

      uint16_t reading = analogRead(A4);
      double voltage = (reading * 3.3) / 4095.0;
      double temperature = (voltage - 0.5) * 100;
      String output = "";
        
      //set up output string with celsius and fahrenheit
      output += "Celsius: ";
      output += (String) temperature;
      output += "\n";
      output += "Fahrenheit: ";
      output += (String) ((temperature*9/5)+32);

      //print to display
      printToDisplay(output);

      Blynk.virtualWrite(V0, temperature);

    }

   

  

  //digitalWrite(D4, LOW);
  //digitalWrite(D5, LOW);
  //digitalWrite(D6, LOW);
    
    //get values
    uint16_t value = analogRead(A5); 
    Serial.println(value);
  // because my potentimeter is not working I am hard coding it
  int lightlevel = 1600;
    //set values
    if (lightlevel < lowCalibrated)
    {
      digitalWrite(D5, HIGH);
      digitalWrite(D4, LOW);
      digitalWrite(D6, LOW);
    }
    
    else if (lightlevel > highCalibrated)
    {
  digitalWrite(D4, HIGH);
  digitalWrite(D5, LOW);
  digitalWrite(D6, LOW);
    }
    else if (lightlevel > lowCalibrated && lightlevel < highCalibrated)
    {
    digitalWrite(D4, LOW);
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
    }
  }

     
    
}
