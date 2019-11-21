/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-11-17     LiWeiHao     First implementation
 */

#include <rtdevice.h>
#include <rtthread.h>

#if defined(BSP_USING_PCT2705)

rt_err_t pct2705_read(void)
{
    rt_size_t result;
    struct rt_i2c_bus_device *bus = NULL;
    struct rt_i2c_msg msgs[2];
    rt_uint8_t reg = 0x00;
    rt_uint8_t read_buffer[2] = {0};
    rt_uint16_t value;

    bus = (struct rt_i2c_bus_device *)rt_device_find("i2c4");
    if (bus == NULL)
    {
        rt_kprintf("BUS not found\n");
        return -RT_ERROR;
    }

    msgs[0].addr = 0x4c;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf = &reg;
    msgs[1].len = 1;

    msgs[1].addr = 0x4c;
    msgs[1].flags = RT_I2C_RD;
    msgs[1].buf = read_buffer;
    msgs[1].len = 2;

    result = rt_i2c_transfer(bus, msgs, 2);
    if (result != 2)
    {
        rt_kprintf("Communication failure\n");
        return -RT_ERROR;
    }

    value = read_buffer[0];
    value = (value << 8) + read_buffer[1];
    value = (value >> 5);
    value = (rt_uint16_t)(value * 12.5);

    rt_kprintf("temp: %d.%d\n", value / 100, value % 100 / 10);
    return RT_EOK;
}
MSH_CMD_EXPORT(pct2705_read, pct2705 read);

#endif
