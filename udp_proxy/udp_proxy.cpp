#include <udp_proxy.hpp>
//#includue <srs_kernel_error.hpp>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>

#define ERROR_SUCCESS       0

SrsUdpProxy::SrsUdpProxy(int up, std::string sa, int sp, int clients)
{
    listen = up;
    server_addr = sa;
    server_port = sp;
    max_clients = clients;

    server = RakNet::RakPeerInterface::GetInstance();
    sd = new RakNet::SocketDescriptor(listen, 0);
}

SrsUdpProxy::~SrsUdpProxy()
{
    free(sd);
    RakNet::RakPeerInterface::DestroyInstance(server);
}

int SrsUdpProxy::start()
{
    int ret = ERROR_SUCCESS;

    server->Startup(max_clients, sd, 1);
    server->SetMaximumIncomingConnections(max_clients);

    return ret;
}




