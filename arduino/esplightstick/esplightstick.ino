/*

    ███████╗███████╗██████╗ ██╗     ██╗ ██████╗ ██╗  ██╗████████╗███████╗████████╗██╗ ██████╗██╗  ██╗
    ██╔════╝██╔════╝██╔══██╗██║     ██║██╔════╝ ██║  ██║╚══██╔══╝██╔════╝╚══██╔══╝██║██╔════╝██║ ██╔╝
    █████╗  ███████╗██████╔╝██║     ██║██║  ███╗███████║   ██║   ███████╗   ██║   ██║██║     █████╔╝
    ██╔══╝  ╚════██║██╔═══╝ ██║     ██║██║   ██║██╔══██║   ██║   ╚════██║   ██║   ██║██║     ██╔═██╗
    ███████╗███████║██║     ███████╗██║╚██████╔╝██║  ██║   ██║   ███████║   ██║   ██║╚██████╗██║  ██╗
    ╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝   ╚═╝   ╚═╝ ╚═════╝╚═╝  ╚═╝

  A Wifi configurable Lightstick. Runs Bitmap files stored on an SD Card Line by Line on a Neopixel LED Stripe. 

  Circuit:
    * Fritzing Diagramm:
    * PCB:
        * Eagle
        * Gerber
    * Hardware:
        * Wemos D1 Mini
        * D1 Mini MicroSD Shield
        * Adafruit SSD1306 Wemos Mini Oled Shield 0,66" 64*48
        * Neopixel WS2812B 144/m 1m
        * Batteriepack 7x AA
        * Toggle Switch (Power Button)
        * Momentary Switch (Wifi/Runtime Button)
        * L7805 (Voltage Regulator 5V)
        * 2x 10 µF Capacitor
        * Resistor 470 Ohm 
        * 5 Button AD Keyboard
              Alternatively
              5 Monumentary Buttons
              Resistors: 2k, 330, 620, 1k, 3.3k Ohm
      
  Menu:
    1 - Select File
    2 - Brightness
    3 - Initial Delay
    4 - Frame Delay
    5 - Repeat Times (The number of times the current file will be repated)
    6 - Repeat Delay (Delay between repeats)
    7 - Frame Off time (To set a time between frames - 0 fits in most cases)
    8 - Cycle All (Cycle throuh all images on card)

  Wificonnection:
    SSID and PW can be defined in config.h
      Default 
        SSID: LEDSTICK
        PW:   CarlosLight
    
  Webserver:
    Reachable on http://192.168.1.1 or https://192.168.1.1
    * show available Images
    * add Image (not implemented yet)
    * delete Image (not implemented yet)
    * Configuration - same as Menu (not implemented yet)

  Libaries used:
      Name                              Version
    * Adafruit_GFX                      1.7.5
    * Adafruit_NeoPixel                 1.3.4
    * Adafruit SSD1306 Wemos Mini Oled  1.1.2
    * SDFat                             1.1.1
    * ESP8266 Core                      2.5.2

  Created 02.02.2020
    by Mommel
    Inspired by the work of Michael Ross

  Tutorial:
      GITHUB: tbd
      VIDEO: tbd
*/
#define VERSION "0.3.0"


/*

██╗  ██╗ █████╗ ██████╗ ██████╗ ██╗    ██╗ █████╗ ██████╗ ███████╗
██║  ██║██╔══██╗██╔══██╗██╔══██╗██║    ██║██╔══██╗██╔══██╗██╔════╝
███████║███████║██████╔╝██║  ██║██║ █╗ ██║███████║██████╔╝█████╗  
██╔══██║██╔══██║██╔══██╗██║  ██║██║███╗██║██╔══██║██╔══██╗██╔══╝  
██║  ██║██║  ██║██║  ██║██████╔╝╚███╔███╔╝██║  ██║██║  ██║███████╗
╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝  ╚══╝╚══╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝


WEMOS D1 MINI
Upload Speed 921600
CPU 80 MHz
Flash 4M (3M SPIFFS)                                                                                                              
*/

#define A0                      17
#define D0                      16
#define D1                      5
#define D2                      4
#define D3                      0  // CS2
#define D4                      2  // TXD1
#define D5                      14 // CLK
#define D6                      12 // MISO
#define D7                      13 // MOSI  CTS0 RXD2
#define D8                      15 // CS    RTS0 TXD2
#define TX                         // CS1   TXD0
#define RX                         // RXD0
#define LED_BUILTIN             2

/*

 ██████╗ ██████╗ ███╗   ██╗███████╗██╗ ██████╗
██╔════╝██╔═══██╗████╗  ██║██╔════╝██║██╔════╝
██║     ██║   ██║██╔██╗ ██║█████╗  ██║██║  ███╗
██║     ██║   ██║██║╚██╗██║██╔══╝  ██║██║   ██║
╚██████╗╚██████╔╝██║ ╚████║██║     ██║╚██████╔╝
 ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝     ╚═╝ ╚═════╝

    All configuration values must be changed
                in config.h
*/

/* ████████ Application Configuration ████████ */
#define LED_AMOUNT      144
#define FRAME_TTL       20
#define LED_BRIGHTNESS  90
#define IMAGE_FOLDER_SD "/images"
#define LEDCOLORORDER   "RGB"

/* ████████   Function Pin Matching   ████████ */
#define SD_CS_PIN       D4
#define OLED_SCL        D1
#define OLED_SDA        D2
#define OLED_RESET      D3
#define NEOPX_DATA      D0
#define AD_KEYBOARD     D5
#define AUXBTN          D8

/* ████████    WIFI SSID Password    ████████ */
#define APSSID          "LEDSTICK"
#define APPSK           "CarlosLight"
#define HOSTNAME        "esplightstick"

/* Configuration below can be changed by 
 * commenting   
 * // #define SOMETHING
 * or uncommenting
 * #define SOMETHING
 * 
 * STARTSSL activates https on port 443
 * SERIALDEBUG activates debug messages to the serial
 */

//#define STARTSSL // Uncomment this to enable SSL

#define SERIALDEBUG // Comment this Line to disable serial debug

#define HTTP_GZ_ACTIVE // Uncomment to activate HTTP compression

//#define FORMATSPIFFS // Uncomment if you need to format your spiffs

#define SPECIAL_SETUP_MODE // Uncomment if you need the special unrestricted upload for setup

/*

██╗     ██╗██████╗ ██████╗  █████╗ ██████╗ ██╗███████╗███████╗
██║     ██║██╔══██╗██╔══██╗██╔══██╗██╔══██╗██║██╔════╝██╔════╝
██║     ██║██████╔╝██████╔╝███████║██████╔╝██║█████╗  ███████╗
██║     ██║██╔══██╗██╔══██╗██╔══██║██╔══██╗██║██╔══╝  ╚════██║
███████╗██║██████╔╝██║  ██║██║  ██║██║  ██║██║███████╗███████║
╚══════╝╚═╝╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚══════╝╚══════╝

*/
#include <SdFat.h>
#include "sdios.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#define  FS_NO_GLOBALS
#include <FS.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

/*

██████╗ ███████╗███████╗██╗███╗   ██╗██╗████████╗██╗ ██████╗ ███╗   ██╗███████╗
██╔══██╗██╔════╝██╔════╝██║████╗  ██║██║╚══██╔══╝██║██╔═══██╗████╗  ██║██╔════╝
██║  ██║█████╗  █████╗  ██║██╔██╗ ██║██║   ██║   ██║██║   ██║██╔██╗ ██║███████╗
██║  ██║██╔══╝  ██╔══╝  ██║██║╚██╗██║██║   ██║   ██║██║   ██║██║╚██╗██║╚════██║
██████╔╝███████╗██║     ██║██║ ╚████║██║   ██║   ██║╚██████╔╝██║ ╚████║███████║
╚═════╝ ╚══════╝╚═╝     ╚═╝╚═╝  ╚═══╝╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

*/

// Buttons
#define BTN_RIGHT               0
#define BTN_UP                  1
#define BTN_DOWN                2
#define BTN_LEFT                3
#define BTN_SELECT              4
#define BTN_AMOUNT              5

int adcKeyVal[5] ={ 30, 170, 390, 600, 800 };

// VALUE
#define VAL_INCREASE            1
#define VAL_DECREASE            2

// MENUITEMS
#define MI_FILESELECT           1
#define MI_BRIGHTNESS           2
#define MI_INIT_DELAY           3
#define MI_FRAME_DELAY          4
#define MI_REPEAT_TIMES         5
#define MI_REPEAT_DELAY         6
#define MI_FRAME_BLANK_DELAY    7
#define MI_CYCLE_ALL_IMAGES     8
#define MI_GOWIFI               9

// LED FILE READER
#define LFR_BMP_BF_TYPE         0x4D42
#define LFR_BMP_BF_OFF_BITS     54
#define LFR_BMP_BI_SIZE         40
#define LFR_BMP_BI_RGB          0L
#define LFR_BMP_BI_RLE8         1L
#define LFR_BMP_BI_RLE4         2L
#define LFR_BMP_BI_BITFIELDS    3L

extern const uint8_t gammaTable[];
extern const char HTML_HEADER[];
extern const char HTML_END[];



/*

 █████╗ ██████╗ ██████╗ ██╗     ██╗ ██████╗ █████╗ ████████╗██╗ ██████╗ ███╗   ██╗
██╔══██╗██╔══██╗██╔══██╗██║     ██║██╔════╝██╔══██╗╚══██╔══╝██║██╔═══██╗████╗  ██║
███████║██████╔╝██████╔╝██║     ██║██║     ███████║   ██║   ██║██║   ██║██╔██╗ ██║
██╔══██║██╔═══╝ ██╔═══╝ ██║     ██║██║     ██╔══██║   ██║   ██║██║   ██║██║╚██╗██║
██║  ██║██║     ██║     ███████╗██║╚██████╗██║  ██║   ██║   ██║╚██████╔╝██║ ╚████║
╚═╝  ╚═╝╚═╝     ╚═╝     ╚══════╝╚═╝ ╚═════╝╚═╝  ╚═╝   ╚═╝   ╚═╝ ╚═════╝ ╚═╝  ╚═══╝

*/

// SD vars
SdFat sd;
File sdFileToSend;
File sdFileToSave;
fs::File fileToSend;
fs::File fileToSave;
SdFile sdFile;
SdFile sdDirectory;

String activeFileName = "";
int fileIndex = 0;
int amountOfUsableFiles = 0;
String fileNames[200];

// Screen vars
#define XPOS          0
#define YPOS          0
#define BITMAP_HEIGHT 48
#define BITMAP_WIDTH  64

// Keypad vars
int adKeyIn;
int adKey = -1;
int adKeyOld = -1;

// Status vars
// bool systemReady = true;
bool wifiMode = false;
int activeMenuEntry = 1;

// Application vars
boolean updateScreen = true;
int initDelay = 0;
int brightness = 90;
int frameDelay = 15;
int repeatTimes = 1;
int repeatDelay = 0;
int frameBlankDelay = 0;
boolean cycleAllImages = false;
boolean cycleAllImagesOneshot = false;
int cycleImageCount= 0;
int loopCounter = 0;
int abortEvent = 0;
char color1;
char color2;
char color3;
int greenValue = 0;
int redValue = 0;
int blueValue = 0;

// System Informations vars
uint32_t cardSize;
String cardType;
uint32_t freeSpace;
int fatType;
int fileCount;
String ledColorOrder = LEDCOLORORDER;

String custom_wifi_ssid = "";
String custom_wifi_password = "";
String custom_wifi_mode = "";
bool fallbackWifi = true;

const char *wifi_ssid = APSSID;
const char *wifi_password = APPSK;

IPAddress serverIp(0, 0, 0, 0);
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

Adafruit_SSD1306 display(OLED_RESET);

Adafruit_NeoPixel ledStripe =
        Adafruit_NeoPixel(LED_AMOUNT, NEOPX_DATA, NEO_GRB + NEO_KHZ800);

const char* host = HOSTNAME;

#ifndef STARTSSL
ESP8266WebServer server(80);
#endif
#ifdef STARTSSL
ESP8266WebServer server(443);
#endif

String detectSdCardType() {
#ifdef SERIALDEBUG
  Serial.println("╚> Detecting Card Type");
#endif
    switch (sd.card()->type()) {
        case SD_CARD_TYPE_SD1:
#ifdef SERIALDEBUG
            Serial.println("╚=> SD");
#endif
            return "SD";
        case SD_CARD_TYPE_SD2:
#ifdef SERIALDEBUG
            Serial.println("╚=> SD2");
#endif
            return "SD";
        case SD_CARD_TYPE_SDHC:
            if(cardSize < 70000000) {
#ifdef SERIALDEBUG
                Serial.println("╚=> SDHC");
#endif
                return "SDHC";
            }else{
#ifdef SERIALDEBUG
                Serial.println("╚=> SDXC");
#endif
                return "SDXC";
            }
        default:
#ifdef SERIALDEBUG
            Serial.println("╚=> Unknown");
#endif
            return "Unknown";
    }
}
const unsigned char espLightStickLogo[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x03,
        0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x01, 0x01, 0x01, 0x03, 0x03, 0x03, 0x07, 0x07, 0x0F, 0x1F, 0x1F,
        0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0x7F, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x80, 0x80, 0xC0, 0xD0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFE,
        0xFE, 0xFE, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xD0, 0xC0,
        0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0F, 0x3F, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0xF0, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0x80, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x07,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFC, 0xF0, 0xE0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x01, 0x01, 0x03, 0x0B, 0x0F, 0x1F, 0x1F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F,
        0x7F, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x03,
        0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0xE0, 0xE0, 0xF8, 0xFC, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFE, 0xFE, 0xFC, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0, 0xC0, 0xC0,
        0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0xC0,
        0xC0, 0xE0, 0xE0, 0xF0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*

  ██╗  ██╗███████╗██╗     ██████╗ ███████╗██████╗ 
  ██║  ██║██╔════╝██║     ██╔══██╗██╔════╝██╔══██╗
  ███████║█████╗  ██║     ██████╔╝█████╗  ██████╔╝
  ██╔══██║██╔══╝  ██║     ██╔═══╝ ██╔══╝  ██╔══██╗
  ██║  ██║███████╗███████╗██║     ███████╗██║  ██║
  ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝
                                                

*/

// MimeTypes
String getContentType(String fileName) {
    fileName.toLowerCase();
    if (server.hasArg("download"))
        return "application/octet-stream";
    else if (fileName.endsWith(".htm"))
        return "text/html";
    else if (fileName.endsWith(".html"))
        return "text/html";
    else if (fileName.endsWith(".css"))
        return "text/css";
    else if (fileName.endsWith(".js"))
        return "application/javascript";
    else if (fileName.endsWith(".png"))
        return "image/png";
    else if (fileName.endsWith(".gif"))
        return "image/gif";
    else if (fileName.endsWith(".jpg"))
        return "image/jpeg";
    else if (fileName.endsWith(".ico"))
        return "image/x-icon";
    else if (fileName.endsWith(".xml"))
        return "text/xml";
    else if (fileName.endsWith(".pdf"))
        return "application/x-pdf";
    else if (fileName.endsWith(".zip"))
        return "application/x-zip";
    else if (fileName.endsWith(".gz"))
        return "application/x-gzip";
    return "text/plain";
}

void runErrorRutine(String msg) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("ERROR");
    display.println(msg);
    display.display();
    delay(2000);
    display.clearDisplay();
    display.display();
    #ifdef SERIALDEBUG
      Serial.println("╚>ERROR");
      Serial.print("╚═>");
      Serial.println(msg);
    #endif
    resetLedStripe();
}

void resetLedStripe() {
    for (int ledId = 0; ledId < LED_AMOUNT; ledId++) {
        ledStripe.setPixelColor(ledId, 0);
    }
    ledStripe.show();
}

/*
  
  ██████╗  ██████╗  ██████╗ ████████╗██████╗ ██████╗  ██████╗  ██████╗███████╗███████╗███████╗
  ██╔══██╗██╔═══██╗██╔═══██╗╚══██╔══╝██╔══██╗██╔══██╗██╔═══██╗██╔════╝██╔════╝██╔════╝██╔════╝
  ██████╔╝██║   ██║██║   ██║   ██║   ██████╔╝██████╔╝██║   ██║██║     █████╗  ███████╗███████╗
  ██╔══██╗██║   ██║██║   ██║   ██║   ██╔═══╝ ██╔══██╗██║   ██║██║     ██╔══╝  ╚════██║╚════██║
  ██████╔╝╚██████╔╝╚██████╔╝   ██║   ██║     ██║  ██║╚██████╔╝╚██████╗███████╗███████║███████║
  ╚═════╝  ╚═════╝  ╚═════╝    ╚═╝   ╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝╚══════╝╚══════╝╚══════╝
  
*/
void setupDisplay() {
#ifdef SERIALDEBUG
  Serial.println(" ");
  Serial.println("Oled init");
#endif
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#ifdef SERIALDEBUG
  Serial.println("╚> DONE");
#endif
  //display.drawBitmap(XPOS, YPOS, espLightStickLogo, BITMAP_WIDTH, BITMAP_HEIGHT,
  //                   WHITE);
  //display.display();
  //delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Version: ");
  display.println(VERSION);
  display.display();
  delay(1000);
  display.clearDisplay();
  display.display();
  delay(10);
}

void getFilenamesFromImageFolder() {
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.print("Start fetching valid Filenames from folder: ");
    Serial.println(IMAGE_FOLDER_SD);
#endif
    char cname[256];
    if(sdDirectory.isOpen())
    {
        sdDirectory.close();
    }
    sdDirectory.open(IMAGE_FOLDER_SD, O_RDONLY);
    int fileCount = 0;
    String entryFileName = "";
#ifdef SERIALDEBUG
    Serial.print("╚═>");
#endif
    while (sdFile.openNext(&sdDirectory, O_RDONLY)) {
        if (!sdFile.isHidden()) {
            sdFile.getName(cname, 255);
            entryFileName = String(cname);
            if (!sdFile.isHidden() && !entryFileName.startsWith("_") && !entryFileName.startsWith(".")) {
                if (entryFileName.endsWith(".bmp") || entryFileName.endsWith(".BMP") ) {
                    fileNames[fileCount] = entryFileName;
#ifdef SERIALDEBUG
                    Serial.print(" ");
                    Serial.print(entryFileName);
#endif
                    fileCount++;
                }
            }
        }
        sdFile.close();
    }
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.print("╚> Found ");
    Serial.print(fileCount);
    Serial.println(" valid images.");
#endif
    sdDirectory.close();
    amountOfUsableFiles = fileCount;
}

bool checkConfig(String checkValue){
    checkValue.toLowerCase();
    char color1 = checkValue[0];
    char color2 = checkValue[1];
    char color3 = checkValue[2];
    byte rcounter = 0;
    byte gcounter = 0;
    byte bcounter = 0;
    if ( color1 == 'r' || color2 == 'r' || color3 == 'r' ) rcounter++;
    if ( color1 == 'b' || color2 == 'b' || color3 == 'b' ) bcounter++;
    if ( color1 == 'g' || color2 == 'g' || color3 == 'g' ) gcounter++;
    if ( rcounter != 1 || gcounter != 1 || bcounter != 1 ) {
        return false;
    } else {
        return true;
    }
}

void checkSPIFFSConfig() {
#ifdef SERIALDEBUG
  Serial.println(" ");
  Serial.println("Setup SPIFFS");
#endif
  if ( SPIFFS.begin() ) {
    String path ="config.json";
#ifdef FORMATSPIFFS
    if (!SPIFFS.exists("/formatComplete.txt")) {
      Serial.println("Please wait 30 secs for SPIFFS to be formatted");
      SPIFFS.format();
      Serial.println("Spiffs formatted");     
      fs::File f = SPIFFS.open("/formatComplete.txt", "w");
      if (!f) {
          Serial.println("file open failed");
      } else {
          f.println("Format Complete");
      }
    } else {
      Serial.println("SPIFFS is formatted. Moving along...");
    }
#endif    
#ifdef SERIALDEBUG
    Serial.println("╚> done");
    Serial.print("╚> Check if ");
    Serial.print(path);
    Serial.println(" is present");
#endif
    if (SPIFFS.exists(path)) {
#ifdef SERIALDEBUG
      Serial.println("╚═> File found");
      Serial.println("╚═> Trying to parse data");
#endif
      fs::File configFile = SPIFFS.open(path, "r");
      String contentType = getContentType(path);
      size_t sent = server.streamFile(configFile, contentType);
      String jsonConfig = configFile.readString();
      configFile.close();      
      StaticJsonDocument<200> doc;
      DeserializationError jsonerror = deserializeJson(doc, jsonConfig);      
      if (!jsonerror) {
        String jsonString = doc["wifi"]["ssid"].as<String>();
        if(jsonString.length() > 0) {
          custom_wifi_ssid = jsonString;
#ifdef SERIALDEBUG
          Serial.println("╚══> Found SSID in SPIFFS config");
#endif
        }
        jsonString = doc["wifi"]["password"].as<String>();
        if(jsonString.length() > 0) {
          custom_wifi_password = jsonString;
#ifdef SERIALDEBUG
          Serial.println("╚══> Found Wifi PW in SPIFFS config");
#endif
        }
        jsonString = doc["wifi"]["mode"].as<String>();
        if(jsonString.length() > 0) {
          custom_wifi_mode = jsonString;
#ifdef SERIALDEBUG
          Serial.println("╚══> Found Wifi Mode in SPIFFS config");
#endif
        }
        jsonString = doc["ledColorOrder"].as<String>();
        if(checkConfig(jsonString)) {
          ledColorOrder = jsonString;
#ifdef SERIALDEBUG
          Serial.println("╚══> Found ledColorOrder in SPIFFS config");
#endif
        }
      } // !jsonerror
#ifdef SERIALDEBUG
      else {  
        Serial.println("╚══> Not parsable");
      }
#endif
    } // SPIFFS.exists(path)
#ifdef SERIALDEBUG
    else {  
      Serial.println("╚═> File not present");
    }
#endif
  } //  !SPIFFS.begin() else
#ifdef SERIALDEBUG
  else{
      Serial.println("╚> not available");
  }    
#endif
}

/*


  ██╗     ███████╗██████╗ ███████╗████████╗██╗ ██████╗██╗  ██╗
  ██║     ██╔════╝██╔══██╗██╔════╝╚══██╔══╝██║██╔════╝██║ ██╔╝
  ██║     █████╗  ██║  ██║███████╗   ██║   ██║██║     █████╔╝ 
  ██║     ██╔══╝  ██║  ██║╚════██║   ██║   ██║██║     ██╔═██╗ 
  ███████╗███████╗██████╔╝███████║   ██║   ██║╚██████╗██║  ██╗
  ╚══════╝╚══════╝╚═════╝ ╚══════╝   ╚═╝   ╚═╝ ╚═════╝╚═╝  ╚═╝
                                                              

*/

void showFileLED(String fileName) {
#ifdef SERIALDEBUG
    Serial.println("Show File on LED: " + fileName);
#endif
    char cFilePathName[512];
    String filePath = "";
    filePath.concat(F(IMAGE_FOLDER_SD));
    filePath = filePath + "/" + fileName;
    fileName.toCharArray(cFilePathName,512);
    sdFileToSend = sd.open(cFilePathName, O_READ);
    if (sdFileToSend) {
        ledFileReader();
        sdFileToSend.close();
        if ( abortEvent >=3 ){
            delay (100);
        }
#ifdef SERIALDEBUG
        Serial.println("╚> Done");
#endif
    }
    else {
#ifdef SERIALDEBUG
        Serial.println("╚> Failed to read file");
#endif
        runErrorRutine("Error reading file");
        delay(1000);
        setupSDCardStorage();
        return;
    }
}

void latchanddelay(int duration) {
    ledStripe.show();
    delay(duration);
}

void azSort(String *entry, int length) {
    for (int i = 1; i < length; ++i) {
        String entryString = entry[i];
        int sortid;
        for(sortid = i - 1; (sortid >= 0) && (entryString < entry[sortid]); sortid--) {
            entry[sortid + 1] = entry[sortid];
        }
        entry[sortid +1] = entryString;
    }
}

String outCardInfo(int type) {
    char data[64];
    switch (type) {
        case 1:
            sprintf(data, "Found: %s", cardType.c_str());
            break;
        case 2:
            sprintf(data, "FAT%f", fatType);
            break;
        case 3:            
            sprintf(data, "Max: %f MB", cardSize * 0.000512);
            break;
        case 4:
            sprintf(data, "Free: %f MB", freeSpace);
            break;
    }
    return data;
}

void ledFileReader() {
    uint16_t bmpType = readFileInt();
    uint32_t bmpSize = readFileLong();
    uint16_t bmpReserved1 = readFileInt();
    uint16_t bmpReserved2 = readFileInt();
    uint32_t bmpOffBits = readFileLong();
    bmpOffBits = 54;

    if (bmpType != LFR_BMP_BF_TYPE) {
        runErrorRutine("Error file not a bitmap");
        delay(1000);
        return;
    }

    uint32_t imgSize = readFileLong();
    uint32_t imgWidth = readFileLong();
    uint32_t imgHeight = readFileLong();
    uint16_t imgPlanes = readFileInt();
    uint16_t imgBitCount = readFileInt();
    uint32_t imgCompression = readFileLong();
    uint32_t imgSizeImage = readFileLong();
    uint32_t imgXPelsPerMeter = readFileLong();
    uint32_t imgYPelsPerMeter = readFileLong();
    uint32_t imgClrUsed = readFileLong();
    uint32_t imgClrImportant = readFileLong();

    if(
      imgSize != LFR_BMP_BI_SIZE || imgWidth <= 0 ||
      imgHeight <= 0 || imgPlanes != 1 ||
      imgBitCount != 24 || imgCompression != LFR_BMP_BI_RGB ||
      imgSizeImage == 0
      ) {
        runErrorRutine("Unsupported Bitmap Use 24bpp");
        delay(1000);
        return;
    }

    int displayWidth = imgWidth;
    if (imgWidth > LED_AMOUNT) {
        displayWidth = LED_AMOUNT;
    }

    uint32_t lineLength = imgWidth * 3;
    if ((lineLength % 4) != 0)
        lineLength = (lineLength / 4 + 1) * 4;
    for(int y = imgHeight; y > 0; y--) {
        int bufpos=0;
        if (( abortEvent <= 3 ) && ( y <= ( imgHeight -5 ))) {
            int keypress = keypadRead();
            if (( keypress == BTN_SELECT ) || ( digitalRead( AUXBTN ) == LOW )) {
                abortEvent += 1;
            }
            if ( abortEvent >=3 ){
                cycleAllImagesOneshot = 0;
                resetLedStripe();
                break;
            }
        }
        for(int x=0; x < displayWidth; x++) {
            uint32_t offset = ( LFR_BMP_BF_OFF_BITS + ((( y-1 )* lineLength ) + ( x*3 ))) ;
            sdFile.seekSet(offset); // seek(offset);
            getRGBwithGamma();
            ledStripe.setPixelColor(x,color1,color2,color3);
        }
        latchanddelay(frameDelay);
        if(frameBlankDelay > 0){
            resetLedStripe();
            delay(frameBlankDelay);
        }
    }
}

void checkCard() {
    cardSize = sd.card()->cardSize();
    cardType = detectSdCardType();
    uint32_t volFree = sd.vol()->freeClusterCount();
    freeSpace = 0.000512 * volFree * sd.vol()->blocksPerCluster();
    fatType = sd.vol()->fatType();
    Serial.print("Free: %d MB");
    Serial.println(sd.card()->cardSize() / 2048 - sd.vwd()->fileSize() / 512);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("Card initialized");
    display.println("SD_FAT VER");
    display.println(SD_FAT_VERSION);
#ifdef SERIALDEBUG
    Serial.print("╚> SdFat Libary Version: ");
    Serial.println(SD_FAT_VERSION);
#endif
    display.display();
    delay(1000);
#ifdef SERIALDEBUG
    Serial.println("╚=> " + outCardInfo(1));
    Serial.println("╚=> " + outCardInfo(2));
    Serial.println("╚=> " + outCardInfo(3));
    Serial.println("╚=> " + outCardInfo(4));
#endif
    display.clearDisplay();
    display.println(outCardInfo(1));
    display.println(outCardInfo(2));
    display.display();
    delay(1000);
    display.clearDisplay();
    display.println(outCardInfo(3));
    display.println(outCardInfo(4));
    display.display();
    delay(1000);
}

void setupSDCardStorage() {
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.println("Setup SD Card.");
#endif
    if (!sd.begin(SD_CS_PIN, SD_SCK_MHZ(50))) {
        if (sd.card()->errorCode()) {
            runErrorRutine("Card initialize failed");
            while(1){
                delay(2000);
            }
        }
    }
    if (!sdDirectory.open("/")) {
        runErrorRutine("Open root failed");
        while(1);
    }
    sdDirectory.close();
#ifdef SERIALDEBUG
    Serial.println("╚> Done");
#endif
    checkCard();
    getFilenamesFromImageFolder();
    azSort(fileNames, amountOfUsableFiles);
    showActiveFilename();
}

void setupLedStripe() {
    ledStripe.begin();
    ledStripe.show();
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.println("Setup LedStripe");
#endif
    for (int round = 0; round < 3; round++) {
        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
        switch (round) {
            case 0:
                red = 255;
                break;
            case 1:
                blue = 255;
                break;
            default:
                green = 255;
                break;
        }
        for (int activeLedId = 0; activeLedId < LED_AMOUNT; activeLedId++) {
            ledStripe.setPixelColor(activeLedId, red, green, blue);
            uint8_t activeBrightness = LED_BRIGHTNESS - LED_AMOUNT + activeLedId;
            if (activeBrightness < 10) {
                activeBrightness = 10;
            }
            ledStripe.setBrightness(activeBrightness);
            ledStripe.show();
            delay(4);
        }
    }
    resetLedStripe();
#ifdef SERIALDEBUG
    Serial.println("╚> Done");
#endif
}

/*

  ███╗   ███╗███████╗███╗   ██╗██╗   ██╗
  ████╗ ████║██╔════╝████╗  ██║██║   ██║
  ██╔████╔██║█████╗  ██╔██╗ ██║██║   ██║
  ██║╚██╔╝██║██╔══╝  ██║╚██╗██║██║   ██║
  ██║ ╚═╝ ██║███████╗██║ ╚████║╚██████╔╝
  ╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝ ╚═════╝ 

*/

void menuHanlder() {
    loopCounter +=1;
    if(abortEvent >= 3){
        delay (250);
        sdFile.close();
        abortEvent = 0;
    }
    if (loopCounter > 2000){
        int keypress = -1;
        keypress = keypadRead();
        delay(50);
        if (( digitalRead(AUXBTN) == LOW )){
            keypress = BTN_SELECT;
        }
        switch (keypress) {
            case BTN_SELECT:
                actionSelect();
                break;
            case BTN_RIGHT:
                changeValue(VAL_INCREASE);
                break;
            case BTN_LEFT:
                changeValue(VAL_DECREASE);
                break;
            case BTN_UP:
                move(VAL_INCREASE);
                break;
            case BTN_DOWN:
                move(VAL_DECREASE);
                break;
        }
    }
}

void showMenuEntry(int entry) {
    switch (entry) {
        case MI_FILESELECT:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("1) Select File:");
            display.println(activeFileName);
            display.display();
            break;
        case MI_BRIGHTNESS:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("2) Brightness");
            display.println(brightness);
            if (brightness == 100) {
                display.setCursor(3, 1);
            }
            else {
                display.setCursor(2, 1);
            }
            display.print("%");
            display.display();
            break;
        case MI_INIT_DELAY:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("3) Init Delay");
            display.println(initDelay);
            display.display();
            break;
        case MI_FRAME_DELAY:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("4) Frame Delay");
            display.println(frameDelay);
            display.display();
            break;
        case MI_REPEAT_TIMES:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("5) Repeat Times");
            display.println(repeatTimes);
            display.display();
            break;
        case MI_REPEAT_DELAY:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("6) Repeat Delay");
            display.println(repeatDelay);
            display.display();
            break;
        case MI_FRAME_BLANK_DELAY:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("7) Frame Off Time");
            display.println(frameBlankDelay);
            display.display();
            break;
        case MI_CYCLE_ALL_IMAGES:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("8) Cycle All");
            if(cycleAllImages){
                display.println("Yes");
            }else{
                display.println("No");
            }
            display.display();
            break;
        case MI_GOWIFI:
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("9) Goto WifiMode");
            display.display();
            break;
    }
}

void showActiveFilename() {
    activeFileName = fileNames[fileIndex];
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print(amountOfUsableFiles);
    display.println(" files");
    display.println(activeFileName);
    display.display();
}

void actionSelect() {
#ifdef SERIALDEBUG
    Serial.println("Select");
#endif
    if(activeMenuEntry == MI_GOWIFI) {
      wifiMode = true;
      return;
    }
    loopCounter = 0;
    delay(initDelay+100);
    cycleImageCount = 0;
    do{
        cycleAllImagesOneshot = cycleAllImages;
        if (cycleAllImages){
            activeFileName = fileNames[cycleImageCount];
            if (cycleImageCount  < amountOfUsableFiles){
                activeFileName = fileNames[cycleImageCount];
            }else{
                break;
            }
        }
        abortEvent = 0;
        if (repeatTimes > 1) {
            for (int x = repeatTimes; x > 0; x--) {
                if (abortEvent >= 3){
                    abortEvent = 0;
                    cycleAllImagesOneshot = 0;
                    break;
                }
                showFileLED(activeFileName);
                if (repeatDelay >0) {
                    resetLedStripe();
                }
                delay(repeatDelay);
            }
        }else{
            abortEvent = 0;
            showFileLED(activeFileName);
        }
        resetLedStripe();
        updateScreen = false;
        cycleImageCount +=1;
        if (repeatDelay >0) {
            resetLedStripe();
        }
        delay(repeatDelay);
    } while ((cycleAllImagesOneshot) && (cycleImageCount < amountOfUsableFiles));
}

void changeValue(byte direction) {
#ifdef SERIALDEBUG
    Serial.print("Change Entry: ");
    Serial.print(activeMenuEntry);
    Serial.print(" ");
    Serial.println( (direction == VAL_INCREASE) ? "add" : "reduce" );
#endif
    loopCounter = 0;
    updateScreen = true;
    switch (activeMenuEntry) {
        case MI_FILESELECT:
            if(direction == VAL_INCREASE){
                if (fileIndex < amountOfUsableFiles -1) {
                    fileIndex++;
                }
                else {
                    fileIndex = 0;
                }
            }else{
                if (fileIndex > 0) {
                    fileIndex--;
                }
                else {
                    fileIndex = amountOfUsableFiles -1;
                }
            }
            showActiveFilename();
            delay(500);
            break;
        case MI_BRIGHTNESS:
            if (direction == VAL_INCREASE && brightness < 100) {
                brightness += 1;
            }
            if (direction == VAL_DECREASE && brightness > 1) {
                brightness -= 1;
            }
            break;
        case MI_INIT_DELAY:
            if (direction == VAL_INCREASE) {
                initDelay += 1000;
            }else{
                initDelay -= 1000;
                if (initDelay < 0) {
                    initDelay = 0;
                }
            }
            break;
        case MI_FRAME_DELAY:
            if (direction == VAL_INCREASE) {
                frameDelay += 1;
            }else{
                if (frameDelay > 0) {
                    frameDelay -= 1;
                }
            }
            break;
        case MI_REPEAT_TIMES:
            if (direction == VAL_INCREASE) {
                repeatTimes += 1;
            }else{
                if (repeatTimes > 1) {
                    repeatTimes -= 1;
                }
            }
            break;
        case MI_REPEAT_DELAY:
            if (direction == VAL_INCREASE) {
                repeatDelay += 100;
            }else{
                repeatDelay -= 100;
                if (repeatDelay < 0) {
                    repeatDelay = 0;
                }
            };
            break;
        case MI_FRAME_BLANK_DELAY:
            if (direction == VAL_INCREASE) {
                frameBlankDelay += 1;
            }else{
                if (frameBlankDelay > 1) {
                    frameBlankDelay -= 1;
                }
            }
            break;
        case MI_CYCLE_ALL_IMAGES:
            cycleAllImages = !cycleAllImages;
            break;
    }
}

void move(byte direction) {
#ifdef SERIALDEBUG
    Serial.print("Menu move ");
    Serial.println( (direction == VAL_INCREASE) ? "UP" : "DOWN" );
#endif
    loopCounter = 0;
    updateScreen = true;  
    if (direction == VAL_INCREASE) {
        if (activeMenuEntry == 1) {
            activeMenuEntry = 8;
        }
        else {
            activeMenuEntry -= 1;
        }
    }else{
        if (activeMenuEntry == 8) {
            activeMenuEntry = 1;
        }
        else {
            activeMenuEntry += 1;
        }
    }
}

int keypadRead() {
    adKeyIn = analogRead( 0 );
    digitalWrite( AD_KEYBOARD, HIGH );
    adKey = getKey(adKeyIn);

    if (adKey != adKeyOld) {
        delay( 100 );
        adKeyIn = analogRead(0);
        adKey = getKey( adKeyIn );
        if ( adKey != adKeyOld) {
            adKeyOld = adKey;
            if ( adKey >=0 ){
                return adKey;
            }
        }
    }
    return adKey;
}

int getKey(unsigned int input) {
    int k;
    for (k = 0; k < BTN_AMOUNT; k++) {
        if (input < adcKeyVal[k]) {
            return k;
        }
    }
    if (k >= BTN_AMOUNT)
        k = -1;
    return k;
}


void appHandler() {
    if (updateScreen){
        updateScreen = false;
        showMenuEntry(activeMenuEntry);
    }
#ifdef DEMOMODE
    delay(2000);
    activeMenuEntry++;
    if(activeMenuEntry<=8){
      updateScreen = true;
    }else{
      wifiMode = true;
    }
#else
    menuHanlder();
#endif
}

MD5Builder md5;
char* firmwareFile = "fwupdate.bin";
String firmwareVer = "1.05";

void updateFw() {
    if (sd.exists(firmwareFile)) {              
        File updateFile;
#ifdef SERIALDEBUG        
        Serial.println("Update file found");
#endif
        updateFile = sd.open(firmwareFile, FILE_READ);
        if (updateFile) {
            size_t updateSize = updateFile.size();
            if (updateSize > 0) {
                md5.begin();
                md5.addStream(updateFile, updateSize);
                md5.calculate();
                String md5Hash = md5.toString();
#ifdef SERIALDEBUG                
                Serial.println("Update file hash: " + md5Hash);
#endif
                updateFile.close();
                updateFile = sd.open(firmwareFile, FILE_READ);
                if (updateFile) {
                    uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
                    if (!Update.begin(maxSketchSpace, U_FLASH)) {
                        Update.printError(Serial);
                        updateFile.close();
                        return;
                    }
                    int md5BufSize = md5Hash.length() + 1;
                    char md5Buf[md5BufSize];
                    md5Hash.toCharArray(md5Buf, md5BufSize);
                    Update.setMD5(md5Buf);
#ifdef SERIALDEBUG                   
                    Serial.println("Updating firmware...");
#endif
                    long bsent = 0;
                    int cprog = 0;
                    while (updateFile.available()) {
                        uint8_t ibuffer[1];
                        updateFile.read((uint8_t *) ibuffer, 1);
                        Update.write(ibuffer, sizeof(ibuffer));
                        bsent++;
                        int progr = ((double) bsent / updateSize) * 100;
                        if (progr >= cprog) {
                            cprog = progr + 10;
#ifdef SERIALDEBUG
                            Serial.println(String(progr) + "%");
#endif
                        }
                    }
                    updateFile.close();
                    if (Update.end(true)) {
#ifdef SERIALDEBUG
                        Serial.println("Installed firmware hash: " + Update.md5String());
                        Serial.println("Update complete");
#endif
                        sd.remove(firmwareFile);
                        ESP.restart();
                    }
                    else {
#ifdef SERIALDEBUG
                        Serial.println("Update failed");
                        Update.printError(Serial);
#endif
                    }
                }
            }
            else {
#ifdef SERIALDEBUG
                Serial.println("Error, file is invalid");
#endif
                updateFile.close();
                sd.remove(firmwareFile);
                return;
            }
        }
    }
#ifdef SERIALDEBUG
    else { 
      Serial.println("No update file found");  
    }
#endif
}

/*
  
  ██████╗ ███████╗███████╗████████╗███████╗██╗   ██╗██╗     
  ██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔════╝██║   ██║██║     
  ██████╔╝█████╗  ███████╗   ██║   █████╗  ██║   ██║██║     
  ██╔══██╗██╔══╝  ╚════██║   ██║   ██╔══╝  ██║   ██║██║     
  ██║  ██║███████╗███████║   ██║   ██║     ╚██████╔╝███████╗
  ╚═╝  ╚═╝╚══════╝╚══════╝   ╚═╝   ╚═╝      ╚═════╝ ╚══════╝

*/

void restGETImages() {
#ifdef SERIALDEBUG
  Serial.println("RESTFUL => GET => images");
  Serial.print("Found ");
  Serial.print(amountOfUsableFiles);
  Serial.println(" useable Images");    
#endif
  StaticJsonDocument<200> doc;
  JsonArray data = doc.createNestedArray("data");
  for( int i = 0; i <= (amountOfUsableFiles -1); i++ ) {
    String entry = fileNames[i];
    data.add(entry);
#ifdef SERIALDEBUG
    Serial.print(i);
    Serial.print(". ");
    Serial.println(entry);
#endif  
  }
  int contentLength = measureJsonPretty(doc);
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  String content = "";
  serializeJson(doc, content);
#ifdef SERIALDEBUG
  Serial.println("RESTFUL => send RESPONSE");
  Serial.print("with ContentLeng ");
  Serial.println(contentLength);
  Serial.println(content);
#endif 
  server.send(200, "application/json", content);
  WiFiClient client = server.client();
  server.sendContent(F(""));
  client.stop();
  getFilenamesFromImageFolder();
  return ;
}

void restDELETEImage() {
if (server.args() == 0) {
      server.send(500, "text/plain", "BAD ARGS");
      return;
    }
#ifdef SERIALDEBUG
    Serial.println("DELETEFILE: " + server.arg(0));
#endif   
    String filename = server.arg(0);
    int lastIndexOfSlash = filename.lastIndexOf("=");
    lastIndexOfSlash++;
    String checkFilename = filename.substring(0, lastIndexOfSlash -1);
    if (checkFilename != "filename") {
       server.send(400, "text/plain", "");
       return;
    }
    String subFilename = filename.substring(lastIndexOfSlash);
    filename = subFilename;   
    filename.toLowerCase();
    if (!filename.endsWith(".bmp")) {
       server.send(403, "text/plain", "");
       return;
    }
    sdDirectory.open(IMAGE_FOLDER_SD, O_READ);
    if(sdDirectory.exists(filename.c_str())) {
      filename = String(IMAGE_FOLDER_SD) + "/" + filename;
#ifdef SERIALDEBUG      
      Serial.println("handleFileDelete: " + filename);
      sd.remove(filename.c_str());
      if(!sdDirectory.exists(filename.c_str())) {
        Serial.println("DELETED: " + filename); 
      }
#ifdef SERIALDEBUG    
      sdDirectory.ls();
#endif
      server.send(200, "text/plain", "DELETED");
#endif
    } else {
#ifdef SERIALDEBUG     
      Serial.println("File " + filename + " does not exist"); 
#endif
      server.send(200, "text/plain", "FileNotFound");
    }
    getFilenamesFromImageFolder();
    return ;
}

/*
  
  ██╗    ██╗███████╗██████╗ ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗ 
  ██║    ██║██╔════╝██╔══██╗██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗
  ██║ █╗ ██║█████╗  ██████╔╝███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝
  ██║███╗██║██╔══╝  ██╔══██╗╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗
  ╚███╔███╔╝███████╗██████╔╝███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║
   ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝
                                                                           
*/

bool loadFromSdCard(String filePath) {
    String loadStringFromSD = filePath;  
#ifdef SERIALDEBUG
    Serial.print( "Client asked for: ");
    Serial.println( loadStringFromSD );
 #endif     
 
    if(sdDirectory.isOpen())
    {
        sdDirectory.close();
    }
    int lastIndexOfSlash = loadStringFromSD.lastIndexOf("/");
    lastIndexOfSlash--;
    String path = loadStringFromSD.substring(0, lastIndexOfSlash);    
    String contentType = getContentType(loadStringFromSD);
#ifdef HTTP_GZ_ACTIVE
    String pathWithGz = loadStringFromSD + ".gz";
    if (sd.exists(pathWithGz.c_str()) ) {
        loadStringFromSD = pathWithGz;    
        server.sendHeader("Content-Encoding", "gzip");
        server.sendHeader("Vary", "Accept-Encoding");
    } else {
        Serial.print( " Not found gz ");
        Serial.print( loadStringFromSD.c_str() );
    }  
#endif   
    sdFileToSend = sd.open(loadStringFromSD.c_str(), O_READ);
#ifdef SERIALDEBUG
    Serial.println( "Sending File" + filePath );
#endif
    if (server.streamFile(sdFileToSend, contentType) != sdFileToSend.size()) {
#ifdef SERIALDEBUG
        Serial.println("Sent less data than expected!");
#endif
    }
    sdFileToSend.close();     
    return true;
}
bool loadFromSpiffs(String filePath) {
    fs::File fileToSendSpiffs = SPIFFS.open(filePath, "r");
    if (fileToSendSpiffs)
    {
      String contentType = getContentType(filePath);
      if (server.streamFile(fileToSendSpiffs, contentType) != fileToSendSpiffs.size()) {
        #ifdef SERIALDEBUG
          Serial.println("Sent less data than expected!");
        #endif
      }
      fileToSendSpiffs.close();
      return true;
    }
    else
        return false;
}

File filToUpload;

void handleFileUpload() {
    HTTPUpload& hfUpload = server.upload();
    String uploadpath = "/";  
    if (hfUpload.status == UPLOAD_FILE_START) {
      String filename;
        filename = hfUpload.filename;
        if(!filename.startsWith("/")) {
          filename = "/" + filename;
        }
        int lastIndexOfSlash = filename.lastIndexOf("/");
        lastIndexOfSlash++;
        String subFilename = filename.substring(lastIndexOfSlash);        
        filename = subFilename;   
        filename.toLowerCase();
#ifdef SERIALDEBUG        
        Serial.print("lowercase testfilename: ");
        Serial.println(filename);   
#endif    
        if (
          filename.endsWith(".htm")  ||
          filename.endsWith(".html") ||
          filename.endsWith(".css")  ||
          filename.endsWith(".js")   ||
          filename.endsWith(".svg")  ||
          filename.endsWith(".woff2")
          ) {
            uploadpath = "/web/";
        }       
        if (filename.endsWith(".bmp")) {
          uploadpath = String(IMAGE_FOLDER_SD) + "/";
        }        
#ifdef SERIALDEBUG        
        Serial.print("Uploadpath chosen: ");
        Serial.println(uploadpath);   
#endif
        if ( uploadpath == "/" ) {
          server.send(403);
        }
        String fullfileName = uploadpath + filename;     
#ifdef SERIALDEBUG           
        Serial.print("Check if file already exists: ");
        Serial.println(fullfileName);  
#endif
        while (sd.exists(fullfileName.c_str()))
        {
#ifdef SERIALDEBUG          
          Serial.print("File exist and will be removed");
#endif          
          sd.remove(fullfileName.c_str());
          delay(200);
        }
        (filToUpload = sd.open(fullfileName.c_str(), FILE_WRITE)) ? Serial.println("SD opening successfull") : Serial.println("SD opening failed");
        filToUpload.seek(0);
#ifdef SERIALDEBUG
        Serial.print("Upload: START, filename: ");
        Serial.println(filename);
#endif
    } else if (hfUpload.status == UPLOAD_FILE_WRITE) {
        if (filToUpload) {
            filToUpload.write(hfUpload.buf, hfUpload.currentSize);
        }
#ifdef SERIALDEBUG
          Serial.print("Upload: WRITE, Bytes: ");
          Serial.println(hfUpload.currentSize);
#endif
    } else if (hfUpload.status == UPLOAD_FILE_END) {
        if (filToUpload) {
            filToUpload.close();
        }
#ifdef SERIALDEBUG
        Serial.print("Upload: END, Size: ");
        Serial.println(hfUpload.totalSize);
#endif
        if ( uploadpath == String(IMAGE_FOLDER_SD) + "/" ) {
          getFilenamesFromImageFolder();
        }
    }
}

#ifdef SPECIAL_SETUP_MODE
File fsUploadFile2Sd;
void handleFileUpload2Sd() {
  HTTPUpload& sdUpload = server.upload();
  if(sdUpload.status == UPLOAD_FILE_START){
    Serial.println("UPLOAD_FILE_START");
    String sdFilename = sdUpload.filename;
    if(!sdFilename.startsWith("/"))
      sdFilename  = "/"+sdFilename ;
    Serial.print("sdFilename Name: ");
    Serial.println(sdFilename );
    fsUploadFile2Sd = sd.open(sdFilename.c_str(), O_WRITE);
    sdFilename  = String();
  } else if(sdUpload.status == UPLOAD_FILE_WRITE){
    Serial.println("UPLOAD_FILE_WRITE");
    if(fsUploadFile2Sd) {
      fsUploadFile2Sd.write(sdUpload.buf, sdUpload.currentSize);
    }
  } else if(sdUpload.status == UPLOAD_FILE_END){
    Serial.println("UPLOAD_FILE_END");
    if(fsUploadFile2Sd) {      
      Serial.println("fsUploadFile"); 
      fsUploadFile2Sd.close();      
      Serial.print("handleFileUpload Size: "); 
      Serial.println(sdUpload.totalSize);
      server.sendHeader("Location","/web/index.html");
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

fs::File fsUploadFile2Spiffs;
  void handleFileUpload2Spiffs() {
  HTTPUpload& spiffsUpload = server.upload();
  if(spiffsUpload.status == UPLOAD_FILE_START){
    Serial.println("UPLOAD_FILE_START");
    String spiffsFilename = spiffsUpload.filename;
    if(!spiffsFilename .startsWith("/"))
      spiffsFilename  = "/"+spiffsFilename ;
    Serial.print("handleFileUpload Name: ");
    Serial.println(spiffsFilename );
    fsUploadFile2Spiffs = SPIFFS.open(spiffsFilename , "w");
    spiffsFilename  = String();
  } else if(spiffsUpload.status == UPLOAD_FILE_WRITE){
    Serial.println("UPLOAD_FILE_WRITE");
    if(fsUploadFile2Spiffs) {
      fsUploadFile2Spiffs.write(spiffsUpload.buf, spiffsUpload.currentSize);
    }
  } else if(spiffsUpload.status == UPLOAD_FILE_END){
    Serial.println("UPLOAD_FILE_END");
    if(fsUploadFile2Spiffs) {      
      Serial.println("fsUploadFile"); 
      fsUploadFile2Spiffs.close();
      Serial.print("handleFileUpload Size: "); 
      Serial.println(spiffsUpload.totalSize);
      server.sendHeader("Location","/web/index.spiffs");
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

void handleFileUpload2Page() {
    String htmlContent;
    const char INDEX_HTML[] = R"=====(
    <!DOCTYPE html>
    <html>
      <form method='post' enctype='multipart/form-data'><input type='file' name='name'><input class='button' type='submit' value='Upload'></form>
    </html>
    )=====" ;
    server.send(200, "text/html", INDEX_HTML);
    WiFiClient client = server.client();
    client.stop();
}
#endif


void handleNotFound() {
    if (loadFromSdCard(server.uri())) {
        return;
    }
    if (loadFromSpiffs(server.uri())) {
        return;
    }
    server.send(404, "text/plain", "");
#ifdef SERIALDEBUG
    Serial.print("File not found");
#endif
}


void returnAccepted() {
#ifdef SERIALDEBUG
    Serial.println("Sending HTTP 202");
#endif
    server.send(202, "text/plain", "");
}


void handleNeedsSetup() {
    server.send(200, "text/html", HTML_HEADER);
    WiFiClient client = server.client();
    server.sendContent("<h1> Please setup your sd card before wifi usage</h1><p>Maybe you want to #define SPECIAL_SETUP_MODE for an unrestricted setup mode.</p>");
    server.sendContent(HTML_END);
    server.sendContent(" ");
    client.stop();
}

void returnOK() {
#ifdef SERIALDEBUG
    Serial.println("Sending HTTP 200 ");
#endif
    server.send(200, "text/plain", "");
}

void toConfig() {
  String configSD = "/web/index.html";
  if (sd.exists(configSD.c_str())) {
#ifdef SERIALDEBUG
    Serial.println("Redirecting HTTP 307 to SD Card");
#endif
    server.sendHeader("Location", String("/web/index.html"), true);
    server.send ( 307, "text/plain", "Config available on SD Card");
  } else {
#ifdef SERIALDEBUG
    Serial.println("Sending Roor Hanlde");
#endif
    handleNeedsSetup();
  }
}

void setupServer() {
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.println("Setup Mdns");
#endif
    if (MDNS.begin(host)) {
#ifndef STARTSSL
        MDNS.addService("http", "tcp", 80);
#else
        MDNS.addService("https", "tcp", 443);
#endif
#ifdef SERIALDEBUG
        Serial.println("╚> MDNS responder started");
        Serial.print("╚> You can now connect to http");
#ifndef STARTSSL
        Serial.print("s");
#endif
        Serial.print("://");
        Serial.print(host);
        Serial.println(".local");
#endif
    }else {
#ifdef SERIALDEBUG
      Serial.println("╚> failed");
#endif
    }
#ifdef SERIALDEBUG
    Serial.println(" ");
    Serial.println("Setup Webserver Endpoints");
#endif

/*

  ███████╗███████╗██████╗ ██╗   ██╗███████╗██████╗      ██████╗ ███╗   ██╗
  ██╔════╝██╔════╝██╔══██╗██║   ██║██╔════╝██╔══██╗    ██╔═══██╗████╗  ██║
  ███████╗█████╗  ██████╔╝██║   ██║█████╗  ██████╔╝    ██║   ██║██╔██╗ ██║
  ╚════██║██╔══╝  ██╔══██╗╚██╗ ██╔╝██╔══╝  ██╔══██╗    ██║   ██║██║╚██╗██║
  ███████║███████╗██║  ██║ ╚████╔╝ ███████╗██║  ██║    ╚██████╔╝██║ ╚████║
  ╚══════╝╚══════╝╚═╝  ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝     ╚═════╝ ╚═╝  ╚═══╝
                                                                          
*/
    server.on("/", toConfig);
    server.on("/config", toConfig);
    server.on("/images/", HTTP_GET, restGETImages);
    server.on("/images/", HTTP_DELETE, restDELETEImage);
    server.on("/images/", HTTP_POST, []() {
        server.send(200);
    }, handleFileUpload);
    server.on("/images/", HTTP_PUT, []() {
        returnAccepted();
    }, handleFileUpload);
#ifdef SPECIAL_SETUP_MODE
    server.on("/upload", HTTP_GET, handleFileUpload2Page);    
    server.on("/upload", HTTP_POST, [](){ server.send(200); },
      handleFileUpload
    );
    server.on("/uploadSd", HTTP_GET, handleFileUpload2Page);    
    server.on("/uploadSd", HTTP_POST, [](){ server.send(200); },
      handleFileUpload2Sd
    );    
    server.on("/uploadSpiffs", HTTP_GET, handleFileUpload2Page);    
    server.on("/uploadSpiffs", HTTP_POST, [](){ server.send(200); },
      handleFileUpload2Spiffs
    );
#endif
    server.serveStatic("/web/Material.woff2", SPIFFS, "/Material.woff2");
    server.serveStatic("/favicon.ico", SPIFFS, "/favicon.ico");
    server.onNotFound(handleNotFound);
    server.begin();
#ifdef SERIALDEBUG
    Serial.println("╚> done");
    Serial.println(" ");
    Serial.println("██████╗ ███████╗ █████╗ ██████╗ ██╗   ██╗");
    Serial.println("██╔══██╗██╔════╝██╔══██╗██╔══██╗╚██╗ ██╔╝");
    Serial.println("██████╔╝█████╗  ███████║██║  ██║ ╚████╔╝ ");
    Serial.println("██╔══██╗██╔══╝  ██╔══██║██║  ██║  ╚██╔╝  ");
    Serial.println("██║  ██║███████╗██║  ██║██████╔╝   ██║   ");
    Serial.println("╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝    ╚═╝   ");
    Serial.println(" ");                                         
#endif
}

/*

  ██╗    ██╗██╗███████╗██╗
  ██║    ██║██║██╔════╝██║
  ██║ █╗ ██║██║█████╗  ██║
  ██║███╗██║██║██╔══╝  ██║
  ╚███╔███╔╝██║██║     ██║
   ╚══╝╚══╝ ╚═╝╚═╝     ╚═╝
                        

*/

void setupWifi() {
#ifdef SERIALDEBUG
  Serial.println(" ");
  Serial.println("START WIFI");
#endif

  if( custom_wifi_mode == "STA" ) {
    #ifdef SERIALDEBUG
      Serial.println("╚> Trying to connect to Wifi Network");
      Serial.println("╚=> SSID" + custom_wifi_ssid);
      Serial.println("╚=> PW" + custom_wifi_password);
      Serial.println("╚=> MODE" + custom_wifi_mode);
    #endif
    fallbackWifi = false;
    WiFi.mode(WIFI_STA);
    WiFi.begin(custom_wifi_ssid.c_str(), custom_wifi_password.c_str());
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      #ifdef SERIALDEBUG
        Serial.println("╚==> Connection Failed!");
      #endif
      delay(5000);
      fallbackWifi = true;
    } // while WiFi.waitForConnectResult
    if ( fallbackWifi == false )
      serverIp = WiFi.localIP();
  }
  if ( custom_wifi_mode != "STA" && custom_wifi_ssid != "" && custom_wifi_password != "") {
#ifdef SERIALDEBUG
    Serial.println("╚> Trying to start WIFI with values from SPIFFS");
    Serial.println("╚=> SSID" + custom_wifi_ssid);
    Serial.println("╚=> PW" + custom_wifi_password);
    Serial.print("╚=> MODE");
    Serial.println(WIFI_AP);
#endif
    WiFi.mode(WIFI_AP);
    if (!WiFi.softAP(custom_wifi_ssid.c_str(), custom_wifi_password.c_str())) {
#ifdef SERIALDEBUG
        Serial.println("╚==> Connection Failed!");
#endif
        fallbackWifi = true;
    }else{
      WiFi.softAPConfig(local_ip, gateway, subnet);
      serverIp = WiFi.softAPIP();
    }    
  }
  if(fallbackWifi) {
 #ifdef SERIALDEBUG
    Serial.println("╚> Starting WIFI with default values:");
    Serial.print("╚=> SSID: ");
    Serial.println(wifi_ssid);
    Serial.print("╚=> PW: ");
    Serial.println(wifi_password);
    Serial.print("╚=> Wifi Mode");
    Serial.println(WIFI_AP);
#endif   
    WiFi.mode(WIFI_AP);  
    if (!WiFi.softAP(wifi_ssid, wifi_password) ) {
        Serial.println("╚==> Wifi Failed!");
        return;
    }      
    WiFi.softAPConfig(local_ip, gateway, subnet);
    serverIp = WiFi.softAPIP();
  } 
#ifdef SERIALDEBUG
  if( custom_wifi_mode == "STA" ) {
    long rssi = WiFi.RSSI();
    Serial.print("╚==> signal strength (RSSI):");
    Serial.println(rssi);
  }
  Serial.print("╚==> WIFI started at ");
  Serial.println(serverIp);
#endif
}

void wifiHandler() {
    server.handleClient();
    MDNS.update();
    int keypress = -1;
    keypress = keypadRead();
    if( keypress == BTN_SELECT ) {
      Serial.println("==> Leaving WIFI mode now.");
      wifiMode = false;
    }
}

/*
  
  ██████╗ ███╗   ███╗██████╗     ██████╗ ███████╗ █████╗ ██████╗ 
  ██╔══██╗████╗ ████║██╔══██╗    ██╔══██╗██╔════╝██╔══██╗██╔══██╗
  ██████╔╝██╔████╔██║██████╔╝    ██████╔╝█████╗  ███████║██║  ██║
  ██╔══██╗██║╚██╔╝██║██╔═══╝     ██╔══██╗██╔══╝  ██╔══██║██║  ██║
  ██████╔╝██║ ╚═╝ ██║██║         ██║  ██║███████╗██║  ██║██████╔╝
  ╚═════╝ ╚═╝     ╚═╝╚═╝         ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═════╝ 
                                                                 

*/

inline byte gamma() {
    byte x = readFileByte();
    return pgm_read_byte(&gammaTable[x]);
}

void getRGBwithGamma() {
    greenValue  = gamma() * (brightness * 0.01);
    blueValue   = gamma() * (brightness * 0.01);
    redValue    = gamma() * (brightness * 0.01);
}

int readFileByte() {
    int retbyte =- 1;
    while ( retbyte < 0 )
        retbyte= sdFileToSend.read();
    return retbyte;
}

uint32_t readFileLong() {
    uint32_t retValue;
    byte incomingbyte;

    incomingbyte=readFileByte();
    retValue=(uint32_t)((byte)incomingbyte);

    incomingbyte=readFileByte();
    retValue+=(uint32_t)((byte)incomingbyte)<<8;

    incomingbyte=readFileByte();
    retValue+=(uint32_t)((byte)incomingbyte)<<16;

    incomingbyte=readFileByte();
    retValue+=(uint32_t)((byte)incomingbyte)<<24;

    return retValue;
}

uint16_t readFileInt() {
    byte incomingbyte;
    uint16_t retValue;

    incomingbyte=readFileByte();
    retValue+=(uint16_t)((byte)incomingbyte);

    incomingbyte=readFileByte();
    retValue+=(uint16_t)((byte)incomingbyte)<<8;

    return retValue;
}

/*

   █████╗ ██████╗ ██████╗ ██╗   ██╗██╗███╗   ██╗ ██████╗ 
  ██╔══██╗██╔══██╗██╔══██╗██║   ██║██║████╗  ██║██╔═══██╗
  ███████║██████╔╝██║  ██║██║   ██║██║██╔██╗ ██║██║   ██║
  ██╔══██║██╔══██╗██║  ██║██║   ██║██║██║╚██╗██║██║   ██║
  ██║  ██║██║  ██║██████╔╝╚██████╔╝██║██║ ╚████║╚██████╔╝
  ╚═╝  ╚═╝╚═╝  ╚═╝╚═════╝  ╚═════╝ ╚═╝╚═╝  ╚═══╝ ╚═════╝ 
                                                         
  ███╗   ███╗ █████╗ ██╗███╗   ██╗                       
  ████╗ ████║██╔══██╗██║████╗  ██║                       
  ██╔████╔██║███████║██║██╔██╗ ██║                       
  ██║╚██╔╝██║██╔══██║██║██║╚██╗██║                       
  ██║ ╚═╝ ██║██║  ██║██║██║ ╚████║                       
  ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝                       
                                                       

*/

void setup() {
#ifdef SERIALDEBUG
  Serial.begin(9600);
  while(!Serial) {
    delay(10000);
  }
  Serial.println("");
  kickstart();
  Serial.println(" ");
  Serial.println("#define SERIALDEBUG active");
#endif
#ifdef STARTSSL
  Serial.println("#define STARTSSL active");
#endif
#ifdef HTTP_GZ_ACTIVE
  Serial.println("#define HTTP_GZ_ACTIVE active");
#endif
#ifdef FORMATSPIFFS
  Serial.println("#define FORMATSPIFFS active");
#endif
    checkSPIFFSConfig();
    if(!checkConfig(ledColorOrder)) {
        runErrorRutine("Config LED Color order Mismatch");
        delay(1000);
        while(1){
            delay(1000);
        }
    }
    setupDisplay();
    setupSDCardStorage();
    setupLedStripe();
    setupWifi();
    setupServer();
    updateFw();
#ifdef DEMOMODE
    wifiMode = false;
    updateScreen = true;
    activeMenuEntry = 1;
#endif
    display.clearDisplay();
    display.display();
}

void loop() {
    if( wifiMode ) {
        wifiHandler();
    } else {
        appHandler();
    }
}

/*

  ████████╗██╗  ██╗███████╗    ██╗   ██╗ ██████╗ ██╗  ██╗   ██╗
  ╚══██╔══╝██║  ██║██╔════╝    ██║   ██║██╔════╝ ██║  ╚██╗ ██╔╝
     ██║   ███████║█████╗      ██║   ██║██║  ███╗██║   ╚████╔╝ 
     ██║   ██╔══██║██╔══╝      ██║   ██║██║   ██║██║    ╚██╔╝  
     ██║   ██║  ██║███████╗    ╚██████╔╝╚██████╔╝███████╗██║   
     ╚═╝   ╚═╝  ╚═╝╚══════╝     ╚═════╝  ╚═════╝ ╚══════╝╚═╝   
                                                             

*/

const char HTML_HEADER[] =
        "<!DOCTYPE HTML>"
                "<html>"
                "<head>"
                "<meta name = \"viewport\" content = \"width = device-width, initial-scale "
                "= 1.0, maximum-scale = 1.0, user-scalable=0\">"
                "<title>ESPLightStick - SETUP</title>"
                "<style>"
                "body { background-color: #8d9aa5; color: #333; font-family: Arial, Helvetica,""Sans-Serif; Color: #000000;font-size:12pt; }"
                "</style>"
                "</head>"
                "<body><div style='margin-left:30px;'>";

const char HTML_END[] =
        "</div></body>"
                "</html>";

void kickstart() {
  Serial.print("███████╗███████╗██████╗     ██╗     ██╗");
  Serial.print(" ██████╗ ██╗  ██╗████████╗███████╗████");
  Serial.println(" ████╗██╗ ██████╗██╗  ██╗");
  Serial.print("██╔════╝██╔════╝██╔══██╗    ██║     ██║");
  Serial.print("██╔════╝ ██║  ██║╚══██╔══╝██╔════╝╚══█");
  Serial.println("█╔══╝██║██╔════╝██║ ██╔╝");
  Serial.print("█████╗  ███████╗██████╔╝    ██║     ██║");
  Serial.print("██║  ███╗███████║   ██║   ███████╗   █");
  Serial.println("█║   ██║██║     █████╔╝");
  Serial.print("██╔══╝  ╚════██║██╔═══╝     ██║     ██║");
  Serial.print("██║   ██║██╔══██║   ██║   ╚════██║   █");
  Serial.println("█║   ██║██║     ██╔═██╗");
  Serial.print("███████╗███████║██║         ███████╗██║");
  Serial.print("╚██████╔╝██║  ██║   ██║   ███████║   █");
  Serial.println("█║   ██║╚██████╗██║  ██╗");
  Serial.print("╚══════╝╚══════╝╚═╝         ╚══════╝╚═╝");
  Serial.print("╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚══════╝   ╚");
  Serial.println("═╝   ╚═╝ ╚═════╝╚═╝  ╚═╝");
}

const uint8_t gammaTable[] PROGMEM = {
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
        1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,
        2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,
        4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  6,  7,  7,
        7,  7,  7,  8,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 10, 11,
        11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 16, 16,
        16, 17, 17, 17, 18, 18, 18, 19, 19, 20, 20, 21, 21, 21, 22, 22,
        23, 23, 24, 24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30,
        30, 31, 32, 32, 33, 33, 34, 34, 35, 35, 36, 37, 37, 38, 38, 39,
        40, 40, 41, 41, 42, 43, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50,
        50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62,
        62, 63, 64, 65, 66, 67, 67, 68, 69, 70, 71, 72, 73, 74, 74, 75,
        76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
        92, 93, 94, 95, 96, 97, 98, 99,100,101,102,104,105,106,107,108,
        109,110,111,113,114,115,116,117,118,120,121,122,123,125,126,127,
};
