#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define IR_RECEIVE_PIN 15
#define BTN_1 = 0XBA45FF00
#define BTN_2 = 0XB946FF00
#define BTN_3 = 0XB847FF00
#define BTN_OK = 0XE31CFF00
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint32_t lastCode = 0;
int pressCount = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Display failed!");
    while(1);
  }
  
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("IR Remote Ready");
  display.println("");
  display.println("Press any button");
  display.display();
  
  Serial.println("Ready!");
}

void updateDisplay() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("Last Button:");
  display.println("");
  display.setTextSize(2);
  
  if(lastCode == 0xBA45FF00) {
    display.println("Power!");
  } else if(lastCode == 0xBF40FF00) {
    display.println("Custom!");
  } else if(lastcode == 0XB946FF00) {
    display.println("a$$")
    } else {
    display.println(lastCode, HEX);
  }
  
  display.setTextSize(1);
  display.println("");
  display.print("Presses: ");
  display.println(pressCount);
  display.display();
}

void loop() {
  if (IrReceiver.decode()) {
    uint32_t code = IrReceiver.decodedIRData.decodedRawData;
    
    if(code != 0) {
      // New button press
      lastCode = code;
      pressCount++;
      
      Serial.print("Press #");
      Serial.print(pressCount);
      Serial.print(" - Code: ");
      Serial.println(code, HEX);
      
      updateDisplay();
    } else {
      // Repeat code (long press)
      Serial.print("Holding: ");
      Serial.println(lastCode, HEX);
    }
    
    IrReceiver.resume();
  }
}