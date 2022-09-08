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

#include "editserver.h"
#ifndef EDIT_EXTERNAL
#include "editpage.h"
#endif

struct Editserver
{
    char username[34];
    char password[34];
    char mountpoint[34];
};

Editserver es;

String editPageProcessor(const String &var)
{
    if (var == F("FREESPIFFS"))
    {
        return humanReadableSize((SPIFFS.totalBytes() - SPIFFS.usedBytes()));
    }

    if (var == F("USEDSPIFFS"))
    {
        return humanReadableSize(SPIFFS.usedBytes());
    }

    if (var == F("TOTALSPIFFS"))
    {
        return humanReadableSize(SPIFFS.totalBytes());
    }
    return "";
}

void configureEditPages(AsyncWebServer *editServer, char * username, char * password, char * mountpoint)
{
    strlcpy(es.username, username, sizeof(es.username));
    strlcpy(es.password, password, sizeof(es.password));
    // Add a leading slash to mountpoint for SPIFFS
    strcpy(es.mountpoint, "/");
    strcat(es.mountpoint, mountpoint);

#ifdef EDIT_DEBUG
    String debugLine = ("Mounting edit page at: " + String(es.mountpoint) + " with username: " + String(es.username) + ", password: " + String(es.password) + ".");
    debugPrint(debugLine);
#endif

    editServer->onFileUpload(handleUpload);

    editServer->on(es.mountpoint, HTTP_GET, [](AsyncWebServerRequest *request)
                   {
#ifdef EDIT_DEBUG
        String logmessage = "(Edit Page) Client: " + request->client()->remoteIP().toString() + +" " + request->url();
        debugPrint(logmessage);
#endif
        if (checkUserWebAuth(request))
        {
            AsyncWebServerResponse *response = request->beginResponse(200);
            response->addHeader("Cache-Control","no-cache");
#ifndef EDIT_EXTERNAL
            request->send_P(200, "text/html", edit_htm, editPageProcessor);
#else
            request->send(SPIFFS, "/edit.htm", String(), false, editPageProcessor);
#endif
        }
        else
        {
            return request->requestAuthentication();
        } });

    editServer->on("/listfiles", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
#ifdef EDIT_DEBUG
        String logmessage = "(List Files) Client: " + request->client()->remoteIP().toString() + " " + request->url();
        debugPrint(logmessage);
#endif
        if (checkUserWebAuth(request)) {
            request->send(200, "text/plain", listFiles(true));
        } else {
            return request->requestAuthentication();
        } });

    editServer->on("/file", HTTP_GET, [](AsyncWebServerRequest *request)
                   {
#ifdef EDIT_DEBUG
        String logmessage = "(File) Client: " + request->client()->remoteIP().toString() + " " + request->url();
        debugPrint(logmessage);
#endif
        if (checkUserWebAuth(request))
        {
            if (request->hasParam("name") && request->hasParam("action"))
            {
                char fileName[38];   // File path + name
                strcpy(fileName, "/");
                strcat(fileName, request->getParam("name")->value().c_str());
                const char *fileAction = request->getParam("action")->value().c_str();

#ifdef EDIT_DEBUG
                logmessage = "(File Action) Client: " + request->client()->remoteIP().toString() + " " + request->url() + "?name=" + String(fileName) + "&action=" + String(fileAction);
#endif
                if (!SPIFFS.exists(fileName))
                {
#ifdef EDIT_DEBUG
                    logmessage = "(File Action) ERROR: File does not exist.";
                    debugPrint(logmessage);
#endif
                    request->send(400, "text/plain", "ERROR: File does not exist");
                }
                else
                {
#ifdef EDIT_DEBUG
                    logmessage = "(File Action) File exists";
#endif
                    if (strcmp(fileAction, "download") == 0) {
#ifdef EDIT_DEBUG
                        logmessage += " - downloaded.";
#endif
                        request->send(SPIFFS, fileName, "application/octet-stream");
                    } else if (strcmp(fileAction, "delete") == 0) {
#ifdef EDIT_DEBUG
                        logmessage += " - deleted.";
#endif
                        SPIFFS.remove(fileName);
                        request->send(200, "text/plain", "Deleted File: " + String(fileName));
                    } else {
#ifdef EDIT_DEBUG
                        logmessage += " - ERROR: Invalid action param supplied.";
#endif
                        request->send(400, "text/plain", "ERROR: Invalid action param supplied");
                    }
#ifdef EDIT_DEBUG
                    debugPrint(logmessage);
#endif
                }
            }
            else
            {
                request->send(400, "text/plain", "ERROR: Filename and action params required");
            }
        }
        else
        {
            return request->requestAuthentication();
        } });
}

bool checkUserWebAuth(AsyncWebServerRequest *request)
{
    bool isAuthenticated = false;

    if (request->authenticate(es.username, es.password))
    {
#ifdef EDIT_DEBUG
        String logmessage = "(Auth) User is authenticated via username and password.";
        debugPrint(logmessage);
#endif
        isAuthenticated = true;
    }
    else
    {
#ifdef EDIT_DEBUG
        String logmessage = "(Auth) User not suthenticated.";
        debugPrint(logmessage);
#endif
    }
    return isAuthenticated;
}

void handleUpload(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final)
{
    if (checkUserWebAuth(request))
    {
        if (!index)
        {
#ifdef EDIT_DEBUG
            String logmessage = "(Upload File) Client: " + request->client()->remoteIP().toString() + " /" + filename;
            debugPrint(logmessage);
#endif
            String _filename = "/" + filename;
            request->_tempFile = SPIFFS.open(_filename, "w");
        }
        if (request->_tempFile)
        {
            if (len)
            {
                request->_tempFile.write(data, len);
            }
            if (final)
            {
#ifdef EDIT_DEBUG
                String logmessage = "(Upload File) Upload complete.";
                debugPrint(logmessage);
#endif
                request->_tempFile.close();
            }
        }        
    }

}

String listFiles(bool ishtml)
{
    String returnText = "";
    File root = SPIFFS.open("/");
    File foundfile = root.openNextFile();
    if (ishtml)
    {
        returnText += "<table><tr><th align='left'>Name</th><th align='left'>Size</th><th></th><th></th></tr>";
    }
    while (foundfile)
    {
        if (ishtml)
        {
            returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
            returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'download\')\">Download</button>";
            returnText += "<td><button onclick=\"downloadDeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">Delete</button></tr>";
        }
        else
        {
            returnText += "\t" + String(foundfile.name()) + " " + humanReadableSize(foundfile.size()) + "\n";
        }
        foundfile = root.openNextFile();
    }
    if (ishtml)
    {
        returnText += "</table>";
    }
    root.close();
    foundfile.close();
    return returnText;
}

String humanReadableSize(const size_t bytes)
{
    if (bytes < 1024)
        return String(bytes) + " B";
    else if (bytes < (1024 * 1024))
        return String(bytes / 1024.0) + " KB";
    else if (bytes < (1024 * 1024 * 1024))
        return String(bytes / 1024.0 / 1024.0) + " MB";
    else
        return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

#ifdef EDIT_DEBUG
void debugPrint(String &debugline)
{
    String printLine = "[SPIFFS-Edit] " + debugline;
    Serial.println(printLine);
}
#endif
