/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "nano/nano_arduino_c.h"

#include "Arduino.h"

#include <HardwareSerial.h>

#ifndef NANO_ARDUINO_SERIAL_DELAY
#define NANO_ARDUINO_SERIAL_DELAY           10
#endif /* NANO_ARDUINO_SERIAL_DELAY */

#ifndef NANO_ARDUINO_SERIAL_WRITE_U
#define NANO_ARDUINO_SERIAL_WRITE_U         1
#endif /* NANO_ARDUINO_SERIAL_WRITE_U */

int NANO_ARDUINO_SerialPort_begin(
    const NANO_ARDUINO_SerialPort port,
    const int speed)
{
    int rc = 1;

    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        NANO_ARDUINO_SerialPort_0.begin(speed);
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        NANO_ARDUINO_SerialPort_1.begin(speed);
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

    rc = 0;
done:
    return rc;
}


int NANO_ARDUINO_SerialPort_end(
    const NANO_ARDUINO_SerialPort port)
{
    int rc = 1;

    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        NANO_ARDUINO_SerialPort_0.end();
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        NANO_ARDUINO_SerialPort_1.end();
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

    rc = 0;
done:
    return rc;
}

int NANO_ARDUINO_SerialPort_wait_for_available(
    const NANO_ARDUINO_SerialPort port,
    const int delay_ms)
{
    while (!NANO_ARDUINO_SerialPort_is_available(port))
    {
        delay(delay_ms);
    }

    return 0;
}

int NANO_ARDUINO_SerialPort_is_available(
    const NANO_ARDUINO_SerialPort port)
{
    int rc = 0;
    
    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        rc = NANO_ARDUINO_SerialPort_0.available();
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        rc = NANO_ARDUINO_SerialPort_1.available();
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

done:
    return rc;
}

int NANO_ARDUINO_SerialPort_set_timeout(
    const NANO_ARDUINO_SerialPort port,
    const int timeout)
{
    int rc = 1;

    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        NANO_ARDUINO_SerialPort_0.setTimeout(timeout);
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        NANO_ARDUINO_SerialPort_1.setTimeout(timeout);
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

    rc = 0;
done:
    return rc;
}

int NANO_ARDUINO_SerialPort_write(
    const NANO_ARDUINO_SerialPort port,
    const char *const data,
    const unsigned int data_len)
{
    size_t rc = -1;

#if NANO_ARDUINO_SERIAL_WRITE_U
#define write_serial_data(p_) \
{\
    rc = p_.write((const uint8_t*)data, data_len);\
    if (rc > 0)\
    {\
        p_.flush();\
    }\
}
        // delay(NANO_ARDUINO_SERIAL_DELAY);
#else
#define write_serial_data(p_) \
{\
    rc = p_.write(data, data_len);\
    if (rc > 0)\
    {\
        p_.flush();\
        delay(NANO_ARDUINO_SERIAL_DELAY);\
    }\
}
#endif
    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        write_serial_data(NANO_ARDUINO_SerialPort_0);
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        write_serial_data(NANO_ARDUINO_SerialPort_1);
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

done:
    return rc;
}

int NANO_ARDUINO_SerialPort_read(
    const NANO_ARDUINO_SerialPort port,
    char *const data,
    const unsigned int data_len,
    const long timeout_ms)
{
    unsigned long read_len = 0;
#if 1
#define read_serial_data(p_)\
{\
    uint32_t ts_last_byte_ = 0,\
             ts_now_ = millis();\
    while (p_.available() > 0 && read_len < data_len)\
    {\
        data[read_len] = p_.read();\
        read_len += 1;\
        if (timeout_ms >= 0)\
        {\
            ts_now_ = millis();\
            /* Simulate VMIN/VTIME behavior */\
            if (ts_last_byte_ > 0)\
            {\
                if ((ts_now_ - ts_last_byte_) > (unsigned long) timeout_ms)\
                {\
                    break;\
                }\
            }\
            ts_last_byte_ = ts_now_;\
        }\
    }\
}
    // delay(NANO_ARDUINO_SERIAL_DELAY);
#else
#define read_serial_data(p_)\
{\
    uint32_t ts_last_byte_ = 0,\
             ts_now_ = millis();\
    while (p_.available() > 0 && read_len < data_len)\
    {\
        data[read_len] = p_.read();\
        read_len += 1;\
    }\
}
#endif

    switch (port)
    {
    case NANO_ARDUINO_SERIAL_PORT_0:
    {
        read_serial_data(NANO_ARDUINO_SerialPort_0);
        break;
    }
#if NANO_ARDUINO_ENABLE_SERIAL_1
    case NANO_ARDUINO_SERIAL_PORT_1:
    {
        read_serial_data(NANO_ARDUINO_SerialPort_1);
        break;
    }
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */
    default:
    {
        goto done;
    }
    }

done:
    return read_len;
}