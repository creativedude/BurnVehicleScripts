
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(0x27);
int show;

const int ShiftPWM_latchPin=8;

// ** uncomment this part to NOT use the SPI port and change the pin numbers. This is 2.5x slower **
// #define SHIFTPWM_NOSPI
// const int ShiftPWM_dataPin = 11;
// const int ShiftPWM_clockPin = 13;


// If your LED's turn on if the pin is low, set this to true, otherwise set it to false.
const bool ShiftPWM_invertOutputs = false;

// You can enable the option below to shift the PWM phase of each shift register by 8 compared to the previous.
// This will slightly increase the interrupt load, but will prevent all PWM signals from becoming high at the same time.
// This will be a bit easier on your power supply, because the current peaks are distributed.
const bool ShiftPWM_balanceLoad = true;

#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Function prototypes (telling the compiler these functions exist).
void oneByOne(void);

void test(void);

// Here you set the number of brightness levels, the update frequency and the number of shift registers.
// These values affect the load of ShiftPWM.
// Choose them wisely and use the PrintInterruptLoad() function to verify your load.
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
unsigned int numRegisters = 6;
unsigned int numOutputs = numRegisters*8;
unsigned int numRGBLeds = numRegisters*8/3;
unsigned int fadingMode = 0; //start with all LED's off.

bool switcher = true;
bool fadeState = false;
int switchcount = 0;

int curColour = 0;
// unsigned long startTime = 0; // start time for the chosen fading mode

// led arrays
int green[12] = {1, 4, 9, 12, 17, 20, 25, 28, 33, 36, 41, 44};
int red[12] =  {2, 5, 10, 13, 18, 21, 26, 29, 34, 37, 42, 45};
int blue[12] = {3, 6, 11, 14, 19, 22, 27, 30, 35, 38, 43, 46};

int modeSelectorValue = 0;
int mode = 0;
int modePin = A1; 
int modeCount = 11;
int prevMode;
void setup(){
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0x27);
  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  
  ShiftPWM.Start(pwmFrequency,maxBrightness);
  
  lcd.begin(20, 4); // initialize the lcd
  show = 0;

  lcd.setBacklight(255);
  lcd.home(); lcd.clear();
  lcd.print("Initialising...");
   delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome 2 da Pumpkin");
  lcd.setCursor(0, 1);
  lcd.print("Burn baby burn!");
}

void loop()
{    
  
  modeSelectorValue = analogRead(modePin);
  int modeRange = 1024 / modeCount;
  mode = modeSelectorValue / modeRange;
  

  switch (mode) {
    case 0:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Twinkle Twinkle");
      }
      // pulseAll(5); removing this, cause breaking
      // twinkle rainbow

      colourfadetwinkle(red, 30, 10);
      colourfadetwinkle(blue, 30, 4020);
      colourfadetwinkle(green, 30, 6030);
      
      
      break;
      // case 1:
      //   // oneByOne();
      //   // shit, remove: case 2 more useful
      //   test(); // for counting
      //   break;
    case 1:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Rainbows!");
       }
      ////// even around, nice rotating rainbow
      colourfadeEven(red, 30, 10);
      colourfadeEven(blue, 30, 4020);
      colourfadeEven(green, 30, 6030);
      
      break;
    case 2:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Staggered");
       }
      // staggered
      staggeredColour(red, 30, 0, 1);
      staggeredColour(green, 30, 0, 2);
      staggeredColour(blue, 30, 0, 3);
     
      break;
    case 3:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Soft Pulses");
       }
      // pulse, soft, bright, low saturation
      pulse(red, 3000, 0);
      pulse(green, 3000, 1000);
      pulse(blue, 3000, 2000);
      

      break;
    case 4:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Quick pulses");
       }
      //pulse, soft, bright, low saturation, faster.
      pulse(red, 333, 111);
      pulse(green, 333, 22200);
      pulse(blue, 333, 33300);
      
      break;
    case 5:
//      if (prevMode != mode) {
//        lcd.setCursor(0, 2);
//        lcd.print("Mode selected:");
//        prevMode = mode;
//        lcd.setCursor(0, 3);
//        lcd.print("                    ");
//        lcd.setCursor(0, 3);
//        lcd.print("Just blue");
//       }
//      colourfadeReverse(blue, 100, 0, true); // shit, very flashy,
    // case 8:
    //     // shit, remove
    //     pulsePause(red, 1000, 1000);
    //     pulsePause(blue, 1000, 2000);
    //     pulsePause(green, 1000, 3000);
    //   break;
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Warp Speed!");
      }
      
      warp(red, 30, 10);
      warp(blue, 30, 4020); // uncomment to double check
      warp(green, 30, 6030);
      break;
    case 6:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
         lcd.print("Disco baby yeah!");  
       }
        // doubles (serious disco shit)
        doublefade(blue, 30, 30);
        doublefade(green, 60, 600);
        doublefade(red, 90, 900);   
       
    break;

    case 7:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("GreenBlueFade");  
       }
      colourfadeReverse(blue, 50, 0, true);
      colourfadeReverse(green, 50, 0, false); // green not working?
        
      break;

    case 8:
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Pumpkins R Orange");
       }
        // orange mode
      for (int i=0; i< 12; i++) {
        ShiftPWM.SetOne(red[i], 10);
        ShiftPWM.SetOne(green[i], 4);
        ShiftPWM.SetOne(blue[i], 0);
      }

      break;

    case 9:
      
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Low power mode");
       }
      ShiftPWM.SetAll(1);

      break;
    default:
      
      if (prevMode != mode) {
        lcd.setCursor(0, 2);
        lcd.print("Mode selected:");
        prevMode = mode;
        lcd.setCursor(0, 3);
        lcd.print("                    ");
        lcd.setCursor(0, 3);
        lcd.print("Testing...");
        Serial.println("changing");
       }
      ///////////plain around
      around(red);
      around(blue);
      around(green);
      break;
  }
  // warp(red, 100, 10); // delete
}




void test(void){
  for (int i = 30; i < numOutputs; i++) {
    ShiftPWM.SetAll(0);
    ShiftPWM.SetOne(i, 255);
    for (int b = 0; b < i; b++) {
      ShiftPWM.SetOne(1, 255);
      delay(50);
      ShiftPWM.SetOne(1, 0);
      delay(50);
    }
    delay(50);
  }

  ShiftPWM.SetAll(200);
  delay(200);
  ShiftPWM.SetAll(0);
}

int around( int colourArray[12] ){
  for (int i = 0; i < 12; i++) {
    ShiftPWM.SetAll(0);
    ShiftPWM.SetOne(colourArray[i], 255);
    delay(100);
  }
}


int colourfade(int colourArray[12], int fadeTime, int startTime){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLED = timer/(fadeTime*2);
  int previousLED = activeLED - 1;
  if (activeLED == 0) { 
    previousLED = 11;
   };
  int previousLED2 = previousLED - 1;
  if (previousLED == 0) { 
   previousLED2 = 11;
  }; 
  int previousLED3 = previousLED2 - 1;
  if (previousLED2 == 0) { 
   previousLED3 = 11;
  }; 
  int previousLED4 = previousLED3 - 1;
  if (previousLED3 == 0) { 
   previousLED4 = 11;
  }; 
  int previousLED5 = previousLED4 - 1;
  if (previousLED4 == 0) { 
   previousLED5 = 11;
  }; 
  ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
  ShiftPWM.SetOne(colourArray[previousLED], maxBrightness/2);
  ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/4);
  ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/8);
  ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness/16);
  ShiftPWM.SetOne(colourArray[previousLED5], 1);
}

int colourfadeReverse(int colourArray[12], int fadeTime, int startTime, bool reverse){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  
  if (reverse == false) {
    int activeLED = timer/(fadeTime*2);
    int previousLED = activeLED - 1;
    if (activeLED == 0) { 
      previousLED = 11;
     };
    int previousLED2 = previousLED - 1;
    if (previousLED == 0) { 
     previousLED2 = 11;
    }; 
    int previousLED3 = previousLED2 - 1;
    if (previousLED2 == 0) { 
     previousLED3 = 11;
    }; 
    int previousLED4 = previousLED3 - 1;
    if (previousLED3 == 0) { 
     previousLED4 = 11;
    }; 
    int previousLED5 = previousLED4 - 1;
    if (previousLED4 == 0) { 
     previousLED5 = 11;
    };
    ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
    ShiftPWM.SetOne(colourArray[previousLED], maxBrightness/2);
    ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/4);
    ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/8);
    ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness/16);
    ShiftPWM.SetOne(colourArray[previousLED5], 1);
  } else {
    int activeLED = timer/(fadeTime*2);
    int previousLED = activeLED - 1;
    if (activeLED == 0) { 
      previousLED = 11;
     };
    int previousLED2 = previousLED - 1;
    if (previousLED == 0) { 
     previousLED2 = 11;
    }; 
    int previousLED3 = previousLED2 - 1;
    if (previousLED2 == 0) { 
     previousLED3 = 11;
    }; 
    int previousLED4 = previousLED3 - 1;
    if (previousLED3 == 0) { 
     previousLED4 = 11;
    }; 
    int previousLED5 = previousLED4 - 1;
    if (previousLED4 == 0) { 
     previousLED5 = 11;
    };
    int l1 = colourArray[((0-1)*(activeLED-11))];
    int l2 = colourArray[((0-1)*(previousLED-11))];
    int l3 = colourArray[((0-1)*(previousLED2-11))];
    int l4 = colourArray[((0-1)*(previousLED3-11))];
    int l5 = colourArray[((0-1)*(previousLED4-11))];
    int l6 = colourArray[((0-1)*(previousLED5-11))];
    ShiftPWM.SetOne(l1, maxBrightness);
    ShiftPWM.SetOne(l2, maxBrightness/2);
    ShiftPWM.SetOne(l3, maxBrightness/4);
    ShiftPWM.SetOne(l4, maxBrightness/8);
    ShiftPWM.SetOne(l5, maxBrightness/16);
    ShiftPWM.SetOne(l6, 1);
  }
  
}

int doublefade(int colourArray[12], int fadeTime, int startTime){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);
  int activeLED = timer/(fadeTime*2);
  int previousLED = activeLED - 1;
  if (activeLED == 0) { 
    previousLED = 11;
   };
  int previousLED2 = previousLED - 1;
  if (previousLED == 0) { 
   previousLED2 = 11;
  }; 
  int previousLED3 = previousLED2 - 1;
  if (previousLED2 == 0) { 
   previousLED3 = 11;
  }; 
  int previousLED4 = previousLED3 - 1;
  if (previousLED3 == 0) { 
   previousLED4 = 11;
  }; 
  int previousLED5 = previousLED4 - 1;
  if (previousLED4 == 0) { 
   previousLED5 = 11;
  }; 
    ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
    ShiftPWM.SetOne(colourArray[previousLED], maxBrightness/2);
    ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/4);
    ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/8);
    ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness/16);
    ShiftPWM.SetOne(colourArray[previousLED5], 1);
    int l1 = colourArray[((0-1)*(activeLED-11))];
    int l2 = colourArray[((0-1)*(previousLED-11))];
    int l3 = colourArray[((0-1)*(previousLED2-11))];
    int l4 = colourArray[((0-1)*(previousLED3-11))];
    int l5 = colourArray[((0-1)*(previousLED4-11))];
    int l6 = colourArray[((0-1)*(previousLED5-11))];
    ShiftPWM.SetOne(l1, maxBrightness);
    ShiftPWM.SetOne(l2, maxBrightness/2);
    ShiftPWM.SetOne(l3, maxBrightness/4);
    ShiftPWM.SetOne(l4, maxBrightness/8);
    ShiftPWM.SetOne(l5, maxBrightness/16);
    ShiftPWM.SetOne(l6, 1);
  
}


int colourfadeEven(int colourArray[12], int fadeTime, int startTime){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLED = timer/(fadeTime*2);
  int previousLED = activeLED - 1;
  if (activeLED == 0) { 
    previousLED = 11;
   };
  int previousLED2 = previousLED - 1;
  if (previousLED == 0) { 
   previousLED2 = 11;
  }; 
  int previousLED3 = previousLED2 - 1;
  if (previousLED2 == 0) { 
   previousLED3 = 11;
  }; 
  int previousLED4 = previousLED3 - 1;
  if (previousLED3 == 0) { 
   previousLED4 = 11;
  }; 
  int previousLED5 = previousLED4 - 1;
  if (previousLED4 == 0) { 
   previousLED5 = 11;
  }; 


  
  int nextLED = activeLED + 1;
  if (activeLED == 11) { 
    nextLED = 0;
   };
  int nextLED2 = nextLED + 1;
  if (nextLED == 11) { 
   nextLED2 = 0;
  }; 
  int nextLED3 = nextLED2 + 1;
  if (nextLED2 == 11) { 
   nextLED3 = 0;
  }; 
  int nextLED4 = nextLED3 + 1;
  if (nextLED3 == 11) { 
   nextLED4 = 0;
  }; 
  int nextLED5 = nextLED4 + 1;
  if (nextLED4 == 11) { 
   nextLED5 = 0;
  }; 
  ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
  ShiftPWM.SetOne(colourArray[previousLED], maxBrightness/2);
  ShiftPWM.SetOne(colourArray[nextLED], maxBrightness/2);
  ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/4);
  ShiftPWM.SetOne(colourArray[nextLED2], maxBrightness/4);
  ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/8);
  ShiftPWM.SetOne(colourArray[nextLED3], maxBrightness/8);
  ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness/16);
  ShiftPWM.SetOne(colourArray[nextLED4], maxBrightness/16);
  ShiftPWM.SetOne(colourArray[previousLED5], 1);
  ShiftPWM.SetOne(colourArray[nextLED5], 1);
}


int colourfadetwinkle(int colourArray[12], int fadeTime, int startTime){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLED = timer/(fadeTime*2);
  int previousLED = activeLED - 1;
  if (activeLED == 0) { 
    previousLED = 11;
   };
  int previousLED2 = previousLED - 1;
  if (previousLED == 0) { 
   previousLED2 = 11;
  }; 
  int previousLED3 = previousLED2 - 1;
  if (previousLED2 == 0) { 
   previousLED3 = 11;
  }; 
  int previousLED4 = previousLED3 - 1;
  if (previousLED3 == 0) { 
   previousLED4 = 11;
  }; 
  int previousLED5 = previousLED4 - 1;
  if (previousLED4 == 0) { 
   previousLED5 = 11;
  }; 


  
  int nextLED = activeLED + 1;
  if (activeLED == 11) { 
    nextLED = 0;
   };
  int nextLED2 = nextLED + 1;
  if (nextLED == 11) { 
   nextLED2 = 0;
  }; 
  int nextLED3 = nextLED2 + 1;
  if (nextLED2 == 11) { 
   nextLED3 = 0;
  }; 
  int nextLED4 = nextLED3 + 1;
  if (nextLED3 == 11) { 
   nextLED4 = 0;
  }; 
  int nextLED5 = nextLED4 + 1;
  if (nextLED4 == 11) { 
   nextLED5 = 0;
  }; 
  ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
  ShiftPWM.SetOne(colourArray[previousLED], maxBrightness/2);
  ShiftPWM.SetOne(colourArray[nextLED], maxBrightness/2);
  ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/4);
  ShiftPWM.SetOne(colourArray[nextLED2], maxBrightness/4);
  ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/8);
  ShiftPWM.SetOne(colourArray[nextLED3], maxBrightness/8);
  ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness/16);
  ShiftPWM.SetOne(colourArray[nextLED4], maxBrightness/16);
  ShiftPWM.SetOne(colourArray[previousLED5], 1);
  ShiftPWM.SetOne(colourArray[nextLED5], 1);

  //raandom // to test randomness
  if (random(100) == 1) {
    ShiftPWM.SetOne(random(36), (random(50)+ 205));
  }
}

int warp(int colourArray[12], int fadeTime, int startTime){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 6*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLeft = timer/(fadeTime*2);
  ShiftPWM.SetOne(colourArray[activeLeft], maxBrightness);
  // Serial.println("activeLeft      ");
  // Serial.print(activeLeft);
  // Serial.println(colourArray[activeLeft]);
  // Serial.print(",");

  int activeRight = 11 - (timer/(fadeTime*2));
  ShiftPWM.SetOne(colourArray[activeRight], maxBrightness);
  // Serial.println("activeRight      ");
  // Serial.print(activeRight);
  // Serial.println(colourArray[activeRight]);
  // Serial.println(",");

  int previousLeft = activeLeft - 1;
  if (activeLeft == 0) { 
    previousLeft = 5;
   };
  ShiftPWM.SetOne(colourArray[previousLeft], maxBrightness/2);
  // Serial.println("previousLeft");
  // Serial.print(previousLeft);
  // Serial.println(colourArray[previousLeft]);
  // Serial.print(",");

  int previousRight = activeRight + 1;
  if (activeRight == 11) { 
    previousRight = 6;
   };
  ShiftPWM.SetOne(colourArray[previousRight], maxBrightness/2);
  // Serial.println("previousRight");
  // Serial.print(previousRight);
  // Serial.println(colourArray[previousRight]);
  // Serial.println(",");

  int previousLeft2 = previousLeft - 1;
  if (previousLeft == 0) { 
   previousLeft2 = 5;
  }; 
  ShiftPWM.SetOne(colourArray[previousLeft2], maxBrightness/4);
  // Serial.println("previousLeft2");
  // Serial.print(previousLeft2);
  // Serial.println(colourArray[previousLeft2]);
  // Serial.print(",");

  int previousRight2 = previousRight + 1;
  if (previousRight == 11) { 
   previousRight2 = 6;
  }; 
  ShiftPWM.SetOne(colourArray[previousRight2], maxBrightness/4);
  // Serial.println("previousRight2");
  // Serial.print(previousRight2);
  // Serial.println(colourArray[previousRight2]);
  // Serial.println(",");

  int previousLeft3 = previousLeft2 - 1;
  if (previousLeft2 == 0) { 
   previousLeft3 = 5;
  }; 
  ShiftPWM.SetOne(colourArray[previousLeft3], maxBrightness/8);
  // Serial.println("previousLeft3");
  Serial.print(previousLeft3);
  // Serial.println(colourArray[previousLeft3]);
  Serial.print(",");

  int previousRight3 = previousRight2 + 1;
  if (previousRight2 == 11) { 
   previousRight3 = 6;
  }; 
  ShiftPWM.SetOne(colourArray[previousRight3], maxBrightness/8);
  // Serial.println("previousRight3");
  // Serial.print(previousRight3);
  // Serial.println(colourArray[previousRight3]);
  // Serial.println(",");

  int previousLeft4 = previousLeft3 - 1;
  if (previousLeft3 == 0) { 
   previousLeft4 = 5;
  }; 
  ShiftPWM.SetOne(colourArray[previousLeft4], maxBrightness/16);
  // Serial.println("previousLeft4");
  // Serial.print(previousLeft4);
  // Serial.println(colourArray[previousLeft4]);
  // Serial.print(",");

  int previousRight4 = previousRight3 + 1;
  if (previousRight3 == 11) { 
   previousRight4 = 6;
  }; 
  ShiftPWM.SetOne(colourArray[previousRight4], maxBrightness/16);
  // Serial.println("previousRight4");
  // Serial.print(previousRight4);
  // Serial.println(colourArray[previousRight4]);
  // Serial.println(",");

  int previousLeft5 = previousLeft4 - 1;
  if (previousLeft4 == 0) { 
   previousLeft5 = 5;
  };
  ShiftPWM.SetOne(colourArray[previousLeft5], maxBrightness/32);
  // Serial.println("previousLeft5");
  // Serial.print(previousLeft5);
  // Serial.println(colourArray[previousLeft5]);
  // Serial.print(",");

  int previousRight5 = previousRight4 + 1;
  if (previousRight4 == 11) { 
   previousRight5 = 6;
  }; 
  ShiftPWM.SetOne(colourArray[previousRight5], maxBrightness/32);
  // Serial.println("previousRight5");
  // Serial.print(previousRight5);
  // Serial.println(colourArray[previousRight5]);
  // Serial.println(",");
}


int staggeredColour(int colourArray[12], int fadeTime, int startTime, int tail){ // Fade in and fade out all outputs one at a time
  unsigned char brightness;
  unsigned long loopTime = 12*fadeTime*2;
  unsigned long time = millis()-startTime;
  unsigned long timer = time%loopTime;
  unsigned long currentStep = timer%(fadeTime*2);

  int activeLED = timer/(fadeTime*2);
  int previousLED = activeLED - 1;
  if (activeLED == 0) { 
    previousLED = 11;
   };
  int previousLED2 = previousLED - 1;
  if (previousLED == 0) { 
   previousLED2 = 11;
  }; 
  int previousLED3 = previousLED2 - 1;
  if (previousLED2 == 0) { 
   previousLED3 = 11;
  }; 
  int previousLED4 = previousLED3 - 1;
  if (previousLED3 == 0) { 
   previousLED4 = 11;
  }; 
  int previousLED5 = previousLED4 - 1;
  if (previousLED4 == 0) { 
   previousLED5 = 11;
  }; 
  int nextLED = activeLED + 1;
  if (activeLED == 11) { 
    nextLED = 0;
   };
  int nextLED2 = nextLED + 1;
  if (nextLED == 11) { 
   nextLED2 = 0;
  }; 
  int nextLED3 = nextLED2 + 1;
  if (nextLED2 == 11) { 
   nextLED3 = 0;
  }; 
  int nextLED4 = nextLED3 + 1;
  if (nextLED3 == 11) { 
   nextLED4 = 0;
  }; 
  int nextLED5 = nextLED4 + 1;
  if (nextLED4 == 11) { 
   nextLED5 = 0;
  }; 

  int nextLED6 = nextLED5 + 1;
  if (nextLED5 == 11) { 
   nextLED5 = 0;
  }; 


  switch (tail) {
    case 1:
      ShiftPWM.SetOne(colourArray[nextLED5], maxBrightness);
      ShiftPWM.SetOne(colourArray[nextLED4], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[nextLED3], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[nextLED2], maxBrightness);
      ShiftPWM.SetOne(colourArray[nextLED], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[activeLED], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[previousLED], 0);
      ShiftPWM.SetOne(colourArray[previousLED2], 0);
      ShiftPWM.SetOne(colourArray[previousLED3], 0);
      ShiftPWM.SetOne(colourArray[previousLED4], 0);
      ShiftPWM.SetOne(colourArray[previousLED5], 0);
      break;
    case 2:
      ShiftPWM.SetOne(colourArray[nextLED5], maxBrightness);
      ShiftPWM.SetOne(colourArray[nextLED4], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[nextLED3], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[nextLED2], 0);
      ShiftPWM.SetOne(colourArray[nextLED], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[activeLED], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[previousLED], maxBrightness);
      ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/8);
      ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[previousLED4], 0);
      ShiftPWM.SetOne(colourArray[previousLED5], 0);
      break;
    case 3:
      ShiftPWM.SetOne(colourArray[nextLED5], maxBrightness);
      ShiftPWM.SetOne(colourArray[nextLED4], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[nextLED3], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[nextLED2], 0);
      ShiftPWM.SetOne(colourArray[nextLED], 0);
      ShiftPWM.SetOne(colourArray[activeLED], 0);
      ShiftPWM.SetOne(colourArray[previousLED], 0);
      ShiftPWM.SetOne(colourArray[previousLED2], maxBrightness/16);
      ShiftPWM.SetOne(colourArray[previousLED3], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[previousLED4], maxBrightness);
      ShiftPWM.SetOne(colourArray[previousLED5], maxBrightness/4);
      ShiftPWM.SetOne(colourArray[nextLED6], maxBrightness/16);
      break;
    default:
      ShiftPWM.SetOne(colourArray[activeLED], maxBrightness);
  }
}

int pulse(int colourArray[12], int fadeTime, int startTime) {
  unsigned char brightness;
  unsigned long time = millis()-startTime;
  unsigned long currentStep = time%(fadeTime*2);
  if(currentStep <= fadeTime ){
    brightness = currentStep*maxBrightness/fadeTime; ///fading in
  }
  else{
    brightness = maxBrightness-(currentStep-fadeTime)*maxBrightness/fadeTime; ///fading out;
  }
  for(int i = 0; i < 12; i++) {
    ShiftPWM.SetOne(colourArray[i], brightness);
  }
}



int pulsePause(int colourArray[12], int fadeTime, int startTime) {
  unsigned char brightness;
  unsigned long time = millis()-startTime;
  unsigned long currentStep = time%(fadeTime*2);
  unsigned long currentStep2 = time%(fadeTime);
  if(currentStep <= fadeTime ){
    if (fadeState = true && switchcount == 3) {
      switcher = true;
      switchcount = 0;
    }
    if (fadeState = true && switchcount == 1) {
      switcher = false;
    }
    if (switcher == true ) {
      brightness = currentStep*maxBrightness/fadeTime; ///fading in
    } else {
      brightness = 0;
    }
    fadeState = true;
  }
  else{
    
    if (switcher == true) {
      brightness = maxBrightness-(currentStep-fadeTime)*maxBrightness/fadeTime; ///fading out;
    } else {
      brightness = 0;
    }
    if (fadeState == true) {
      switchcount++;
    }
    fadeState = false;
  }
  for(int i = 0; i < 12; i++) {
    ShiftPWM.SetOne(colourArray[i], brightness);
  }



}

int pulseAll(int delaymilli) {
  for (int i = 0; i < 3; i++) {
    for (int b = 0; b < 256; b++) {
      switch (i) {
        case 0:
          for(int i = 0; i < 12; i++) {
            ShiftPWM.SetOne(red[i],b); // going up
            ShiftPWM.SetOne(green[i],maxBrightness-b);// going down
            ShiftPWM.SetOne(blue[i],0);
          }
          break;
        case 1:
          for(int i = 0; i < 12; i++) {
            ShiftPWM.SetOne(blue[i],b); // going up
            ShiftPWM.SetOne(red[i],maxBrightness-b); // going down
            ShiftPWM.SetOne(green[i],0);
          }
          break;

        case 2:
          for(int i = 0; i < 12; i++) {
            ShiftPWM.SetOne(green[i],b);
            ShiftPWM.SetOne(blue[i],maxBrightness-b);
            ShiftPWM.SetOne(red[i],0);
          }
          break;
      }
      delay(delaymilli);
    }
  }
  

  // curColour
}
// reference: https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
// todo: add twinkle, fire


