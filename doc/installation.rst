.. include:: vars.rst

.. _section-install:

************
Installation
************

|RTI| |NANO_CLIENT| is distributed in source format. The project's
:link_nano_client_git:`Git repository <>`
contains all available components in buildable source form.

The repository may be cloned locally and built using one
of the supported methods (see :ref:`section-building`).

Alternatively, the library may be installed via the 
:link_arduino_ide:`Arduino IDE <>` or the 
:link_platformio:`PlatformIO <>` platform.


.. _section-install-src:

Installation From Source
========================

Clone |RTI| |NANO_CLIENT|'s
:link_nano_client_git:`Git repository <>`:

.. parsed-literal::

    git clone |link_nano_client_git|

If you intend to use the Arduino-compatible API, an Arduino-compatible packaging of the
version will first have to be generated. See :ref:`section-install-arduino`.

When building |RTI| |NANO_CLIENT| with CMake, you can take advantage of the generated
``install`` target to copy all build artifacts to your desired location.

.. _section-install-arduino:

Arduino Package
===============

In order to support Arduino-compatible systems and better integrate with their typical
development workflows, |RTI| |NANO_CLIENT| can be exported and packaged into
Arduino's library format.

This extra packaging step is required to copy the library's source files in a
directory structure that is compatible with Arduino's limited library format.

The generated package can be manually installed into Arduino's Sketchbook
directories, or it can be automatically copied by the `make`-based generation
script.

For example, on Linux, where the Sketchbook directory defaults to ``~/Arduino``,
the following command will install library ``nano-client-arduino`` in
``~/Arduino/libraries``:

.. code-block:: sh

    make -C /path/to/nano-client/extras/nano-client-arduino INSTALL_DIR=~/Arduino/libraries

Alternatively, you can download a |archive_link|.

.. |archive_link| raw:: html

    <a href="_static/downloads/nano-client-arduino-0.1.0.zip" target="_blank">pre-generated archive</a>

Please refer to :link_arduino_libs:`Arduino's documentation <>`
for more information on how to install the library.

.. _section-install-platformio:

PlatformIO Installation
=======================

|RTI| |NANO_CLIENT|'s Arduino API can be installed via the
:link_platformio:`PlatformIO <>` build system.

Simply include the Git repository in your build environment's ``lib_deps``:

.. parsed-literal::

    [env:my_xrce_app]
    lib_deps = |link_nano_client_git|

Alternatively, for example if you want to use a local clone of the repository,
you can add ``nano-client-arduino`` to your build environment's ``lib_deps``,
and include the directory containing the clone (not the clone's directory itself)
to ``lib_extra_dirs``, e.g.:

.. code-block:: ini

    [env:my_xrce_app]
    lib_deps = nano-client-arduino
    lib_extra_dirs = /path/to/nano-clients/parent/directory

Please refer to :link_platformio_docs_libs:`PlatformIO's documentation <>` for 
more information on how to configure library dependencies.

.. _section-install-docs:

Documentation
=============

|RTI| |NANO_CLIENT|'s user manual is written using :link_sphinx:`Sphinx <>`,
while API documentation is generated using :link_doxygen:`Doxygen <>`.

Once these tools are installed on your system (and available in your ``PATH``
environment variable), documentation can be generated using the CMake build 
script by enabling option ``BUILD_DOCS``. You can also use the ``BUILD_LIBRARIES``
option to skip compilation of the source code.

.. code-block:: sh

    mkdir build && cd build

    cmake /path/to/nano-client -DBUILD_DOCS=ON \
                               -DBUILD_LIBRARIES=OFF
    
    cmake --build . --target install

    # Manual:   ./install/doc/manual/nano-client/html
    # API Ref:  ./install/doc/api/nano-client/html

``BUILD_DOCS_MANUAL`` and ``BUILD_DOCS_API`` can be used to further 
enable/disable the generation of the Sphinx user manual, and the Doxygen
API documentation, respectively.
