#ifndef FT_PING_H
#define FT_PING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <ctype.h>
#include <netdb.h>


#define PACKET_SIZE 64
#define TIMEOUT 1
#define TRUE 1
#define FALSE 0

typedef struct t_flags
{
    int verbose_mode;   // -v
    int flood_mode;     // -f
    int preload;        // -l
    int numeric_output; // -n
    int timeout;        // -w
    int time_to_live;   // --ttl
    int ip_timestamp;   // --ip-timestamp

} s_flags;

extern s_flags global_flags;

/*****                          flags                           *****/
void    flag_verbose_mode( void );            // -v
void    flag_help();                            // -? or -h
void    flag_flood_mode( void );              // -f
void    flag_preload();                         // -l
void    flag_numeric_output();                  // -n
void    flag_timeout( int ac, char *av[], int i );     // -w
void    flag_time_to_live( int ac, char *av[], int i );                    // --ttl
void    flag_ip_timestamp();                    // --ip-timestamp

/*****                          is_valid_funcs                  *****/
int is_flag_valid( int ac, char *av[] );
int is_valid_av( int ac, char *av[] );
int is_valid_ip_or_domain( const char *ip );

#endif // FT_PING_H