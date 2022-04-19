#include <Adafruit_NeoPixel.h>

char bajt;
byte counter = 0;

#define PIN            6
#define NUMPIXELS      24
#define POT_ADDR       4
#define FLASH          12
#define EXTRA          11
#define FLASH_INV      10
#define EXTRA_INV      9

#define STOP_LIGHT     3
#define STOP_LIGHT_INV 4

#define STOP_LIGHT_VAL 255
#define LIGHT_VAL      60
#define MODE_TOGG_DUR  1200
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

bool start_bit = false;
bool stop_bit = true;
int byte_counter = 0;
short data[256];
bool valid = true;
short last_state;
unsigned long headlight_timer;
int headlight_counter;

bool headlight = false;
bool state_help = false;
bool silent_mode = false;
bool extra_gpio = false;

void setup() {
  pinMode(FLASH, OUTPUT);
  Serial.begin(115200);
  pixels.begin();

  digitalWrite(FLASH, LOW);
}

void loop() {
  while(Serial.available()){      // READING PACKET
    Serial.readBytes(&bajt, 1);
    if(byte_counter == 0 && (bajt != 2)) valid = false;
    if(valid) data[byte_counter] = bajt;
    byte_counter ++;
  }
  
  valid = true;

  for(int i = 0; i < 16; i ++){  // PRINT 16 bytes from PACKET
    Serial.print(data[i]);
    Serial.print(" ");
  }
  Serial.println();
  
  byte_counter = 0;
  
  if(data[POT_ADDR] == last_state && data[POT_ADDR] != 0) set_light(data[POT_ADDR]); // FILTER
  check_flash(data[POT_ADDR]);
  
  last_state = data[POT_ADDR];
}

void check_flash(int val){
  if(val < 20 && val > 0 && state_help == false){
    if(headlight_counter == 0){
      headlight_timer = millis();
      headlight_counter ++;
    }else if (millis() - headlight_timer < MODE_TOGG_DUR){
      headlight_counter ++;
    }
    state_help = true;
  }else if((val < 0 || val > 80) && state_help == true) state_help = false;
  
  if (millis() - headlight_timer >= MODE_TOGG_DUR){
    if(headlight_counter == 3){       // TOGGLE GPIO 1 / HEADLIGHT
      headlight = !headlight;
    }else if(headlight_counter == 4){ // SILENT MODE, NO LIGHT
      silent_mode = !silent_mode;  
    }else if(headlight_counter == 5){ // TOGGLE GPIO 2
      extra_gpio = !extra_gpio;  
    }
  
    headlight_counter = 0;
    state_help = false;
  }
}

void set_light(int val){
  if(!silent_mode){
    for(int i = 0; i < NUMPIXELS; i ++){  // LIGHT WS2812
      if(val < -126 || val > 0)pixels.setPixelColor(i, pixels.Color(STOP_LIGHT_VAL,0,0));
      else pixels.setPixelColor(i, pixels.Color(LIGHT_VAL,0,0));
    }

    if(val < -126 || val > 0){ // IF NO WS2812 THEN USE THIS GPIOs
      digitalWrite(STOP_LIGHT, HIGH);
      digitalWrite(STOP_LIGHT_INV, LOW);
    }else{
      digitalWrite(STOP_LIGHT, LOW);
      digitalWrite(STOP_LIGHT_INV, HIGH);
    }

    digitalWrite(FLASH, headlight);
    digitalWrite(EXTRA, extra_gpio);
    digitalWrite(FLASH_INV, !headlight);
    digitalWrite(EXTRA_INV, !extra_gpio);
  }else {
      pixels.clear();
      digitalWrite(FLASH, LOW);
      digitalWrite(EXTRA, LOW);
      digitalWrite(FLASH_INV, HIGH);
      digitalWrite(EXTRA_INV, HIGH);
      digitalWrite(STOP_LIGHT, LOW);
      digitalWrite(STOP_LIGHT_INV, HIGH);
  }

  pixels.show();

  
}
