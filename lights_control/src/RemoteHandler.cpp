#include "RemoteHandler.h"
#include <IRremote.hpp>

RemoteHandler::RemoteHandler(int pin) : _pin(pin), _lastCode(0) {}

void RemoteHandler::begin() {
    IrReceiver.begin(_pin, ENABLE_LED_FEEDBACK);
    Serial.println("[IR] Remote initialized");
}

void RemoteHandler::handle() {
    if (IrReceiver.decode()) {
        uint32_t code = IrReceiver.decodedIRData.decodedRawData;
        
        if (code != 0) {
            _lastCode = code;
            
            // Call appropriate callback
            if (code == BTN_0 && _btn0Callback) {
                _btn0Callback();
            } else if (code == BTN_1 && _btn1Callback) {
                _btn1Callback();
            } else if (code == BTN_2 && _btn2Callback) {
                _btn2Callback();
            } else if (code == BTN_3 && _btn3Callback) {
                _btn3Callback();
            } else if (code == BTN_4 && _btn4Callback) {
                _btn4Callback();
            } else if (code == BTN_UP && _btnUpCallback) {
                _btnUpCallback();
            } else if (code == BTN_DOWN && _btnDownCallback) {
                _btnDownCallback();
            } else if (code == BTN_OK && _btnOkCallback) {
                _btnOkCallback();
            }
        }
        
        IrReceiver.resume();
    }
}

void RemoteHandler::onButton0(ButtonCallback callback) { _btn0Callback = callback; }
void RemoteHandler::onButton1(ButtonCallback callback) { _btn1Callback = callback; }
void RemoteHandler::onButton2(ButtonCallback callback) { _btn2Callback = callback; }
void RemoteHandler::onButton3(ButtonCallback callback) { _btn3Callback = callback; }
void RemoteHandler::onButton4(ButtonCallback callback) { _btn4Callback = callback; }
void RemoteHandler::onButtonUp(ButtonCallback callback) { _btnUpCallback = callback; }
void RemoteHandler::onButtonDown(ButtonCallback callback) { _btnDownCallback = callback; }
void RemoteHandler::onButtonOk(ButtonCallback callback) { _btnOkCallback = callback; }