# Library provide few control operation for **Flir Tau 2** thermal camera via UART port

*Cross platform, tested on: Windows, Arduino*

## Initialization

First you have to declare callback function to read(), write(), available() according to your platformfi and serail library

```cpp
    #include "TauCam.h"

    void write(uint8_t byte) {
        // SomeSerialLib.write(byte);
    }
    uint8_t read() {
        // return SomeSerialLib.read();
    }
    uint32_t available() {
        // return SomeSerialLib.available();
    }

    void delay(uint32_t ms) {
        // sleep(ms); // some sleep() call depends on your platform
    };

    // then init main class and pass wrapper functions
    TauCam cam = TauCam(write, read, available, delay);

```

## Method list

```cpp
    TauCam cam = TauCam(write, read, available, delay); // read the section above

    // commands from the TauCmd table in TauCommands.h
    cam.execute(TauCmd::DoFfc);
    cam.set(TauCmd::VideoPalette, TauPalette::BlackHot);
    cam.set(TauCmd::Contrast, 100);
    cam.set(TauCmd::VideoMode, TauVideoMode::Zoom2x);

    TauOrientation orientation;
    cam.get(TauCmd::VideoOrientation, orientation);

    int16_t fpaTemp;
    cam.get(TauCmd::ReadSensor, TauSensorReading::FpaTempC10, fpaTemp);

    uint8_t part[TauCmd::CameraPart.getReplyBytes];
    cam.getBytes(TauCmd::CameraPart, part, sizeof(part));

    uint32_t cameraSerial, sensorSerial;
    cam.getSerialNumbers(cameraSerial, sensorSerial);

    // OSD drawing methods
    cam.printText(...)
    cam.drawRectangle(...)
    cam.drawOutlineRectangle(...)

    // send raw custom command
    cam.sendCommand(...)
```

All of these return the camera status code or a `TAU_RES_ERR_*` code, with
`TAU_OK` on success. A parameter from another command does not compile, and an
operation the command does not have returns `TAU_RES_ERR_UNSUPPORTED` without
sending anything.

`sendCommand` takes any function code. Commands that return data can provide a
response buffer and receive the decoded payload length:

```cpp
    uint8_t response[16];
    uint16_t responseLen = 0;
    uint8_t status = cam.sendCommand(
        TauCmd::GetRevision.code,
        NULL,
        0,
        response,
        sizeof(response),
        &responseLen
    );
```

The response payload is copied only after the frame length and checksums have
been validated. If the payload does not fit, `sendCommand` returns
`TAU_RES_ERR_BUFFER` and reports the required length in `responseLen`.