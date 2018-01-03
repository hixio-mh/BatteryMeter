/* Handle Various Modes */

// Load Current Mode
void loadMode(){
  switch(mode){
    case 0: homeScreen(); break;
    case 1: bigVolt(); break;
    case 2: graphVolt(); break;
    case 3: help(); break;
    case 4: info(); break;
  }
  // Clear Screen if Mode Changes
  if (lastMode != mode){
    lastMode = mode;
    display.clearDisplay();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////
// BATTERY MONITOR //
////////////////////////////////////////////////////////////////////////////////////////////////
int checkBattery(){
  return analogRead(1);
}

static const unsigned char PROGMEM batt[] =
{ B00111111, B11111000,
  B01000000, B00000100,
  B01011000, B00000110,
  B01011000, B00000110,
  B01011000, B00000110,
  B01011000, B00000110,
  B01000000, B00000100,
  B00111111, B11111000,
 };

////////////////////////////////////////////////////////////////////////////////////////////////
// HOME SCREEN //
////////////////////////////////////////////////////////////////////////////////////////////////

int curSelection = 0;

void homeScreen(){
  // Button Input
  int b = checkButtons();
  if (b == 1){curSelection = max(0, curSelection - 1);}
  if (b == 2){curSelection = min(3, curSelection + 1);}
  if (b == 3){mode = curSelection + 1;}
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("HackBerry Lab");
  display.println("Battery Monitor");

  if(checkBattery < 100 || debug){display.drawBitmap(110, 0, batt, 16, 8, 1);}

  // Volt Meter
  display.setTextSize(1); display.setCursor(0,20);
  if (curSelection == 0){display.fillRect(0, 19, 127, 10, WHITE); display.setTextColor(BLACK);}
  else{display.fillRect(0, 19, 128, 10, BLACK); display.setTextColor(WHITE);}
  display.println("+ Volt Meter");

  // Volt Grapher
  display.setTextSize(1); display.setCursor(0,30);
  if (curSelection == 1){display.fillRect(0, 29, 127, 10, WHITE); display.setTextColor(BLACK);}
  else{display.fillRect(0, 29, 128, 10, BLACK); display.setTextColor(WHITE);}
  display.println("+ Volt Grapher");

  // Help
  display.setTextSize(1); display.setCursor(0,40);
  if (curSelection == 2){display.fillRect(0, 39, 127, 10, WHITE); display.setTextColor(BLACK);}
  else{display.fillRect(0, 39, 128, 10, BLACK); display.setTextColor(WHITE);}
  display.println("+ Help");
  
  // Info
  display.setTextSize(1); display.setCursor(0,50);
  if (curSelection == 3){display.fillRect(0, 49, 127, 10, WHITE); display.setTextColor(BLACK);}
  else{display.fillRect(0, 49, 128, 10, BLACK); display.setTextColor(WHITE);}
  display.println("+ Info");
  
  display.display();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// VOLT METER //
////////////////////////////////////////////////////////////////////////////////////////////////
float thresh = 5;

void bigVolt(){
  // Button Input
  int b = checkButtons();
  if (b == 1){thresh = min(12, thresh + .1);} // Increase threshold
  if (b == 2){thresh = max(0, thresh - .1);} // Decrease threshold
  if (b == 3){mode = 0;}  // Return to Main Screen

  float v = getVolt();
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("Voltage Meter");
  display.print("Threshold: ");
  display.println(thresh);
  if (v > thresh){display.println("****WARNING****");}
  else{display.println("                 ");}

  display.setTextSize(5);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,30);
  display.println(v);

  display.display();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// VOLT GRAPHER //
////////////////////////////////////////////////////////////////////////////////////////////////

byte vLife[128];
long vTimer = 0;
long vInterval = 100;
int vIndex = 0;
int graphSize = 128;
int graphMin = 61;
int graphMax = 16;
int intMin = 100;
int intMax = 10000;

void initVLife(){
  for (int i = 0; i < graphSize; i++){
    vLife[i] = graphMin;
  }
}

void voltTimer(){
  
  if (millis() - vTimer > vInterval){
    float raw = getVolt();
    float newRaw = graphMin - ((raw/5) * (graphMin - graphMax));
    if (debug){Serial.print(raw); Serial.print("\t"); Serial.println(newRaw);}
    
    byte v = char(newRaw);
    vLife[vIndex] = v;
    vTimer = millis();
    vIndex = (vIndex + 1) % graphSize;
  }
}

void graphVolt(){
  // Button Input
  int b = checkButtons();
  if (b == 1){vInterval = min(10000, vInterval + 10);} // Raise sampling interval
  if (b == 2){vInterval = max(10, vInterval - 10);} // Do nothing
  if (b == 3){mode = 0;}  // Return to Main Screen
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("Voltage Graph");
  display.print("V: ");
  display.print(getVolt());
  display.print(" T: ");
  display.print(vInterval);
  display.print(" ms");

  voltTimer();

  for (int i = 0; i < graphSize; i++){
    display.drawLine(i, graphMin, i, graphMax, BLACK);
    display.drawLine(i, graphMin, i, vLife[i], WHITE);
  }
  
  drawGrid();
  
  display.drawPixel((vIndex + (graphSize - 1)) % graphSize, 63, BLACK);
  display.drawPixel(vIndex, 63, WHITE); // Draw current reading
  display.display();
}

void drawGrid(){
  for (int i = 0; i <= 5; i++){
    int t = map(i, 0, 5, graphMin, graphMax);
    display.drawLine(0, t, 128, t, BLACK);
  }
}


/* VOLTAGE FUNCTIONS */

// Returns the Number of Volts
float getVolt(){
  int rawInput = 0;
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 100000.0; // resistance of R1 (100K)
  float R2 = 10000.0; // resistance of R2 (10K)
  int value = 0;
  
  value = analogRead(0);
  vout = (value * 5.2) / 1024.0;
  vin = vout / (R2 / (R1 + R2)); 
  if (vin < 0.09) {vin = 0.0;} 

  return vin;
}


////////////////////////////////////////////////////////////////////////////////////////////////
// HELP //
////////////////////////////////////////////////////////////////////////////////////////////////

void help(){
  // Button Input
  int b = checkButtons();
  if (b == 1){} // Do nothing
  if (b == 2){} // Do nothing
  if (b == 3){mode = 0;}  // Return to Main Screen
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("Battery Monitor");
  display.println("Help");
  display.println("Battery Grapher:");
  display.println("Up and Down buttons");
  display.println("Change sampling rate");

  display.display();
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Info //
////////////////////////////////////////////////////////////////////////////////////////////////

void info(){
  // Button Input
  int b = checkButtons();
  if (b == 1){} // Do nothing
  if (b == 2){} // Do nothing
  if (b == 3){mode = 0;}  // Return to Main Screen
  
  display.setTextSize(1);
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0,0);
  display.println("About Battery Monitor");
  display.println("By Zane Cochran");
  display.println("Created for CRT 420");
  display.println("Advanced Prototyping");
  display.println("Spring 2018");

  display.display();
}


