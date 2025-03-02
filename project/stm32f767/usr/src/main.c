/**
 * Copyright (C) LibDriver 2015-2021 All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_w25qxx_basic.h"
#include "driver_w25qxx_advance.h"
#include "driver_w25qxx_read_test.h"
#include "driver_w25qxx_register_test.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
uint16_t g_len;                            /**< uart buffer length */

/**
 * @brief     w25qxx full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *             - 0 success
 *             - 1 run failed
 *             - 5 param is invalid
 * @note      none
 */
uint8_t w25qxx(uint8_t argc, char **argv)
{
    if (argc == 1)
    {
        goto help;
    }
    else if (argc == 2)
    {
        if (strcmp("-i", argv[1]) == 0)
        {
            w25qxx_info_t info;
            
            /* print w25qxx info */
            w25qxx_info(&info);
            w25qxx_interface_debug_print("w25qxx: chip is %s.\n", info.chip_name);
            w25qxx_interface_debug_print("w25qxx: manufacturer is %s.\n", info.manufacturer_name);
            w25qxx_interface_debug_print("w25qxx: interface is %s.\n", info.interface);
            w25qxx_interface_debug_print("w25qxx: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
            w25qxx_interface_debug_print("w25qxx: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
            w25qxx_interface_debug_print("w25qxx: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
            w25qxx_interface_debug_print("w25qxx: max current is %0.2fmA.\n", info.max_current_ma);
            w25qxx_interface_debug_print("w25qxx: max temperature is %0.1fC.\n", info.temperature_max);
            w25qxx_interface_debug_print("w25qxx: min temperature is %0.1fC.\n", info.temperature_min);
            
            return 0;
        }
        else if (strcmp("-p", argv[1]) == 0)
        {
            /* print pin connection */
            w25qxx_interface_debug_print("w25qxx: QSPI interface SCK connected to GPIOB PIN2.\n");
            w25qxx_interface_debug_print("w25qxx: QSPI interface IO0 connected to GPIOF PIN8.\n");
            w25qxx_interface_debug_print("w25qxx: QSPI interface IO1 connected to GPIOF PIN9.\n");
            w25qxx_interface_debug_print("w25qxx: QSPI interface IO2 connected to GPIOF PIN7.\n");
            w25qxx_interface_debug_print("w25qxx: QSPI interface IO3 connected to GPIOF PIN6.\n");
            w25qxx_interface_debug_print("w25qxx: QSPI interface CS connected to GPIOB PIN6.\n");
            
            return 0;
        }
        else if (strcmp("-h", argv[1]) == 0)
        {
            /* show w25qxx help */
            help:
            
            w25qxx_interface_debug_print("w25qxx -i\n\tshow w25qxx chip and driver information.\n");
            w25qxx_interface_debug_print("w25qxx -h\n\tshow w25qxx help.\n");
            w25qxx_interface_debug_print("w25qxx -p\n\tshow w25qxx pin connections of the current board.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> power_down (-spi| -qspi)\n\trun w25qxx basic power down function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> wake_up (-spi| -qspi)\n\trun w25qxx basic wake up function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> chip_erase (-spi| -qspi)\n\trun w25qxx basic chip erase function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> get_id (-spi| -qspi)\n\trun w25qxx basic get id function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> read <addr> (-spi| -qspi)\n\trun w25qxx basic read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c basic -type <type> write <addr> <data> (-spi| -qspi)\n\trun w25qxx basic write function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> power_down (-spi| -qspi)\n\trun w25qxx advance power down function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> wake_up (-spi| -qspi)\n\trun w25qxx advance wake up function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> chip_erase (-spi| -qspi)\n\trun w25qxx advance chip erase function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> get_id (-spi| -qspi)\n\trun w25qxx advance get id function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> read <addr> (-spi| -qspi)\n\trun w25qxx advance read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> write <addr> <data> (-spi| -qspi)\n\trun w25qxx advance write function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> page_program <addr> <data> (-spi| -qspi)\n\trun w25qxx advance page program function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> erase_4k <addr> (-spi| -qspi)\n\trun w25qxx advance erase 4k function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> erase_32k <addr> (-spi| -qspi)\n\trun w25qxx advance erase 32k function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> erase_64k <addr> (-spi| -qspi)\n\trun w25qxx advance erase 64k function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> fast_read <addr> (-spi| -qspi)\n\trun w25qxx advance fast read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> get_status1 (-spi| -qspi)\n\trun w25qxx advance get status1 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> get_status2 (-spi| -qspi)\n\trun w25qxx advance get status2 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> get_status3 (-spi| -qspi)\n\trun w25qxx advance get status3 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> set_status1 <status> (-spi| -qspi)\n\trun w25qxx advance set status1 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "status is the set status.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> set_status2 <status> (-spi| -qspi)\n\trun w25qxx advance set status2 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "status is the set status.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> set_status3 <status> (-spi| -qspi)\n\trun w25qxx advance set status3 function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "status is the set status.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> get_jedec_id (-spi| -qspi)\n\trun w25qxx advance get jedec id function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> global_lock (-spi| -qspi)\n\trun w25qxx advance global lock function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> global_unlock (-spi| -qspi)\n\trun w25qxx advance global unlock function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> read_block <addr> (-spi| -qspi)\n\trun w25qxx advance read block function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> reset (-spi| -qspi)\n\trun w25qxx advance reset function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_read <addr>\n\trun w25qxx advance spi read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_dual_output_read <addr>\n\trun w25qxx advance spi dual output read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_quad_output_read <addr>\n\trun w25qxx advance spi quad output read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_dual_io_read <addr>\n\trun w25qxx advance spi dual io read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_quad_io_read <addr>\n\trun w25qxx advance spi quad io read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_word_quad_io_read <addr>\n\trun w25qxx advance spi word quad io read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_octal_word_quad_io_read <addr>\n\trun w25qxx advance spi octal word quad io read function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_page_program_quad_input <addr> <data>\n\trun w25qxx advance page program quad input function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.data is the written data.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_get_id_dual_io\n\trun w25qxx advance get id dual io function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_get_id_quad_io\n\trun w25qxx advance get id quad io function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_get_sfdp\n\trun w25qxx advance get sfdp function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_write_security_reg <num> <data>\n\trun w25qxx advance write security register function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "addr is the flash address.num is the security register number.data is the written data.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_read_security_reg <num>\n\trun w25qxx advance read security register function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "num is the security register number.num can be 1, 2, 3.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> qspi_set_read_parameters <dummy> <length>\n\trun w25qxx advance set read parameters function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "dummy is the read dummy and it can be DUMMY_2_33MHZ, DUMMY_4_55MHZ, DUMMY_6_80MHZ, DUMMY_8_80MHZ."
                                         "length is the wrap length and it can be 8, 16, 32 or 64.\n");
            w25qxx_interface_debug_print("w25qxx -c advance -type <type> spi_set_burst <wrap>\n\trun w25qxx advance set burst function.");
            w25qxx_interface_debug_print("type is the chip type and type can be W25Q80, W25Q16, W25Q32, W25Q64, W25Q128 or W25Q256."
                                         "wrap is the burst wrap and it can be WRAP_NONE,WRAP_8_BYTE,WRAP_16_BYTE,WRAP_32_BYTE,WRAP_64_BYTE.\n");
            
            return 0;
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 6)
    {
        if (strcmp("-t", argv[1]) == 0)
        {
            if (strcmp("reg", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[5]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[5]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = w25qxx_register_test(type, interface, W25QXX_BOOL_TRUE);
                    if (res)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("read", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[5]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[5]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    res = w25qxx_read_test(type, interface, W25QXX_BOOL_TRUE);
                    if (res)
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("spi_get_id_dual_io", argv[5]) == 0)
                    {
                        volatile uint8_t manufacturer;
                        volatile uint8_t device_id;
                        
                        res = w25qxx_advance_init(type, W25QXX_INTERFACE_SPI, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_get_manufacturer_device_id_dual_io((uint8_t *)&manufacturer,
                                                                                         (uint8_t *)&device_id);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n",
                                                         manufacturer, device_id);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_get_id_quad_io", argv[5]) == 0)
                    {
                        volatile uint8_t manufacturer;
                        volatile uint8_t device_id;
                        
                        res = w25qxx_advance_init(type, W25QXX_INTERFACE_SPI, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_get_manufacturer_device_id_quad_io((uint8_t *)&manufacturer,
                                                                                         (uint8_t *)&device_id);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n",
                                                         manufacturer, device_id);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_get_sfdp", argv[5]) == 0)
                    {
                        volatile uint8_t sfdp[256];
                        
                        res = w25qxx_advance_init(type, W25QXX_INTERFACE_SPI, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_get_sfdp((uint8_t *)sfdp);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            for (uint16_t j = 0; j < 256; j += 8)
                            {
                                w25qxx_interface_debug_print("w25qxx: sdfp[%d-%d] is 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n", 
                                                             j , j + 7,
                                                             sfdp[j + 0], sfdp[j + 1], sfdp[j + 2],
                                                             sfdp[j + 3], sfdp[j + 4], sfdp[j + 5],
                                                             sfdp[j + 6], sfdp[j + 7]);
                            }
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 7)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[6]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[6]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("power_down", argv[5]) == 0)
                    {
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_basic_power_down();
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: power down successful.\n");
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("wake_up", argv[5]) == 0)
                    {
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_basic_wake_up();
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: wake up successful.\n");
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("chip_erase", argv[5]) == 0)
                    {
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        w25qxx_interface_debug_print("w25qxx: start chip erase.\n");
                        res = w25qxx_basic_chip_erase();
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_id", argv[5]) == 0)
                    {
                        volatile uint8_t manufacturer;
                        volatile uint8_t device_id;
                        
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_basic_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n", 
                                                         manufacturer, device_id);
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[6]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[6]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        if (strstr(argv[5], "spi_"))
                        {
                            interface = W25QXX_INTERFACE_SPI;
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    if (strcmp("power_down", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_power_down();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: power down successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("wake_up", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_wake_up();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: wake up successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("chip_erase", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        w25qxx_interface_debug_print("w25qxx: start chip erase.\n");
                        res = w25qxx_advance_chip_erase();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: chip erase successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_id", argv[5]) == 0)
                    {
                        volatile uint8_t manufacturer;
                        volatile uint8_t device_id;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X.\n", 
                                                                                     manufacturer, device_id);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_jedec_id", argv[5]) == 0)
                    {
                        volatile uint8_t manufacturer;
                        volatile uint8_t device_id[2];
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_get_get_jedec_id((uint8_t *)&manufacturer, (uint8_t *)device_id);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: manufacturer is 0x%02X device id is 0x%02X 0x%02X.\n", 
                                                          manufacturer, device_id[0], device_id[1]);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("global_lock", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_global_block_lock();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: global block lock successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("global_unlock", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_global_block_unlock();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: global block unlock successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("reset", argv[5]) == 0)
                    {
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_reset();
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: reset successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_status1", argv[5]) == 0)
                    {
                        volatile uint8_t status;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_get_status1((uint8_t *)&status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: get status1 is 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_status2", argv[5]) == 0)
                    {
                        volatile uint8_t status;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_get_status2((uint8_t *)&status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: get status2 is 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("get_status3", argv[5]) == 0)
                    {
                        volatile uint8_t status;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_get_status3((uint8_t *)&status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: get status3 is 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_read(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_dual_output_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_fast_read_dual_output(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi dual output read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_quad_output_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_fast_read_quad_output(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi quad output read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_dual_io_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_fast_read_dual_io(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi dual io read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_quad_io_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_fast_read_quad_io(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi quad io read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_word_quad_io_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_word_read_quad_io(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi word quad io read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_octal_word_quad_io_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_octal_word_read_quad_io(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d spi octal word quad io read is 0x%02X.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_set_burst", argv[5]) == 0)
                    {
                        w25qxx_burst_wrap_t wrap;
                        
                        if (strcmp("WRAP_NONE", argv[6]) == 0)
                        {
                            wrap = W25QXX_BURST_WRAP_NONE;
                        }
                        else if (strcmp("WRAP_8_BYTE", argv[6]) == 0)
                        {
                            wrap = W25QXX_BURST_WRAP_8_BYTE;
                        }
                        else if (strcmp("WRAP_16_BYTE", argv[6]) == 0)
                        {
                            wrap = W25QXX_BURST_WRAP_16_BYTE;
                        }
                        else if (strcmp("WRAP_32_BYTE", argv[6]) == 0)
                        {
                            wrap = W25QXX_BURST_WRAP_32_BYTE;
                        }
                        else if (strcmp("WRAP_64_BYTE", argv[6]) == 0)
                        {
                            wrap = W25QXX_BURST_WRAP_64_BYTE;
                        }
                        else
                        {
                            return 5;
                        }
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_set_burst_with_wrap(wrap);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: set burst with wrap successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_read_security_reg", argv[5]) == 0)
                    {
                        volatile uint8_t n = atoi(argv[6]);
                        w25qxx_security_register_t num;
                        volatile uint8_t data[256];
                        
                        if (n == 1)
                        {
                            num = W25QXX_SECURITY_REGISTER_1;
                        }
                        else if (n == 2)
                        {
                            num = W25QXX_SECURITY_REGISTER_2;
                        }
                        else if (n == 3)
                        {
                            num = W25QXX_SECURITY_REGISTER_3;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_read_security_register(num, (uint8_t *)data);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: read %d security register.\n", n);
                            for (uint16_t i = 0; i < 256; i += 8)
                            {
                                w25qxx_interface_debug_print("w25qxx: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X.\n",
                                                             data[i + 0], data[i + 1], data[i + 2], data[i + 3],
                                                             data[i + 4], data[i + 5], data[i + 6], data[i + 7]
                                                            );
                            }
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 8)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[7]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[7]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_basic_read(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, data);
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[7]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[7]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        if (strstr(argv[5], "qspi_"))
                        {
                            interface = W25QXX_INTERFACE_QSPI;
                        }
                        else if (strstr(argv[5], "spi_"))
                        {
                            interface = W25QXX_INTERFACE_SPI;
                        }
                        else
                        {
                            return 5;
                        }
                    }
                    
                    if (strcmp("read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_read(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("erase_4k", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_sector_erase_4k(addr);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: erase 4k addr %d successful.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("erase_32k", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_block_erase_32k(addr);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: erase 32k addr %d successful.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("erase_64k", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_block_erase_64k(addr);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: erase 64k addr %d successful.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("fast_read", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_fast_read(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d is %d.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("set_status1", argv[5]) == 0)
                    {
                        volatile uint8_t status = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_set_status1(status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: set status1 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("set_status2", argv[5]) == 0)
                    {
                        volatile uint8_t status = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_set_status2(status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: set status2 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("set_status3", argv[5]) == 0)
                    {
                        volatile uint8_t status = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_set_status3(status);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: set status3 0x%02X.\n", status);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("block_lock", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_individual_block_lock(addr);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: block lock addr %d.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("block_unlock", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_individual_block_unlock(addr);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: block unlock addr %d.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("read_block", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t value;
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_read_block_lock(addr, (uint8_t *)&value);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: read block addr %d is %d.\n", addr, value);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_page_program_quad_input", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data = atoi(argv[7]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_spi_page_program_quad_input(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: add %d page program quad input successful.\n", addr);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("spi_write_security_reg", argv[5]) == 0)
                    {
                        volatile uint8_t n = atoi(argv[6]);
                        volatile uint8_t data = atoi(argv[7]);
                        volatile uint8_t buf[256];
                        w25qxx_security_register_t num;
                        
                        if (n == 1)
                        {
                            num = W25QXX_SECURITY_REGISTER_1;
                        }
                        else if (n == 2)
                        {
                            num = W25QXX_SECURITY_REGISTER_2;
                        }
                        else if (n == 3)
                        {
                            num = W25QXX_SECURITY_REGISTER_3;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        memset((uint8_t *)buf, data, 256);
                        res = w25qxx_advance_only_spi_write_security_register(num, (uint8_t *)buf);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: write %d security register successful.\n", n);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("qspi_set_read_parameters", argv[5]) == 0)
                    {
                        w25qxx_qspi_read_dummy_t dummy;
                        w25qxx_qspi_read_wrap_length_t length;
                        
                        if (strcmp("DUMMY_2_33MHZ", argv[6]) == 0)
                        {
                            dummy = W25QXX_QSPI_READ_DUMMY_2_33MHZ;
                        }
                        else if (strcmp("DUMMY_4_55MHZ", argv[6]) == 0)
                        {
                            dummy = W25QXX_QSPI_READ_DUMMY_4_55MHZ;
                        }
                        else if (strcmp("DUMMY_6_80MHZ", argv[6]) == 0)
                        {
                            dummy = W25QXX_QSPI_READ_DUMMY_6_80MHZ;
                        }
                        else if (strcmp("DUMMY_8_80MHZ", argv[6]) == 0)
                        {
                            dummy = W25QXX_QSPI_READ_DUMMY_8_80MHZ;
                        }
                        else
                        {
                            return 5;
                        }
                        if (strcmp("8", argv[7]) == 0)
                        {
                            length = W25QXX_QSPI_READ_WRAP_LENGTH_8_BYTE;
                        }
                        else if (strcmp("16", argv[7]) == 0)
                        {
                            length = W25QXX_QSPI_READ_WRAP_LENGTH_16_BYTE;
                        }
                        else if (strcmp("32", argv[7]) == 0)
                        {
                            length = W25QXX_QSPI_READ_WRAP_LENGTH_32_BYTE;
                        }
                        else if (strcmp("64", argv[7]) == 0)
                        {
                            length = W25QXX_QSPI_READ_WRAP_LENGTH_64_BYTE;
                        }
                        else
                        {
                            return 5;
                        }
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_only_qspi_set_read_parameters(dummy, length);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: set read parameters successful.\n");
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }
    else if (argc == 9)
    {
        if (strcmp("-c", argv[1]) == 0)
        {
            if (strcmp("basic", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[8]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[8]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("write", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data = atoi(argv[7]);
                        
                        res = w25qxx_basic_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_basic_write(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d write data %d.\n", addr, data);
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else if (strcmp("advance", argv[2]) == 0)
            {
                if (strcmp("-type", argv[3]) == 0)
                {
                    volatile uint8_t res;
                    w25qxx_type_t type;
                    w25qxx_interface_t interface;
                    
                    if (strcmp("W25Q80", argv[4]) == 0)
                    {
                        type = W25Q80;
                    }
                    else if (strcmp("W25Q16", argv[4]) == 0)
                    {
                        type = W25Q16;
                    }
                    else if (strcmp("W25Q32", argv[4]) == 0)
                    {
                        type = W25Q32;
                    }
                    else if (strcmp("W25Q64", argv[4]) == 0)
                    {
                        type = W25Q64;
                    }
                    else if (strcmp("W25Q128", argv[4]) == 0)
                    {
                        type = W25Q128;
                    }
                    else if (strcmp("W25Q256", argv[4]) == 0)
                    {
                        type = W25Q256;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("-spi", argv[8]) == 0)
                    {
                        interface = W25QXX_INTERFACE_SPI;
                    }
                    else if (strcmp("-qspi", argv[8]) == 0)
                    {
                        interface = W25QXX_INTERFACE_QSPI;
                    }
                    else
                    {
                        return 5;
                    }
                    
                    if (strcmp("write", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data = atoi(argv[7]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_write(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_advance_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d write data %d.\n", addr, data);
                        }
                        
                        w25qxx_advance_deinit();
                        
                        return 0;
                    }
                    else if (strcmp("page_program", argv[5]) == 0)
                    {
                        volatile uint32_t addr = atoi(argv[6]);
                        volatile uint8_t data = atoi(argv[7]);
                        
                        res = w25qxx_advance_init(type, interface, W25QXX_BOOL_TRUE);
                        if (res)
                        {
                            return 1;
                        }
                        res = w25qxx_advance_page_program(addr, (uint8_t *)&data, 1);
                        if (res)
                        {
                            w25qxx_basic_deinit();
                            
                            return 1;
                        }
                        else
                        {
                            w25qxx_interface_debug_print("w25qxx: addr %d page program data %d.\n", addr, data);
                        }
                        
                        w25qxx_basic_deinit();
                        
                        return 0;
                    }
                    else
                    {
                        return 5;
                    }
                }
                else
                {
                    return 5;
                }
            }
            else
            {
                return 5;
            }
        }
        else
        {
            return 5;
        }
    }

    /* param is invalid */
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    volatile uint8_t res;
    
    /* stm32f767 clock init and hal init */
    clock_init();
    
    /* delay init */
    delay_init();
    
    /* uart1 init */
    uart1_init(115200);
    
    /* shell init && register w25qxx fuction */
    shell_init();
    shell_register("w25qxx", w25qxx);
    uart1_print("w25qxx: welcome to libdriver w25qxx.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart1_read(g_buf, 256);
        if (g_len)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart1_print("w25qxx: run failed.\n");
            }
            else if (res == 2)
            {
                uart1_print("w25qxx: unknow command.\n");
            }
            else if (res == 3)
            {
                uart1_print("w25qxx: length is too long.\n");
            }
            else if (res == 4)
            {
                uart1_print("w25qxx: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart1_print("w25qxx: param is invalid.\n");
            }
            else
            {
                uart1_print("w25qxx: unknow status code.\n");
            }
            uart1_flush();
        }
        delay_ms(100);
    }
}
