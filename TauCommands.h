#pragma once
#include <stdint.h>

// TAU 2 command table and parameters
// Source: Tau 2 / Quark Software IDD, 102-PS242-43 Rev. 120, Table 3-5


// Command parameters
enum class TauBaudRate : uint16_t {
    Auto = 0x0000, Baud9600 = 0x0001, Baud19200 = 0x0002, Baud28800 = 0x0003,
    Baud57600 = 0x0004, Baud115200 = 0x0005, Baud460800 = 0x0006, Baud921600 = 0x0007
};
enum class TauGainMode : uint16_t { Automatic = 0x0000, LowGainOnly = 0x0001, HighGainOnly = 0x0002, Manual = 0x0003 };
enum class TauFfcMode : uint16_t { Manual = 0x0000, Automatic = 0x0001, External = 0x0002 };
enum class TauFfcType : uint16_t { Short = 0x0000, Long = 0x0001 };

// The camera accepts palette numbers 0-29, other numbers can be passed with static_cast<TauPalette>(n)
enum class TauPalette : uint16_t {
    WhiteHot = 0x00, BlackHot = 0x01, Fusion = 0x02, Ironbow = 0x03, Rainbow = 0x04,
    RainbowHc = 0x05, Sepia = 0x06, Arctic = 0x07, Lava = 0x08, OutdoorAlert = 0x09,
    IceFire = 0x0A, Glowbow = 0x0B, Color1 = 0x0C, Color2 = 0x0D
};
enum class TauOrientation : uint16_t { Normal = 0x0000, Invert = 0x0001, Revert = 0x0002, InvertRevert = 0x0003 };
enum class TauDigitalOutput : uint16_t { Enabled = 0x0000, Disabled = 0x0002 };
enum class TauAgcType : uint16_t { PlateauHistogram = 0x0000, OnceBright = 0x0001, AutoBright = 0x0002, Manual = 0x0003, Linear = 0x0005 };
enum class TauLens : uint16_t { Lens0 = 0x0000, Lens1 = 0x0001 };
enum class TauSpotMeterMode : uint16_t { Off = 0x0000, OnFahrenheit = 0x0001, OnCelsius = 0x0002 };
enum class TauSensorReading : uint16_t { FpaTempC10 = 0x0000, FpaTempRaw = 0x0001, HousingTempC100 = 0x000A, StatusBits = 0x0011 };
enum class TauExternalSync : uint16_t { Disabled = 0x0000, Slave = 0x0001, Master = 0x0002 };
enum class TauIsotherm : uint16_t { Disabled = 0x0000, Enabled = 0x0001 };
enum class TauTestPattern : uint16_t { Off = 0x0000, AscendingRamp = 0x0001, BigVertical = 0x0003, HorizontalShade = 0x0004, ColorBars = 0x0006, RampWithSteps = 0x0008 };
enum class TauColorMode : uint16_t { Monochrome = 0x0000, ColorEnabled = 0x0001 };
enum class TauSpotDisplay : uint16_t { Off = 0x0000, NumericOnly = 0x0001, ThermometerOnly = 0x0002, NumericAndThermometer = 0x0003 };
enum class TauSymbolAction : uint16_t { Unfreeze = 0x0000, Freeze = 0x0001, Paint = 0x0002, Write = 0x0003 };
enum class TauSymbolType : uint16_t { None = 0x0000, Rectangle = 0x0001, Text = 0x0002, Bitmap = 0x0003, OutlineRectangle = 0x0004 };
enum class TauVideoStandard : uint16_t { Ntsc30 = 0x0000, Pal25 = 0x0001, Ntsc60 = 0x0004, Pal50 = 0x0005 };
enum class TauShutterPosition : uint16_t { Open = 0x0000, Close = 0x0001, Unknown = 0xFFFF };

// VIDEO_MODE is a bit field, flags can be combined with |
namespace TauVideoMode {
    constexpr uint16_t Freeze         = 0x0001;
    constexpr uint16_t AnalogDisabled = 0x0002;
    constexpr uint16_t Zoom1x         = 0x0000;
    constexpr uint16_t Zoom2x         = 0x0004;
    constexpr uint16_t Zoom4x         = 0x0008;
    constexpr uint16_t Zoom8x         = 0x0010;
    constexpr uint16_t IgnoreZoomBits = 0x0200;
}


// T is the value type used by TauCam::get() and TauCam::set()
template <typename T = uint16_t>
struct TauCommand {
    uint8_t code;
    uint16_t getArgBytes, getReplyBytes;
    uint16_t setArgBytes, setReplyBytes;
};

namespace TauCmd {
    constexpr uint16_t NA = 0xFFFF;     // operation not available for this command
    constexpr uint16_t VAR = 0xFFFE;    // length depends on the arguments

    // Function codes are overloaded by byte count, overloads have their own entry
    //                                                       code  get:arg,reply  set:arg,reply
    constexpr TauCommand<>                   NoOp               {0x00, NA, NA,   0,  0};
    constexpr TauCommand<>                   SetDefaults        {0x01, NA, NA,   0,  0};   // non-blocking, poll MemoryStatus
    constexpr TauCommand<>                   CameraReset        {0x02, NA, NA,   0,  0};
    constexpr TauCommand<>                   RestoreFactoryDefaults {0x03, NA, NA, 0, 0};
    constexpr TauCommand<>                   SerialNumber       {0x04,  0,  8,  NA, NA};   // 0-3: camera, 4-7: sensor
    constexpr TauCommand<>                   GetRevision        {0x05,  0,  8,  NA, NA};   // SW major, SW minor, FW major, FW minor
    constexpr TauCommand<TauBaudRate>        BaudRate           {0x07,  0,  2,   2,  2};   // reply arrives at the new baud rate
    constexpr TauCommand<TauGainMode>        GainMode           {0x0A,  0,  2,   2,  2};
    constexpr TauCommand<TauFfcMode>         FfcMode            {0x0B,  0,  2,   2,  2};
    constexpr TauCommand<>                   DoFfc              {0x0C, NA, NA,   0,  0};   // short FFC, non-blocking
    constexpr TauCommand<TauFfcType>         DoFfcType          {0x0C, NA, NA,   2,  2};   // non-blocking
    constexpr TauCommand<>                   FfcPeriod          {0x0D,  0,  4,   4,  4};   // 0-1: high gain, 2-3: low gain
    constexpr TauCommand<>                   FfcPeriodCurrentGain {0x0D, NA, NA, 2,  2};   // 0 to 30000 frames
    constexpr TauCommand<>                   FfcTempDelta       {0x0E,  0,  4,   4,  4};   // 0-1: high gain, 2-3: low gain
    constexpr TauCommand<>                   FfcTempDeltaCurrentGain {0x0E, NA, NA, 2, 2}; // 0 to 1000 (0.1C to 100.1C)
    constexpr TauCommand<>                   VideoMode          {0x0F,  0,  2,   2,  2};   // TauVideoMode flags
    constexpr TauCommand<TauPalette>         VideoPalette       {0x10,  0,  2,   2,  2};
    constexpr TauCommand<TauOrientation>     VideoOrientation   {0x11,  0,  2,   2,  2};
    constexpr TauCommand<TauDigitalOutput>   DigitalOutputMode  {0x12,  0,  2,   2,  2};
    constexpr TauCommand<>                   DigitalOutputSubMode {0x12, 2, 2,   2,  2};   // byte 0: selector, byte 1: value (IDD p. 19-20)
    constexpr TauCommand<TauAgcType>         AgcType            {0x13,  0,  2,   2,  2};
    constexpr TauCommand<>                   Contrast           {0x14,  0,  2,   2,  2};   // 0 to 255
    constexpr TauCommand<>                   Brightness         {0x15,  0,  2,   2,  2};   // 0 to 16383
    constexpr TauCommand<int16_t>            BrightnessBias     {0x18,  0,  2,   2,  2};   // -16384 to 16383
    constexpr TauCommand<TauLens>            LensNumber         {0x1E,  0,  2,   2,  2};
    constexpr TauCommand<TauSpotMeterMode>   SpotMeterMode      {0x1F,  0,  2,   2,  2};
    constexpr TauCommand<int16_t>            ReadSensor         {0x20,  2,  2,  NA, NA};   // selector: TauSensorReading
    constexpr TauCommand<>                   ReadAccelerometer  {0x20,  2,  8,  NA, NA};   // arg 0x000B, 0-1: X, 2-3: Y, 4-5: Z (0.01 g)
    constexpr TauCommand<TauExternalSync>    ExternalSync       {0x21,  0,  2,   2,  2};
    constexpr TauCommand<TauIsotherm>        Isotherm           {0x22,  0,  2,   2,  2};
    constexpr TauCommand<>                   IsothermThresholds {0x23,  0,  6,   6,  6};   // lower, middle, upper
    constexpr TauCommand<TauTestPattern>     TestPattern        {0x25,  0,  2,   2,  2};   // non-blocking
    constexpr TauCommand<TauColorMode>       VideoColorMode     {0x26,  0,  2,   2,  2};
    constexpr TauCommand<int16_t>            GetSpotMeter       {0x2A,  0,  2,  NA, NA};   // deg C
    constexpr TauCommand<TauSpotDisplay>     SpotDisplay        {0x2B,  0,  2,   2,  2};
    constexpr TauCommand<>                   DdeGain            {0x2C,  0,  2,   2,  2};   // 0 to 255
    constexpr TauCommand<TauSymbolAction>    SymbolAction       {0x2F, NA, NA,   2,  2};
    constexpr TauCommand<>                   SymbolDraw         {0x2F, NA, NA, VAR, VAR};  // 14 to 46 arg bytes, non-blocking
    constexpr TauCommand<>                   SplashControl      {0x31,  0,  4,   4,  4};   // 0-1: splash number, 2-3: timeout
    constexpr TauCommand<>                   EzoomWidth         {0x32,  0,  2,  NA, NA};
    constexpr TauCommand<>                   EzoomControl       {0x32,  2,  2,   4,  0};   // get: 0 width, 4 max width; set: 1 set, 2 inc, 3 dec
    constexpr TauCommand<>                   FfcWarnTime        {0x3C,  0,  2,   2,  2};   // 0 to 600 frames
    constexpr TauCommand<>                   AgcFilter          {0x3E,  0,  2,   2,  2};   // 0 to 255
    constexpr TauCommand<>                   PlateauLevel       {0x3F,  0,  2,   2,  2};   // 0 to 1000
    constexpr TauCommand<>                   SpotMeterData      {0x43,  0,  2,  NA, NA};   // deg C or counts
    constexpr TauCommand<>                   SpotMeterStats     {0x43,  2, 20,  NA, NA};   // arg: 0 counts, 1 C x 10, 2 K x 100
    constexpr TauCommand<>                   SpotMeterCoordinates {0x43, 2, 12,  8,  4};   // get arg: 0x0100
    constexpr TauCommand<>                   AgcRoi             {0x4C,  0,  8,  NA, NA};   // left, top, right, bottom
    constexpr TauCommand<>                   ShutterTemp        {0x4D,  0,  2,   2,  0};   // deg C x 100 (signed), 32768: use internal
    constexpr TauCommand<>                   AgcMidpoint        {0x55,  0,  2,   2,  2};   // 0 to 255
    constexpr TauCommand<>                   SerialNumberLegacy {0x65,  0,  8,  NA, NA};
    constexpr TauCommand<>                   CameraPart         {0x66,  0, 32,  NA, NA};   // ASCII
    constexpr TauCommand<>                   ReadArrayAverage   {0x68,  0,  4,  NA, NA};   // 0-1: mean, 2-3: histogram width
    constexpr TauCommand<>                   MaxAgcGain         {0x6A,  0,  2,   2,  2};   // 0 to 2047
    constexpr TauCommand<>                   PanAndTilt         {0x70,  0,  4,   4,  4};   // 0-1: tilt, 2-3: pan
    constexpr TauCommand<TauVideoStandard>   VideoStandard      {0x72,  0,  2,   2,  2};
    constexpr TauCommand<TauShutterPosition> ShutterPosition    {0x79,  0,  2,   2,  2};   // non-blocking
    constexpr TauCommand<>                   TransferFrame      {0x82, NA, NA,   4,  4};   // non-blocking
    constexpr TauCommand<>                   TlinCommands       {0x8E,  2,  2,   4,  0};   // selector: 0x0010 resolution, 0x0040 enable
    constexpr TauCommand<>                   CorrectionMask     {0xB1,  0,  2,   2,  2};
    constexpr TauCommand<>                   MemoryStatus       {0xC4,  0,  2,  NA, NA};   // 0: complete, 0xFFFF: erase err, 0xFFFE: write err
    constexpr TauCommand<>                   WriteNvffcTable    {0xC6, NA, NA,   0,  0};   // non-blocking, poll MemoryStatus
    constexpr TauCommand<>                   ReadMemory         {0xD2,  6, VAR, NA, NA};   // 0-3: address, 4-5: byte count
    constexpr TauCommand<>                   EraseMemoryBlock   {0xD4, NA, NA,   2,  2};   // non-blocking, poll MemoryStatus
    constexpr TauCommand<>                   GetNvMemorySize    {0xD5,  2,  8,  NA, NA};   // arg 0xFFFF, 0-3: base, 4-7: block size
    constexpr TauCommand<>                   GetMemoryAddress   {0xD6,  4,  8,  NA, NA};
    constexpr TauCommand<>                   GainSwitchParams   {0xDB,  0,  8,   8,  8};
    constexpr TauCommand<>                   DdeThreshold       {0xE2,  0,  2,   2,  2};   // 0 to 255
    constexpr TauCommand<>                   SpatialThreshold   {0xE3,  0,  2,   2,  2};   // 0x0000-0x000F manual, 0x0100-0x013F auto
    constexpr TauCommand<>                   LensResponseParams {0xE5,  2,  4,   6,  0};   // lens number, F/#, transmission
    constexpr TauCommand<>                   RadiometryParam    {0xE5,  2,  2,   4,  0};   // selector: 0x0100 to 0x0107
}
