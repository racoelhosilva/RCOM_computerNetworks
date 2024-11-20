#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ATTR_MAX_LENGTH 2048
#define FTP_PORT 21

typedef struct {
    char username[ATTR_MAX_LENGTH];
    char password[ATTR_MAX_LENGTH];
    char domain[ATTR_MAX_LENGTH];
    char path[ATTR_MAX_LENGTH];
} FtpUrl;


int parse_url(char *url, FtpUrl *ftp_url) {
    char *scheme = strtok(url, ":"); // Remove scheme
    char *schemeless_url = strtok(NULL, "");

    printf("%s %s\n", scheme, schemeless_url);
    if (scheme == NULL || strncmp(schemeless_url, "//", 2) != 0) {
        fprintf(stderr, "Bad URL\n");
        return -1;
    }

    char *username_password, *domain;
    if (strchr(schemeless_url + 2, '@') != NULL) {
        username_password = strtok(schemeless_url + 2, "@");
        domain = strtok(NULL, "/");
    } else {
        username_password = NULL;
        domain = strtok(schemeless_url + 2, "/");
    }
    char *path = strtok(NULL, "");

    if (domain == NULL || path == NULL) {
        fprintf(stderr, "Bad URL\n");
        return -1;
    }

    if (strcmp(scheme, "ftp") != 0) {
        fprintf(stderr, "Invalid schema %s\n", scheme);
        return -1;
    }

    char *username, *password;
    if (username_password == NULL
        || (username = strtok(username_password + 2, ":")) == NULL
        || (password = strtok(NULL, "")) == NULL) {
        
        username = "anonymous";
        password = "anonymous";
    }

    strncpy(ftp_url->username, username, ATTR_MAX_LENGTH);
    strncpy(ftp_url->password, password, ATTR_MAX_LENGTH);
    strncpy(ftp_url->domain, domain, ATTR_MAX_LENGTH);
    strncpy(ftp_url->path, path, ATTR_MAX_LENGTH);

    return 0;
}

struct hostent *get_host(const char *domain) {
    struct hostent *host;
    if ((host = gethostbyname(domain)) == NULL) {
        perror("gethostbyname()");
    }

    return host;
}

int get_socket_fd(const struct hostent *host, int port) {
    struct sockaddr_in server_addr;
    int sockfd;

    const char *addr = inet_ntoa(*((struct in_addr *) host->h_addr));

    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(addr);
    server_addr.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("connect()");
        return -1;
    }

    printf("Connected to %s:%d\n", addr, port);
    return sockfd;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <FTP URL>\n", argv[0]);
        return 1;
    }

    char *url = argv[1];
    FtpUrl ftp_url;

    if (parse_url(url, &ftp_url) != 0) {
        return 1;
    }

    struct hostent *host = get_host(ftp_url.domain);
    if (host == NULL) {
        return 1;
    }

    int sockfd = get_socket_fd(host, FTP_PORT);
    if (sockfd < 0) {
        return 1;
    }

    return 0;
}