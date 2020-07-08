.. include:: vars.rst

.. _section-xrce:

********
DDS-XRCE
********

.. figure:: dds_xrce_architecture.png
    :figwidth: 90 %
    :alt: DDS-XRCE System Architecture
    :name: FigureNanoXrceArchitecture
    :align: center

    DDS-XRCE System Architecture

Thanks to XRCE, limited devices such as microcontrollers and other embedded
targets can become part of a DDS system.

`DDS-XRCE <https://www.omg.org/spec/DDS-XRCE/About-DDS-XRCE/>`_ (or "DDS for eXtremely Resource Constrained Environments") introduces 
an alternative interface to DDS which offloads all management of DDS entities
from applications to an external Agent process.

Applications connect to the XRCE Agent as clients, and they use the
*XRCE Client API* to:

- Create and configure DDS entities on the Agent.
- Write DDS samples using a DataWriter on the Agent.
- Read DDS samples received by a DataReader on the Agent.

XRCE's client/server model significantly reduces the minimal memory
footprint required by an application to use DDS.

Additionally, XRCE is a message-oriented protocol which may be carried out
over any transport with a Maximum Transport Unit of at least 24 bytes, with
standard mappings for TCP/UDP sockets, and Serial lines.

The XRCE standard also defines a custom reliability protocol for reliable
delivery of messages over unreliable transports, and it supports fragmentation
(and reconstruction) of large payloads which exceed the transport's MTU.
