# File-Exfil
A simple way to transfer files over tcp. Made for windows and uses no adrinistrative privlidges. The file is base64 encoded and then sent line by line over tcp to the server. The bug where binary files are corrupted has been fixed and now any type of file can be sent.

> Warning: The client.cpp/exe files are only windows compatible, and the server.py script is made for linux

Please use the flags below when compiling:
```CMD
g++ client.cpp -o client.exe -lws2_32 -fno-exceptions
```
The server (python listener) takes 2 arguments (one of which is compulsery), `-p` for the port and `-o` for the output destination for the files. The client script takes 3 positional arguments, and all are needed. `client.exe [File to send] [ip address of target] [port]`.




