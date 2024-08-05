#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    SetConsoleOutputCP(CP_UTF8);
    WSADATA wsaData;
    SOCKET sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr;
    int len;

    // Инициализация Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Ошибка инициализации Winsock. Код ошибки: %d\n", WSAGetLastError());
        return 1;
    }

    // Создаем сокет
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Ошибка при создании сокета. Код ошибки: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Заполняем информацию о сервере
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // Любой IP адрес
    servaddr.sin_port = htons(PORT); // Порт

    // Привязываем сокет к порту
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == SOCKET_ERROR) {
        printf("Ошибка привязки сокета. Код ошибки: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    while (1) {
        len = sizeof(cliaddr);
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if (n == SOCKET_ERROR) {
            printf("Ошибка получения данных. Код ошибки: %d\n", WSAGetLastError());
            break;
        }
        buffer[n] = '\0'; // Завершаем строку

        printf("Recieved message: %s\n", buffer);

        // Отправляем ответ обратно клиенту
        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len);
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
