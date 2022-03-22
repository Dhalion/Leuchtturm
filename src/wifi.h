#pragma once

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>     //Local WebServer used to serve the configuration portal
#include <ESPAsyncWiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic


void setupWiFi();
void handleWiFi();
void notFound(AsyncWebServerRequest *request);
