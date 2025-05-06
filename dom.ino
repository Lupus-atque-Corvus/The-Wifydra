// Include necessary libraries for the project
#include <WiFi.h>
#include <esp_now.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include <SD.h>
#include <string>

// Define constants for file logging
#define LOG_FILE_PREFIX "/gpslog"  // Prefix for log file names
#define MAX_LOG_FILES 100         // Maximum number of log files before cycling
#define LOG_FILE_SUFFIX "csv"     // File extension for log files
#define LOG_COLUMN_COUNT 11       // Number of columns in the log file
#define LOG_RATE 500              // Logging rate in milliseconds
#define NOTE_DH2 661              // Not used in this context
const int chipSelect = 10;       // Pin for SD card chip select

// GPS-related constants
static const uint32_t GPSBaud = 9600;   // Baud rate for GPS serial communication
static const int RXPin = 12, TXPin = 13; // Pins for GPS serial communication

// Wigle CSV header format
const String wigleHeaderFileFormat = "WigleWifi-1.4,appRelease=2.26,model=Feather,release=0.0.0,device=The_Wifydra,display=3fea5e7,board=esp8266,brand=Adafruit";

// Column names for the log file
char* log_col_names[LOG_COLUMN_COUNT] = {
  "MAC", "SSID", "AuthMode", "FirstSeen", "Channel", "RSSI", "CurrentLatitude", "CurrentLongitude", "AltitudeMeters", "AccuracyMeters", "Type"
};

// Initialize the TFT display
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Variables for tracking SSIDs
String recentSSID;
String recentSSID1;
String recentSSID2;

// Arrays to track SSIDs and counts for up to 14 boards
String boardSSID[15];   // Index 0 unused, boards 1-14
int boardSeen[15] = {0}; // Index 0 unused, boards 1-14

// Data structure for incoming messages
typedef struct struct_message {
  char bssid[64];       // BSSID of the network
  char ssid[32];        // SSID of the network
  char encryptionType[16]; // Encryption type (e.g., WEP, WPA, etc.)
  int32_t channel;      // Channel number
  int32_t rssi;         // RSSI value
  int boardID;          // ID of the board (1-14)
} struct_message;

struct_message myData; // Instance of the message structure

// Initialize GPS serial port
HardwareSerial GPSSerial(2);
TinyGPSPlus gps; // GPS object for parsing data

// Function to format a MAC address from a byte array
String formatMACAddress(const uint8_t* mac) {
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return buffer;
}

// Function to format an SSID with quotes and escape characters
String formatSSID(const String& ssid) {
  String formattedSSID = "\"";
  for (char c : ssid) {
    if (c == '\"') {
      formattedSSID += "\"\"";
    } else {
      formattedSSID += c;
    }
  }
  formattedSSID += "\"";
  return formattedSSID;
}

// Function to update the log file name with cyclic file management
void updateFileName() {
  for (int i = 0; i < MAX_LOG_FILES; i++) {
    memset(logFileName, 0, sizeof(logFileName));
    snprintf(logFileName, sizeof(logFileName), "%s%d.%s", LOG_FILE_PREFIX, i, LOG_FILE_SUFFIX);
    if (!SD.exists(logFileName)) {
      Serial.println("New log file: ");
      Serial.println(logFileName);
      return;
    }
  }
  // If all files exist, overwrite the oldest
  snprintf(logFileName, sizeof(logFileName), "%s0.%s", LOG_FILE_PREFIX, LOG_FILE_SUFFIX);
  Serial.println("Overwriting oldest log file: ");
  Serial.println(logFileName);
}

// Function to print the CSV header to the log file
void printHeader() {
  File logFile = SD.open(logFileName, FILE_WRITE);
  if (logFile) {
    logFile.println(wigleHeaderFileFormat);
    for (int i = 0; i < LOG_COLUMN_COUNT; i++) {
      logFile.print(log_col_names[i]);
      if (i < LOG_COLUMN_COUNT - 1) {
        logFile.print(',');
      } else {
        logFile.println();
      }
    }
    logFile.close();
  }
}

// Callback function for ESP-NOW data reception
void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
  File logFile = SD.open(logFileName, FILE_APPEND);
  if (logFile) {
    memcpy(&myData, incomingData, sizeof(myData));
    
    // Format and log MAC address
    String macAddress = formatMACAddress(mac);
    logFile.print(macAddress);
    logFile.print(',');
    
    // Format and log SSID
    String ssid = formatSSID(myData.ssid);
    logFile.print(ssid);
    logFile.print(',');
    
    logFile.print(myData.encryptionType);
    logFile.print(',');
    
    // Format and log date and time in UTC
    char dateTime[20];
    snprintf(dateTime, sizeof(dateTime), "%04d-%02d-%02dT%02d:%02d:%02dZ",
             gps.date.year(), gps.date.month(), gps.date.day(),
             gps.time.hour(), gps.time.minute(), gps.time.second());
    logFile.print(dateTime);
    logFile.print(',');
    
    logFile.print(myData.channel);
    logFile.print(',');
    
    logFile.print(myData.rssi);
    logFile.print(',');
    
    // Log coordinates with 6 decimal places
    logFile.print(gps.location.lat(), 6);
    logFile.print(',');
    logFile.print(gps.location.lng(), 6);
    logFile.print(',');
    
    int altitude = static_cast<int>(gps.altitude.meters());
    logFile.print(altitude);
    logFile.print(',');
    
    logFile.print(gps.hdop.value());
    logFile.print(',');
    
    logFile.print(myData.boardID);
    logFile.print('.');
    logFile.print(0, 1); // Placeholder for accuracy meters
    logFile.print(',');
    
    logFile.println("WIFI");
    
    logFile.close();
    
    // Update board SSID and seen count
    if (myData.boardID >= 1 && myData.boardID <= 14) {
      boardSSID[myData.boardID] = myData.ssid;
      boardSeen[myData.boardID]++;
    }
    
    recentSSID2 = recentSSID1;
    recentSSID1 = recentSSID;
    recentSSID = myData.ssid;
    
    totalNetworks++;
  }
}

void setup() {
  // Initialize serial communication at 115200 baud
  Serial.begin(115200);
  
  // Initialize GPS serial port
  GPSSerial.begin(GPSBaud, SERIAL_8N1, RXPin, TXPin);
  Serial.println("GPS initialized");
  
  // Initialize TFT display
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);
  tft.init(135, 240); // Initialize ST7789 with 240x135 resolution
  tft.setRotation(0);  // Set rotation to 0 degrees
  tft.fillScreen(ST77XX_BLACK); // Fill screen with black
  tft.setCursor(0, 0); // Set cursor to top-left corner
  
  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    tft.println("SD card initialization failed.");
    while (true)
      ;
  }
  
  // Initialize Wi-Fi and ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback for ESP-NOW data reception
  esp_now_register_recv_cb(OnDataRecv);
  
  // Update log file name and print header
  updateFileName();
  printHeader();
}

void loop() {
  // Update display information
  displayinfo();
  
  // Smart delay for GPS processing
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - lastMillis >= LOG_RATE) {
    lastMillis = currentMillis;
    smartDelay(LOG_RATE);
  }
}

// Smart delay function with GPS processing
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (GPSSerial.available())
      gps.encode(GPSSerial.read());
  } while (millis() - start < ms);
}

// Function to display information on the TFT screen
void displayinfo() {
  tft.fillScreen(ST77XX_BLACK); // Clear the screen
  tft.setCursor(0, 0); // Set cursor to top-left corner
  tft.setTextWrap(true); // Enable text wrapping
  tft.setTextSize(1); // Set text size
  
  // Display GPS information
  tft.print("LT ");
  tft.print(gps.location.lat(), 4); // Print latitude with 4 decimal places
  tft.print(" LN");
  tft.print(gps.location.lng(), 4); // Print longitude with 4 decimal places
  tft.print(" S");
  tft.println(gps.satellites.value()); // Print number of satellites
  
  // Display board SSID and seen counts
  for (int i = 1; i <= 14; i++) {
    tft.print(i); // Print board number
    tft.print(":");
    tft.print(boardSeen[i]); // Print number of times seen
    tft.print(" | ");
    if (i % 2 == 0) {
      tft.println(); // New line after every two boards
    }
  }
  
  // Display recent SSIDs
  tft.println(recentSSID);
  tft.println(recentSSID1);
  tft.println(recentSSID2);
  tft.println();
  tft.print("Nets: ");
  tft.print(totalNetworks); // Print total number of networks
}