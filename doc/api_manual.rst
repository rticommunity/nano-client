.. include:: vars.rst

.. _section-apiman:

**********************
XRCE Client API Manual
**********************

This section provides some examples on how to use |RTI| |NANO_CLIENT|'s APIs
to connect to an XRCE Agent and make use of its services.

The manual uses the C API as the main reference for code snippets, but it also
includes equivalent examples for other language bindings.

.. _section-apiman-init:

XRCE Client Configuration
=========================

The XRCE protocol adopts a client/server model where applications access the
DDS Global Data Space by exchanging XRCE messages with an XRCE Agent process
over one of the supported transports.

In order to use the Agent's services, an application must first create a unique
client session on the Agent, identified by a 32-bit *client key*, and an 8-bit
*session id*. This remote session is represented locally by an object of type
``NANO_XRCE_Client``.

A ``NANO_XRCE_Client`` is configured using the ``NANO_XRCE_ClientProperties``
structure. At a minimum, an application must use this data structure to specify:

  - The Client's 32-bit client key.
  - The Client's 8-bit session id.
  - The Client's storage object.
  - The Client transport implementation.
  - The Agent's locator (depending on transport).

Since |RTI| |NANO_CLIENT| does not perform any dynamic memory allocation, all
"variable state" stored by a ``NANO_XRCE_Client`` is encapsulated by a "storage
object" which applications must provide during client initialization. This
object, abstracted by type ``NANO_XRCE_ClientStorage``, will be used by the client
to allocate data structures at runtime (e.g. buffers for XRCE messages) based
on the application's requests. Developers can use this interface to fine tune
the amount of memory consumed by their applications in accordance with available
resources and application requirements. Since this is a somewhat advanced use
case, |RTI| |NANO_CLIENT| provides "default" storage classes that should be
suitable for simpler scenarios.

A ``NANO_XRCE_Client`` requires a transport object to exchange messages with the 
Agent. Transports are modelled by the ``NANO_XRCE_ClientTransport`` interface, and
|RTI| |NANO_CLIENT| includes implementations of this interface for UDPv4 sockets
(``NANO_XRCE_Udpv4ClientTransport``), and Serial lines
(``NANO_XRCE_SerialClientTransport``).

Each transport object abstracts the client's connection to the Agent and it
should accept the basic configuration parameters defined by the
``NANO_XRCE_ClientTransportProperties`` data structure. These include the
Agent's locator and the MTU that should be enforced on the transport link.
Each transport implementation will typically define their own properties type,
which extends the base structure with additonal, transport-specific parameters.

Transport implementations should also not perform any dynamic memory allocation,
and they will typically require applications to pass in a buffer for storing
received messages.

The following snippet shows how to initialize a ``NANO_XRCE_Client`` using the
UDPv4 transport plugin:

.. code-block:: c

    #include "nano/nano_client.h"

    /* Clients can select any arbitrary, non-zero, 32-bit key */
    #define CLIENT_KEY          0xABCDABCD

    /* Client's can select any non-zero session id, with the exception of values 
    0x01 and 0x80, which are reserved by the protocol. Depending on the selected
    session id, the exchanged XRCE messages will include the client's key in each
    message's header (if session id < 0x80) or omit it (if session id > 0x80).
    If the client key is omitted, the XRCE Agent must be able to correlate
    the client's transport with the client's session (e.g. by "binding" the
    client's source IP address/port to the client's key provided during
    connection). */
    #define SESSION_ID          0x81

    /* Transport MTU is arbitrary and defined by the application. Acceptable values
    will depend on the specific transport and hardware available on the target
    system */

    #define TRANSPORT_MTU               1024

    /* The receive buffer should be aligned `CREATE_CLIENT` at a boundary of at least 4 bytes.
    Type NANO_MessageBufferData will always be defined to be at least 32-bit,
    and it can be used to guarantee this alignment. */

    #define TRANSPORT_BUFFER_SIZE \
        (NANO_OSAPI_Memory_align_size_up( \
            TRANSPORT_MTU, sizeof(NANO_MessageBufferData)) / \
            sizeof(NANO_MessageBufferData))

    int main(void)
    {
        /* All variables can be stack-allocated */

        NANO_XRCE_Client client = NANO_XRCE_CLIENT_INITIALIZER;

        NANO_XRCE_ClientProperties client_props =
                NANO_XRCE_CLIENTPROPERTIES_INITIALIZER;

        NANO_XRCE_DefaultClientStorage client_storage =
                NANO_XRCE_DEFAULTCLIENTSTORAGE_INITIALIZER;

        NANO_XRCE_Udpv4ClientTransport client_transport =
                NANO_XRCE_UDPV4CLIENTTRANSPORT_INITIALIZER;

        NANO_XRCE_Udpv4ClientTransportProperties client_transport_props =
                NANO_XRCE_UDPV4CLIENTTRANSPORTPROPERTIES_INITIALIZER;

        NANO_MessageBufferData client_transport_recv_buffer[TRANSPORT_BUFFER_SIZE];

        const uint8_t agent_addr[] = { 127, 0, 0, 1 };

        /* Configure transport with agent's IP address. The UDPv4 transport uses the 
        "medium" locator format (32-bit address, 16-bit port).
        The port field may be left to 0 to use the default Agent port (7401) */
        client_transport_props.agent_address.format = NANO_XRCE_ADDRESS_FORMAT_MEDIUM;
        NANO_OSAPI_Memory_copy(
            client_transport_props.base.agent_address.value.medium.address,
            agent_addr,
            sizeof(agent_addr));

        /* Select an MTU and pass in a receive buffer */
        client_transport_props.base.mtu = TRANSPORT_MTU;

        client_transport_props.recv_buffer = (NANO_u8*) &client_transport_recv_buffer;
        client_transport_props.recv_buffer_size = sizeof(client_transport_recv_buffer);

        /* Initialize the client storage object */
        NANO_XRCE_DefaultClientStorage_initialize(&client_storage);

        /* Configure client's properties with a client key, session id, storage,
        and the selected transport implementation */

        NANO_XRCE_ClientKey_from_u32(&client_props.key, CLIENT_KEY);
        client_props.id = SESSION_ID;

        client_props.storage = &client_storage.base;
        client_props.transport = &client_transport.base;
        client_props.transport_properties = &client_transport.base;
 
        /* Initialize client. This operation will also initialize the transport
        object, but it will not send any XRCE messages yet. */
        if (NANO_RETCODE_OK != NANO_XRCE_Client_initialize(&client, &client_props))
        {
            return 1;
        }

        /* Perform XRCE operations */

        /* Finalize client and perform clean transport shutdown */
        NANO_XRCE_Client_finalize(&client);

        return 0;
    }


|RTI| |NANO_CLIENT| includes wrapper classes to simplify the initialization
process, and to reduce the number of objects that an application must
declare when using one the included transport plugins.

The previous example is equivalent to the following snippet which uses
wrapper class ``NANO_XRCE_Udpv4Client``:

.. code-block:: c

    #include "nano/nano_client_udpv4.h"

    #define CLIENT_KEY          0xABCDABCD

    #define SESSION_ID          0x81

    #define TRANSPORT_MTU               1024

    #define TRANSPORT_BUFFER_SIZE \
        (NANO_OSAPI_Memory_align_size_up( \
            TRANSPORT_MTU, sizeof(NANO_MessageBufferData)) / \
            sizeof(NANO_MessageBufferData))

    int main(void)
    {
        /* Wrapper class replaces client object, UDPv4 transport, and default client 
        storage */
        NANO_XRCE_Udpv4Client udp_client = NANO_XRCE_UDPV4CLIENT_INITIALIZER;

        /* Cache pointer to client object */
        NANO_XRCE_Client *const client = &udp_client.client;

        /* Receive buffer must still be provided by application */
        NANO_MessageBufferData client_transport_recv_buffer[TRANSPORT_BUFFER_SIZE];

        const uint8_t agent_addr[] = { 127, 0, 0, 1 };

        if (NANO_RETCODE_OK != 
                    NANO_XRCE_Udpv4Client_initialize(
                        &udp_client,
                        CLIENT_KEY,
                        SESSION_ID,
                        TRANSPORT_MTU,
                        (NANO_u8*)client_transport_recv_buffer,
                        sizeof(client_transport_recv_buffer),
                        agent_addr,
                        0 /* default agent port */,
                        NULL /* default bind address */,
                        0 /* default bind port */))
        {
            return 1;
        }

        /* perform XRCE operations */

        NANO_XRCE_Client_finalize(client);
        return 0;
    }

Macro ``NANO_XRCE_Client_finalize()`` can be use to dispose the state of a client
and request the client's transport to finalize any open resources (e.g. close
an open socket). Since |RTI| |NANO_CLIENT| does not perform dynamic memory
allocation, there is no memory to be freed, and this call may be omitted if
the application's process never terminates and/or does not need to initialize
the client multiple times (as long as all resources are disposed by the
operating system once the process terminates).

.. _section-apiman-init-cpp:

C++ API
-------

The C++ API exposes class ``rti::nano::xrce::Client`` to access the services of
an XRCE Agent. Client transports are represented by instances of
``rti::nano::xrce::ClientTransport``. These classes resemble those exposed by
the C API, but partially reduce the number of objects that applications must
define and they take advantage of C++ object-oriented features to simplify the
API usage.

The following snippet initializes a client with an instance of the UDPv4
transport:

.. code-block:: cpp

    #include <nano/nano_client_client.hpp>
    #include <nano/nano_client_transport_udpv4.hpp>

    #define CLIENT_KEY          0xABCDABCD

    #define TRANSPORT_MTU               1024

    #define TRANSPORT_BUFFER_SIZE \
        XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)

    int main(void)
    {
        uint8_t agent_addr[] = { 127, 0, 0, 1 };

        rti::nano::xrce::Data transport_recv_buffer[TRANSPORT_BUFFER_SIZE];

        rti::nano::xrce::UdpTransport transport(
            transport_recv_buffer,
            sizeof(transport_recv_buffer),
            agent_addr);
        
        rti::nano::xrce::Client client(
            transport,
            CLIENT_KEY); /* defaults to session id 0x81 */

        if (!client.initialize())
        {
            return 1;
        }

        /* Perform XRCE operations */

        client.finalize();

        return 0;
    }

.. _section-apiman-init-arduino:

Arduino-compatible API
----------------------

The Arduino-compatible API is equivalent to the C++ one, except that it also exposes
the API's classes in the default namespace with ``Xrce`` prefix.

The following snippet shows a basic sketch which declares and initializes an
XRCE client:

.. code-block:: cpp

    #include <nano_client_arduino.h>

    #define CLIENT_KEY          0xABCDABCD

    #define TRANSPORT_MTU               1024

    #define TRANSPORT_BUFFER_SIZE \
        XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)

    const uint8_t agent_address[4] = { 192, 168, 1, 1 };

    XrceData transport_recv_buffer[
        XRCE_TRANSPORT_RECV_BUFFER_SIZE(TRANSPORT_MTU)] = { 0 };

    XrceUdpTransport transport(
        transport_recv_buffer,
        sizeof(transport_recv_buffer),
        agent_address);

    XrceClient client(transport, CLIENT_KEY); /* defaults to session id 0x81 */

    void setup()
    {
        /* Initialize board's UDP/IP network stack (e.g. connect to WiFi, enable
        Ethernet connection, etc.) */

        /* Initialize XRCE client */
        if (!client.initialize())
        {
            /* failed to initialize client, loop forever */
            while (1) {}
        }
    }

    void loop()
    {
        /* Application's main loop */
    }

.. _section-apiman-connect:

Connect to an XRCE Agent
========================

In order to access DDS, an XRCE application must first connect to an XRCE agent,
and establish a unique client session on it.

An XRCE Client connects to an XRCE Agent by sending a ``CREATE_CLIENT``
message, and then waiting for a ``STATUS_AGENT`` reply to be received from the
Agent.

After initializing a ``NANO_XRCE_Client`` object, applications can use method
`NANO_XRCE_Client_connect()` to send a connection message, and to block until
a reply is received:

.. code-block:: c

    #include "nano/nano_client.h"

    int connect_to_agent(NANO_XRCE_Client *const client)
    {
        NANO_RetCode rc = NANO_RETCOD_ERROR;

        while (!NANO_XRCE_Client_connected(client))
        {
            rc = NANO_XRCE_Client_connect(client, 5000 /* timeout: 5s */);
            
            if (NANO_RETCODE_OK != rc && NANO_RETCODE_TIMEOUT != rc)
            {
                /* Unexpected error */
                return 1;
            }
        }

        return 0;
    }

The client's stores the "connection status" in an internal flag, which can be
checked using macro ``NANO_XRCE_Client_connected()``.

The flag is lifted upon reception of a connection reply from the Agent, and it
will only be reset if:

* The application calls ``NANO_XRCE_Client_disconnect()`` to perform a "clean
  disconnection" from the Agent. This operation sends a disconnection message,
  and waits for a confirmation from the Agent that the session was properly
  disposed.

* The application calls ``NANO_XRCE_Client_mark_disconnected()`` to unilaterlly
  reset the client's state without contacting the Agent.

* The application calls ``NANO_XRCE_Client_connect()``. This operation will
  automatically mark the client as no longer connected.

|RTI| |NANO_CLIENT| will not try to detect whether an Agent is still
available. It is left up to applications to detect this condition and to decide
how to resolve it.

When a client is marked as disconnected, all reliable XRCE messages that have
not been acknowledged by the Agent will be dropped. Similarly, the Agent will
drop all reliable messages not acknowledged by a Client whenever a session
is created or reset by the reception of a ``CREATE_CLIENT`` message.


.. _section-apiman-connect-cpp:

C++ API
-------

.. code-block:: cpp

    #include <nano/nano_client_client.hpp>

    bool connect_to_agent(rti::nano::xrce::Client& client)
    {
        bool timedout = false;

        while (!client.connected())
        {
            if (!client.connect(5000 /* timeout: 5s */, &timedout))
            {
                if (!timedout)
                {
                    return false;
                }
            }
        }
        return true;
    }

.. _section-apiman-connect-arduino:

Arduino-compatible API
----------------------

.. code-block:: c

    #include <nano_client_arduino.h>

    bool connect_to_agent(XrceClient& client)
    {
        bool timedout = false;

        while (!client.connected())
        {
            if (!client.connect(5000 /* timeout: 5s */, &timedout))
            {
                if (!timedout)
                {
                    return false;
                }
            }
        }
        return true;
    }

.. _section-apiman-entities:

Access to DDS Entities
======================

The Agent manages DDS entities on behalf of the applications, and applications
can reference them using a unique 12-bit identifier of their choosing.


.. _section-apiman-streams:

Message Streams and Storage
===========================

Since XRCE is a message-oriented protocol, each operation request is mapped to
an XRCE message sent by a Client to the Agent.

Every XRCE message is sent on a message stream, identified by an 8-bit
identifier. 

Based on the stream identifier, XRCE determines whether it should carry out its
reliability protocol to guarantee the delivery of the message, or deliver the
message with "best-effort" semantics (i.e. don't wait for an acknowledgement
from the receiver nor try to resend the message).

Streams with ID between 0x01 (1) and 0x7F (127) are "best-effort", while those
with id between 0x80 (128) and 0xFF (255) are "reliable". ID 0x01 identifies
the special "built-in best-effort stream", while ID 0x80 is reserved for the
"built-in reliable stream". ID 0x00 identifies the special "none stream" which
is used by XRCE to send connection, reliability, and discovery messages.
