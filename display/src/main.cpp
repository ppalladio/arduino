#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <IRremote.h>
#include <RemoteCodes.h> 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define IR_RECEIVE_PIN 15

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
  switch (lastCode)
  {
  case BTN_0:  // ✅ CORRECT
    display.println("0 is pressed");
    break;
  
  case BTN_1:
    display.println("1 is pressed");
    break;
  
  case BTN_2:
    display.println("2 is pressed");
    break;
  
  case BTN_UP:
    display.println("UP is pressed");
    break;
  
  case BTN_DOWN:
    display.println("DOWN is pressed");
    break;
  
  case BTN_OK:
    display.println("OK is pressed");
    break;
  
  default:
    display.print("Unknown: 0x");
    display.println(lastCode, HEX);
    break;
  }
   
  
  display.setTextSize(5);
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