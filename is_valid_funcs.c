#include "ft_ping.h"

// int is_flag_valid(int ac, char *av[])
// {
//     if (ac < 3)
//     {
//         return TRUE;
//     }
//     else if (strcmp(av[2], "-v") == 0)
//     {
//         flag_verbose_mode( ac, av );
//         return TRUE;
//     }
//     else if (strcmp(av[2], "-?") == 0)
//     {
//         return TRUE;
//     }
//     else if (strcmp(av[2], "-f") == 0)
//     {
//         flag_flood_mode( ac, av );
//     }
//     else if (strcmp(av[2], "-l") == 0)
//     {
//         return TRUE;
//     }
//     else if (strcmp(av[2], "-n") == 0)
//     {
//         return TRUE;
//     }
//     else if (strcmp(av[2], "-w") == 0)
//     {
//         flag_timeout( ac, av );
//         return TRUE;
//     }
//     else if (strcmp(av[2], "--ttl") == 0)
//     {
//         return TRUE;
//     }
//     else if (strcmp(av[2], "--ip-timestamp") == 0)
//     {
//         return TRUE;
//     }

//     return FALSE;
// }


// Flag var mı, varsa geçerli mi kontrol ediyorum
int is_flag_valid(int ac, char *av[])
{
    int result = (ac <= 2);

    for (int i = 2; i < ac; i++)
    {
        result = TRUE;

        if (strcmp(av[i], "-v") == 0)
        {
            flag_verbose_mode( );
        }
        else if (strcmp(av[i], "-?") == 0 || strcmp(av[i], "-h") == 0)
        {
            flag_help( );
        }
        else if (strcmp(av[i], "-f") == 0)
        {
            flag_flood_mode( );
            
        }
        else if (strcmp(av[i], "-l") == 0)
        {
            
        }
        else if (strcmp(av[i], "-n") == 0)
        {
            flag_numeric_output( );
        }
        else if (strcmp(av[i], "-w") == 0)
        {
            flag_timeout( ac, av, i + 1 );
            
            i++;
        }
        else if (strcmp(av[i], "--ttl") == 0)
        {
            flag_time_to_live( ac, av, i + 1 );

            i++;
        }
        else if (strcmp(av[i], "--ip-timestamp") == 0)
        {
            flag_ip_timestamp( );
        }
        else
        {
            result = FALSE;
            break;
        }
    }

    return result;
}

// int is_valid_ip(const char *ip)
// {
//     char ip_copy[16];
//     strncpy(ip_copy, ip, sizeof(ip_copy) - 1);
//     ip_copy[sizeof(ip_copy) - 1] = '\0';

//     char *token = strtok(ip_copy, ".");
//     int count = 0;

//     while (token != NULL) 
//     {
//         for (int i = 0; token[i] != '\0'; i++)
//         {
//             if (!isdigit(token[i]))
//             {
//                 return 0;
//             }
//         }

//         int num = atoi(token);
//         if (num < 0 || num > 255)
//         {
//             return 0;
//         }

//         count++;
//         token = strtok(NULL, ".");
//     }

//     return count == 4 ? TRUE : FALSE;

// }


int is_valid_ip_or_domain(const char *input) {
    struct sockaddr_in sa;

    // IP kontrolü (IPv4)
    if (inet_pton(AF_INET, input, &(sa.sin_addr)) == 1) {
        return TRUE; // IP adresi geçerli
    }

    // Domain kontrolü (DNS çözümlemesi)
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // Sadece IPv4 desteklenir
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(input, NULL, &hints, &res) == 0) {
        freeaddrinfo(res); // Belleği temizle
        return TRUE;       // Domain geçerli
    }

    return FALSE; // Ne IP ne de geçerli bir domain
}

int is_valid_av(int ac, char *av[])
{
    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s <destination> <flag(optional)>\n", av[0]);
        exit(EXIT_FAILURE);
    }
    else if (is_valid_ip_or_domain(av[1]) == 0)
    {
        fprintf(stderr, "IP address is not valid\n");
        exit(EXIT_FAILURE);  
    }
    else if (is_flag_valid(ac, av) == 0)
    {
        fprintf(stderr, "Flag is not valid\n");
        exit(EXIT_FAILURE);
    }
    return TRUE;
}