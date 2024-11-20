#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct {
    char *username;
    char *password;
    char *domain;
    char *path;
} FtpUrl;

int parse_url(char *url, FtpUrl *ftp_url) {
    char *schema = strtok(url, ":"); // Remove scheme
    char *username_password = strtok(NULL, "@");
    char *domain = strtok(NULL, "/");
    char *path = strtok(NULL, "");

    if (schema == NULL || strncmp(username_password, "//", 2) || domain == NULL || path == NULL) {
        fprintf(stderr, "Bad URL\n");
        return -1;
    }
    if (strcmp(schema, "ftp") != 0) {
        fprintf(stderr, "Invalid schema %s\n", schema);
        return -1;
    }

    if (username_password == NULL
        || (ftp_url->username = strtok(username_password + 2, ":")) == NULL
        || (ftp_url->password = strtok(NULL, "")) == NULL) {
        
        ftp_url->username = "anonymous";
        ftp_url->password = "anonymous";
    }

    ftp_url->domain = domain;
    ftp_url->path = path;

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

    printf("Username: %s\n", ftp_url.username);
    printf("Password: %s\n", ftp_url.password);
    printf("Domain: %s\n", ftp_url.domain);
    printf("Path: %s\n", ftp_url.path);

    return 0;
}