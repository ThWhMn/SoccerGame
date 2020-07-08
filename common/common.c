/*************************************************************************
> File Name: common.c
> Author: rtqed
> Mail: rt_qed@163.com
> Created Time: Mon 06 Jul 2020 04:26:39 PM CST
************************************************************************/

#include "head.h"
#include "common.h"

char* conf_str[50];

int socket_create(int port) {
    int sockfd;
    struct sockaddr_in server;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        return -1;
    }

    if (listen(sockfd, 10) < 0) {
        return -1;
    }

    return sockfd;
}

void make_non_block(int fd) {
    unsigned long ul = 1;
    ioctl(fd, FIONBIO, &ul);
}

void make_block(int fd) {
    unsigned long ul = 0;
    ioctl(fd, FIONBIO, &ul);
}

char *get_conf_value(const char *path, const char *key) {
    if (key == NULL) {
        fprintf(stderr, "Error in argument!");
        return NULL;
    }

    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        perror("fopen()");
        return NULL;
    }

    while (fgets(conf_str, 50, fp) != NULL) {
        int i = 0;
        int len = strlen(conf_str);
        while (i < len) {
            if (conf_str[i] == '=' && strncmp(conf_str, key, i) == 0) {
                fclose(fp);
                return conf_str + i + 1;
            }
            i++;
        }
    }

    fclose(fp);
    return NULL;
}
