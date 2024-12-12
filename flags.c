#include "ft_ping.h"

void    flag_verbose_mode( void )  // -v
{
    global_flags.verbose_mode = TRUE;
}

void    flag_help()   // -?
{
    printf("Usage: ft_ping [destination] [options]\n");
    printf("Options:\n");
    printf("  -v                Verbose mode. Print more details.\n");
    printf("  -? or -h          Display this help message.\n");
    printf("  -f                Enable flood mode.\n");
    printf("  -w <timeout>      Specify a timeout value.\n");
    printf("  --ttl <value>     Set Time-To-Live (TTL) for packets.\n");
    printf("  --ip-timestamp    Enable IP timestamp option.\n");
    exit(EXIT_SUCCESS);
}

void    flag_flood_mode( void )   // -f
{
    global_flags.flood_mode = 0;
}

void    flag_preload()  // -l
{

}

void    flag_numeric_output()   // -n
{
    global_flags.numeric_output = TRUE;
}

void    flag_timeout( int ac, char *av[], int i )  // -w
{
    if (ac <= i)
    {
        printf("Usage: %s <destination> -w <timeout>\n", av[0]);
        exit(EXIT_FAILURE);
    }

    global_flags.timeout = atoi(av[i]);

    if (global_flags.timeout <= 0)
    {
        printf("Timeout value must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
}

void    flag_time_to_live( int ac, char *av[], int i ) // --ttl
{
    if (ac <= i)
    {
        printf("Usage: %s <destination> --ttl <value>\n", av[0]);
        exit(EXIT_FAILURE);
    }

    global_flags.time_to_live = atoi(av[i]);

    if (global_flags.time_to_live <= 0)
    {
        printf("TTL value must be greater than 0\n");
        exit(EXIT_FAILURE);
    }
}

void    flag_ip_timestamp() // --ip-timestamp
{
    global_flags.ip_timestamp = TRUE;
}