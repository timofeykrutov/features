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
    char *message = "Hello сервер!!!";
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr;

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

    // Заполняем информацию о сервере
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_port = htons(PORT); // Порт
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // IP адрес сервера

    // Отправляем сообщение серверу
    sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&servaddr, sizeof(servaddr));

    // Получаем ответ от сервера
    int len = sizeof(servaddr);
    int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&servaddr, &len);
    if (n == SOCKET_ERROR) {
        printf("Get data error, error code: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    buffer[n] = '\0'; // Завершаем строку

    printf("Server answer: %s\n", buffer);

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
