#include "oled-wing-adafruit.h"
#include "SparkFun_VCNL4040_Arduino_Library.h"
#include <blynk.h>
/*
 * Project Lab-9
 * Description:
 * Author:
 * Date:
 */
SYSTEM_THREAD(ENABLED);  
int low = INT_MAX;
int high = INT_MAX;
int state = 1;
bool on = false;
OledWingAdafruit display;
VCNL4040 light;
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(D8, INPUT);
  pinMode(A2, INPUT);
  pinMode(D7, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D0, INPUT);
  display.setup();
  display.clearDisplay();
  display.display();  // cause the display to be updated
  Wire.begin();
  Blynk.begin("hlr-DuRj4ciMPWwhCiRE11ubo6uQHdl_", IPAddress(167, 172, 234, 162), 9090);

}
BLYNK_WRITE(V0)  // button attached to Virtual Pin 1 in SWITCH mode
{
  int i = param.asInt();
  if(i == 1){
    on = true;  
  }
  else{
    on = false; 
  }
}

void loop() {
  Blynk.run();
  display.clearDisplay();
  display.loop();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  // The core of your code will likely live here.
  
  double voltage = (analogRead(A1) * 3.3) / 4095.0;
  double temperature = (voltage - 0.5) * 100;
  Blynk.virtualWrite(V1, temperature);
  if(!on){
    int potentiometer = analogRead(A0);
    if(display.pressedA()){state = 1;}
    if(display.pressedB()){state = 2;}
    if(state == 1){
      if(low == INT_MAX){
        display.print("Your low value of light will be ");
        display.println(potentiometer);
        display.display();
        delay(10);
        if(digitalRead(D8)) {
          low = potentiometer;
          delay(1000);
        }
      }
      else if(high == INT_MAX){
        display.print("Your high value of light will be ");
        display.println(potentiometer);
        display.display();
        delay(10);
        if(digitalRead(D8)){
          high = potentiometer;
          delay(1000);
        }
      }
      else if(analogRead(A2) > high){
        digitalWrite(D5, LOW);
        digitalWrite(D6, LOW);
        digitalWrite(D7, HIGH);
        Blynk.notify("BLUE!");
      }
      else if(analogRead(A2) < low){
        digitalWrite(D5, HIGH);
        digitalWrite(D6, LOW);
        digitalWrite(D7, LOW);
        Blynk.notify("GREEN!");
      }
      else{
        digitalWrite(D5, LOW);
        digitalWrite(D6, HIGH);
        digitalWrite(D7, LOW);
        Blynk.notify("RED!");
      }
    }
    else{

    display.print("The temperature is ");
    display.print((int)(temperature+0.5));
    display.println(" in Celsius");
    display.print("The temperature is ");
    display.print((int)(temperature*9/5 + 32 + 0.5));
    display.println(" in Farenheit");
    display.display();
    delay(1000);
    }
  }
}