#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define redpin 4
#define greenpin 6
#define yellowpin 2
#define indicatorpin1 7
#define indicatorpin2 8

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
  Serial.begin(9600);
  digitalWrite(indicatorpin1, LOW);
  digitalWrite(indicatorpin2, LOW);

  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1); // halt!
  }
  pinMode(redpin, OUTPUT);
  pinMode(greenpin, OUTPUT);
  pinMode(yellowpin, OUTPUT);
  pinMode(indicatorpin1, OUTPUT);
  pinMode(indicatorpin2, OUTPUT);
}

void getnextcolor(float& red, float& green, float& blue){
  tcs.setInterrupt(false);  // turn on LED

  delay(60);  // takes 50ms to read

  tcs.getRGB(&red, &green, &blue);
  
  tcs.setInterrupt(true);  // turn off LED
}

int sprintf(const char *format, ...)
{
    static char buffer[1024]; // Adjust to your needs
    va_list aptr;
    va_start(aptr, format);
    int results = vsnprintf(buffer, sizeof(buffer), format, aptr);
    va_end(aptr);
    Serial.print(buffer);
    return results;
}

void printcolor(int time, float red, float green, float blue) {
  sprintf("red[%d] = %d, green[%d] = %d, blue[%d] = %d\n", time, (int)red, time, (int)green, time, (int)blue);
}

void loop() {
  //Step 1:  turn on green indicator for 3 second
  digitalWrite(indicatorpin1, HIGH);
  delay(3000);
  //Step 2: turn off green indicator
  digitalWrite(indicatorpin1, LOW);
  //Step 3: test the first spot rgb
  float r1, g1, b1;
  getnextcolor(r1, g1, b1);
  printcolor(1, r1, g1, b1);
  delay(2000);
  //Step 4: turn on clear indicator for 2 seconds
  digitalWrite(indicatorpin2, HIGH);
  delay(3000);
  //Step 5: turn off clear indicator
  digitalWrite(indicatorpin2, LOW);
  //Step 6: test the second spot rgb
  float r2, g2, b2;
  getnextcolor(r2, g2, b2);
  printcolor(2, r2, g2, b2);
  delay(2000);
  //Step 7: turn on the clear indicator for 2 seconds
  digitalWrite(indicatorpin2, HIGH);
  delay(3000);
  //Step 8: turn off clear indicator
  digitalWrite(indicatorpin2, LOW);
  //Step 9: test the third spot rgb
  float r3, g3, b3;
  getnextcolor(r3, g3, b3);
  printcolor(3, r3, g3, b3);
  delay(2000);
  //Step 10: turn on the clear indicator for 2 seconds
  digitalWrite(indicatorpin2, HIGH);
  delay(3000);
  //Step 11: turn off clear indicator
  digitalWrite(indicatorpin2, LOW);
  //Step 12: test the fourth spot rgb
  float r4, g4, b4;
  getnextcolor(r4, g4, b4);
  printcolor(4, r4, g4, b4);
  //Step 13: collect results
  float avgR=(r1+r2+r3+r4)/4;
  float avgG=(g1+g2+g3+g4)/4;
  float avgB=(b1+b2+b3+b4)/4;
  sprintf("avgR: %d, avgG: %d, avgB: %d\n", (int)avgR, (int)avgG, (int)avgB);
  //Step 14: calculate the healthiness score of the leaf based on the four spot rgbs
  float devR=abs(avgR-r1)+abs(avgR-r2)+abs(avgR-r3)+abs(avgR-r4);
  float devG=abs(avgG-g1)+abs(avgG-g2)+abs(avgG-g3)+abs(avgG-g4);
  float devB=abs(avgB-b1)+abs(avgB-b2)+abs(avgB-b3)+abs(avgB-b4);
  sprintf("devR: %d, devG: %d, devB: %d\n", (int)devR, (int)devG, (int)devB);

  float healthscore=devR+devG+devB;
  sprintf("score: %d\n", (int)healthscore);
  //Step 15: based on the healthiness score, 
  //Range 0-15 is healthy
  //Range 15-25 is medium	
  //Above 25 is unhealthy	
  if (healthscore<15){
    digitalWrite(greenpin, HIGH);
  } else if(healthscore<25){
    digitalWrite(yellowpin, HIGH);
  } else {
    digitalWrite(redpin, HIGH);
  }
  delay(5000);
  digitalWrite(greenpin, LOW);
  digitalWrite(yellowpin, LOW);
  digitalWrite(redpin, LOW);
}
