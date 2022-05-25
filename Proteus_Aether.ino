#include "MegunoLink.h"
#include "CommandHandler.h"
#include "PString.h"  // http://arduiniana.org/libraries/pstring/
#include <LiquidCrystal.h>
#include <Wire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>
#include <ThingSpeak.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <EEPROM.h>

int writeStringToEEPROM(int addrOffset, String &strToWrite) //https://roboticsbackend.com/arduino-write-string-in-eeprom/
{
  byte len = strToWrite.length();
  EEPROM.write(addrOffset, len);
  for (int i = 0; i < len; i++)
  {
    EEPROM.write(addrOffset + 1 + i, strToWrite[i]);
  }
  return addrOffset + 1 + len;
}
int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];
  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; 
  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}

unsigned long StartMillis;
unsigned long currentMillis;
unsigned long InitialThingSpeakMillis = millis();
unsigned long ThingSpeakMillis;
unsigned long delayThingSpeakMillis = 10000;



String String1;
String String2;

String String3;
String String4;

String String5;
String String6;

String String7;
String String8;

String String9 = " "; //to add a blank in the password box when entering the wifi name

String newStr1;
String newStr2;
String newStr3;
String newStr4;
String newStr5;

#define SECRET_CH_ID "1593183"
#define SECRET_WRITE_APIKEY "FVXY1T5RBOO7L1SN"


int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


const int SENSOR_PIN = 13; // Arduino pin connected to DS18B20 sensor's DQ pin

OneWire oneWire(SENSOR_PIN);         // setup a oneWire instance
DallasTemperature sensors(&oneWire); // pass oneWire to DallasTemperature library
LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x27, 16 column and 2 rows

float tempCelsius;    // temperature in Celsius

//Command Handler
CommandHandler<> SerialCommandHandler;
InterfacePanel MyPanel;



void Cmd_SETSYSTEM(CommandParameter &Parameters)
{  
  char buffer[1024]; //Create character buffer for PString to use
  PString String1(buffer, sizeof(buffer)); //Initialise PString object
  String1.print(Parameters.RemainingParameters()); //Print the characters from MegunoLink into PString buffer
  String2 = String1;
  int eepromOffset = 0;
  int String2AddrOffset = writeStringToEEPROM(eepromOffset, String2);
  int String4AddrOffset = writeStringToEEPROM(String2AddrOffset, String7);
 
       
}

void Cmd_SETSYSTEM2(CommandParameter &Parameters)
{  
  char buffer[1024]; //Create character buffer for PString to use
  PString String3(buffer, sizeof(buffer)); //Initialise PString object
  String3.print(Parameters.RemainingParameters()); //Print the characters from MegunoLink into PString buffer
  String4 = String3;
 int eepromOffset = 0;
 int String2AddrOffset = writeStringToEEPROM(eepromOffset, String2);
 int String4AddrOffset = writeStringToEEPROM(String2AddrOffset, String4);
    
}

void Cmd_SETEEPROM(CommandParameter &Parameters)
{  
 int eepromOffset = 0;
  String str1 = "EEPROM";
  String str2 = "Cleared";
  String str3 = "Thanks for reading!";
  int str1AddrOffset = writeStringToEEPROM(eepromOffset, str1);
  int str2AddrOffset = writeStringToEEPROM(str1AddrOffset, str2);
  writeStringToEEPROM(str2AddrOffset, str3);
}



void setup()
{
  Serial.begin(9600);  // Initialize serial

SerialCommandHandler.AddCommand(F("SETSYSTEM"), Cmd_SETSYSTEM);
SerialCommandHandler.AddCommand(F("SETSYSTEM2"), Cmd_SETSYSTEM2);
SerialCommandHandler.AddCommand(F("SETEEPROM"), Cmd_SETEEPROM);
  StartMillis = millis();
  
  sensors.begin();    // initialize the sensor
  lcd.init();         // initialize the lcd
  lcd.backlight();    // open the backlight 

  int eepromOffset = 0;
  int newStr1AddrOffset = readStringFromEEPROM(eepromOffset, &newStr1);
  int newStr2AddrOffset = readStringFromEEPROM(newStr1AddrOffset, &newStr2);
  readStringFromEEPROM(newStr2AddrOffset, &newStr3);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
    
  ThingSpeak.begin(client);  //Initialize ThingSpeak
     
}

void loop()
{SerialCommandHandler.Process();        // Check for serial commands and dispatch them.
currentMillis = millis();

sensors.requestTemperatures();             // send the command to get temperatures
  tempCelsius = sensors.getTempCByIndex(0);  // read temperature in Celsius
  lcd.clear();
  lcd.setCursor(0, 0);       // start to print at the first row - set the cursor to column 4, line 0
  lcd.print("Proteus Aether ");    // Machine name
  lcd.print("1");    // machine number
  lcd.setCursor(0, 1);       // start to print at the second row - set the cursor to column 4, line 1
  lcd.print("Temp    ");    // print the temperature 
  lcd.print(tempCelsius); // print the temperature in Celsius
  lcd.print((char)223);      // print ° character
  lcd.print("C");


int eepromOffset = 0;
int newStr1AddrOffset = readStringFromEEPROM(eepromOffset, &newStr1);
int newStr2AddrOffset = readStringFromEEPROM(newStr1AddrOffset, &newStr2);
readStringFromEEPROM(newStr2AddrOffset, &newStr3);
  
MyPanel.SetText(F("EEPROM1"), newStr1);
MyPanel.SetText(F("EEPROM2"), newStr2);


if((WiFi.status() != WL_CONNECTED) && (currentMillis-StartMillis >= 240000)){ // 4 minutes is 240000 milliseconds 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(String1);
    while(WiFi.status() != WL_CONNECTED){
    String ssid = newStr1;
    String pass = newStr2;
    char cssid[ssid.length()+1]; //https://forum.arduino.cc/t/string-to-char-array-conversion/425097
    char cpass[pass.length()+1];
    ssid.toCharArray(cssid, ssid.length()+1);
    pass.toCharArray(cpass, pass.length()+1);
    WiFi.begin(cssid, cpass);
    Serial.print(".");
     } 
Serial.println("\nConnected.");
  }
  
  
  // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
  // pieces of information in a channel.  Here, we write to field 1.
  ThingSpeakMillis = millis();
if (ThingSpeakMillis - InitialThingSpeakMillis > delayThingSpeakMillis){
ThingSpeak.writeField(myChannelNumber, 1, tempCelsius, myWriteAPIKey); // Wait 10 seconds to update the channel again
}

 

}
