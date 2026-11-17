
#include "../lib/LanInspector.hpp"

bool arp(std::vector<struct in_addr> *hosts, struct DefaultRoute *route) {
    if (hosts == nullptr || route == nullptr)
        return false;

    int sockfd = open_arp_socket();

    if (sockfd < 0)
        return false;

    // 1. Creazione dei frame
    // 2. Invio delle richieste
    // 3. Ricezione delle risposte

    close(sockfd);

    return true;
}


Frame create_arp_request(
    const NetworkInterface& interface,
    const struct in_addr& target_ip
) {
    Frame frame{};

    // Ethernet header
    memset(frame.ethernet.h_dest, 0xFF, ETH_ALEN);

    memcpy(
        frame.ethernet.h_source,
        interface.mac.data(),
        ETH_ALEN
    );

    frame.ethernet.h_proto = htons(ETH_P_ARP);

    // ARP packet
    frame.arp.arp_hrd = htons(ARPHRD_ETHER);
    frame.arp.arp_pro = htons(ETH_P_IP);
    frame.arp.arp_hln = ETH_ALEN;
    frame.arp.arp_pln = sizeof(struct in_addr);
    frame.arp.arp_op = htons(ARPOP_REQUEST);

    memcpy(
        frame.arp.arp_sha,
        interface.mac.data(),
        ETH_ALEN
    );

    memcpy(
        frame.arp.arp_spa,
        &interface.ip.s_addr,
        4
    );

    memset(frame.arp.arp_tha, 0, ETH_ALEN);

    memcpy(
        frame.arp.arp_tpa,
        &target_ip.s_addr,
        4
    );

    return frame;
}


int send_arp_request(
    int sockfd,
    const NetworkInterface *interface,
    const Frame *frame
) {
    if (sockfd < 0 || interface == nullptr || frame == nullptr)
        return -1;

    unsigned int interface_index =
        if_nametoindex(interface->name.c_str());

    if (interface_index == 0)
        return -1;

    struct sockaddr_ll socket_address{};

    socket_address.sll_family = AF_PACKET;
    socket_address.sll_protocol = htons(ETH_P_ARP);
    socket_address.sll_ifindex = interface_index;
    socket_address.sll_halen = ETH_ALEN;

    memset(
        socket_address.sll_addr,
        0xFF,
        ETH_ALEN
    );

    int res = sendto(
        sockfd,
        frame,
        sizeof(Frame),
        0,
        reinterpret_cast<struct sockaddr*>(&socket_address),
        sizeof(socket_address)
    );

    if (res < 0) {
        perror("sendto");
        return -1;
    }

    return res;
}


int open_arp_socket()
{
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    return sockfd;
}


int receive_arp_response() {
    // TODO: implementare la ricezione delle risposte ARP.
    return 0;
}


std::vector<struct in_addr> get_network_hosts(
    struct in_addr ip,
    struct in_addr netmask
) {
    std::vector<struct in_addr> hosts;

    uint32_t local_ip = ntohl(ip.s_addr);
    uint32_t mask = ntohl(netmask.s_addr);

    uint32_t network = local_ip & mask;
    uint32_t broadcast = network | ~mask;

    for (uint32_t addr = network + 1; addr < broadcast; addr++) {

        if (addr == local_ip)
            continue;

        struct in_addr host{};
        host.s_addr = htonl(addr);

        hosts.push_back(host);
    }

    return hosts;
}
