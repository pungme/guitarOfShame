// Simple NeoPixel test.  Lights just a few pixels at a time so a
// 1m strip can safely be powered from Arduino 5V pin.  Arduino
// may nonetheless hiccup when LEDs are first connected and not
// accept code.  So upload code first, unplug USB, connect pixels
// to GND FIRST, then +5V and digital pin 6, then re-plug USB.
// A working strip will show a few pixels moving down the line,
// cycling between red, green and blue.  If you get no response,
// might be connected to wrong end of strip (the end wires, if
// any, are no indication -- look instead for the data direction
// arrows printed on the strip).
 
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN      6
#define PIN2     7
//#define N_LEDS 25
#define N_LEDS 43
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N_LEDS, PIN2, NEO_RGBW + NEO_KHZ800);

//AudioFrequencyMeter meter;

int soundSensorPin=A0;
int soundReading=0;
int soundThreshold=400;
int intensity[3]={0,0,0};
int LEDPins[3] = {3,5,6};
int numberOfPins=3;
int currentPin=0;
int fadeCounter=0;
int fadeDelay=50;

int belowCounter = 0;
int overCounter = 0;

void setup() {

   #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  
  pinMode(soundSensorPin, INPUT);

  for(int i=0; i<numberOfPins;i++){
      pinMode(LEDPins[i],OUTPUT);
  }

  Serial.begin(9600);

  strip.begin();
  strip.show(); 

  strip2.begin();
  strip2.show(); 
//    meter.setBandwidth(70.00, 1500);    // Ignore frequency out of this range
//  meter.begin(A0, 960000);   
}
 
void loop() {
//  delay(10);
   soundReading=analogRead(soundSensorPin);
  if(soundReading > soundThreshold){
     overCounter++;
     if(overCounter > 50){
      soundReading=remap(soundReading, soundThreshold, 900,0, N_LEDS);
      showTheLight(soundReading);
      belowCounter = 0;
      overCounter = 0; 
     }
  }else{
    // check if the
      belowCounter++;
      if(belowCounter > 350){
        goDown(); 
        belowCounter = 0;
      }
   }
}

void showTheLight(uint16_t soundReading){
  // now process it to 0 to 120

  if(soundReading >N_LEDS){
    soundReading = N_LEDS;
  }

  
  for(uint16_t i=0; i<soundReading; i++){
    //go for the last first this time  

    uint32_t theColor = strip.Color(255, 0, 0);
    
    //TODO: split this to 3 states
    if(i > 3 && i <= 7){
        theColor = strip.Color(0, 0, 255);
    }else if( i > 7){
        theColor = strip.Color(0,255,255);  
    }
    
    strip.setPixelColor(i,theColor); 
    strip.show();

    strip2.setPixelColor(i,strip2.Color(0, 255, 0)); 
    strip2.show();
    
    delay(3);
  }
//   goDown();
}


static float remap(float x, float oMin, float oMax, float nMin, float nMax ){

    //range check
    if( oMin == oMax) {
        //std::cout<< "Warning: Zero input range";
        return -1;    }
    
    if( nMin == nMax){
        //std::cout<<"Warning: Zero output range";
        return -1;        }
    
    //check reversed input range
    bool reverseInput = false;
    float oldMin = min( oMin, oMax );
    float oldMax = max( oMin, oMax );
    if (oldMin == oMin)
        reverseInput = true;
    
    //check reversed output range
    bool reverseOutput = false;  
    float newMin = min( nMin, nMax );
    float newMax = max( nMin, nMax );
    if (newMin == nMin)
        reverseOutput = true;
    
    float portion = (x-oldMin)*(newMax-newMin)/(oldMax-oldMin);
    if (reverseInput)
        portion = (oldMax-x)*(newMax-newMin)/(oldMax-oldMin);
    
    float result = portion + newMin;
    if (reverseOutput)
        result = newMax - portion;
    
    return result; 
}
static void goDown(){
  for(uint16_t i=0; i<=N_LEDS; i++){
    //go for the last first this time  
    strip.setPixelColor(N_LEDS - i,0); // turn  if off
    strip.show();
    
    strip2.setPixelColor(N_LEDS - i,0); // turn  if off
    strip2.show();
    delay(5);
  }
}
//
static void goUp(uint32_t c) {
  for(uint16_t i=0; i<N_LEDS; i++) {
      strip.setPixelColor(i  ,c); // Draw new pixel
//      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(15);
  }
}
