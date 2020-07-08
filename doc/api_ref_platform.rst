.. _section-api-platform:
Platform Abstraction Layer
=========================

The Platform Abstraction Layer includes a series of API to abstract
all external functions and services needed by nano-client.

These abstractions allow nano-client to be more easily ported
to new hardware platforms and operating systems.

The Platform Abstraction Layer is composed of four groups of API, each
defined by its implementation header file (and associated source files).

.. doxygengroup:: nano_api_platform
   :members:

.. _section-api-platform-cc:

Compiler Abstractions
---------------------

The Platform Abstraction Layer includes a set of abstractions which 
try to normalize features which are typically compiler dependent,
for example by defining a consistent set of primitive types.

A custom implementation of the compiler abstractions can be specified
by passing the include path of an implementation header using variable
`NANO_CC_DEF_H`.

If this variable is not set during build, the Platform Abstraction Layer
will try to automatically detect the target platform, and include a
compatible implementation if available.

.. doxygengroup:: nano_api_platform_cc
   :members:

.. _section-api-platform-cc-prim:

Primitive Types
^^^^^^^^^^^^^^^

.. doxygengroup:: nano_api_platform_cc_prim
   :members:

.. _section-api-platform-sys:

System Abstractions
-------------------
.. doxygengroup:: nano_api_platform_sys
   :members:

.. _section-api-platform-sys-str:

Strings
^^^^^^^

.. doxygengroup:: nano_api_platform_sys_str
   :members:

.. _section-api-platform-sys-mem:

Memory
^^^^^^

.. doxygengroup:: nano_api_platform_sys_mem
   :members:

.. _section-api-platform-sys-clock:

System Clock
^^^^^^^^^^^^

.. doxygengroup:: nano_api_platform_sys_clock
   :members:

.. _section-api-platform-net:

Network Abstractions
--------------------

.. doxygengroup:: nano_api_platform_net
   :members:

.. _section-api-platform-net-udp:

UDP Socket
^^^^^^^^^^

.. doxygengroup:: nano_api_platform_net_udp
   :members:

.. _section-api-platform-serial:

Serial Abstractions
-------------------

.. doxygengroup:: nano_api_platform_serial
   :members:

.. _section-api-platform-serial-conn:

Serial Connection
^^^^^^^^^^^^^^^^^

.. doxygengroup:: nano_api_platform_serial_conn
   :members:
