# File-Exfil
A simple way to transfer files over tcp. Made for windows and uses no adrinistrative privlidges or system calls. In the future i plan on making a way for the data to be encoded before its sent (to make it more stealthy).

> Warning: There seems to be problems when transfering binary files, this hasnt been fixed yet

To compile run this command:
```CMD
g++ client.cpp -o client.exe -lws2_32 -lwininet -s -ffunction-sections -fdata-sections -Wno-write-strings -fno-exceptions -fmerge-all-constants -static-libstdc++ -static-libgcc
```
The server (python listener) takes 2 arguments (one of which is compulsery), `-p` for the port and `-o` for the output destination for the files. The client script takes 3 arguments, and all are needed. `client.exe [File to send] [ip address of target] [port]`.




