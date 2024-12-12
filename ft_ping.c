#include "ft_ping.h"


s_flags global_flags = {
    .verbose_mode = FALSE,
    .flood_mode = 1,
    .preload = 0,
    .numeric_output = 0,
    .timeout = 0,
    .time_to_live = 64,
    .ip_timestamp = FALSE
};

unsigned short  calculate_checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

long    get_current_time()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

void set_ip_timestamp_option(int sockfd) {
    unsigned char opt[40];
    memset(opt, 0, sizeof(opt));

    opt[0] = 0x44; // tip
    opt[1] = 40;   // uzunluk
    opt[2] = 5;    // pointer
    opt[3] = 0;    // flag

    // Sokete IP opsiyonlarını bağla
    if (setsockopt(sockfd, IPPROTO_IP, IP_OPTIONS, opt, sizeof(opt)) < 0) {
        perror("setsockopt (IP_OPTIONS)");
        exit(EXIT_FAILURE);
    }
}


void resolve_address(const char *destination, struct sockaddr_in *dest_addr) {
    if (inet_pton(AF_INET, destination, &dest_addr->sin_addr) > 0)
    {
        // printf("Resolved as IP address: %s\n", destination);
        return;
    }
    else
    {
        // printf("inet_pton failed, trying DNS resolution...\n");

        struct addrinfo hints, *res;
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(destination, NULL, &hints, &res) == 0)
        {
            struct sockaddr_in *addr = (struct sockaddr_in *)res->ai_addr;
            memcpy(dest_addr, addr, sizeof(struct sockaddr_in));
            // printf("Resolved domain to IP: %s\n", inet_ntoa(dest_addr->sin_addr));
            freeaddrinfo(res);
            return;
        }
        else
        {
            perror("DNS resolution failed");
            exit(EXIT_FAILURE);
        }
    }
}

int main(int ac, char *av[])
{
    is_valid_av(ac, av);

    const char *destination = av[1];
    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));

    dest_addr.sin_family = AF_INET;
    resolve_address(destination, &dest_addr);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &global_flags.time_to_live, sizeof(global_flags.time_to_live)) < 0) {
        perror("setsockopt (IP_TTL)");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (global_flags.ip_timestamp) {
        set_ip_timestamp_option(sockfd);
    }

    struct icmp packet;
    memset(&packet, 0, sizeof(packet));

    packet.icmp_type = ICMP_ECHO;
    packet.icmp_code = 0;
    packet.icmp_id = getpid();

    char recv_buffer[1024];
    int seq = 0;

    printf("PING %s (%s): %d data bytes\n", destination, destination, PACKET_SIZE);

    long timeout_time = get_current_time() / 1000000.0;

int received_packets = 0;

    while (TRUE)
    {
        seq++;
        packet.icmp_seq = seq;
        packet.icmp_cksum = 0;
        packet.icmp_cksum = calculate_checksum(&packet, sizeof(packet));

        long start_time = get_current_time();

        if ((global_flags.timeout > 0) && ((start_time / 1000000.0) - timeout_time > global_flags.timeout))
        {
            seq--;
            printf("\n--- %s ping statistics ---\n", destination);
            int packet_loss = ((seq - received_packets) * 100) / seq;
            printf("%d packets transmitted, %d received, %d%% packet loss, time %.2f ms\n",
                seq, received_packets, packet_loss, ((start_time / 1000000.0) - timeout_time) * 1000.0);
            break;
        }

        // Paket gönderimi
        if (global_flags.verbose_mode) {
            printf("[VERBOSE] Sending ICMP packet to %s: icmp_seq=%d, checksum=%u\n",
                destination, seq, packet.icmp_cksum);
        }

        if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) <= 0)
        {
            perror("sendto");
            continue;
        }

        socklen_t addr_len = sizeof(dest_addr);

        // Paket alımı
        if (recvfrom(sockfd, recv_buffer, sizeof(recv_buffer), 0, (struct sockaddr *)&dest_addr, &addr_len) <= 0)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                if (global_flags.verbose_mode) {
                    printf("[VERBOSE] ICMP request timeout for sequence %d\n", seq);
                }
                printf("Request timeout for icmp_seq %d\n", seq);
            }
            else
            {
                perror("recvfrom");
            }
            continue;
        }

        received_packets++;

        struct iphdr *ip_header = (struct iphdr *)recv_buffer;
        struct icmp *icmp_header = (struct icmp *)(recv_buffer + sizeof(struct iphdr));
        int ttl = ip_header->ttl;

        if (global_flags.verbose_mode) {
            printf("[VERBOSE] Received ICMP packet: type=%d, code=%d, id=%d, ttl=%d\n",
                icmp_header->icmp_type, icmp_header->icmp_code, icmp_header->icmp_id, ttl);
        }

        long end_time = get_current_time();
        // printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
        //     PACKET_SIZE, destination, seq, ttl, (end_time - start_time) / 1000.0);


        if (global_flags.numeric_output) {
            printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
                PACKET_SIZE, inet_ntoa(dest_addr.sin_addr), seq, ttl, (end_time - start_time) / 1000.0);
        } else {
            printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
                PACKET_SIZE, destination, inet_ntoa(dest_addr.sin_addr), seq, ttl, (end_time - start_time) / 1000.0);
        }

        sleep(global_flags.flood_mode);
    }




    close(sockfd);
    return 0;
}