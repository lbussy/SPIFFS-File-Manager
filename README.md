# SPIFFS File Manager

This is work I was inspired to create based on smford's [esp32-asyncwebserver-fileupload-example](https://github.com/smford/esp32-asyncwebserver-fileupload-example/).  With respect to smford's hard work, I could not use his examples in my projects due to a licensing conflict (I use MIT and he uses Apache 2.0) so I took what I learned and created my own.

I owe a lot to smford for getting me off the bubble, since the [ESP_AsyncFSBrowser](https://github.com/me-no-dev/ESPAsyncWebServer/tree/master/examples/ESP_AsyncFSBrowser) no longer works with SPIFFS (and I am [not ready to move to LittleFS with some of my projects](https://github.com/lbussy/SPIFFS-File-Manager/issues/34).)

This was tested on PlatformIO with:
```
Platform espressif32 @ 5.0.0 (required: espressif32 @ ~5.0.0)
├── framework-arduinoespressif32 @ 3.20003.220626 (required: platformio/framework-arduinoespressif32 @ ~3.20003.0)
├── tool-esptoolpy @ 1.30300.0 (required: platformio/tool-esptoolpy @ ~1.30300.0)
├── tool-mkspiffs @ 2.230.0 (required: platformio/tool-mkspiffs @ ~2.230.0)
└── toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch3 (required: espressif/toolchain-xtensa-esp32 @ 8.4.0+2021r2-patch3)

Libraries
├── ArduinoLog @ 1.0.3+sha.be71b9e (required: git+https://github.com/lbussy/Arduino-Log @ ~1.0.3)
├── AsyncTCP @ 1.1.1+sha.b6d4c78 (required: git+https://github.com/lbussy/AsyncTCP @ ~1.1.1)
└── ESP Async WebServer @ 1.2.3+sha.35b2ec1 (required: git+https://github.com/lbussy/ESPAsyncWebServer @ ~1.2.3)
```

## Current Limitations

Right now this is limited to SPIFFS and only tested on an ESP32.  The reaosn being LittleFS is now the default on the ESP8266, and the [ESP_AsyncFSBrowser](https://github.com/me-no-dev/ESPAsyncWebServer/tree/master/examples/ESP_AsyncFSBrowser) works on that platform and is far more capable.

## Usage

1. Properly instantiate a working `ESP Async Web Server`
1. Include `editserver.h` in your sketch
1. Pass a pointer to your server, the edit page username and password password, and the desired mountpoint with no leading slashes (i.e. `edit`) when instantiating the edit pages: `configureEditPages(server, edituser, editpassword, mountpoint);`
1. (Optional) Define `EDIT_EXTERNAL` to use an `edit.htm` file in your SPIFFS image.  This file uses the template functionality, it it must not be GZIPed.  Be sure to copy this file into your data partition.
1. (Optional) Define `EDIT_DEBUG` to see debug messages.
