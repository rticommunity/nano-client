.. include:: vars.rst

.. _section-building:

********
Building
********

|RTI| |NANO_CLIENT| can be easily compiled for most target platforms using
:link_cmake:`CMake <>`.

The most notable exception is the Arduino SDK, which is supported via the
:link_arduino_ide:`Arduino IDE <>`, and the :link_platformio:`PlatformIO <>`
build system.

When building with CMake, it is recommended to use build target ``install``
to trigger the generation of all build artifacts, and then copy
them into single, better organized, directory (by default:
``<build-dir>/install``).

This makes it easier to pick up the library from other projects, by
consolidating all header files and libraries into a single location.

Additionally, the build process will generate CMake Config files which
trivialize the inclusion of the library in any CMake build using CMake's
``find_package()`` function.


.. _section-building-cmake-c:

Building the C API with CMake
=============================

The C API can be compiled by building the repository's root
directory. For example:

.. code-block:: sh

    # Create a build directory and enter it
    mkdir build && cd build

    # Run cmake to configure build
    cmake /path/to/nano-client

    # Call native build tool
    cmake --build . --target install


.. _section-building-cmake-cpp:

Building the C++ API with CMake
===============================

The C++ API can be compiled by building directory ``extras/nano-client-cpp``.
The C API will also be automatically built.

E.g.:

.. code-block:: sh

    # Create a build directory and enter it
    mkdir build && cd build

    # Run cmake to configure build
    cmake /path/to/nano-client/extras/nano-client-cpp

    # Call native build tool
    cmake --build . --target install


.. _section-building-arduino:

Building with the Arduino IDE
=============================

After generating and installing the Arduino Package (see
:ref:`section-install-arduino`), you can enable |RTI| |NANO_CLIENT| in your 
sketch by including header file ``Nano.h``:

.. code-block:: cpp

    #include <nano_client_arduino.h>


.. _section-building-platformio:

Building with PlatformIO
========================

After installing the Arduino Package as described in
:ref:`section-install-platformio`, you can enable |RTI| |NANO_CLIENT|
by including its main header file in your application:

.. code-block:: cpp

    #include <nano_client_arduino.h>

This ``#include`` should allow PlatformIO's library manager to detect the
library and include it in the build.

You can also explicitly enable |RTI| |NANO_CLIENT| in your ``platformio.ini``
file by adding it to your environment's ``lib_deps`` entry:

.. code-block:: ini

    [env:MyEnv]
    lib_deps = nano-client-arduino

.. _section-building-xcc:

Cross-compilation with CMake
============================

.. _section-building-xcc-rpi:

Raspberry Pi
------------

.. code-block:: cmake

    #
    # This toolchain file can be used to cross-compile RTI nano-client for
    # Raspberry Pi.
    #
    # Make sure that the following variables are set in your shell's 
    # environment:
    #
    #   - RPI_TOOLS_DIR : clone of https://github.com/raspberrypi/tools
    #
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_VERSION 1)

    # This toolchain can be used for both RPi2 and RPi3.
    # Might need a different toolchain for RPi4/64bit
    set(RPI_TOOLCHAIN_DIR
        $ENV{RPI_TOOLS_DIR}/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian)

    set(CMAKE_C_COMPILER   ${RPI_TOOLCHAIN_DIR}/bin/arm-linux-gnueabihf-gcc)
    set(CMAKE_CXX_COMPILER ${RPI_TOOLCHAIN_DIR}/bin/arm-linux-gnueabihf-g++)

    list(APPEND CMAKE_FIND_ROOT_PATH      ${RPI_TOOLCHAIN_DIR})

    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    # Explicitly link rt library to nanocore library
    set(NANO_CORE_EXTRA_LIBS            rt)


Save the previous block in a file named ``rpi_toolchain.cmake`` and
build |RTI| |NANO_CLIENT| with the following snippet:

.. code-block:: sh

    # Clone RPi build tools and export their location as RPI_TOOLS_DIR
    git clone https://github.com/raspberrypi/tools
    export RPI_TOOLS_DIR=$(pwd)/tools

    # Create a build directory and enter it
    mkdir build-rpi && cd build-rpi

    # Configure build using the custom toolchain. Optional: build examples.
    cmake /path/to/nano-client -DCMAKE_TOOLCHAIN_FILE=rpi_toolchain.cmake \
                               -DENABLE_EXAMPLES=ON
    
    # Compile and copy to install location (./install)
    cmake --build . --target install -- -j8

    # Copy install tree to RPi home directory, e.g. with rsync:
    rsync -ra ./install/nano pi@my-rpi:~/


.. _section-building-cmake-options:

CMake Build Options
===================

This section provides information of variables that can be used to control the
behavior of |RTI| |NANO_CLIENT|'s CMake build script.


.. _section-building-cmake-options-buildexecutables:

BUILD_EXECUTABLES
-----------------

Description:
    Build included executables (e.g. examples).

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``ON``

.. _section-building-cmake-options-buildlibraries:

BUILD_LIBRARIES
---------------

Description:
    Build included libraries.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``ON``

.. _section-building-cmake-options-buildsharedlibs:

BUILD_SHARED_LIBS
-----------------

Description:
    :link_cmake_var:`Standard CMake option <BUILD_SHARED_LIBS>`
    which controls whether libraries and executables will be linked statically
    or dynamically.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``OFF``

.. _section-building-cmake-options-cmakebuildtype:

CMAKE_BUILD_TYPE
----------------

Description:
    :link_cmake_var:`Standard CMake option <CMAKE_BUILD_TYPE>`
    which controls the type of libraries and executables to build.

Accepted Values:
    ``"Release"``, ``"Debug"``, ``"RelWithDebInfo"``, ``"MinSizeRel"``

Default Value:
    ``Release``

.. _section-building-cmake-options-cmakeinstallprefix:

CMAKE_INSTALL_PREFIX
--------------------

Description:
    :link_cmake_var:`Standard CMake option <CMAKE_INSTALL_PREFIX>`
    which controls the type of libraries and executables to build.

Accepted Values:
    A valid path in CMake syntax (i.e. using forward-slashes ``/`` only).

Default Value:
    ``${CMAKE_CURRENT_BUILD_DIR}/install``

.. _section-building-cmake-options-enablearduino:

ENABLE_ARDUINO
--------------

Description:
    Include additional code required to support the Arduino SDK.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``OFF``

.. _section-building-cmake-options-enableexamples:

ENABLE_EXAMPLES
---------------

Description:
    Include examples (copy to install directory, and optionally build them
    if `BUILD_EXECUTABLES` is also enabled).

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``OFF``

.. _section-building-cmake-options-enabletransportall:

ENABLE_TRANSPORT_ALL
--------------------

Description:
    Include all available transport plugins.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``ON``

.. _section-building-cmake-options-enabletransportserial:

ENABLE_TRANSPORT_SERIAL
-----------------------

Description:
    Include Serial transport plugins.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``ON``

.. _section-building-cmake-options-enabletransportudpv4:

ENABLE_TRANSPORT_UDPV4
----------------------

Description:
    Include UDPv4 transport plugins.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``ON``

.. _section-building-cmake-options-installsource:

INSTALL_SOURCE
--------------

Description:
    Export source files to install location.

Accepted Values:
    ``ON``, ``OFF``

Default Value:
    ``OFF``


.. _section-building-libraries:

Generated Libraries
===================

The following table summarizes the libraries that will be generated by the
build process. Each library is associated with a CMake target (scoped 
within the ``nano::`` namespace) which is configured so that the library can
be easily linked using ``target_link_libraries(<my-tgt> [PRIVATE|PUBLIC|INTERFACE] <lib-tgt>)``
(if an application or library is also built using CMake).

The names of the generated library files are generated from the base target names
(without the namespace prefix) plus an optional suffix, which depends on 
whether the library is a static one (``z``), and whether it was built using the
``"Debug"`` configuration (``d``).

For example, target ``nano::nanoclient`` will generate ``libnanoclientz.a`` (or
``libnanoclientz.lib`` on Windows) when built into a static library using
``"Release"`` configuration, and ``libnanoclientzd.a`` (``libnanoclientzd.lib``
on Windows) when built with ``"Debug"``.

Similarly, the generated file will be named ``libnanoclient.so`` (or
``libnanoclient.dll`` on Windwos) when built into a dynamic library in
``"Release"`` configuration, and ``libnanoclientd.so`` (or
``libnanoclientd.dll`` on Windows) with ``"Debug"``.

.. _section-building-libraries-nanoclient:

nano::nanoclient
----------------

Description:
    |RTI| |NANO_CLIENT| C API

Dependencies:
    :ref:`section-building-libraries-nanocore`

.. _section-building-libraries-nanoclientudpv4:

nano::nanoclientudpv4
---------------------

Description:
    Wrapper library which links both the C API and the UDPv4 transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanoclient`,
    :ref:`section-building-libraries-nanotransportudpv4`

.. _section-building-libraries-nanoclientserial:

nano::nanoclientserial
----------------------

Description:
    Wrapper library which links both the C API and the Serial transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanoclient`,
    :ref:`section-building-libraries-nanotransportserial`

.. _section-building-libraries-nanocore:

nano::nanocore
--------------

Description:
    Core XRCE functionality shared with |RTI| |NANO_AGENT|.

Dependencies:
    None

.. _section-building-libraries-nanotransportserial:

nano::nanotransportserial
-------------------------

Description:
    Serial transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanocore`

.. _section-building-libraries-nanotransportudpv4:

nano::nanotransportudpv4
------------------------

Description:
    UDPv4 transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanocore`

.. _section-building-libraries-nanoarduinoc:

nano::nanoarduinoc
------------------

Description:
    C wrapper for some of the C++ Arduino SDK functionalities. Built only if
    ``ENABLE_ARDUINO`` is enabled.

Dependencies:
    Arduino SDK

.. _section-building-libraries-nanocppclient:

nano::nanocppclient
-------------------

Description:
    |RTI| |NANO_CLIENT| C++ API.

Dependencies:
    :ref:`section-building-libraries-nanocppclient`

.. _section-building-libraries-nanocpptransportserial:

nano::nanocpptransportserial
----------------------------

Description:
    C++ wrapper for the Serial transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanocppclient`
    :ref:`section-building-libraries-nanotransportserial`

.. _section-building-libraries-nanocpptransportudpv4:

nano::nanocpptransportudpv4
---------------------------

Description:
    C++ wrapper for the UDPV4 transport plugin.

Dependencies:
    :ref:`section-building-libraries-nanocppclient`
    :ref:`section-building-libraries-nanotransportudpv4`


.. _section-building-alt:

Alternative build systems
=========================

|RTI| |NANO_CLIENT| can be easily embedded into any existing C/C++ project by
including the library's source files in the project's compilation unit.

This can be useful when CMake is not used to build the project.

.. _section-building-alt-export:

Exporting Source with CMake
---------------------------

The CMake build script can also be used to copy |RTI| |NANO_CLIENT|'s source
files into a single location, without compiling them.

Use variables ``BUILD_LIBRARIES``, and ``INSTALL_SOURCE`` to control this
behavior. For example, to export both the C and C++ APIs:

.. code-block:: sh

    mkdir build && cd build
    cmake /path/to/nano-client/extras/nano-client-cpp \
            -DBUILD_LIBRARIES=OFF \
            -DINSTALL_SOURCE=ON \
            -DCMAKE_INSTALL_PREFIX=/install/location
    cmake --build . --target install
    # source code copied to /install/location/src
    # List out exported files (if make is available)
    make -C /install/location/src

The installation location will contain a ``makefile`` which can be used to load
variables that can be used to include |RTI| |NANO_CLIENT| in a ``make``-based
build.

For every library included in |RTI| |NANO_CLIENT|, the ``makefile`` contains
the following variables (scoped by a prefix associated with the library):

* ``$(LIBRARY)_DIR``
* ``$(LIBRARY)_SRC``
* ``$(LIBRARY)_INC``
* ``$(LIBRARY)_INC_PUB``
* ``$(LIBRARY)_DEFINES``
* ``$(LIBRARY)_LIBS``
* ``$(LIBRARY)_SRC_FILES``
* ``$(LIBRARY)_INC_FILES``
* ``$(LIBRARY)_INC_PUB_FILES``
* ``$(LIBRARY)_INC_DIRS``
* ``$(LIBRARY)_SOURCE_FILES``

Libraries are identified by the following prefixes:

+-----------------------------------+---------------------------+
| Library                           | Variable Prefix           |
+===================================+===========================+
|``nano::nanocore``                 |``NANO_CORE``              |
+-----------------------------------+---------------------------+
|``nano::nanotransportserial``      |``NANO_TRANSPORT_SERIAL``  |
+-----------------------------------+---------------------------+
|``nano::nanotransportudpv4``       |``NANO_TRANSPORT_UDPV4``   |
+-----------------------------------+---------------------------+
|``nano::nanoarduinoc``             |``NANO_ARDUINO_C``         |
+-----------------------------------+---------------------------+
|``nano::nanoclient``               |``NANO_CLIENT``            |
+-----------------------------------+---------------------------+
|``nano::nanoclientserial``         |``NANO_CLIENT_SERIAL``     |
+-----------------------------------+---------------------------+
|``nano::nanoclientudpv4``          |``NANO_CLIENT_UDPV4``      |
+-----------------------------------+---------------------------+
|``nano::nanocppclient``            |``NANO_CLIENT_CPP``        |
+-----------------------------------+---------------------------+
|``nano::nanocpptransportudpv4``    |``NANO_CLIENT_CPP_UDPV4``  |
+-----------------------------------+---------------------------+
|``nano::nanocpptransportserial``   |``NANO_CLIENT_CPP_SERIAL`` |
+-----------------------------------+---------------------------+

.. _section-building-alt-make:

Building exported source with Make
----------------------------------

The following ``makefile`` could be used to build |RTI| |NANO_CLIENT|
and the UDPv4 transport plugin along with a simple C application:

.. code-block:: make

    # Base location where nano-client's source code was exported
    # (i.e. CMAKE_INSTALL_PREFIX)
    NANO_INSTALL_DIR    ?= /path/to/install/directory

    include $(NANO_INSTALL_DIR)/src/makefile

    MYAPP_SRC_FILES := MyApplication.c \
                    $(NANO_CLIENT_SRC_FILES) \
                    $(NANO_CORE_SRC_FILES) \
                    $(NANO_TRANSPORT_UDPV4_SRC_FILES)

    MYAPP_INC_FILES := MyApplication.h \
                    $(NANO_CLIENT_INC_FILES) \
                    $(NANO_CLIENT_INC_PUB_FILES) \
                    $(NANO_CORE_INC_FILES) \
                    $(NANO_CORE_INC_PUB_FILES) \
                    $(NANO_TRANSPORT_UDPV4_INC_FILES) \
                    $(NANO_TRANSPORT_UDPV4_INC_PUB_FILES)

    MYAPP_INC_DIRS  := $(shell pwd) \
                    $(NANO_CLIENT_INC_DIRS) \
                    $(NANO_CORE_INC_DIRS) \
                    $(NANO_TRANSPORT_UDPV4_INC_DIRS)

    MYAPP_BUILD_DIR ?= $(shell pwd)/build

    MYAPP_OBJ_FILES := $(MYAPP_SRC_FILES:%.c=$(MYAPP_BUILD_DIR)/%.o)

    MYAPP_EXEC      ?= myapp

    CC              ?= gcc

    .PHONY : $(MYAPP_EXEC)

    $(MYAPP_EXEC): $(MYAPP_BUILD_DIR)/$(MYAPP_EXEC)
        echo "Built executable: $<"

    $(MYAPP_BUILD_DIR)/$(MYAPP_EXEC): $(MYAPP_OBJ_FILES)
        $(CC) -o $@ $<

    $(MYAPP_BUILD_DIR)/%.o : %.c
        mkdir -p $$(dirname $@)
        $(CC) -o $@ -c $< -I $(MYAPP_INC_DIRS)

.. _section-building-manual-c:

C API Manual Build
------------------

The C API and C transport plugins can be built by passing
the following source files and include paths to your C compiler of choice:

* Source Files:
    * ``src/**/*.c``
    * ``core/src/**/*.c``

* Include Paths:
    * ``include``
    * ``core/include``

.. _section-building-manual-cpp:

C++ API Manual Build
--------------------

If building the C++ API, the following source files and paths should be
included:

* Source Files:
    * ``src/**/*.c``
    * ``core/src/**/*.c``
    * ``extras/nano-client-cpp/src/**/*.cpp``

* Include Paths:
    * ``include``
    * ``core/include``
    * ``extras/nano-client-cpp/include``

.. _section-building-manual-arduino:

Arduino-compatible API Manual Build
-----------------------------------

If building for the Arduino-compatible API, the following source files and 
paths should be included:

* Source Files:
    * ``src/**/*.c``
    * ``core/src/**/*.c``
    * ``extras/nano-client-cpp/src/**/*.cpp``
    * ``extras/arduino-c/src/*.cpp``

* Include Paths:
    * ``include``
    * ``core/include``
    * ``extras/nano-client-cpp/include``
    * ``extras/arduino-c/include``
