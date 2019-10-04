/* Copyright 2019 Raphael Randschau
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "wait.h"
#include "oled.h"
#include "i2c_master.h"
#include "m24m01.h"
#include "print.h"
#include "debug.h"

// write address: 0xA0
// read address: 0x50
#define EEPROM_PAGESIZE        128

uint8_t init_m24m01(void) {
    palSetPadMode(M24M01_WC_PAD, M24M01_WC_PIN, PAL_MODE_OUTPUT_PUSHPULL);
    // palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);
    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);

    return i2c2_isDeviceReady(EEPROM_ADDRESS, EEPROM_LONG_TIMEOUT);
}

uint8_t m24m01_byte_read(uint8_t address, uint8_t *data) {
    uint8_t status = i2c2_receive((address<<1)|1, data, 1, EEPROM_LONG_TIMEOUT);
    i2c2_stop();
    return status;
}

uint8_t m24m01_page_read(uint8_t address, uint16_t eepromAddr, uint8_t*data, uint16_t length)
{
    uint8_t location[2] = {(eepromAddr >> 8) & 0x00FF, (eepromAddr & 0x00FF)};
    uint8_t status = i2c2_transceive((address<<1), &location[0], 2, &data[0], length, EEPROM_LONG_TIMEOUT);
    i2c2_stop();
    return status;
}

uint8_t m24m01_page_write(uint8_t address, uint16_t eepromAddr, const uint8_t *data, uint16_t length)
{
    palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);
    wait_ms(6);

    uint8_t complete_packet[length + 2];
    for (uint8_t i = 0; i < length; i++) {
        complete_packet[i + 2] = data[i];
    }
    complete_packet[0] = (eepromAddr >> 8) & 0x00FF;
    complete_packet[1] = (eepromAddr & 0x00FF);

    uint8_t writeAddress = address << 1;
    uint8_t status = i2c2_transmit(writeAddress, &complete_packet[0], length+2, EEPROM_LONG_TIMEOUT);
    i2c2_stop();

    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);
    return status;
}

uint8_t m24m01_buffer_write(uint8_t address, uint16_t eepromAddr, const uint8_t *data, uint16_t length) {
//      uint16_t numofpage = 0, numofsingle = 0, count = 0;
//   uint16_t addr = 0;
//   uint32_t dataindex = 0;
//   uint32_t status = EEPROM_OK;

//   addr = WriteAddr % EEPROMPageSize;
//   count = EEPROMPageSize - addr;
//   numofpage =  NumByteToWrite / EEPROMPageSize;
//   numofsingle = NumByteToWrite % EEPROMPageSize;

//   /*!< If WriteAddr is EEPROM_PAGESIZE aligned  */
//   if(addr == 0)
//   {
//     /*!< If NumByteToWrite < EEPROM_PAGESIZE */
//     if(numofpage == 0)
//     {
//       /* Store the number of data to be written */
//       dataindex = numofsingle;
//       /* Start writing data */
//       status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//       if (status != EEPROM_OK)
//       {
//         return status;
//       }
//     }
//     /*!< If NumByteToWrite > EEPROM_PAGESIZE */
//     else
//     {
//       while(numofpage--)
//       {
//         /* Store the number of data to be written */
//         dataindex = EEPROMPageSize;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }

//         WriteAddr +=  EEPROMPageSize;
//         pBuffer += EEPROMPageSize;
//       }

//       if(numofsingle!=0)
//       {
//         /* Store the number of data to be written */
//         dataindex = numofsingle;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//       }
//     }
//   }
//   /*!< If WriteAddr is not EEPROM_PAGESIZE aligned  */
//   else
//   {
//     /*!< If NumByteToWrite < EEPROM_PAGESIZE */
//     if(numofpage== 0)
//     {
//       /*!< If the number of data to be written is more than the remaining space
//       in the current page: */
//       if (NumByteToWrite > count)
//       {
//         /* Store the number of data to be written */
//         dataindex = count;
//         /*!< Write the data contained in same page */
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }

//         /* Store the number of data to be written */
//         dataindex = (NumByteToWrite - count);
//         /*!< Write the remaining data in the following page */
//         status = EEPROM_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//       }
//       else
//       {
//         /* Store the number of data to be written */
//         dataindex = numofsingle;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//       }
//     }
//     /*!< If NumByteToWrite > EEPROM_PAGESIZE */
//     else
//     {
//       NumByteToWrite -= count;
//       numofpage =  NumByteToWrite / EEPROMPageSize;
//       numofsingle = NumByteToWrite % EEPROMPageSize;

//       if(count != 0)
//       {
//         /* Store the number of data to be written */
//         dataindex = count;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//         WriteAddr += count;
//         pBuffer += count;
//       }

//       while(numofpage--)
//       {
//         /* Store the number of data to be written */
//         dataindex = EEPROMPageSize;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//         WriteAddr +=  EEPROMPageSize;
//         pBuffer += EEPROMPageSize;
//       }
//       if(numofsingle != 0)
//       {
//         /* Store the number of data to be written */
//         dataindex = numofsingle;
//         status = EEPROM_WritePage(pBuffer, WriteAddr, (uint32_t*)(&dataindex));
//         if (status != EEPROM_OK)
//         {
//           return status;
//         }
//       }
//     }
//   }

    return 0;
}

uint8_t m24m01_random_byte_read(uint8_t address, uint16_t eepromAddr, uint8_t *data) {
    const uint8_t location[2] = {(eepromAddr >> 8) & 0x00FF, (eepromAddr & 0x00FF)};
    uint8_t status = i2c2_transmit((address<<1), &location[0], 2, EEPROM_LONG_TIMEOUT);
    status = i2c2_receive((address<<1)|1, data, 1, EEPROM_LONG_TIMEOUT);
    i2c2_stop();
    return status;
}

uint8_t m24m01_byte_write(uint8_t address, uint16_t eepromAddr, uint8_t data) {
    const uint8_t location_and_byte[3] = {(eepromAddr >> 8) & 0x00FF, (eepromAddr & 0x00FF), data};
    palClearPad(M24M01_WC_PAD, M24M01_WC_PIN);

    uint8_t status = i2c2_transmit(address<<1, &location_and_byte[0], 3, EEPROM_LONG_TIMEOUT);
    i2c2_stop();

    palSetPad(M24M01_WC_PAD, M24M01_WC_PIN);

    // this is a hack - waiting until the device is ready should be done instead
    wait_ms(6);
    return status;
}
