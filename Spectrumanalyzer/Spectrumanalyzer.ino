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

#define HALFCLOCK  42000000 // Half the clock cycle of the Arduino Due
#define debounceDelay  5000 // debounce delay in microseconds

#define SAMPLES    4096   // Must be a power of 2
#define xres        256   // Total number of columns in the display, must be <= SAMPLES/2
#define yres        200   // Total number of rows in the display

#define zeroPixelY  210   // At what pixel on the y axis are we at a value of 0

#define refreshRate 1000  // The time to wait before updating the display after drawing a graph

MCUFRIEND_kbv tft;      // Display class object

double vReal[SAMPLES];

uint8_t scale;
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
  attachInterrupt(19, toggleHold, FALLING);

  // Set up FFT on/off function and attach it to a custom interrupt service routine
  pinMode(20, INPUT_PULLUP);
  attachInterrupt(20, toggleFFT, FALLING);

  // Set up Scaling function and attach it to a custom interrupt service routine
  pinMode(21, INPUT_PULLUP);
  attachInterrupt(21, toggleScale, FALLING);

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

  FFTOn = true;
  holdGraph = false;
  setScale(2);
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
    }

    delay(refreshRate);
  }
}

void ADCSetup(unsigned long ADCClock)
{
  adc_init(ADC, SystemCoreClock, ADCClock, ADC_STARTUP_FAST);

  ADC->ADC_MR |= 0x80;    // Set the ADC to free-running mode

  ADC->ADC_CHER = 0x02;  // Enable the ADC on analog pin 6 (ADC channel 1, check due diagram for ref.)

  ADC->ADC_CR |= 0x02;    // Start ADC sampling
}

void ADCSample()
{
  int tti = SAMPLES;
  if(!FFTOn)
    tti = SAMPLES / (SAMPLES / xres);

  uint16_t tmpArray[SAMPLES] = {0};

  for(int i = 0; i < tti; i++)
  {
    while((ADC->ADC_ISR & 0x02) == 0);   // Wait for ADC to complete current conversion on AD channel 1
    tmpArray[i] = ADC->ADC_CDR[1];       // Read the conversion result on the AD channel 1 conversion results register
  }

  for(int i = 0; i < tti; i++)
    vReal[i] = (static_cast<float>(tmpArray[i]) - 2048.0); // subtract DC bias and place in double array
}

void drawRaw()
{
  // If we are pausing / switching to FFT or switching scale then drop the current graph
  if(holdGraph || FFTOn || dropGraph)
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

  int yresHalf = yres / 2;
  int zeroPixelYHalf = zeroPixelY / 2;

  for(int i = 0; i < xres; i++)
  {
    //Serial.println(vReal[i]);
    vReal[i] = map(vReal[i], -2047, 2047, -yresHalf, yresHalf);       // remap values to yres
    vReal[i] = constrain(vReal[i], -yresHalf, yresHalf);

    // ++ Continous line graph
    tft.drawPixel(i, zeroPixelYHalf - vReal[i], BLUE);
    if(i > 0)
      tft.drawLine(i - 1, zeroPixelYHalf - vReal[i - 1], i, zeroPixelYHalf - vReal[i], BLUE);
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
    // squeeze down into a managable and displayable size (magnitutes are in the 5 digit range)
    data_avgs[i] = map(data_avgs[i], 0, 165000UL, 0, yres);       // remap averaged values to yres
    data_avgs[i] = constrain(data_avgs[i], 0, yres);
    tft.drawLine(i, zeroPixelY-data_avgs[i], i, zeroPixelY, RED);
  }

  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(0, 0);

  if(peakFreq >= 1000.0)
  {
    peakFreq /= 1000.0;
    tft.print(peakFreq);
    tft.print(" kHz");
  }
  else
  {
    tft.print(peakFreq);
    tft.print(" Hz");
  }

  // Set text size back to normal
  tft.setTextSize(1);
}

void setScale(uint8_t value)
{
  // Clear the lower right-hand corner, the lower bar and the right-hand side
  tft.fillRect(0, 213, 256, 36, BLACK);
  tft.fillRect(258, 213, 61, 36, BLACK);
  tft.fillRect(260, 0, 61, 210, BLACK);
  tft.setTextColor(WHITE);

  scale = value;
  uint8_t preScale;
  bool specialCase = false;

  switch(scale)
  {
    case 9:
      preScale = 1;       // Special case, running the ADC in an unsafe/unstable way (but with a sample rate upwards of 1.9 MSPS!)
      specialCase = true;
      break;
    case 8:
      preScale = 2;
      break;
    case 7:
      preScale = 4;
      break;
    case 6:
      preScale = 8;
      break;
    case 5:
      preScale = 16;
      break;
    case 4:
      preScale = 32;
      break;
    case 2:
      preScale = 64;
      break;
    case 1:
      preScale = 128;
      break;
    default:                    // Should never reach the default case
      break;
  }

  float tmpFreqVal = static_cast<float>(HALFCLOCK) / static_cast<float>(preScale);

  ADCSetup(tmpFreqVal);

  // (84 MHz clock-speed, 42MHz halfclock (forced divisionFactor of 4, but we can circumvent it), 21 clock cycles per conversion)
  sampleFreq = (specialCase ? (tmpFreqVal / 22.0) : (tmpFreqVal / 21.0));
  
  if(FFTOn) // Displaying FFT
  {
    float k = 1.0;
    if(scale == 1)
      k = 0.5;
    else if(scale == 3)
      k = 2.0;
    else if(scale == 4)
      k = 4.0;
    else if(scale == 5)
      k = 8.0;
    else if(scale == 6)
      k = 16.0;
    else if(scale == 7)
      k = 32.0;
    else if(scale == 8)
      k = 64.0;
    else if(scale == 9)
      k = 128.0;

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
      tft.print(1.67 - (static_cast<float>(i) * 0.33), 1);

      if(i == 5)
        tft.print(" Volt");
    }

    /*********** Convert the sample rate into samples per square and display it *************/
    float samplesPerSq = (1.0 / sampleFreq) * ((static_cast<float>(SAMPLES) / 2.0) / 12.0);

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

    if(sampleFreq >= 1000000.0)
    {
      tft.print(sampleFreq / 1000000.0);
      tft.print("MS/s");
    }
    else
    {
      tft.print(sampleFreq / 1000.0);
      tft.print("kS/s");
    }
  }

  dropGraph = true;
}

void toggleHold()
{
  do{delayMicroseconds(debounceDelay);} // Software debounce delay on both press and release
    while(digitalRead(19) == LOW);      // Wait for the button to be released

  holdGraph = !holdGraph;
}

void toggleFFT()
{
  do{delayMicroseconds(debounceDelay);} // Software debounce delay on both press and release
    while(digitalRead(20) == LOW);      // Wait for the button to be released

  FFTOn = !FFTOn;
  setScale(scale);
}

void toggleScale()
{
  do{delayMicroseconds(debounceDelay);} // Software debounce delay on both press and release
    while(digitalRead(21) == LOW);      // Wait for the button to be released

  if(scale <= 8)
    setScale(scale + 1);
  else
    setScale(1);

  if(drawingGraph)
    dropGraph = true;
}
