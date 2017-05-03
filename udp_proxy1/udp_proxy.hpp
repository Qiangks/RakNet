#ifndef UDP_PROXY_HPP
#define UDP_PROXY_HPP

/*
#include <udp_proxy.hpp>
*/

#define PROXY_VERSION   "0.0.1"
#define PROXY_VER       "proxy/" PROXY_VERSION

#define DEFAULT_PROXY_PORT      8935
#define DEFAULT_SRS_SERVER      "127.0.0.1:1935"

#include <string>
#include <RakPeerInterface.h>
#include <RakNetTypes.h>

class SrsUdpProxy
{
private:
    int port;
    std::string server_addr;
    int server_port;
    int max_clients;
private:
    RakNet::RakPeerInterface *server;
    RakNet::SocketDescriptor *sd;
public:
    /**
     * @param up: udp proxy listen port
     * @param sa: srs server ip addr
     * @param sp: srs rtmp listen port
     * @param clients: udp proxy support max clients
     */
    SrsUdpProxy(int p, std::string sa, int sp, int clients);
    virtual ~SrsUdpProxy();
public:
    int start();
    int receive();
private:
    int proxy(RakNet::Packet* p, char* data);
};

#endif
