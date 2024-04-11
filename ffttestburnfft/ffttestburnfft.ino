/* ESP8266/32 Audio Spectrum Analyser on an SSD1306/SH1106 Display
 * The MIT License (MIT) Copyright (c) 2017 by David Bird. 
 * The formulation and display of an AUdio Spectrum using an ESp8266 or ESP32 and SSD1306 or SH1106 OLED Display using a Fast Fourier Transform
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files 
 * (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, 
 * publish, distribute, but not to use it commercially for profit making or to sub-license and/or to sell copies of the Software or to 
 * permit persons to whom the Software is furnished to do so, subject to the following conditions:  
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software. 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES 
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE 
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 * See more at http://dsbird.org.uk 
*/

// #include <Wire.h>
#include "arduinoFFT.h" // Standard Arduino FFT library 
// https://github.com/kosme/arduinoFFT, in IDE, Sketch, Include Library, Manage Library, then search for FFT
arduinoFFT FFT = arduinoFFT();



#define SAMPLES 128              // Must be a power of 2
#define SAMPLING_FREQUENCY 40000 // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
//#define amplitude 1000            // Depending on your audio source level, you may need to increase this value
#define soundIn A3
#define ampIn A5 // volume control
unsigned int sampling_period_us;
unsigned long microseconds;
byte peak[] = {0,0,0,0,0,0,0};
double vReal[SAMPLES];
double vImag[SAMPLES];
unsigned long newTime, oldTime;
int evenfilter[12];
int multifilter[12];
int amplitude;
/////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  // Wire.begin(5,4); // SDA, SCL
//  display.init();
//  display.setFont(ArialMT_Plain_10);
//  display.flipScreenVertically(); // Adjust to suit or remove
  sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));
}

void loop() {
  amplitude = (4200 - analogRead(ampIn))  * 10;
  Serial.println("running");
  Serial.println(amplitude);
  for (int i = 0; i < SAMPLES; i++) {
    newTime = micros()-oldTime;
    oldTime = newTime;
    vReal[i] = analogRead(soundIn); // A conversion takes about 1uS on an ESP32
    vImag[i] = 0;
    while (micros() < (newTime + sampling_period_us)) { /* do nothing to wait */ }
  }
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
  int bins[12] = {-2, -2, -2, -4, -4, -8, -7, -11, -12, -11, -11, -15};

  for (int i = 2; i < (SAMPLES/2); i++){ // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
    // Serial.println(vReal[i]);
    // if (vReal[i] > 2000) { // Add a crude noise filter, 10 x amplitude or more
    //   if (i<=2 )             displayBand(0,(int)vReal[i]/amplitude); // 125Hz
    //   if (i >3   && i<=5 )   displayBand(1,(int)vReal[i]/amplitude); // 250Hz
    //   if (i >5   && i<=7 )   displayBand(2,(int)vReal[i]/amplitude); // 500Hz
    //   if (i >7   && i<=15 )  displayBand(3,(int)vReal[i]/amplitude); // 1000Hz
    //   if (i >15  && i<=30 )  displayBand(4,(int)vReal[i]/amplitude); // 2000Hz
    //   if (i >30  && i<=53 )  displayBand(5,(int)vReal[i]/amplitude); // 4000Hz
    //   if (i >53  && i<=200 ) displayBand(6,(int)vReal[i]/amplitude); // 8000Hz
    //   if (i >200           ) displayBand(7,(int)vReal[i]/amplitude); // 16000Hz
    //   Serial.println(i);
    // }
    //if (vReal[i] > (amplitude*10)) {
    if (i<=3 ) {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[0]++;
        if (bins[0] > 0) {
          Serial.print("#");
        }
      }
    }             // displayBand(0,(int)vReal[i]/amplitude); // 125Hz
    if (i == 3 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >3 && i<=5 )   {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[1]++;
        if (bins[1] > 0) {
          Serial.print("#");
        }
      }
    }   // displayBand(1,(int)vReal[i]/amplitude); // 250Hz
    if (i == 5 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >5 && i<=7 )   {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[2]++;
        if (bins[2] > 0) {
          Serial.print("#");
        }
      }
    }   // displayBand(2,(int)vReal[i]/amplitude); // 500Hz
    if (i == 7 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >7 && i<=11 )    {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[3]++;
        if (bins[3] > 0) {
          Serial.print("#");
        }
      }
    } // displayBand(3,(int)vReal[i]/amplitude); // 1000Hz
    if (i == 11 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }


    if (i >11 && i<=15 )    {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[4]++;
        if (bins[3] > 0) {
          Serial.print("#");
        }
      }
    } // displayBand(3,(int)vReal[i]/amplitude); // 1000Hz
    if (i == 15 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >15 && i<=23 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[5]++;
        if (bins[4] > 0) {
          Serial.print("#");
        }
      }
    }  // displayBand(4,(int)vReal[i]/amplitude); // 2000Hz
    if (i == 23 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }
    if (i >23 && i<=30 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[6]++;
        if (bins[4] > 0) {
          Serial.print("#");
        }
      }
    }  // displayBand(4,(int)vReal[i]/amplitude); // 2000Hz
    if (i == 30 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >30 && i<=41 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[7]++;
        if (bins[5] > 0) {
          Serial.print("#");
        }
      }
    }  // displayBand(5,(int)vReal[i]/amplitude); // 4000Hz
    if (i == 41 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }
    if (i >41 && i<=53 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[8]++;
        if (bins[5] > 0) {
          Serial.print("#");
        }
      }
    }  // displayBand(5,(int)vReal[i]/amplitude); // 4000Hz
    if (i == 53 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >53 && i<=64 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[9]++;
        if (bins[6] > 0) {
          Serial.print("#");
        }
      }
    } // displayBand(6,(int)vReal[i]/amplitude); // 8000Hz
    if (i == 64 ) {
      Serial.println(' ');
      Serial.println('i');
    }


    if (i >64 && i<=75 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[10]++;
        if (bins[6] > 0) {
          Serial.print("#");
        }
      }
    } // displayBand(6,(int)vReal[i]/amplitude); // 8000Hz
    if (i == 75 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }

    if (i >75 && i<=90 )  {
      for (int t= 0; t < vReal[i]/amplitude; t++) {
        bins[11]++;
        if (bins[7] > 0) {
          Serial.print("#");
        }
      }
    } // displayBand(6,(int)vReal[i]/amplitude); // 8000Hz
    if (i == 90 ) {
      Serial.println(' ');
      Serial.println('i');
      Serial.println(i);
    }
  }
  Serial.println(" ");
  
  Serial.println(analogRead(soundIn));
  Serial.println(analogRead(ampIn));
  Serial.println("done");
  for (int i = 0; i < 12; i++){
    Serial.print(bins[i]);
    Serial.print(", ");
  }
 //  Serial.println(" ");
  if (millis()%4 == 0) {for (byte band = 0; band <= 6; band++) {if (peak[band] > 0) peak[band] -= 1;}} // Decay the peak
  // display.display();
}

void displayBand(int band, int dsize){
  // display stuff
  int dmax = 50;
  if (dsize > dmax) dsize = dmax;
  // if (band == 7) display.drawHorizontalLine(18*6,0, 14);
  for (int s = 0; s <= dsize; s=s+2){
    // display.drawHorizontalLine(18*band,64-s, 14);
    Serial.println("band");
    Serial.println(band);
    Serial.println("s");
  }
  if (dsize > peak[band]) {peak[band] = dsize;}
}



