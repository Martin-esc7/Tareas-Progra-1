#include <iostream>
#include <winsock2.h>
#include <windows.h>

using namespace std;

int main() {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(3306);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
        cout << "Conexion exitosa con MySQL!" << endl;
    } else {
        cout << "Error al conectar" << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}