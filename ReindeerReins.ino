#include <dummy.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiServerSecure.h>
#include <WiFiUdp.h>

//#include <TinyWireM.h>
//#include <USI_TWI_Master.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
//#include <Adafruit_Sensor.h>
//#include <Adafruit_LSM303_U.h>
//#include <Adafruit_FloraPixel.h>

#define NOSEPIN 0
#define RIGHTPIN 14
#define LEFTPIN 12
#define NOSE_PIXEL 1
#define REIN_PIXELS 150
#define RIGHT_PIXEL REIN_PIXELS
#define LEFT_PIXEL REIN_PIXELS

#define compass 1
#define twinkle 2

//const char* ssid     = "49CN6";         // The SSID (name) of the Wi-Fi network you want to connect to
//const char* password = "P3NTZHQFCXZYXVTZ";     // The password of the Wi-Fi network

unsigned long AccelMillis;
unsigned long MagMillis;
unsigned long ModeMillis;
int mode = compass;

//function prototypes
void LED_modes(int local_mode);
void Twinkle(uint32_t max_wait);
void Twinkle_Color(int color, uint32_t max_wait);
void setAllColor(uint32_t c);
void colorWipe(int colors, uint8_t wait, Adafruit_NeoPixel locStrip);
void rainbow_nodelay(uint8_t wait, uint8_t current_time);
void rainbowCycle(uint8_t wait, int numOfIterations);
void theaterChase(int color, uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel locStrip);
void CandyCane(int *color1, int *color2, int sets,int width,int wait);
void fadeAllColors(int fade_speed, int wait);
void testCode();
void changeNose();
//end function prototypes

// Parameter 1 = number of pixels in left
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel nose = Adafruit_NeoPixel(NOSE_PIXEL, NOSEPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel rightRein = Adafruit_NeoPixel(RIGHT_PIXEL, RIGHTPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel leftRein = Adafruit_NeoPixel(LEFT_PIXEL, LEFTPIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip[]={rightRein, leftRein};

/* Assign a unique ID to this sensor at the same time */

int blank[]={0, 0, 0};
int red[]={255, 0, 0};
int white[] = {255, 255, 255};
int blue[] = {0, 0, 255};
int green[] = {0, 255, 0};
int gold[] = {212, 175, 55};
int yellow[] = {254, 254, 51};
int purple[] = {191, 0, 255};
int safetyOrange[] = {255, 103, 0};
int *colors[]={red, white, blue, green, gold, yellow, purple, safetyOrange};

int noseOn = 0;


void setup() {
  // put your setup code here, to run once:
  ////Serial.begin(9600);
//  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
//  delay(10);
//  Serial.println('\n');
//  
//  WiFi.begin(ssid, password);             // Connect to the network
//  Serial.print("Connecting to ");
//  Serial.print(ssid); Serial.println(" ...");

    /* Enable auto-gain */
  pinMode(0,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(14,OUTPUT);
  nose.begin();
  leftRein.begin();
  rightRein.begin();
  // Update the left, to start they are all 'off'
  nose.show();
  leftRein.show();
  rightRein.show();
  
  /* Display some basic information on this sensor */
  nose.setBrightness(64);
  leftRein.setBrightness(64);
  rightRein.setBrightness(64);
}

void loop() {
  // put your main code here, to run repeatedly:
  /* Get a new sensor event */ 
  unsigned long currentmillis = millis();
 
  mode = compass;

//  changeNose();
  
//  LED_modes(mode);
  testCode();
}

//*********************************************************************************************
//*************************         LED_modes         *****************************************
//*********************************************************************************************

void LED_modes(int local_mode){
  uint8_t theaterWait = 0;
  float headingLocal;
  unsigned long lCurrentMillis = millis();
  uint32_t lColor;
  int index;
  
  switch (local_mode){
    case compass:
//        ////Serial.println("Entering Compass Mode");

      ////Serial.print("Current Millis:  ");
      ////Serial.print(lCurrentMillis);
      ////Serial.println("");
      ////Serial.print("Mode Millis:  ");
      ////Serial.print(ModeMillis);
      ////Serial.println("");

      if((lCurrentMillis>=0)&&(lCurrentMillis<=ModeMillis+300000)){
        changeNose();
        ////Serial.println("First Mode");
        index = random(0, sizeof(colors));
        ////Serial.println("Color Index:");////Serial.print(index);////Serial.println("\ln");
        colorWipe(colors[index],50,strip[0]);
        colorWipe(colors[index],50,strip[1]);
      } else if((lCurrentMillis > ModeMillis+300001) && (lCurrentMillis<=ModeMillis+600000)){
        changeNose();
        ////Serial.println("Second Mode");
        index = random(0, sizeof(colors));
        theaterWait = random(0, 75);
        theaterChase(colors[index], theaterWait);
      } else if((lCurrentMillis > ModeMillis+600001) && (lCurrentMillis<=ModeMillis+900000)){
        changeNose();
        ////Serial.println("Third Mode");
        for(int x =0; x<25;x++){
        index = random(0, sizeof(colors));
          Twinkle_Color(colors[index], 20);
        }
      } else if((lCurrentMillis > ModeMillis+9000001) && (lCurrentMillis<=ModeMillis+1200000)){
        changeNose();
        ////Serial.println("Fourth Mode");
          rainbowCycle(20,3);
      } else if((lCurrentMillis > ModeMillis+1200001) && (lCurrentMillis<=ModeMillis+1500000)){
        changeNose();
        ////Serial.println("Fifth Mode");
         for(int m=0;m<5;m++) {
            CandyCane(red, white, 10,8,50);    //  This is a good one 10, 8, 50
         }
          ModeMillis = lCurrentMillis;
      } else if(lCurrentMillis > ModeMillis+1500001){
          ModeMillis = lCurrentMillis;
      } else if(lCurrentMillis < ModeMillis){
          ModeMillis = lCurrentMillis;
          AccelMillis = lCurrentMillis;
          MagMillis = lCurrentMillis;
      }
      break;
    case twinkle:
      ////Serial.println("Entering Twinkle Mode on Jump");
      for(int x =0; x<25;x++){
       Twinkle(20);
      }
      local_mode = compass;
      ModeMillis = lCurrentMillis;
      break;
    default:
//      setAllColor(left.Color(255,0, 0));
      break;
  }
//  return local_mode;
  
}


//*********************************************************************************************
//*************************          Twinkle          *****************************************
//*********************************************************************************************

//maybe make the time delay the delta of acceleration 
void Twinkle(uint32_t max_wait){
  uint32_t brightness = random(0, 255);
  uint32_t pixel = random(0, LEFT_PIXEL);


  for(int i=0;i<sizeof(strip);i++){
    colorWipe(blank,max_wait,strip[i]);
    strip[i].show();
    strip[i].setPixelColor(pixel, strip[i].Color(brightness,brightness,brightness));
    strip[i].show();
  }
  delay(max_wait);
  for(int i=0;i<sizeof(strip);i++){
    strip[i].setPixelColor(pixel, strip[i].Color(0,0,0));
    strip[i].show();
  }
}

//*********************************************************************************************
//*************************       Twinkle_Color       *****************************************
//*********************************************************************************************

//maybe make the time delay the delta of acceleration 
void Twinkle_Color(int lcolor[], uint32_t max_wait){
  uint32_t pixel = random(0, LEFT_PIXEL);
  uint32_t c;

  for(int i=0;i<sizeof(strip);i++){
    c = strip[i].Color(lcolor[0],lcolor[1],lcolor[2]);
    colorWipe(blank,max_wait,strip[i]);
    strip[i].show();
    strip[i].setPixelColor(pixel, c);
    strip[i].show();
  }
  delay(max_wait);
  for(int i=0;i<sizeof(strip);i++){
    strip[i].setPixelColor(pixel, strip[i].Color(0,0,0));
    strip[i].show();
  }
}

//*********************************************************************************************
//*************************        setAllColor        *****************************************
//*********************************************************************************************

void setAllColor(uint32_t c){
  for(int i=0;i<sizeof(strip);i++){
    for(int j=0; j<strip[0].numPixels(); j++) {
      strip[i].setPixelColor(j, c);
    }
    strip[i].show();
  }
}

//*********************************************************************************************
//*************************         colorWipe         *****************************************
//*********************************************************************************************

// Fill the dots one after the other with a color
void colorWipe(int color[], uint8_t wait, Adafruit_NeoPixel locStrip) {
  uint32_t c;
    c = locStrip.Color(color[0],color[1],color[2]);

  for(int i=0; i < locStrip.numPixels(); i++) {

    locStrip.setPixelColor(i, c);
    locStrip.show();
    delay(wait);
  }
}

//*********************************************************************************************
//*************************      rainbow_nodelay      *****************************************
//*********************************************************************************************

void rainbow_nodelay(uint8_t wait, uint8_t current_time) {
  static uint32_t local_time,i,j;

  //dont update unless its time to
  if(current_time>local_time+wait){
    local_time=current_time;

    //for each color
    if(j<256){
      for(int k=0;k<sizeof(strip);k++){
        //for each pixel
        if(i < strip[k].numPixels()) {
          //set the color
          strip[k].setPixelColor(i, Wheel((i+j) & 255,strip[k]));
          i++; //next pixel
        }
        else{
          i=0; //go to beginning pixel
        }
        j++; //inc on color wheel
        strip[k].show();
      }  
    }
    else{
      j=0; //reset to beginning of color wheel
    }
  }
}

//*********************************************************************************************
//*************************        rainbowCycle       *****************************************
//*********************************************************************************************

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait, int numOfIterations) {
  int i, j;

  for(j=0; j<256*numOfIterations; j++) { // 5 cycles of all colors on wheel
    for(int i=0;i<sizeof(strip);i++){
      for(int k=0; k< strip[i].numPixels(); k++) {
        strip[i].setPixelColor(k, Wheel(((k * 256 / strip[i].numPixels()) + j) & 255,strip[i]));
      }
      strip[i].show();
      delay(wait);
    }
  }
}


//*********************************************************************************************
//*************************        theaterChase       *****************************************
//*********************************************************************************************

//Theatre-style crawling lights.
void theaterChase(int color[], uint8_t wait) {
  uint32_t c;
  
  for(int j=0; j<10; j++) {  //do 10 cycles of chasing
    for(int q=0; q < 3; q++) {
      for(int x=0;x<sizeof(strip);x++){
        c = strip[x].Color(color[0],color[1],color[2]);
        for (int i=0; i < strip[x].numPixels(); i=i+3) {
          strip[x].setPixelColor(i+q, c);    //turn every third pixel on
        }
        strip[x].show();
      }
      
      delay(wait);

      for(int x=0;x<sizeof(strip);x++){   
        for (int i=0; i < strip[x].numPixels(); i=i+3) {
          strip[x].setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
  }
}

//*********************************************************************************************
//*************************    theaterChaseRainbow    *****************************************
//*********************************************************************************************

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
 
      for(int x=0;x<sizeof(strip);x++){   
        for (int i=0; i < strip[x].numPixels(); i=i+3) {
          strip[x].setPixelColor(i+q, Wheel( (i+j) % 255,strip[x]));    //turn every third pixel on
        }
        strip[x].show();
      }
      delay(wait);

      for(int x=0;x<sizeof(strip);x++){   
        for (int i=0; i < strip[x].numPixels(); i=i+3) {
          strip[x].setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
  }
}

//*********************************************************************************************
//*************************           Wheel           *****************************************
//*********************************************************************************************

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos, Adafruit_NeoPixel locStrip) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return locStrip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return locStrip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return locStrip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

//*********************************************************************************************
//*************************           Candy Cane           ************************************
//*********************************************************************************************

void CandyCane(int color1[], int color2[], int sets,int width,int wait) {
  int L;
  uint32_t c1;
  uint32_t c2;
  
  for(int j=0;j<(sets*width);j++) {
    for(int x=0;x<sizeof(strip);x++){   

      c1 = strip[x].Color(color1[0],color1[1],color1[2]);
      c2 = strip[x].Color(color2[0],color2[1],color2[2]);

      for(int i=0;i< strip[x].numPixels();i++) {
        L=strip[x].numPixels()-i-1;
        if ( ((i+j) % (width*2) )<width){
          strip[x].setPixelColor(L,c1);
        }else{
          strip[x].setPixelColor(L,c2);
        }
      }
      strip[x].show();
    }
    delay(wait);
  }
}

//*********************************************************************************************
//*************************           Fade All Colors           *******************************
//*********************************************************************************************

void fadeAllColors(int fade_speed, int wait) {
  uint32_t fadeValue;
  for(int j=0;sizeof(colors);j++) {
    for(int i=0;i< 255;i+=fade_speed) {
      fadeValue = leftRein.Color(colors[j][0]/(255-i), colors[j][1]/(255-i), colors[j][2]/(255-i));
      setAllColor(fadeValue);
      delay(wait);
    }
      delay(wait*75);

    for(int k=254;k> 0;k-=fade_speed) {
      fadeValue = leftRein.Color(colors[j][0]/(255-k), colors[j][1]/(255-k), colors[j][2]/(255-k));
      setAllColor(fadeValue);
      delay(wait);
    }
  }
}
//*********************************************************************************************
//*************************           Test Code Function           ****************************
//*********************************************************************************************

void testCode() {
        int index;

//        changeNose();
//        index = random(0, sizeof(colors));
        colorWipe(red,5,rightRein);
        colorWipe(blue,5,leftRein);


//  CandyCane(red, white, 10,8,50);    //  This is a good one 10, 8, 50
//*  CandyCane(green, gold, 10,8,50);    //  This is a good one 10, 8, 50
//*  CandyCane(yellow, purple, 10,8,50);    //  This is a good one 10, 8, 50
//*  CandyCane(blue, safetyOrange, 10,8,50);    //  This is a good one 10, 8, 50

}

void changeNose(){
  if(noseOn){
    nose.setPixelColor(0, nose.Color(red[0],red[1],red[2]));
    nose.show();
    noseOn = 1;
  }else{
    nose.setPixelColor(0, 0);
    nose.show();
    noseOn = 0;
  }
}

