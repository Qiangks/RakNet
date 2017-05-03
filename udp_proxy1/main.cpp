//#include <RakPeerInterface.h>
#include <udp_proxy.hpp>
//#include <srs_kernel_error.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

// TODO: remove it
#define ERROR_SUCCESS      0
#define ERROR_PARSE_OPTIONS_FAILED      1

const int MAX_CLINETS = 1024;

bool show_help = false;
bool show_version = false;
int port = DEFAULT_PROXY_PORT;
string srs_addr = DEFAULT_SRS_SERVER;
int max_clients = MAX_CLINETS;

void srs_parse_host(string host, string& ip, int& port)
{
    ssize_t pos;
    if ((pos = host.find(":")) != string::npos) {
        ip = host.substr(0, pos);
        port = ::atoi(host.substr(pos + 1).c_str());
    }

    return;
}

int print_help(char* argv[])
{
    printf(
            "Usage: %s [-?hvV] [-l listen] [-p proxy] [-m max_clients]\n"
                    "\n"
                    "Options:\n"
                    "    -? -h        : show this help and exit(0).\n"
                    "    -v           : show this version and exit(0).\n"
                    "    -l           : this udp proxy server listen port, default: 8935.\n"
                    "    -p           : SRS server addresss, default: 127.0.0.1:1935.\n"
                    "    -m           : set this udp proxy server max clients, default: 1024.\n"
                    "\n"
                    "For example:\n"
                    "    %s -h\n"
                    "    %s -v\n"
                    "    %s -l 8935 -p 127.0.0.1:1935 -m 1024\n", argv[0], argv[0], argv[0], argv[0]);
}

int parse_args(int argc, char* argv[])
{
    int ret = ERROR_SUCCESS;

    if (argc == 1) {
        show_help = true;
        return ret;
    }

    char *p;
    for (int i = 1; i < argc; i++) {
        p = argv[i];
        if (*p++ != '-') {
            show_help = true;
            printf("invalid options: \"%s\"", p);
        }

        while (*p) {
            switch (*p++) {
                case '?':
                case 'h':
                    show_help = true;
                    break;
                case 'v':
                case 'V':
                    show_version = true;
                    break;
                case 'l':
                case 'L':
                    if (argv[++i]) {
                        port = ::atoi(argv[i]);
                        goto next;
                    }
                case 'p':
                case 'P':
                    if (argv[++i]) {
                        srs_addr = argv[i];
                        goto next;
                    }
                    break;
                case 'm':
                case 'M':
                    if (argv[++i]) {
                        max_clients = ::atoi(argv[i]);
                        goto next;
                    }
                default:
                    printf("invalid options: \"%s\"\n", *(p - 1));
                    return ERROR_PARSE_OPTIONS_FAILED;
            }
        }

        next:
            continue;
    }

    return ret;
}

int main(int argc, char* argv[])
{
    int ret = ERROR_SUCCESS;

    if ((ret = parse_args(argc, argv)) != ERROR_SUCCESS) {
        printf("parse options failed, ret=%d\n", ret);
        return ret;
    }

    if (show_version) {
        printf(PROXY_VER"\n");
    }

    if (show_help) {
        print_help(argv);
    }

    string srs_ip;
    int srs_port;
    srs_parse_host(srs_addr, srs_ip, srs_port);
    SrsUdpProxy* proxy = new SrsUdpProxy(port, srs_ip, srs_port, max_clients);
    if ((ret = proxy->start()) != ERROR_SUCCESS) {
        printf("proxy start failed, ret=%d\n", ret);
        return ret;
    }




    return ret; 
}
