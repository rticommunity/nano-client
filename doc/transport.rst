.. include:: vars.rst

.. _section-transport:

*****************
Transport Support
*****************

|RTI| |NANO_CLIENT| includes a pluggable *XRCE Client Transport API* which
enables XRCE applications to use the transport layer that more suits their needs.

|RTI| |NANO_CLIENT| includes a transport implementation over UDPv4 sockets,
and one for Serial lines.

These transports are built over the communication abstractions defined by
|RTI| |NANO_CLIENT|'s portability layer.

