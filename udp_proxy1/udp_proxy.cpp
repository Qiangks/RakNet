#include <udp_proxy.hpp>
//#includue <srs_kernel_error.hpp>
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

// TODO:  remove it
#define ERROR_SUCCESS       0
#define ERROR_SERVER_STARTUP_FAILED         100

SrsUdpProxy::SrsUdpProxy(int p, std::string sa, int sp, int clients)
{
    port = p;
    server_addr = sa;
    server_port = sp;
    max_clients = clients;

    server = RakNet::RakPeerInterface::GetInstance();
    sd = new RakNet::SocketDescriptor(port, 0);
}

SrsUdpProxy::~SrsUdpProxy()
{
    delete sd;
    RakNet::RakPeerInterface::DestroyInstance(server);
}

int SrsUdpProxy::start()
{
    int ret = ERROR_SUCCESS;

    if (server->Startup(max_clients, sd, 1) != RakNet::RAKNET_STARTED) {
        ret = ERROR_SERVER_STARTUP_FAILED;
        printf("udp proxy server start up failed, ret=%d\n", ret);
        return ret;
    }
    server->SetMaximumIncomingConnections(max_clients);

    return ret;
}

int SrsUdpProxy::receive()
{
    int ret = ERROR_SUCCESS;
    // TODO: implement it.
    RakNet::SystemAddress clientID=RakNet::UNASSIGNED_SYSTEM_ADDRESS;

    RakNet::Packet* p = server->Receive();
    while(p) {
        switch(p->data[0]) {
            case ID_DISCONNECTION_NOTIFICATION:
                // Connection lost normally
                printf("ID_DISCONNECTION_NOTIFICATION from %s\n", p->systemAddress.ToString(true));;
                break;
            case ID_NEW_INCOMING_CONNECTION:
                // Somebody connected.  We have their IP now
                printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", p->systemAddress.ToString(true), p->guid.ToString());
                clientID=p->systemAddress; // Record the player ID of the client

                printf("Remote internal IDs:\n");
                for (int index=0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
                {
                    RakNet::SystemAddress internalId = server->GetInternalID(p->systemAddress, index);
                    if (internalId!=RakNet::UNASSIGNED_SYSTEM_ADDRESS)
                    {
                        printf("%i. %s\n", index+1, internalId.ToString(true));
                    }
                }

                break;

            case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
                break;

            case ID_CONNECTED_PING:
            case ID_UNCONNECTED_PING:
                printf("Ping from %s\n", p->systemAddress.ToString(true));
                break;

            case ID_CONNECTION_LOST:
                // Couldn't deliver a reliable packet - i.e. the other system was abnormally
                // terminated
                printf("ID_CONNECTION_LOST from %s\n", p->systemAddress.ToString(true));;
                break;
            default:
                char buf[1024];
                if ((ret = proxy(p, buf)) != ERROR_SUCCESS) {
                    printf("proxy packet data: %s failed, ret=%d\n", p->data, ret);
                    return ret;
                }

                // TODO: implement it.
//                server->send();
        }
    }

    server->Shutdown(300);
    return ret;
}

int SrsUdpProxy::proxy(RakNet::Packet* p, char* data)
{
    int ret = ERROR_SUCCESS;
    // connect to the srs and send the packet to server
    return ret;
}




