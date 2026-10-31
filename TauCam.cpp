#include "TauCam.h"
#include <stdlib.h>
#include <string.h>


// IMPORTANT: little endian to big endian converting in argument bytes

void TauCam::setPalette(uint8_t paletteNum) {
    // palette num in 0-29 range
    uint8_t args[2] = {0, paletteNum};
    TauCam::sendCommand(TAU_CMD_VIDEO_PALETTE, &args, sizeof(args));
}

void TauCam::setZoom(uint8_t zoomMode) {
    uint8_t args[2] = {0, zoomMode};
    TauCam::sendCommand(TAU_CMD_VIDEO_MODE, &args, sizeof(args));
}

void TauCam::setSpotMeter(uint8_t mode) {
    uint8_t args[2] = {0, mode};
    TauCam::sendCommand(TAU_CMD_SPOT_METER_MODE, &args, sizeof(args));
}

void TauCam::setOrientation(uint8_t orientMode) {
    uint8_t args[2] = {0, orientMode};
    TauCam::sendCommand(TAU_CMD_VIDEO_ORIENTATION, &args, sizeof(args));
}

void TauCam::setVideoStandart(uint8_t standart) {
    uint8_t args[2] = {0, standart};
    TauCam::sendCommand(TAU_CMD_VIDEO_STANDARD, &args, sizeof(args));
    TauCam::sendCommand(TAU_CMD_DO_FFC, NULL, 0);
}

void TauCam::printText(
    uint16_t symbolNum,
    const char *text,
    uint16_t posX,
    uint16_t posY,
    uint8_t colorBgrnd,
    uint8_t colorFrgrnd, 
    uint16_t aligment,
    uint16_t font
){
    uint8_t textLen = strlen(text);
    uint8_t argsLen = 14 + (textLen % 2 ? textLen + 1 : textLen) ;
    uint8_t *args = (uint8_t *) calloc(argsLen, 1);

    uint16_t type = TAU_SYMBOL_TEXT;

    args[0] = MSB(symbolNum); args[1] = LSB(symbolNum);             // Bytes 0-1: Symbol Number (0-99)
    args[2] = MSB(type); args[3] = LSB(type);           // Bytes 2-3: Symbol Type (Rectangle=1, Text=2, Bitmap=3)
    args[4] = MSB(posX); args[5] = LSB(posX);           // Bytes 4-5: X-coord
    args[6] = MSB(posY); args[7] = LSB(posY);           // Bytes 6-7: Y-coord
    args[8] = MSB(aligment); args[9] = LSB(aligment);   // Bytes 8-9: Width (rectangle) or Alignment (text)
    args[10] = MSB(font); args[11] = LSB(font);         // Bytes 10-11: Height (rectangle) or  Font (text)
    args[12] = colorBgrnd; args[13] = colorFrgrnd;      // Bytes 12-13: Background Color and Foreground Color

    memcpy(args+14, text, textLen);    
    TauCam::sendCommand(TAU_CMD_SYMBOL_CONTROL, args, argsLen);
    free(args);
};

// canvas width = 640, height 512

void TauCam::drawRect(
uint16_t symbolNum,
    uint16_t type,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    uint8_t argsLen = 14;
    uint8_t *args = (uint8_t *) calloc(argsLen, 1);

    args[0] = MSB(symbolNum); args[1] = LSB(symbolNum);             // Bytes 0-1: Symbol Number (0-99)
    args[2] = MSB(type); args[3] = LSB(type);           // Bytes 2-3: Symbol Type (Rectangle=1, Text=2, Bitmap=3)
    args[4] = MSB(posX); args[5] = LSB(posX);           // Bytes 4-5: X-coord
    args[6] = MSB(posY); args[7] = LSB(posY);           // Bytes 6-7: Y-coord
    args[8] = MSB(width); args[9] = LSB(width);         // Bytes 8-9: Width (rectangle) or Alignment (text)
    args[10] = MSB(height); args[11] = LSB(height);     // Bytes 10-11: Height (rectangle) or  Font (text)
    args[12] = TAU_COLOR_TRANSPRNT; args[13] = color;   // Bytes 12-13: Background Color and Foreground Color

    TauCam::sendCommand(TAU_CMD_SYMBOL_CONTROL, args, argsLen);
    free(args);
};

void TauCam::drawRectangle(
uint16_t symbolNum,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    TauCam::drawRect(symbolNum, TAU_SYMBOL_RECTANGLE, posX, posY, width, height, color);
}

void TauCam::drawOutlineRectangle(
    uint16_t symbolNum,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    TauCam::drawRect(symbolNum, TAU_SYMBOL_OUTLINE_RECT, posX, posY, width, height, color);
}
