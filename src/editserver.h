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

#ifndef _WEBSERVER_H
#define _WEBSEERVER_H
#pragma once

#include <Arduino.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

String editPageProcessor(const String &var);
void configureEditPages(AsyncWebServer &editServer, char * username, char * password, char * mountpoint);
bool checkUserWebAuth(AsyncWebServerRequest *request);
String listFiles(bool ishtml = false);
String humanReadableSize(const size_t bytes);
void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
#ifdef EDIT_DEBUG
void debugPrint(String &debugline);
#endif

#endif // _WEBSERVER_H
