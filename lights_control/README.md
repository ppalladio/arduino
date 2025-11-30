# ESP32 Smart Home Remote Controller

A WiFi-enabled IR remote controller for Philips Hue lights using ESP32 and PlatformIO. Control your smart lights with a simple infrared remote!

## 🌟 Features

- **IR Remote Control** - Use any compatible IR remote to control Philips Hue lights
- **WiFi Connected** - Communicates with Hue Bridge over your local network
- **Auto Light Discovery** - Automatically discovers and assigns Hue lights by name
- **Individual & Group Control** - Toggle individual lights or all lights at once
- **Easy Setup** - First-time setup wizard for Hue Bridge authentication
- **Extensible** - Easy to add new button mappings and features

## 🎮 Default Button Mapping

| Button | Function |
|--------|----------|
| **0** | Create Hue user (first-time setup) |
| **1** | Toggle ALL lights |
| **2** | Toggle Ceiling light |
| **3** | Toggle Desk light |
| **4** | Toggle both Lamps (1 & 2) |
| **UP** | Available for future features |
| **DOWN** | Available for future features |
| **OK** | Available for future features |

## 📋 Hardware Requirements

### Required Components:
- **ESP32 Development Board** (any variant)
- **IR Receiver Module** (e.g., TSOP38238, VS1838B)
- **IR Remote Control** (any standard remote)
- **Philips Hue Bridge** (v2 recommended)
- **Philips Hue Lights** (any compatible lights)

### Optional:
- OLED Display (for status feedback)
- Buttons for manual control

## 🔌 Wiring Diagram

```
ESP32          IR Receiver
━━━━━━━━━━━━━━━━━━━━━━━━━━
GPIO 15   -->  OUT (Data)
3.3V      -->  VCC
GND       -->  GND
```

**Note:** You can change the GPIO pin in `config.h`

## 🚀 Getting Started

### Step 1: Install PlatformIO

Install PlatformIO IDE:
- [VS Code Extension](https://platformio.org/install/ide?install=vscode) (Recommended)
- [CLion Plugin](https://platformio.org/install/ide?install=clion)
- [Command Line](https://platformio.org/install/cli)

### Step 2: Clone and Configure

1. Clone this repository:
```bash
git clone <your-repo-url>
cd esp32-hue-remote
```

2. Create `config.h` from template:
```bash
cp config.h.example config.h
```

3. Edit `config.h` with your settings:
```cpp
// WiFi Configuration
#define WIFI_SSID "Your_WiFi_Name"
#define WIFI_PASSWORD "Your_WiFi_Password"

// Hue Bridge Configuration
#define HUE_BRIDGE_IP "192.168.1.XXX"  // Find via Hue app
#define HUE_USERNAME "YOUR_HUE_USERNAME_HERE"  // Leave as-is for first setup

// Hardware Configuration
#define IR_RECEIVE_PIN 15  // GPIO pin for IR receiver
```

### Step 3: Find Your Hue Bridge IP

**Option 1 - Philips Hue App:**
1. Open Hue app → Settings → Hue Bridges
2. Tap the (i) icon
3. Note the IP address

**Option 2 - Discovery:**
Visit https://discovery.meethue.com/ in your browser

**Option 3 - Router:**
Check your router's DHCP client list for "Philips Hue"

### Step 4: Upload to ESP32

1. Connect ESP32 via USB
2. In PlatformIO:
   ```bash
   pio run --target upload
   ```
   Or click the **Upload** button in VS Code

3. Open Serial Monitor (115200 baud):
   ```bash
   pio device monitor
   ```

### Step 5: First-Time Setup

1. **Press the button on your Hue Bridge** (it will glow blue)
2. **Within 30 seconds**, press **Button 0** on your IR remote
3. The serial monitor will display your new username:
   ```
   [HUE] User created: AbCdEf123456789...
   Update HUE_USERNAME in config.h
   ```
4. **Copy this username** into `config.h`:
   ```cpp
   #define HUE_USERNAME "AbCdEf123456789..."
   ```
5. Re-upload the code
6. The device will now auto-discover your lights!

### Step 6: Configure IR Codes (Optional)

If your remote uses different codes:

1. Open Serial Monitor
2. Press each button on your remote
3. Note the hex codes displayed (e.g., `0xBA45FF00`)
4. Update `RemoteHandler.h`:
```cpp
#define BTN_0    0xYOUR_CODE_HERE
#define BTN_1    0xYOUR_CODE_HERE
// ... etc
```


## 🔧 Customization

### Add New Button Functions

Edit `main.cpp` in the `setupRemote()` function:

```cpp
// Example: Button 5 to set brightness
remote.onButton5([]() {
    Serial.println("[BTN 5] Set brightness to 50%");
    hue.setBrightness(cellingId, 128);  // 0-254
});
```

### Change Light Assignments

Lights are auto-assigned by name, but you can manually set IDs in `main.cpp`:

```cpp
void setupHue() {
    hue.begin();
    hue.discoverLights();
    
    // Manual assignment
    cellingId = 1;
    deskId = 2;
    lamp1Id = 3;
    lamp2Id = 4;
}
```

### Add More Features

The `HueController` class supports:
- `setLight(id, on)` - Turn light on/off
- `setBrightness(id, level)` - Set brightness (0-254)
- `setColor(id, hue, sat)` - Change color
- `setGroup(groupId, on)` - Control groups
- `setScene(sceneId)` - Activate scenes

## 🐛 Troubleshooting

### WiFi Not Connecting
- Check SSID and password in `config.h`
- Ensure ESP32 is in range
- Try 2.4GHz network only (ESP32 doesn't support 5GHz)

### Hue Bridge Not Responding
- Verify Bridge IP address is correct
- Ensure ESP32 and Bridge are on same network
- Check if username was created successfully
- Try pinging the Bridge: `ping 192.168.1.XXX`

### IR Remote Not Working
- Check wiring (especially GND!)
- Verify GPIO pin in `config.h` matches your wiring
- Test IR receiver with Serial Monitor - codes should appear when pressing buttons
- Try different IR codes in `RemoteHandler.h`

### Lights Not Discovered
- Check Hue username is set correctly
- Ensure lights are powered on
- Verify lights are connected to Bridge (check Hue app)
- Check Serial Monitor for discovery output

### Serial Monitor Shows Garbage
- Set baud rate to **115200**
- Ensure correct COM port is selected
- Try pressing EN/RST button on ESP32
