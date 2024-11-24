/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Sweep through all 7-bit I2C addresses, to see if any slaves are present on
// the I2C bus. Print out a table that looks like this:
//
// I2C Bus Scan
//    0 1 2 3 4 5 6 7 8 9 A B C D E F
// 00 . . . . . . . . . . . . . . . .
// 10 . . @ . . . . . . . . . . . . .
// 20 . . . . . . . . . . . . . . . .
// 30 . . . . @ . . . . . . . . . . .
// 40 . . . . . . . . . . . . . . . .
// 50 . . . . . . . . . . . . . . . .
// 60 . . . . . . . . . . . . . . . .
// 70 . . . . . . . . . . . . . . . .
// E.g. if addresses 0x12 and 0x34 were acknowledged.

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"


// I2C reserves some addresses for special purposes. We exclude these from the scan.
// These are any addresses of the form 000 0xxx or 111 1xxx
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}

#define SCAN_LED 11

int scan_i2c_bus(i2c_inst_t *i2c_ins, int loop_delay) {
    int i2c_addreses[130];
    gpio_init(SCAN_LED);
    gpio_set_dir(SCAN_LED, GPIO_OUT);
    bool led_state = 0;
    uint i=0;


        gpio_put(SCAN_LED, 1);
        printf("\nI2C Bus Scan\n");
        printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

        for (int addr = 0; addr < (1 << 7); ++addr) {
            if (addr % 16 == 0) {
                printf("%02x ", addr);
            }

            // Perform a 1-byte dummy read from the probe address. If a slave
            // acknowledges this address, the function returns the number of bytes
            // transferred. If the address byte is ignored, the function returns
            // -1.

            // Skip over any reserved addresses.
            int ret;
            uint8_t rxdata;
            if (reserved_addr(addr))
                ret = PICO_ERROR_GENERIC;
            else
                ret = i2c_read_blocking(i2c_ins, addr, &rxdata, 1, false);

            if(ret > 0){
                printf("@");
                i2c_addreses[addr] = addr;
            }else {
                printf(".");
                i2c_addreses[addr] = -1;
            }

            printf(addr % 16 == 15 ? "\n" : "  ");

            sleep_ms(10);
        }
      
        sleep_ms(loop_delay);
        gpio_put(SCAN_LED, 0);
        sleep_ms(100);
        gpio_put(SCAN_LED, 1);
        sleep_ms(100);
        gpio_put(SCAN_LED, 0);
        sleep_ms(100);
        printf("Scan loop Done.\n");

        printf("Addresses found on BUS: %d: \n", I2C_NUM(i2c_ins));

        for(uint i=0; i < 130; i++)
        {
            if(i2c_addreses[i] != -1){
                printf("\t%d\n", i2c_addreses[i]);
            }
        }
  
    return 0;
}