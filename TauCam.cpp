#include "TauCam.h"
#include <string.h>


// IMPORTANT: little endian to big endian converting in argument bytes

// Send command and check the status and the reply length from the command table
uint8_t TauCam::exchange(
    uint8_t cmd,
    const void *pArgs,
    uint16_t argsLen,
    void *pReply,
    uint16_t replyCapacity,
    uint16_t expectedReplyLen
) {
    uint16_t replyLen = 0;
    uint8_t status = TauCam::sendCommand(cmd, pArgs, argsLen, pReply, replyCapacity, &replyLen);
    if (status != TAU_OK) return status;
    if (expectedReplyLen != TauCmd::VAR && replyLen != expectedReplyLen) return TAU_RES_ERR_LEN;
    return TAU_OK;
}

uint8_t TauCam::getSerialNumbers(uint32_t &cameraSerial, uint32_t &sensorSerial) {
    uint8_t reply[TauCmd::SerialNumber.getReplyBytes];
    uint8_t status = TauCam::getBytes(TauCmd::SerialNumber, reply, sizeof(reply));
    if (status != TAU_OK) return status;
    cameraSerial = (static_cast<uint32_t>(reply[0]) << 24) | (static_cast<uint32_t>(reply[1]) << 16) |
                   (static_cast<uint32_t>(reply[2]) << 8) | reply[3];
    sensorSerial = (static_cast<uint32_t>(reply[4]) << 24) | (static_cast<uint32_t>(reply[5]) << 16) |
                   (static_cast<uint32_t>(reply[6]) << 8) | reply[7];
    return TAU_OK;
}



static bool isPal(TauVideoStandard standard) {
    return standard == TauVideoStandard::Pal25 || standard == TauVideoStandard::Pal50;
}

// Switching between PAL and NTSC requires a FFC afterwards
uint8_t TauCam::setVideoStandard(TauVideoStandard standard) {
    TauVideoStandard current;
    uint8_t status = TauCam::get(TauCmd::VideoStandard, current);
    if (status != TAU_OK) return status;

    status = TauCam::set(TauCmd::VideoStandard, standard);
    if (status != TAU_OK) return status;

    if (isPal(current) != isPal(standard)) {
        return TauCam::execute(TauCmd::DoFfc);
    }
    return TAU_OK;
}

uint8_t TauCam::printText(
    uint16_t symbolNum,
    const char *text,
    uint16_t posX,
    uint16_t posY,
    uint8_t colorBgrnd,
    uint8_t colorFrgrnd,
    uint16_t aligment,
    uint16_t font
){
    size_t textLen = strlen(text);
    if (textLen > TAU_SYMBOL_MAX_TEXT_LEN) return TAU_RES_ERR_LEN;

    uint16_t argsLen = TAU_SYMBOL_HEADER_LEN + (textLen % 2 ? textLen + 1 : textLen);
    uint8_t args[TAU_SYMBOL_MAX_ARGS] = {0};

    uint16_t type = static_cast<uint16_t>(TauSymbolType::Text);

    args[0] = MSB(symbolNum); args[1] = LSB(symbolNum);             // Bytes 0-1: Symbol Number (0-99)
    args[2] = MSB(type); args[3] = LSB(type);           // Bytes 2-3: Symbol Type (Rectangle=1, Text=2, Bitmap=3)
    args[4] = MSB(posX); args[5] = LSB(posX);           // Bytes 4-5: X-coord
    args[6] = MSB(posY); args[7] = LSB(posY);           // Bytes 6-7: Y-coord
    args[8] = MSB(aligment); args[9] = LSB(aligment);   // Bytes 8-9: Width (rectangle) or Alignment (text)
    args[10] = MSB(font); args[11] = LSB(font);         // Bytes 10-11: Height (rectangle) or  Font (text)
    args[12] = colorBgrnd; args[13] = colorFrgrnd;      // Bytes 12-13: Background Color and Foreground Color

    memcpy(args + TAU_SYMBOL_HEADER_LEN, text, textLen);
    return TauCam::exchange(TauCmd::SymbolDraw.code, args, argsLen, NULL, 0, TauCmd::SymbolDraw.setReplyBytes);
};

// canvas width = 640, height 512

uint8_t TauCam::drawRect(
uint16_t symbolNum,
    TauSymbolType type,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    uint8_t args[TAU_SYMBOL_HEADER_LEN];
    uint16_t rawType = static_cast<uint16_t>(type);

    args[0] = MSB(symbolNum); args[1] = LSB(symbolNum);             // Bytes 0-1: Symbol Number (0-99)
    args[2] = MSB(rawType); args[3] = LSB(rawType);     // Bytes 2-3: Symbol Type (Rectangle=1, Text=2, Bitmap=3)
    args[4] = MSB(posX); args[5] = LSB(posX);           // Bytes 4-5: X-coord
    args[6] = MSB(posY); args[7] = LSB(posY);           // Bytes 6-7: Y-coord
    args[8] = MSB(width); args[9] = LSB(width);         // Bytes 8-9: Width (rectangle) or Alignment (text)
    args[10] = MSB(height); args[11] = LSB(height);     // Bytes 10-11: Height (rectangle) or  Font (text)
    args[12] = TAU_COLOR_TRANSPRNT; args[13] = color;   // Bytes 12-13: Background Color and Foreground Color

    return TauCam::exchange(TauCmd::SymbolDraw.code, args, sizeof(args), NULL, 0, TauCmd::SymbolDraw.setReplyBytes);
};

uint8_t TauCam::drawRectangle(
uint16_t symbolNum,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    return TauCam::drawRect(symbolNum, TauSymbolType::Rectangle, posX, posY, width, height, color);
}

uint8_t TauCam::drawOutlineRectangle(
    uint16_t symbolNum,
    uint16_t posX,
    uint16_t posY,
    uint16_t width,
    uint16_t height,
    uint8_t color
){
    return TauCam::drawRect(symbolNum, TauSymbolType::OutlineRectangle, posX, posY, width, height, color);
}
