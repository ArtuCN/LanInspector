#ifndef LANINSPECTOR_HPP
#define LANINSPECTOR_HPP

#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdio>
#include <sstream>
#include <array>
#include <netpacket/packet.h>
#include <vector>
#include <cstdint>

#define TRUE 1

struct NetworkInterface {
    std::string name;
    struct in_addr ip;
    struct in_addr netmask;
    std::array<unsigned char, 6> mac;

    void print() const;

};

struct DefaultRoute {
    std::string gateway;
    NetworkInterface interface;
};

struct sockaddr_in *populate_network_interface(struct ifaddrs *ifaddr, struct DefaultRoute *route);
int get_router_interface_name(struct DefaultRoute *route);
std::vector<struct in_addr> get_network_hosts(struct in_addr ip, struct in_addr netmask);


#endif