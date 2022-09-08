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

#ifndef _MAIN_H
#define _MAIN_H
#pragma once

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <Arduino.h>
#include <ESPmDNS.h>

#define EDIT_DEBUG
#include "editserver.h"

// configuration structure
struct Config
{
    char ssid[33];         // Local WiFi SSID
    char wifipassword[33]; // Local WiFi Password
    char hostname[33];     // Controller Hostname
    char edituser[33];     // Username to access web admin
    char editpassword[33]; // Password to access web admin
    int port;              // Port number for web server
    char mountpoint[33];   // Where to mount the edit page
};

#if __has_include("./secret.h")
#include "secret.h"
const char *ssid = AP_NAME;
const char *password = PASSWORD;
const char *hostname = HOSTNAME;
const char *edituser = ADMIN_NAME;
const char *editpassword = ADMIN_PSWD;
const int port = PORT;
const char *mountpoint = MOUNTPOINT;
#else
const char *ssid = "my_ap";
const char *password = "passsword";
const char *hostname = "test";
const char *edituser = "admin";
const char *editpassword = "admin";
const int port = 80;
const char *mountpoint = "edit";
#endif

void setup();
void loop();
void loadConfig();
bool beginSPIFFS();
void beginWiFi();
bool beginMDNS();
void beginWebServer();
void listNetworkInfo();
void configureWebHandlers(AsyncWebServer *thisServer);
void die();

#endif // _MAIN_H
