# Arduino_SpectrumAnalyzer
A spectrumanalyzer made for the Arduino MEGA and DUE microcontroller boards using the arduino IDE.

The program expects an analog signal in on pin 6, it then analyzes that signal and displays the results out on a 2.8 inch tft display.
The library used for fast-fourier-transform is https://github.com/kosme/arduinoFFT.
The libraries used for the tft display are Adafruit_GFX and MCUFRIEND_kbv , both of which can be found with the internal library install manager within the arduino IDE.
