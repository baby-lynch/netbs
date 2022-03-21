#ifndef _header_h_
#define _headher_h

#include <pcap.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include "util.h"

#define BUFSIZE 65535
#define STRSIZE 256

// Big-Little Endian Conversion
u_int16_t BSWAP_16(u_int16_t x)
{
    return __bswap_16(x);
}
u_int32_t BSWAP_32(u_int32_t x)
{
    return __bswap_32(x);
}

// PCAP file header is redefined here
/*@ Annotation @
  In the pcap.h file, tv_sec and tv_usec(in struct pcap_pkthdr.timaval)are declared as long type
  long type takes up 8 bytes in Linux64 so that pcap_pkthdr takes up 24 bytes
  pcap_pkthdr is redefined here as pcap_packet_header so as to take up 16 bytes
*/
struct time_val
{
    u_int32_t tv_sec;  /* seconds */
    u_int32_t tv_usec; /* and microseconds */
};
struct pcap_packet_header
{
    struct time_val ts; /* time stamp */
    bpf_u_int32 caplen; /* length of portion present */
    bpf_u_int32 len;    /* length this packet (off wire) */
};

// Ethernet Header
struct Ethernet_Header
{
    u_char Dst_MAC[6];  // Destination Ethernet Address
    u_char Src_MAC[6];  // Source Ethernet Address
    u_int16_t Eth_Type; // Frame Type
};

// IP Header
struct IP_Header
{
    u_int8_t Header_len : 4; // Length
    u_int8_t Version : 4;    // Version
    u_int8_t TOS : 8;        // Type of Service
    u_int16_t Total_len;     // Total Length
    u_int16_t ID;            // Identifier
    u_int16_t Flag_Segment;  // Flags+Fragmented Offset
    u_int8_t TTL : 8;        // Time to Live
    u_int8_t Protocol : 8;   // Protocol
    u_int16_t Checksum;      // Header Checksum
    u_char Src_IP[4];        // Source IP Address
    u_char Dst_IP[4];        // Destination IP Address
};

// TCP Header
struct TCP_Header
{
    u_int16_t SrcPort;       // Source Port
    u_int16_t DstPort;       // Destination Port
    u_int32_t SeqNO;         // Sequence Number
    u_int32_t AckNO;         // Acknowledgement Number
    u_int8_t HeaderLen;      // Header Length(4 bit) + Reserved(4 bit)
    u_int8_t Flags;          // Flags
    u_int16_t Window;        // Window Size
    u_int16_t Checksum;      // Checksum
    u_int16_t UrgentPointer; // Urgent Pointer
};

struct pcap_file_header *file_header;
struct pcap_packet_header *pkt_header;
struct Ethernet_Header *eth_header;
struct IP_Header *ip_header;
struct TCP_Header *tcp_header;

const int PCAP_HEADER_SIZE = sizeof(struct pcap_file_header);
const int PACKET_HEADER_SIZE = sizeof(struct pcap_packet_header);
const int ETHERNET_HEADER_SIZE = sizeof(struct Ethernet_Header);
const int IP_HEADER_SIZE = sizeof(struct IP_Header);
const int TCP_HEADER_SIZE = sizeof(struct TCP_Header);

extern void tls_info_extr(u_char* payload, int data_len);  // Extract information from TLS packets

#endif