#include "wifi.h"

AsyncWebServer server(80);
DNSServer dns;


void setupWiFi()
{
    // WiFiManager
    // Local intialization. Once its business is done, there is no need to keep it around
    AsyncWiFiManager wifiManager(&server, &dns);
    // reset saved settings
    // wifiManager.resetSettings();
    // set custom ip for portal
    // wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
    // fetches ssid and pass from eeprom and tries to connect
    // if it does not connect it starts an access point with the specified name
    // here  "AutoConnectAP"
    // and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("Leuchtturm");
    // or use this for auto generated name ESP + ChipID
    // wifiManager.autoConnect();
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

    
}

void handleWiFi()
{
    // if you get here you have connected to the
}

