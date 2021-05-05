#include "Adafruit_GFX.h"   // Hardware-specific library
#include <MCUFRIEND_kbv.h>  // Display 'sub'-library
#include <arduinoFFT.h>     // FFT library

// Assign human-readable names to some common colors:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0xC618

#define CLKSPEED 16000000 // 16 MHz clock-speed

#define SAMPLES     512   // Must be a power of 2
#define xres        256   // Total number of  columns in the display, must be <= SAMPLES/2
#define yres        200   // Total number of  rows in the display

#define refreshRate 1000  // The time to wait before updating the display after drawing a RAW graph

MCUFRIEND_kbv tft;      // Display class object

double vReal[SAMPLES];

int scale;
double sampleFreq;
double peakFreq;

bool holdGraph;
bool FFTOn;
bool drawingGraph;
bool dropGraph;

void setup()
{
  Serial.begin(9600);
  tft.begin(tft.readID());

  // Set up Hold/Go function and attach it to a custom interrupt service routine
  pinMode(19, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(19), toggleHold, FALLING);

  // Set up FFT on/off function and attach it to a custom interrupt service routine
  pinMode(20, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(20), toggleFFT, FALLING);

  // Set up Scaling function and attach it to a custom interrupt service routine
  pinMode(21, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(21), toggleScale, FALLING);

  // Set rotation (0 - 3, portrait, landscape, rev. portait, rev. landscape)
  tft.setRotation(1);

  tft.setTextSize(1);

  tft.fillScreen(BLACK);

  /*********** Draw border frames *************/
  tft.drawLine(0, 211, 319, 211, WHITE);
  tft.drawLine(0, 212, 319, 212, WHITE);
  tft.drawLine(256, 0, 256, 239, WHITE);
  tft.drawLine(257, 0, 257, 239, WHITE);

  /******* Draw reference pixel points ********/
  for (int i = 0; i < 26; i++)
    tft.drawPixel(5 + (i * 10), 213, WHITE);

  for (int i = 0; i < 50; i++)
    tft.drawPixel(258, (i * 7), WHITE);
  /********************************************/

  FFTOn = false;
  holdGraph = false;
  setScale(3);

  ADMUX  = 0b01000110;      // use pin A06 and VCC for reference
}

void loop()
{
  if(!holdGraph)
  {
    ADCSample();
    if(FFTOn)
    {
      computeFFT();
      drawFFT();
    }
    else
    {
      drawRaw();
      delay(refreshRate);
    }
  }
}

void ADCSample()
{
  int tti = SAMPLES;
  if(!FFTOn)
    tti = SAMPLES / (SAMPLES / xres);

  for(int i = 0; i < tti; i++)
  {
    while(!(ADCSRA & 0x10));                  // wait for ADC to complete current conversion ie ADIF bit set
    vReal[i] = ADC;                           // Read from ADC
    ADCSRA |= (1 << ADIF);                    // Sets the interrupt-flag, clearing the ADC
  }

  for(int i = 0; i < tti; i++)
    vReal[i] -= 512;                          // subtract DC offset caused value
}

void computeFFT()
{
  // ++ Compute FFT
  double vImag[SAMPLES] = {0.0}; // Initialize and fill array with 0
  arduinoFFT FFT = arduinoFFT(vReal, vImag, SAMPLES, sampleFreq); // FFT object

  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  peakFreq = FFT.MajorPeakParabola();
  // -- FFT
}

void drawFFT()
{
  // ++ re-arrange FFT result to match with no. of columns on display ( xres )
  int step = (SAMPLES/2) / xres; 
  double data_avgs[xres] = {0.0}; // Initialize and fill array with 0
  int c = 0;
  for(int i = 0; i < (SAMPLES / 2); i += step)  
  {
    for (int k = 0; k < step; k++)
      data_avgs[c] = vReal[i+k];

    data_avgs[c] /= step;
    c++;
  }
  // -- re-arrange FFT result to match with no. of columns on display ( xres )

  tft.fillRect(0, 0, 256, 211, BLACK);
  for(int i = 0; i < xres; i++)
  {
    // squeeze down into a managable and displayable size (magnitutes are in the 5 digit numbers)
    data_avgs[i] = map(data_avgs[i], 0, 100000, 0, yres);       // remap averaged values to yres
    data_avgs[i] = constrain(data_avgs[i], 0, yres);
    tft.drawLine(i, 210-data_avgs[i], i, 210, RED);
  }

  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(0, 0);
  tft.print(peakFreq);
  tft.print(" Hz");

  // Set text size back to normal
  tft.setTextSize(1);
}

void drawRaw()
{
  // If we are pausing / switching to FFT or switching scale then drop the current graph
  if(holdGraph || FFTOn ||dropGraph)
  {
    dropGraph = false;
    return;
  }
    
  drawingGraph = true;

  tft.fillRect(0, 0, 256, 211, BLACK);

  // ++ Draw grid on raw output
  for(int i = 1; i < 256; i += 2)
    for(int k = 1; k < 10; k++)
      tft.drawPixel(i, k*21, GRAY);

  for(int i = 1; i < 210; i += 2)
    for(int k = 1; k < 12; k++)
      tft.drawPixel((k*21)+1, i, GRAY);
  // -- Draw grid on raw output

  int yresHalf = yres/2;

  for(int i = 0; i < xres; i++)
  {
    vReal[i] = map(vReal[i], -511, 511, -yresHalf, yresHalf);       // remap values to yres
    vReal[i] = constrain(vReal[i], -yresHalf, yresHalf);

    // ++ Continous line graph
    tft.drawPixel(i, 105 - vReal[i], BLUE);
    if(i > 0)
      tft.drawLine(i - 1, 105 - vReal[i - 1], i, 105 - vReal[i], BLUE);
    // -- Continous line graph

    if(holdGraph || FFTOn)
      break;
    else if(dropGraph)
    {
      dropGraph = false;
      break;
    }
  }

  drawingGraph = false;
}

void setScale(int value)
{
  // Clear the lower right-hand corner, the lower bar and the right-hand side
  tft.fillRect(0, 213, 256, 36, BLACK);
  tft.fillRect(258, 213, 61, 36, BLACK);
  tft.fillRect(260, 0, 61, 210, BLACK);

  scale = value;
  int preScale;

  tft.setTextColor(WHITE);

  ADCSRA |= (1 << ADEN);    // ADEN: ADC-disable

  switch(scale)
  {
    case 5:
      preScale = 8;
      ADCSRA = 0b11100011;      // ADEN; ADC-enable, ADSC; Start ADC conversion, ADATE; ADC Auto Trigger-enable, use a division factor of 8
      break;
    case 4:
      preScale = 16;
      ADCSRA = 0b11100100;      // ADEN; ADC-enable, ADSC; Start ADC conversion, ADATE; ADC Auto Trigger-enable, use a division factor of 16
      break;
    case 3:
      preScale = 32;
      ADCSRA = 0b11100101;      // ADEN; ADC-enable, ADSC; Start ADC conversion, ADATE; ADC Auto Trigger-enable, use a division factor of 32
      break;
    case 2:
      preScale = 64;
      ADCSRA = 0b11100110;      // ADEN; ADC-enable, ADSC; Start ADC conversion, ADATE; ADC Auto Trigger-enable, use a division factor of 64
      break;
    case 1:
      preScale = 128;
      ADCSRA = 0b11100111;      // ADEN; ADC-enable, ADSC; Start ADC conversion, ADATE; ADC Auto Trigger-enable, use a division factor of 128
      break;
    default:                    // Should never reach the default case
      break;
  }

  // Used for calculating peak frequency in FFT output and seconds per square in RAW output
  sampleFreq = static_cast<float>(CLKSPEED) / (static_cast<float>(preScale) * 13.0);         // (16MHz clock-speed / the divisionFactor of preScale Further divided by 13.5 clock cycles it takes per ADC conversion)

  if(FFTOn) // Displaying FFT
  {
    float k = 1.0;
    if(scale == 1)
      k = 0.25;
    else if(scale == 2)
      k = 0.5;
    else if(scale == 4)
      k = 2.0;
    else if(scale == 5)
      k = 4.0;

    tft.setCursor(8, 220);
    tft.print(1.0 * k, 1);

    tft.setCursor(21, 230);
    tft.print(2.0 * k, 1);

    tft.setCursor(34, 220);
    tft.print(3.0 * k, 1);

    tft.setCursor(47, 230);
    tft.print(4.0 * k, 1);

    tft.setCursor(60, 220);
    tft.print(5.0 * k, 1);

    tft.setCursor(73, 230);
    tft.print(6.0 * k, 1);

    tft.setCursor(86, 220);
    tft.print(7.0 * k, 1);

    tft.setCursor(99, 230);
    tft.print(8.0 * k, 1);

    tft.setCursor(112, 220);
    tft.print(9.0 * k, 1);

    tft.setCursor(125, 230);
    tft.print(10.0 * k, 1);

    tft.setCursor(138, 220);
    tft.print(11.0 * k, 1);

    tft.setCursor(151, 230);
    tft.print(12.0 * k, 1);

    tft.setCursor(164, 220);
    tft.print(13.0 * k, 1);

    tft.setCursor(177, 230);
    tft.print(14.0 * k, 1);

    tft.setCursor(190, 220);
    tft.print(15.0 * k, 1);

    tft.setCursor(203, 230);
    tft.print(16.0 * k, 1);

    tft.setCursor(216, 220);
    tft.print(17.0 * k, 1);

    tft.setCursor(229, 230);
    tft.print(18.0 * k, 1);

    tft.setCursor(260, 225);
    tft.print("Hz x 1000");
  }
  else  // Displaying RAW
  {
    for(int i = 1; i < 10; i++)
    {
      tft.setCursor(265, (i * 21) - 3);
      tft.print(2.5 - (static_cast<float>(i) * 0.5), 1);

      if(i == 5)
        tft.print(" Volt");
    }

    /*********** Convert the sample rate into samples per square and display it *************/
    float samplesPerSq = (1.0 / sampleFreq) * ((static_cast<float>(SAMPLES) / 2.0) / 12);

    tft.setCursor(105, 225);
    
    if(samplesPerSq <= 0.001)
    {
      samplesPerSq *= 1000000.0;  // Convert into microseconds
      tft.print(samplesPerSq);
      tft.print(" us/Sq");
    }
    else
    {
      samplesPerSq *= 1000.0;     // Convert into miliseconds
      tft.print(samplesPerSq);
      tft.print(" ms/Sq");
    }
    /****************************************************************************************/

    tft.setCursor(260, 225);
    tft.print(sampleFreq / 1000.0);
    tft.print("kS/s");
  }

  dropGraph = true;
}

void toggleHold()
{
  // Don't react until the button is released again
  do { ; } while ( digitalRead(19) == LOW );

  holdGraph = !holdGraph;
}

void toggleFFT()
{
  // Don't react until the button is released again
  do { ; } while ( digitalRead(20) == LOW );

  FFTOn = !FFTOn;
  setScale(scale);
}

void toggleScale()
{
  // Don't react until the button is released again
  do { ; } while ( digitalRead(21) == LOW );

  if(scale <= 4)
    setScale(scale + 1);
  else
    setScale(1);

  if(drawingGraph)
    dropGraph = true;
}
