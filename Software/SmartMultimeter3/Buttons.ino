// Handle Button Presses

int buttonA = 4;
int buttonB = 5;
int buttonC = 6;
int buttonDelay = 100;

// Initialize All Buttons
void initButtons(){
  pinMode(buttonA, INPUT_PULLUP);
  pinMode(buttonB, INPUT_PULLUP);
  pinMode(buttonC, INPUT_PULLUP);
}

// Check for Button Presses
int checkButtons(){
  if(digitalRead(buttonA) == LOW){delay(buttonDelay); if(debug){Serial.println(1);}  return 1;}
  if(digitalRead(buttonB) == LOW){delay(buttonDelay); if(debug){Serial.println(2);}  return 2;}
  if(digitalRead(buttonC) == LOW){delay(buttonDelay); if(debug){Serial.println(3);}  return 3;}
  return 0;
}

