#ifndef ARPSPOOF_H
#define ARPSPOOF_H

#include <stdio.h>
#include <err.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/ethernet.h>//包括几个以太网的数据结构，ether_addr(mac帧结构),
#include <netinet/ether.h>
#include <net/if_arp.h>   //定义了arpreq结构

#include <libnet.h>
#include <pcap.h>

#endif
