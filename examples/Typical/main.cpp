/* Copyright (C) 2019-2022 Lee C. Bussy (@LBussy)

This file is part of Lee Bussy's SPIFFS File Manager (SPIFFS-File-Manager).

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include "main.h"

Config config;
AsyncWebServer *server;

void setup()
{
    Serial.begin(BAUD);
    Serial.println("\nStarting Application.");
    if (!beginSPIFFS()) die();
    loadConfig();
    beginWiFi();
    if (!beginMDNS()) die();
    listNetworkInfo();
    beginWebServer();
    configureEditPages(server, config.edituser, config.editpassword, config.mountpoint);
    Serial.println("Application started.");
}

void loop() {}

void loadConfig()
{
    Serial.println("Loading Configuration.");
    strlcpy(config.ssid, ssid, sizeof(config.ssid));
    strlcpy(config.wifipassword, password, sizeof(config.wifipassword));
    strlcpy(config.hostname, hostname, sizeof(config.hostname));
    strlcpy(config.edituser, edituser, sizeof(config.edituser));
    strlcpy(config.editpassword, editpassword, sizeof(config.editpassword));
    strlcpy(config.mountpoint, mountpoint, sizeof(config.mountpoint));
    config.port = port;
}

bool beginSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("\nERROR: Cannot mount SPIFFS.");
        return false;
    }
    else
    {
        Serial.println("\nMounted SPIFFS:");
        Serial.print("SPIFFS Free:\t");
        Serial.println(humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes())));
        Serial.print("SPIFFS Used:\t");
        Serial.println(humanReadableSize(SPIFFS.usedBytes()));
        Serial.print("SPIFFS Total:\t");
        Serial.println(humanReadableSize(SPIFFS.totalBytes()));
        Serial.println(listFiles());
        return true;
    }
}

void beginWiFi()
{
    Serial.print("\nConnecting to Wifi ...");
    WiFi.mode(WIFI_MODE_NULL);
    WiFi.setHostname(config.hostname);
    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.wifipassword);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected.");
}

bool beginMDNS()
{
    if (!MDNS.begin(config.hostname))
    {
        Serial.println("Error starting mDNS.");
        return false;
    }
    else
    {
        Serial.print("mDNS started for ");
        Serial.print(WiFi.getHostname());
        Serial.println(".local.");
        return true;
    }
}

void beginWebServer()
{
    Serial.println("Configuring Webserver.");
    server = new AsyncWebServer(config.port);
    Serial.println("Starting Webserver.");
    server->begin();
    configureWebHandlers(server);
    Serial.print("Webserver started on: http://");
    Serial.print(WiFi.getHostname());
    Serial.println(".local");
}

void configureWebHandlers(AsyncWebServer *thisServer)
{
    thisServer->onNotFound([](AsyncWebServerRequest *request)
                           {
        Serial.print("(404 Response) Client: ");
        Serial.print(request->client()->remoteIP().toString());
        Serial.print(" ");
        Serial.println(request->url());
        request->send(404, "text/plain", "File not found."); });

    thisServer->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
        Serial.print("(Index File) Client: ");
        Serial.print(request->client()->remoteIP().toString());
        Serial.print(" ");
        Serial.println(request->url());
        request->send(SPIFFS, "/index.htm"); });
}

void listNetworkInfo()
{
    Serial.println("\n\nNetwork Configuration:");
    Serial.println("----------------------");
    Serial.print("SSID:\t\t");
    Serial.println(WiFi.SSID());
    Serial.print("Wifi Strength:\t");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    Serial.print("Hostname:\t");
    Serial.print(WiFi.getHostname());
    Serial.println(".local");
    Serial.print("MAC:\t\t");
    Serial.println(WiFi.macAddress());
    Serial.print("IP:\t\t");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet:\t\t");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway:\t");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS 1:\t\t");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2:\t\t");
    Serial.println(WiFi.dnsIP(1));
    Serial.print("DNS 3:\t\t");
    Serial.println(WiFi.dnsIP(2));
    Serial.println();
}

void die()
{
    while (1)
    {
        ;
    }
}
