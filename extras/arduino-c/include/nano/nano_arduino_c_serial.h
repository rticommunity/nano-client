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

#ifndef nano_arduino_c_serial_h
#define nano_arduino_c_serial_h

#ifndef NANO_ARDUINO_ENABLE_SERIAL_1
#define NANO_ARDUINO_ENABLE_SERIAL_1        0
#endif /* NANO_ARDUINO_ENABLE_SERIAL_1 */

#ifndef NANO_ARDUINO_SerialPort_0
#define NANO_ARDUINO_SerialPort_0       Serial
#endif /* NANO_ARDUINO_SerialPort_0 */

#ifndef NANO_ARDUINO_SerialPort_1
#define NANO_ARDUINO_SerialPort_1       Serial1
#endif /* NANO_ARDUINO_SerialPort_1 */

typedef enum NANO_ARDUINO_SerialPortI
{
    NANO_ARDUINO_SERIAL_PORT_0,
    NANO_ARDUINO_SERIAL_PORT_1
} NANO_ARDUINO_SerialPort;

int NANO_ARDUINO_SerialPort_begin(
    const NANO_ARDUINO_SerialPort port,
    const int speed);

int NANO_ARDUINO_SerialPort_end(
    const NANO_ARDUINO_SerialPort port);

int NANO_ARDUINO_SerialPort_wait_for_available(
    const NANO_ARDUINO_SerialPort port,
    const int delay);

int NANO_ARDUINO_SerialPort_is_available(
    const NANO_ARDUINO_SerialPort port);

int NANO_ARDUINO_SerialPort_set_timeout(
    const NANO_ARDUINO_SerialPort port,
    const int timeout);

int NANO_ARDUINO_SerialPort_write(
    const NANO_ARDUINO_SerialPort port,
    const char *const data,
    const unsigned int data_len);

int NANO_ARDUINO_SerialPort_read(
    const NANO_ARDUINO_SerialPort port,
    char *const data,
    const unsigned int data_len,
    const long timeout_ms);

#endif /* nano_arduino_c_serial_h */