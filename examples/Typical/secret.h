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

/* Edit the below and rename file to "secret.h" */

#ifndef _SECRET_H
#define _SECRET_H
#pragma once

#undef AP_NAME
#undef PASSWORD
#undef HOSTNAME
#undef ADMIN_NAME
#undef ADMIN_PSWD
#undef MOUNTPOINT
#undef PORT

#define AP_NAME "my_ssid"           // SSID of your local WiFi access point
#define PASSWORD "my_ssid_password" // Password for your SSID
#define HOSTNAME "filemanager"      // The hostname to be assigned to your controller
#define ADMIN_NAME "admin"          // The username for your edit page
#define ADMIN_PSWD "admin"          // The password for your edit page
#define MOUNTPOINT "edit"           // The mountpoint or route without any slashes
#define PORT 80                     // The port of your local web server

#endif // _SECRET_H
