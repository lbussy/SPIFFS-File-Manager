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

#ifndef _EDITPAGES_H
#define _EDITPAGES_H
#pragma once

#include <Arduino.h>

const char edit_htm[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">

<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta charset="UTF-8">
</head>

<body>
    <h1>File System:</h1>
    <p>
        Free Storage: <span id="freespiffs">%FREESPIFFS%</span> <br />
        Used Storage: <span id="usedspiffs">%USEDSPIFFS%</span> <br />
        Total Storage: <span id="totalspiffs">%TOTALSPIFFS%</span>
    </p>
    <p>
        <button onclick="listFiles()">List Files</button>
        <button onclick="showUpload()">Upload File</button>
    </p>
    <p id="status"></p>
    <p id="detailsheader"></p>
    <p id="details"></p>
    <script>

        function listFiles() {
            xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", "/listfiles", false);
            xmlhttp.send();
            document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
            document.getElementById("details").innerHTML = xmlhttp.responseText;
        }

        function downloadDeleteButton(filename, action) {
            var urltocall = "/file?name=" + filename + "&action=" + action;
            xmlhttp = new XMLHttpRequest();
            if (action == "delete") {
                xmlhttp.open("GET", urltocall, false);
                xmlhttp.send();
                document.getElementById("status").innerHTML = xmlhttp.responseText;
                xmlhttp.open("GET", "/listfiles", false);
                xmlhttp.send();
                document.getElementById("details").innerHTML = xmlhttp.responseText;
            }
            if (action == "download") {
                document.getElementById("status").innerHTML = "";
                window.open(urltocall, "_blank");
            }
        }

        function showUpload() {
            document.getElementById("detailsheader").innerHTML = "<h3>Upload File<h3>"
            document.getElementById("status").innerHTML = "";
            var uploadform =
                '<form method = "POST" action = "/" enctype="multipart/form-data">' +
                '<input type="file" name="data"/>' +
                '<input type="submit" name="upload" value="Upload" title = "Upload File"></form>'
            document.getElementById("details").innerHTML = uploadform;
            var uploadform =
                '<form id="upload_form" enctype="multipart/form-data" method="post">' +
                '<input type="file" name="file1" id="file1" onchange="uploadFile()"><br>' +
                '<progress id="progressBar" value="0" max="100" style="width:300px;"></progress>' +
                '<h3 id="status"></h3>' +
                '<p id="loaded_n_total"></p>' +
                '</form>';
            document.getElementById("details").innerHTML = uploadform;
        }

        function _(el) {
            return document.getElementById(el);
        }

        function uploadFile() {
            var file = _("file1").files[0];
            var formdata = new FormData();
            formdata.append("file1", file);
            var ajax = new XMLHttpRequest();
            ajax.upload.addEventListener("progress", progressHandler, false);
            ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
            ajax.addEventListener("error", errorHandler, false);
            ajax.addEventListener("abort", abortHandler, false);
            ajax.open("POST", "/");
            ajax.send(formdata);
        }

        function progressHandler(event) {
            _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
            var percent = (event.loaded / event.total) * 100;
            _("progressBar").value = Math.round(percent);
            _("status").innerHTML = Math.round(percent) + "% uploaded, please wait";
            if (percent >= 100) {
                _("status").innerHTML = "Please wait, writing file to filesystem.";
            }
        }

        function completeHandler(event) {
            _("status").innerHTML = "Upload Complete";
            _("progressBar").value = 0;
            xmlhttp = new XMLHttpRequest();
            xmlhttp.open("GET", "/listfiles", false);
            xmlhttp.send();
            document.getElementById("status").innerHTML = "File Uploaded";
            document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
            document.getElementById("details").innerHTML = xmlhttp.responseText;
        }

        function errorHandler(event) {
            _("status").innerHTML = "Upload Failed";
        }

        function abortHandler(event) {
            _("status").innerHTML = "Upload Aborted";
        }

    </script>
</body>

</html>
)rawliteral";

#endif // _EDITPAGES_H
