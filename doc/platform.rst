.. include:: vars.rst

.. _section-platform:

****************
Platform Support
****************

|RTI| |NANO_CLIENT| supports the major desktop operating
systems (Linux, Darwin, Windows), and the Arduino SDK.

The library includes a minimal portability layer which facilitate porting
|RTI| |NANO_CLIENT| to any platform with a 32-bit CPU/MCU.

The library is written using standard C99, single-threaded, and it does not
perform any dynamic memory allocation.

The porability layer includes abstractions for basic primitive types, memory 
manipulation and clock timestamps, which are often readily available in most
standard C library implementations, and supported by most C compilers,

Additionally, |RTI| |NANO_CLIENT| relies on a set of portable communications
interfaces to abstract the library from the underlying technological stack.

These abstractions are used to facilitate the porting of transport plugins to
different platforms.

.. _section-platform-portability:

Platform Portability Layer
==========================

The following tables summarize the portable interfaces used by |RTI| |NANO_CLIENT|
and the currenlty supported implementations.

The library will try to detect available features during compilation and select
the most appropriate implementation for the target if one is available.

This automatic detection can be disabled at any time by defining the
preprocessor symbol associated with a specific interface to the desired
implementation header, which can be useful to develop custom ports of the
library without modifying the library's header files.

+-----------------------+-----------------------------------+---------------------------+---------------------------------------+---------------------------------------+
|Portable Interface     |Interface Description              |Preprocessor Symbol        |Implementation Header                  |Implementation Description             |
+=======================+===================================+===========================+=======================================+=======================================+
|Compiler Definitions   |Defines primitive floating-point   |``NANO_CC_DEF_H``          |``nano_core_osapi_cc_stdc.h``          |Definitons for standard C compilers    |
|                       |number types (and integral ones, in|                           |                                       |(e.g. gcc, clang), and Microsoft's     |
|                       |case ``<stdint.h>`` is not         |                           |                                       |MSVC.                                  |
|                       |available), detects native         |                           |                                       |                                       |
|                       |endianness, enables 64-bit support.|                           |                                       |                                       |
+-----------------------+-----------------------------------+---------------------------+---------------------------------------+---------------------------------------+
|Platform Services      |Abstraction for system-level       |``NANO_OS_DEF_H``          |``nano_core_osapi_platform_posix.h``   |Implementation for POSIX(-ish)         |
|                       |services, such as clock, memory,   |                           |                                       |platforms, such as Linux, and Darwin.  |
|                       |and string manipulation            |                           +---------------------------------------+---------------------------------------+
|                       |                                   |                           |``nano_core_osapi_platform_win.h``     |Implementation for Windows platforms.  |
|                       |                                   |                           +---------------------------------------+---------------------------------------+
|                       |                                   |                           |``nano_core_osapi_platform_arduino.h`` |Implementation for platforms which     |
|                       |                                   |                           |                                       |support the Arduino SDK.               |
+-----------------------+-----------------------------------+---------------------------+---------------------------------------+---------------------------------------+
|IP Network Stack       |Abstractions for IP networking and |``NANO_IPNET_DEF_H``       |``nano_core_osapi_net_socket.h``       |Implementation for platforms with      |
|                       |UDP sockets.                       |                           |                                       |support for standard BSD sockets       |
|                       |                                   |                           |                                       |(including LwIP with socket support).  |
|                       |                                   |                           +---------------------------------------+---------------------------------------+
|                       |                                   |                           |``nano_core_osapi_net_lwip.h``         |Implementation using LwIP's Raw UDP    |
|                       |                                   |                           |                                       |API.                                   |
|                       |                                   |                           +---------------------------------------+---------------------------------------+
|                       |                                   |                           |``nano_core_osapi_net_win.h``          |Implementation for WinSocket2.         |
+-----------------------+-----------------------------------+---------------------------+---------------------------------------+---------------------------------------+
|Serial Communication   |Abstractions for communicating over|``NANO_SERIAL_DEF_H``      |``nano_core_osapi_serial_std.h``       |Implementation for platforms with      |
|                       |a serial port.                     |                           |                                       |support for the standard POSIX         |
|                       |                                   |                           |                                       |``<termios.h>`` interface.             |
|                       |                                   |                           +---------------------------------------+---------------------------------------+
|                       |                                   |                           |``nano_core_osapi_serial_arduino.h``   |Implementation on top of Arduino's     |
|                       |                                   |                           |                                       |``Serial`` interface.                  |
+-----------------------+-----------------------------------+---------------------------+---------------------------------------+---------------------------------------+
