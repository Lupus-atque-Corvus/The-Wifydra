# **The-Wifydra**  
📡 Multi-headed 802.11 Panopticon  

The Wifydra is a wardriving tool for simultaneously monitoring all 2.4 GHz Wi-Fi channels, capturing network beacons, and geotagging locations. Data is stored on an SD card for easy upload to Wigle.net.

![The Wifydra PCB](https://github.com/lozaning/The_Wifydra/assets/13127833/40c7db88-5f30-40e3-a340-c568a6b9d1a1)  
![Assembled Device](https://github.com/lozaning/The_Wifydra/assets/13127833/05d1311d-2f22-4797-af1c-c54453f760cb)

---

## **Key Features**  
🔍 **Multi-channel scanning**: Monitors all 14 Wi-Fi channels simultaneously.  
🗺️ **Geotagging**: Logs network locations using GPS.  
📁 **SD Card Support**: Stores data for easy upload to Wigle.net.  
🛠️ **Modular Design**: Built with ESP32-S3 and Seeedstudio XIAO modules.  

---

## **Build Notes**  

### **Hardware Setup**  
- **Power**:  
  - Input: 5V (via USB-C or external power source).  
  - Optional: 18650 battery pack with 5V output.  

- **Bodge Cables**:  
  - Required for VIN connections due to PCB design.  

### **Software Setup**  
- **Main Controller**: Flash `Dom.ino` on the ESP32-S3 Feather TFT.  
- **Sub Nodes**: Flash `Sub.ino` on each XIAO module.  
  - **Important**: Manually set a unique `BOARD_ID` for each sub node to scan different channels.  

---

## **Bill of Materials (BOM)**  

| Component                     | Quantity | Manufacturer Part          | Manufacturer       |  
|-------------------------------|----------|----------------------------|--------------------|  
| Adafruit_GPS                  | 1        | Ultimate GPS Breakout v3    | Adafruit          |  
| MicroSD Card Breakout          | 1        | 254                        | Adafruit          |  
| ESP32-S3 Feather TFT          | 1        | 5483                       | Adafruit          |  
| Seeedstudio XIAO (ESP32-C3/S3)| 16       | 113991054                   | Seeedstudio       |  

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

## **Powering the Device**  

- **Recommended**: 5V input via USB-C or external power source.  
- **Optional**: 18650 battery pack with 5V output for portability.  
