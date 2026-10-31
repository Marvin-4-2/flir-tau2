#pragma once
#include "TauCamBase.h"

// TAU 2 Commands
#define TAU_CMD_NO_OP                    0x00
#define TAU_CMD_SET_DEFAULTS             0x01
#define TAU_CMD_CAMERA_RESET             0x02
#define TAU_CMD_RESTORE_FACTORY_DEFAULTS 0x03
#define TAU_CMD_SERIAL_NUMBER            0x04
#define TAU_CMD_GET_REVISION             0x05
#define TAU_CMD_BAUD_RATE                0x07
#define TAU_CMD_GAIN_MODE                0x0A
#define TAU_CMD_FFC_MODE_SELECT          0x0B
#define TAU_CMD_DO_FFC                   0x0C
#define TAU_CMD_FFC_PERIOD               0x0D
#define TAU_CMD_FFC_TEMP_DELTA           0x0E
#define TAU_CMD_VIDEO_MODE               0x0F
#define TAU_CMD_VIDEO_PALETTE            0x10
#define TAU_CMD_VIDEO_ORIENTATION        0x11
#define TAU_CMD_DIGITAL_OUTPUT_MODE      0x12
#define TAU_CMD_AGC_TYPE                 0x13
#define TAU_CMD_CONTRAST                 0x14
#define TAU_CMD_BRIGHTNESS               0x15
#define TAU_CMD_BRIGHTNESS_BIAS          0x18
#define TAU_CMD_LENS_NUMBER              0x1E
#define TAU_CMD_SPOT_METER_MODE          0x1F
#define TAU_CMD_READ_SENSOR              0x20
#define TAU_CMD_EXTERNAL_SYNC            0x21
#define TAU_CMD_ISOTHERM                 0x22
#define TAU_CMD_ISOTHERM_THRESHOLDS      0x23
#define TAU_CMD_TEST_PATTERN             0x25
#define TAU_CMD_VIDEO_COLOR_MODE         0x26
#define TAU_CMD_GET_SPOT_METER           0x2A
#define TAU_CMD_SPOT_DISPLAY             0x2B
#define TAU_CMD_DDE_GAIN                 0x2C
#define TAU_CMD_SYMBOL_CONTROL           0x2F
#define TAU_CMD_SPLASH_CONTROL           0x31
#define TAU_CMD_EZOOM_CONTROL            0x32
#define TAU_CMD_FFC_WARN_TIME            0x3C
#define TAU_CMD_AGC_FILTER               0x3E
#define TAU_CMD_PLATEAU_LEVEL            0x3F
#define TAU_CMD_GET_SPOT_METER_DATA      0x43
#define TAU_CMD_AGC_ROI                  0x4C
#define TAU_CMD_SHUTTER_TEMP             0x4D
#define TAU_CMD_AGC_MIDPOINT             0x55
#define TAU_CMD_SERIAL_NUMBER_LEGACY     0x65
#define TAU_CMD_CAMERA_PART              0x66
#define TAU_CMD_READ_ARRAY_AVERAGE       0x68
#define TAU_CMD_MAX_AGC_GAIN             0x6A
#define TAU_CMD_PAN_AND_TILT             0x70
#define TAU_CMD_VIDEO_STANDARD           0x72
#define TAU_CMD_SHUTTER_POSITION         0x79
#define TAU_CMD_TRANSFER_FRAME           0x82
#define TAU_CMD_TLIN_COMMANDS            0x8E
#define TAU_CMD_CORRECTION_MASK          0xB1
#define TAU_CMD_MEMORY_STATUS            0xC4
#define TAU_CMD_WRITE_NVFFC_TABLE        0xC6
#define TAU_CMD_READ_MEMORY              0xD2
#define TAU_CMD_ERASE_MEMORY_BLOCK       0xD4
#define TAU_CMD_GET_NV_MEMORY_SIZE       0xD5
#define TAU_CMD_GET_MEMORY_ADDRESS       0xD6
#define TAU_CMD_GAIN_SWITCH_PARAMS       0xDB
#define TAU_CMD_DDE_THRESHOLD            0xE2
#define TAU_CMD_SPATIAL_THRESHOLD        0xE3
#define TAU_CMD_LENS_RESPONSE_PARAMS     0xE5


// TAU 2 Command Parameters
#define TAU_ZOOM_1X         0b00000
#define TAU_ZOOM_2X         0b00100
#define TAU_ZOOM_4X         0b01000
#define TAU_ZOOM_8X         0b10000

#define TAU_SPOTMETER_OFF       0
#define TAU_SPOTMETER_ON_F      1
#define TAU_SPOTMETER_ON_C      2

#define TAU_VIDEO_NTSC_30       0x00
#define TAU_VIDEO_PAL_25        0x01
#define TAU_VIDEO_NTSC_60       0x04
#define TAU_VIDEO_PAL_50        0x05

/* 
 * Parameter for TAU_CMD_VIDEO_PALETTE (0x10) 
 *
 * (Built-in Color Palettes (LUTs))
 */
#define TAU_PALETTE_WHITE_HOT      0x00
#define TAU_PALETTE_BLACK_HOT      0x01
#define TAU_PALETTE_FUSION         0x02
#define TAU_PALETTE_IRONBOW        0x03
#define TAU_PALETTE_RAINBOW        0x04
#define TAU_PALETTE_RAINBOW_HC     0x05
#define TAU_PALETTE_SEPIA          0x06
#define TAU_PALETTE_ARCTIC         0x07
#define TAU_PALETTE_LAVA           0x08
#define TAU_PALETTE_OUTDOOR_ALERT  0x09
#define TAU_PALETTE_ICE_FIRE       0x0A
#define TAU_PALETTE_GLOWBOW        0x0B
#define TAU_PALETTE_COLOR_1        0x0C
#define TAU_PALETTE_COLOR_2        0x0D

// Parameter for TAU_CMD_VIDEO_ORIENTATION (0x11)
#define TAU_ORIENT_NORMAL           0x00
#define TAU_ORIENT_INVERT           0x01
#define TAU_ORIENT_REVERT           0x02
#define TAU_ORIENT_INVERT_REVERT    0x03


// Parameters for TAU_CMD_SYMBOL_CONTROL (0x2F)

#define TAU_CANVAS_WIDTH  640
#define TAU_CANVAS_HEIGHT 512

// Two-Byte commands
#define TAU_SYMBOL_UNFREEZE     0x00
#define TAU_SYMBOL_FREEZE       0x01
#define TAU_SYMBOL_PAINT        0x02
#define TAU_SYMBOL_WRITE        0x03


// 14 to 46 Bytes command parameters
// Symbol Types
#define TAU_SYMBOL_RECTANGLE    0x01
#define TAU_SYMBOL_TEXT         0x02
#define TAU_SYMBOL_BITMAP       0x03
#define TAU_SYMBOL_OUTLINE_RECT 0x04


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


class TauCam : TauCamBase {

using TauCamBase::TauCamBase;

private:
    void drawRect(
        uint16_t symbolNum,
        uint16_t type,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );

public:
    using TauCamBase::sendCommand;

    void setPalette(uint8_t paletteNum);
    void setZoom(uint8_t zoomMode);
    void setSpotMeter(uint8_t mode);
    void setOrientation(uint8_t orientMode);
    void setVideoStandart(uint8_t standart);
    
    void printText(
        uint16_t symbolNum, 
        const char *text,
        uint16_t posX,
        uint16_t posY,
        uint8_t colorBgrnd = 0x00,
        uint8_t colorFrgrnd = 0x01, 
        uint16_t aligment = 1,
        uint16_t font = 1
    );
    
    void drawRectangle(
        uint16_t symbolNum,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );

    void drawOutlineRectangle(
        uint16_t symbolNum,
        uint16_t posX,
        uint16_t posY,
        uint16_t width,
        uint16_t height,
        uint8_t color
    );
};
