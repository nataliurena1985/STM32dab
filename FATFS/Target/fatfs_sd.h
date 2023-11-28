#ifndef __FATFS_SD_H
#define __FATFS_SD_H

/* Definitions for MMC/SDC command */
#include "integer.h" //from FatFs middleware library
#include "diskio.h" //from FatFs middleware library
#include "ff_gen_drv.h" //from FatFs middleware library

#define CMD_0     (0x40+0)     // GO_IDLE_STATE
#define CMD_1     (0x40+1)     // SEND_OP_COND
#define CMD_8     (0x40+8)     // SEND_IF_COND
#define CMD_9     (0x40+9)     // SEND_CSD
#define CMD_10    (0x40+10)    // SEND_CID
#define CMD_12    (0x40+12)    // STOP_TRANSMISSION
#define CMD_16    (0x40+16)    // SET_BLOCKLEN
#define CMD_17    (0x40+17)    // READ_SINGLE_BLOCK
#define CMD_18    (0x40+18)    // READ_MULTIPLE_BLOCK
#define CMD_23    (0x40+23)    // SET_BLOCK_COUNT
#define CMD_24    (0x40+24)    // WRITE_BLOCK
#define CMD_25    (0x40+25)    // WRITE_MULTIPLE_BLOCK
#define CMD_41    (0x40+41)    // SEND_OP_COND (ACMD_)
#define CMD_55    (0x40+55)    // APP_CMD_
#define CMD_58    (0x40+58)    // READ_OCR

DSTATUS SD_disk_initialize (BYTE pdrv);
DSTATUS SD_disk_status (BYTE pdrv);
DRESULT SD_disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT SD_disk_ioctl (BYTE pdrv, BYTE CMD_, void* buff);

#define SPI_TIMEOUT 1000

#endif



