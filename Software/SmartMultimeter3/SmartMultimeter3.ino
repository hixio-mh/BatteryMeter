/*********************************************************************
Smart Battery Meter
Zane Cochran
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int mode = 0;   // Current Mode
int modes = 3;  // Total Modes
int lastMode = -1;  // Last Mode

boolean debug = true;

void setup()   {                
  Serial.begin(9600);
  initDisplay();
  initButtons();
  initVLife();
}


void loop() {
  checkBattery();
  loadMode();
}



