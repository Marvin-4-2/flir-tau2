#pragma once
#include "TauCamBase.h"
#include "TauCommands.h"

// Parameters for TauCmd::SymbolDraw

#define TAU_CANVAS_WIDTH  640
#define TAU_CANVAS_HEIGHT 512

#define TAU_SYMBOL_MAX_ARGS     46
#define TAU_SYMBOL_HEADER_LEN   14
#define TAU_SYMBOL_MAX_TEXT_LEN (TAU_SYMBOL_MAX_ARGS - TAU_SYMBOL_HEADER_LEN)


#define TAU_COLOR_YELLOW        251 // 243
#define TAU_COLOR_WHITE         255
#define TAU_COLOR_PINK          253
#define TAU_COLOR_BLUE          252 // 244, 246
#define TAU_COLOR_GREEN         250 // 242
#define TAU_COLOR_RED           249 // 241
#define TAU_COLOR_DARK_RED      241
#define TAU_COLOR_GREY          248 // 247
#define TAU_COLOR_PURPLE        245
#define TAU_COLOR_BLACK         240
#define TAU_COLOR_ORANGE        239
#define TAU_COLOR_BROWN         238
#define TAU_COLOR_TRANSPRNT     0

// Text: Font Sizes and Alignments
#define TAU_FONT_SMALL          0x00
#define TAU_FONT_MIDDLE         0x01
#define TAU_FONT_LARGE          0x02

#define TAU_FONT_ALIGN_CENTER  0x0000
#define TAU_FONT_ALIGN_LEFT    0x0100
#define TAU_FONT_ALIGN_RIGHT   0x8000


// Keeps value out of template deduction, so set(TauCmd::Contrast, 100) compiles
template <typename T>
struct TauNonDeduced {
    typedef T type;
};


class TauCam : TauCamBase {

using TauCamBase::TauCamBase;

private:
    uint8_t exchange(
        uint8_t cmd,
        const void *pArgs,
        uint16_t argsLen,
        void *pReply,
        uint16_t replyCapacity,
        uint16_t expectedReplyLen
    );

    uint8_t drawRect(
        uint16_t symbolNum,
        TauSymbolType type,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );

public:
    using TauCamBase::sendCommand;

    // Run a command without arguments, e.g. execute(TauCmd::DoFfc)
    template <typename T>
    uint8_t execute(const TauCommand<T> &cmd) {
        if (cmd.setArgBytes != 0) return TAU_RES_ERR_UNSUPPORTED;
        return exchange(cmd.code, NULL, 0, NULL, 0, cmd.setReplyBytes);
    }

    // Set a single value, e.g. set(TauCmd::VideoPalette, TauPalette::BlackHot)
    template <typename T>
    uint8_t set(const TauCommand<T> &cmd, typename TauNonDeduced<T>::type value) {
        if (cmd.setArgBytes != 2) return TAU_RES_ERR_UNSUPPORTED;
        uint16_t raw = static_cast<uint16_t>(value);
        uint8_t args[2] = {static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw & 0xFF)};
        return exchange(cmd.code, args, sizeof(args), NULL, 0, cmd.setReplyBytes);
    }

    // Set a value selected by a 2 byte argument, e.g. set(TauCmd::TlinCommands, 0x0040, 1)
    template <typename T, typename S>
    uint8_t set(const TauCommand<T> &cmd, S selector, typename TauNonDeduced<T>::type value) {
        if (cmd.setArgBytes != 4) return TAU_RES_ERR_UNSUPPORTED;
        uint16_t rawSelector = static_cast<uint16_t>(selector);
        uint16_t raw = static_cast<uint16_t>(value);
        uint8_t args[4] = {
            static_cast<uint8_t>(rawSelector >> 8), static_cast<uint8_t>(rawSelector & 0xFF),
            static_cast<uint8_t>(raw >> 8), static_cast<uint8_t>(raw & 0xFF)
        };
        return exchange(cmd.code, args, sizeof(args), NULL, 0, cmd.setReplyBytes);
    }

    // Get a single value, value is only written on TAU_OK
    template <typename T>
    uint8_t get(const TauCommand<T> &cmd, T &value) {
        if (cmd.getArgBytes != 0 || cmd.getReplyBytes != 2) return TAU_RES_ERR_UNSUPPORTED;
        uint8_t reply[2];
        uint8_t status = exchange(cmd.code, NULL, 0, reply, sizeof(reply), cmd.getReplyBytes);
        if (status != TAU_OK) return status;
        value = static_cast<T>((static_cast<uint16_t>(reply[0]) << 8) | reply[1]);
        return TAU_OK;
    }

    // Get a value selected by a 2 byte argument, e.g. get(TauCmd::ReadSensor, TauSensorReading::FpaTempC10, temp)
    template <typename T, typename S>
    uint8_t get(const TauCommand<T> &cmd, S selector, T &value) {
        if (cmd.getArgBytes != 2 || cmd.getReplyBytes != 2) return TAU_RES_ERR_UNSUPPORTED;
        uint16_t rawSelector = static_cast<uint16_t>(selector);
        uint8_t args[2] = {static_cast<uint8_t>(rawSelector >> 8), static_cast<uint8_t>(rawSelector & 0xFF)};
        uint8_t reply[2];
        uint8_t status = exchange(cmd.code, args, sizeof(args), reply, sizeof(reply), cmd.getReplyBytes);
        if (status != TAU_OK) return status;
        value = static_cast<T>((static_cast<uint16_t>(reply[0]) << 8) | reply[1]);
        return TAU_OK;
    }

    // Get a multi-field reply as raw big endian bytes
    template <typename T>
    uint8_t getBytes(const TauCommand<T> &cmd, void *pReply, uint16_t replyCapacity) {
        if (cmd.getArgBytes != 0 || cmd.getReplyBytes == TauCmd::NA) return TAU_RES_ERR_UNSUPPORTED;
        return exchange(cmd.code, NULL, 0, pReply, replyCapacity, cmd.getReplyBytes);
    }

    uint8_t getSerialNumbers(uint32_t &cameraSerial, uint32_t &sensorSerial);

    // text is limited to TAU_SYMBOL_MAX_TEXT_LEN characters
    uint8_t printText(
        uint16_t symbolNum,
        const char *text,
        uint16_t posX,
        uint16_t posY,
        uint8_t colorBgrnd = 0x00,
        uint8_t colorFrgrnd = 0x01,
        uint16_t aligment = 1,
        uint16_t font = 1
    );

    uint8_t drawRectangle(
        uint16_t symbolNum,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );

    uint8_t drawOutlineRectangle(
        uint16_t symbolNum,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );
};
