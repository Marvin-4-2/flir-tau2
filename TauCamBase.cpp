#include "TauCamBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// debug print hex buffer to console
void printHexBuf(const char *title, const void *pBuf, uint32_t len) {
     //printf("%s\n", title);
     //for (uint32_t i = 0; i < len; i++) {
     //    uint8_t byte = *((uint8_t*)pBuf+i);
     //    printf("0x%02X ", byte);
     //}
     //printf("\n\n");
}

TauCamBase::TauCamBase(
    void (*writeByte)(uint8_t),
    uint8_t (*readByte)(),
    uint32_t (*availableBytes)(),
    void (*delayMs)(uint32_t)
) {
    TauCamBase::serialWriteByte = writeByte;
    TauCamBase::serialReadByte = readByte;
    TauCamBase::serialAvailableBytes = availableBytes;
    TauCamBase::serialDelay = delayMs;
};

void TauCamBase::write(const void *pBuf, uint32_t len) {
    // dev debug
    printHexBuf("Send HEX", pBuf, len);
    // write len bytes to serial
    for (uint32_t i = 0; i < len; i++) {
        uint8_t byte = *((uint8_t*)pBuf+i);
        TauCamBase::serialWriteByte(byte);
    }
}

uint32_t TauCamBase::read() {
    // clear buffer
    memset(TauCamBase::buffer, 0, TAU_RES_BUF_LEN);
    // read from serial to buffer
    uint32_t len = 0;
    while (TauCamBase::available() && len < TAU_RES_BUF_LEN) {
        TauCamBase::buffer[len] = TauCamBase::serialReadByte();
        len++;
    }
    // dev debug
    printHexBuf("Read HEX", TauCamBase::buffer, len);
    return len;
}

uint32_t TauCamBase::available() {
    return TauCamBase::serialAvailableBytes();
}

// calc CRC16 checksum. Polynomial = 0x1021, Initial Value = 0x0
uint16_t TauCamBase::crc16(uint8_t *pBlock, uint16_t len) {
    uint16_t crc = 0x0;
    uint8_t i;
    while (len--) {
        crc ^= *pBlock++ << 8;
        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

// Build package structure and write to camera
uint8_t TauCamBase::sendCommand(
    uint8_t cmd,
    const void *pArgs,
    uint16_t argsLen,
    void *pResponseData,
    uint16_t responseDataCapacity,
    uint16_t *pResponseDataLen
) {

    // allocate memory size = (header = 8b) + (arguments = Nb) + (CRC2 = 2b)
    uint16_t packLen = 8 + argsLen + 2;
    uint8_t *pPack = (uint8_t*) calloc(packLen, 1);
    
    // build package 8b standart structure
    pPack[0] = 0x6E;                    // 0x6E on all valid incoming and reply  messages
    pPack[1] = 0x00;                    // status (not used on send)
    pPack[2] = 0x00;                    // not used
    pPack[3] = cmd;                     // call function code
    pPack[4] = MSB(argsLen);            // MSB arguments length
    pPack[5] = LSB(argsLen);            // LSB arguments length
    uint16_t crc1 = TauCamBase::crc16(pPack, 6);
    pPack[6] = MSB(crc1);               // MSB CRC1
    pPack[7] = LSB(crc1);               // LSB CRC1
    
    // add arguments bytes if exists
    if (argsLen != 0) {
        memcpy(pPack+8, pArgs, argsLen);         // copy arguments to package
        uint16_t crc2 = TauCamBase::crc16(pPack, packLen-2); // CRC cal all except last 2 bytes
        pPack[packLen-2] = MSB(crc2);            // MSB CRC2
        pPack[packLen-1] = LSB(crc2);            // LSB CRC2
    }

    // write package
    this->write(pPack, packLen);
    free(pPack);
    
    // wait to cam process command 
    TauCamBase::serialDelay(50);

    // read response and return status code 
    return TauCamBase::readResponse(pResponseData, responseDataCapacity, pResponseDataLen);
};


// Read and parse response
uint8_t TauCamBase::readResponse(
    void *pResponseData,
    uint16_t responseDataCapacity,
    uint16_t *pResponseDataLen
) {
    if (pResponseDataLen != NULL) {
        *pResponseDataLen = 0;
    }

    uint32_t len = TauCamBase::read();
    uint8_t *pBuf = TauCamBase::buffer;
    
    // Validate the fixed response header before reading its declared payload length.
    if (len == 0 || len < 8) return TAU_RES_ERR_LEN;
    if (pBuf[0] != 0x6E) return TAU_RES_ERR_INVALID;

    uint16_t expectedHeaderCrc = TauCamBase::crc16(pBuf, 6);
    uint16_t receivedHeaderCrc = (static_cast<uint16_t>(pBuf[6]) << 8) | pBuf[7];
    if (expectedHeaderCrc != receivedHeaderCrc) return TAU_RES_ERR_CRC;

    uint16_t responseDataLen = (static_cast<uint16_t>(pBuf[4]) << 8) | pBuf[5];
    uint32_t expectedLen = 8 + responseDataLen;
    if (responseDataLen != 0 || len > 8) {
        expectedLen += 2;
    }
    if (len != expectedLen) return TAU_RES_ERR_LEN;

    if (expectedLen > 8) {
        uint16_t expectedDataCrc = TauCamBase::crc16(pBuf, static_cast<uint16_t>(expectedLen - 2));
        uint16_t receivedDataCrc = (static_cast<uint16_t>(pBuf[expectedLen - 2]) << 8) |
                                   pBuf[expectedLen - 1];
        if (expectedDataCrc != receivedDataCrc) return TAU_RES_ERR_CRC;
    }

    if (pResponseDataLen != NULL) {
        *pResponseDataLen = responseDataLen;
    }

    if (pResponseData != NULL) {
        if (responseDataCapacity < responseDataLen) return TAU_RES_ERR_BUFFER;
        if (responseDataLen != 0) {
            memcpy(pResponseData, pBuf + 8, responseDataLen);
        }
    }

    return pBuf[1];
}