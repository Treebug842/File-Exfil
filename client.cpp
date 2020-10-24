#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <errno.h>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << endl << "Error: Not enough arguments!" << endl;
        std::cout << "Usage: client.exe [FILE] [HOST_IP] [PORT]" << endl;
        return 0;
    }

    const char *filename = argv[1];
    const char *host = argv[2];
    char* p;
    errno = 0;
    long _port = strtol(argv[3], &p, 10);
    if (*p != '\0' || errno != 0) {
        std::cout << "Error: intake error, parsing arguments!" << endl;
        return 0;
    }
    const int port = _port;

    SOCKET sock;
    WSADATA wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0) {
        std::cout << "Error: error with dll version!" << endl;
        return 0;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Error: unable to create socket!" << endl;
        return 0;

    }

    HOSTENT *_host = gethostbyname(host);
    if (_host == nullptr) {
        std::cout << "Error: unable to resolve host!" << endl;
        return 0;
    }

    sockaddr_in sin;
    ZeroMemory(&sin, sizeof(sin));
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;
    memcpy(&sin.sin_addr.S_un.S_addr, _host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

    if (connect(sock, (const sockaddr *)&sin, sizeof(sin)) != 0) {
        std::cout << "Error: unable to connect to target!" << endl;
        return 0;
    }

    std::string _info = filename;
    send(sock, _info.c_str(), strlen(_info.c_str()), 0);

    fstream file;
    file.open(filename, ios::in|ios::binary); 
    if (file.is_open()) {
        std::string line;
        while(getline(file, line)) {
            send(sock, line.c_str(), strlen(line.c_str()), 0);
        }
        file.close();
        std::string _success = _info + " sent to " + argv[2] + " on port " + argv[3];
        std::cout << _success;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}


