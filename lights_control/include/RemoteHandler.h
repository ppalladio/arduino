#ifndef REMOTE_HANDLER_H
#define REMOTE_HANDLER_H

#include <stdint.h>
#include <RemoteCodes.h>

typedef void (*ButtonCallback)();

class RemoteHandler {
public:
    RemoteHandler(int pin);
    void begin();
    void handle();
    
    void onButton0(ButtonCallback callback);
    void onButton1(ButtonCallback callback);
    void onButton2(ButtonCallback callback);
    void onButton3(ButtonCallback callback);
    void onButton4(ButtonCallback callback);
    void onButtonUp(ButtonCallback callback);
    void onButtonDown(ButtonCallback callback);
    void onButtonOk(ButtonCallback callback);

private:
    int _pin;
    uint32_t _lastCode;
    ButtonCallback _btn0Callback = nullptr;
    ButtonCallback _btn1Callback = nullptr;
    ButtonCallback _btn2Callback = nullptr;
    ButtonCallback _btn3Callback = nullptr;
    ButtonCallback _btn4Callback = nullptr;
    ButtonCallback _btnUpCallback = nullptr;
    ButtonCallback _btnDownCallback = nullptr;
    ButtonCallback _btnOkCallback = nullptr;
};

#endif