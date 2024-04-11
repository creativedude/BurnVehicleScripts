
const int ShiftPWM_latchPin=8;

const bool ShiftPWM_invertOutputs = false;

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

// unsigned long startTime = 0; // start time for the chosen fading mode

// led arrays
int green[12] = {1, 4, 9, 12, 17, 20, 25, 28, 33, 36, 41, 44};
int red[12] =  {2, 5, 10, 13, 18, 21, 26, 29, 34, 37, 42, 45};
int blue[12] = {3, 6, 11, 14, 19, 22, 27, 30, 35, 38, 43, 46};
void setup(){
  Serial.begin(9600);

  // Sets the number of 8-bit registers that are used.
  ShiftPWM.SetAmountOfRegisters(numRegisters);

  // SetPinGrouping allows flexibility in LED setup. 
  // If your LED's are connected like this: RRRRGGGGBBBBRRRRGGGGBBBB, use SetPinGrouping(4).
  ShiftPWM.SetPinGrouping(1); //This is the default, but I added here to demonstrate how to use the funtion
  
  ShiftPWM.Start(pwmFrequency,maxBrightness);
}

void loop()
{    
  // oneByOne();
  // test(); for counting
  //around(red);
  //around(blue);
  //around(green);

  colourfadeEven(red, 30, 10);
  //colourfadeEven(blue, 30, 4020);
  //colourfadeEven(green, 30, 6030);
}




void test(void){
  for (int i = 30; i < numOutputs; i++) {
    ShiftPWM.SetAll(0);
    ShiftPWM.SetOne(i, 255);
    for (int b = 0; b < i; b++) {
      ShiftPWM.SetOne(1, 255);
      delay(500);
      ShiftPWM.SetOne(1, 0);
      delay(500);
    }
    delay(5000);
  }

  ShiftPWM.SetAll(200);
  delay(2000);
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
  int nextLED2 = previousLED + 1;
  if (nextLED == 11) { 
   nextLED2 = 0;
  }; 
  int nextLED3 = previousLED2 + 1;
  if (nextLED2 == 11) { 
   nextLED3 = 0;
  }; 
  int nextLED4 = previousLED3 + 1;
  if (nextLED3 == 11) { 
   nextLED4 = 0;
  }; 
  int nextLED5 = previousLED4 + 1;
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
