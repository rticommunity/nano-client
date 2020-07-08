# RTI nano-client

*RTI nano-client* provides a lightweight, open-source C/C++ API to access any
DDS Global Data Space using the [standard XRCE protocol][omg-xrce].

Consider [*RTI nano-agent*][nano-agent-git]
for a standard-compliant implementation of the XRCE Agent built with
[RTI Connext DDS][connext-home].

This project is part of RTI's [Experimental Projects][experimental-faq].

## DDS-XRCE

![DDS-XRCE System Architecture][dds-xrce-architecture]

Thanks to XRCE, limited devices such as microcontrollers and other embedded
targets can become part of a DDS system.

[DDS-XRCE][omg-xrce] (or "DDS for e*X*tremely *R*esource *C*onstrained *E*nvironments") introduces 
an alternative interface to DDS which offloads all management of DDS entities
from applications to an external Agent process.

Applications connect to the XRCE Agent as clients, and they use the
*XRCE Client API* to:

* Create and configure DDS entities on the Agent.
* Write DDS samples using a DataWriter on the Agent.
* Read DDS samples received by a DataReader on the Agent.

XRCE's client/server model significantly reduces the minimal memory
footprint required by an application to use DDS.

Additionally, XRCE is a message-oriented protocol which may be carried out
over any transport with a Maximum Transport Unit of at least 24 bytes, with
standard mappings for TCP/UDP sockets, and Serial lines.

The XRCE standard also defines a custom reliability protocol for reliable
delivery of messages over unreliable transports, and it supports fragmentation
(and reconstruction) of large payloads which exceed the transport's MTU.

## Documentation

Please refer to the [User Manual][nano-client-docs] for 
information on how to install, build, and use *RTI nano-client*.

## License

*RTI nano-client* is released under the terms of the Apache 2.0 license. See the LICENSE file for more information.

```text
(c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
```

[omg-xrce]: https://www.omg.org/spec/DDS-XRCE/About-DDS-XRCE/ "OMG DDS-XRCE Specification"
[nano-agent-git]: https://github.com/rticommunity/nano-agent.git "RTI nano-agent Git repository"
[nano-client-docs]: https://community.rti.com/static/documentation/nano/nano-client/latest "RTI nano-client User Manual"
[dds-xrce-architecture]: doc/static/dds_xrce_architecture.png "DDS-XRCE System Architecture"
[experimental-faq]: https://www.rti.com/developers/rti-labs/experimental-product-faq "RTI Experimental Product FAQ"
[connext-home]: https://www.rti.com/products/connext-dds-professional "RTI Connext DDS Professional Homepage"
