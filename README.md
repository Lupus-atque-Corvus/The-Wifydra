# **The Wifydra**  
📡 Multi-headed 802.11 Panopticon  

The Wifydra is a wardriving tool for simultaneously monitoring all 2.4 GHz Wi-Fi channels, capturing network beacons, and geotagging locations. Data is stored on an SD card for easy upload to Wigle.net.

![The Wifydra PCB](https://github.com/lozaning/The_Wifydra/assets/13127833/40c7db88-5f30-40e3-a340-c568a6b9d1a1)  
![Assembled Device](https://github.com/lozaning/The_Wifydra/assets/13127833/05d1311d-2f22-4797-af1c-c54453f760cb)  
![image](https://github.com/miami6/The_Wifydra-2023/blob/main/IMG_9117.JPG)  
![image](https://github.com/miami6/The_Wifydra-2023/blob/main/IMG_9139.JPG)  

---

## **Key Features**  
🔍 **Multi-channel scanning**: Monitors all 14 Wi-Fi channels simultaneously.  
🗺️ **Geotagging**: Logs network locations using GPS.  
📁 **SD Card Support**: Stores data for easy upload to Wigle.net.  
🛠️ **Modular Design**: Built with ESP32-S3 and Seeedstudio XIAO modules.  

---

## **To-Do**  
- Display Customisations  
- Further Code Improvements  
- Improve PCB  

---

## **Project Notes**  
- I have changed and adapted `Dom.ino` to resolve issues with wigle.net and made general corrections. However, I haven't built my own copy yet to test functionality.  
- `Sub.ino` works as-is and doesn't require reworking.  
- The README is designed to be traceable and easy to follow for rebuilding.  
- PCB improvements are planned to fix lead connection issues.  

---

## **Build Notes**  

### **Hardware Issues and Solutions**  
1. **PCB Power Issues**:  
   - Two bodge cables are required due to PCB design.  
   - One bodge cable connects the +3V output from the ESP32-S3 to the VIN of the SD Card or GPS.  
   - This fixes the mismatch between the +5V net on the ESP32-S3 and peripherals vs. the +3V net on sub nodes.  

2. **XIAO Module Footprint**:  
   - The PCB has a larger footprint for Seeedstudio XIAO modules than necessary.  
   - Adjust the modules slightly and ensure proper pin connections.  

---

## **Bill of Materials (BOM)**  

### **Required Components**  
| Component                     | Quantity | Manufacturer Part          | Manufacturer       | Link                                                                                   |  
|-------------------------------|----------|----------------------------|--------------------|---------------------------------------------------------------------------------------|  
| Adafruit_GPS                  | 1        | Ultimate GPS Breakout v3    | Adafruit          | [Adafruit Product Page](https://www.adafruit.com/product/746)                        |  
| MicroSD Card Breakout          | 1        | 254                        | Adafruit          | [Adafruit Product Page](https://www.adafruit.com/product/254)                        |  
| ESP32-S3 Feather TFT          | 1        | 5483                       | Adafruit          | [Adafruit Product Page](https://www.adafruit.com/product/5483)                        |  
| Seeedstudio XIAO (ESP32-C3/S3)| 16       | 113991054                   | Seeedstudio       | [Seeedstudio Product Page](https://www.seeedstudio.com/product/113991054.html)       |  

### **Optional Components**  
| Component | Purpose | 
| --- | --- |
| UF.L TO SMA | Add external RP SMA antenna                      |
| SMA Antenna  | 8 dbi RPSMA Antenna                            |
| LiPo Battery | Power your ESP32-S3 for portable applications |
| Clear case | Harbor Freight Apache 500 series clar case |

---

## **Hardware Setup**  

### **Power Requirements**  
- **Recommended**: 5V input via USB-C or external power source.  
- **Optional**: 18650 battery pack with 5V output for portability.  
- **Note**: Powering via the USB-C port of the ESP32-S3 Feather TFT is possible but not recommended due to unknown power draw implications.  

### **Module Connections**  

#### **SD Reader Module**  
| SD Reader Pin | ESP32-S3 GPIO | Pin |  
| --- | --- | --- |  
| MISO | GPIO37 | MISO |  
| MOSI | GPIO35 | MOSI |  
| SCK | GPIO14 | SCK |  
| CS | GPIO10 | D10 |  

#### **GPS Module**  
| GPS Pin | ESP32-S3 GPIO | Pin |  
| --- | --- | --- |  
| TX | GPIO1 | TX |  
| RX | GPIO2 | RX |  

---

## **Software Setup**  

### **Required Libraries**  
| Library | Purpose |  
| --- | --- |  
| WiFi | For connecting to Wi-Fi networks |  
| esp_now | For ESP-NOW communication between ESP32 modules |  
| Adafruit_GFX | For controlling the TFT display |  
| Adafruit_ST7789 | For OLED display functionality |  
| SPI | For Serial Peripheral Interface communication |  
| HardwareSerial | For serial communication (e.g., GPS module) |  
| TinyGPS++ | For parsing GPS data |  
| SD | For interacting with the SD card |  

### **Firmware Flashing**  
- **Main Controller**: Flash `Dom.ino` on the ESP32-S3 Feather TFT.  
- **Sub Nodes**: Flash `Sub.ino` on each XIAO module.  
  - **Important**: Manually set a unique `BOARD_ID` for each sub node to ensure proper channel scanning (1-14).  

---

## **Getting Started**  

1. **Hardware Assembly**:  
   - Solder components and bodge cables as noted.  
   - Ensure proper power connections (5V).  

2. **Software Flashing**:  
   - Flash `Dom.ino` to the main ESP32-S3 Feather TFT.  
   - Flash `Sub.ino` to each XIAO module, assigning unique `BOARD_ID` values.  

3. **First Boot**:  
   - Connect power and test functionality.  
   - Verify GPS and Wi-Fi scanning.  

---

## **Troubleshooting**  

- **GPS Not Locking**: Ensure the GPS module has a clear view of the sky and is properly connected.  
- **Wi-Fi Channels Not Scanning**: Verify that all sub nodes have unique `BOARD_ID` values and are properly flashed.  
- **SD Card Errors**: Ensure the SD card is properly formatted and securely connected.  

---

This reorganized README maintains all the original information while improving readability and structure. Let me know if you'd like further adjustments!