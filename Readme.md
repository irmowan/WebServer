# WebServer

[The WebServer Project](https://github.com/irmowan/WebServer) is a project written in C language.
It shows event-driven threading on WebServers.
### Introduction
The project contains four C files:

- SimpleServer.c
- SimpleClient.c
- Server.c
- Client.c

The first two files create a simple server & client. They will terminate after only one session.
The latter two files, using methods of processing, could support multi processes.
When Server is running, it will create a new process by using `fork()`, to handle the message from the client. Also, there maybe more than one clients request server at one time.
SimpleServer will read a message from the client and show it, reply a constant string. Server works like SimpleServer, but reply the length of the message to the client. Client will send messages to Server, then get the reply from Server and show it.

### Make file
```
>>> gcc -o SimpleServer SimpleServer.c
>>> gcc -o SimpleClient SimpleClient.c
>>> gcc -o Server Server.c
>>> gcc -o Client Client.c
```
### Usage
```
>>> ./SimpleServer <port>
>>> ./SimpleClient <ip> <port>
>>> ./Server <port>
>>> ./Client <ip> <port>
```
### Demo
#### SimpleServer & SimpleClient
```
# Terminal 1
>>> ./SimpleServer 7001
Welcome to the simple server! The port is 7001.
The server is running...
Here is the message: Hello! This is Client!
```

```
# Terminal 2
>>> ./SimpleClient 127.0.0.1 7001
Please enter the message: Hello! This is Client!
I got your message
```
#### Server & Client
```
# Terminal 1
>>> ./Server 7001
Welcome to the server! The port is 7001.
The server is running...
No.0: Here is the message: This is a simple client.
No.1: Here is the message: Thank you!
No.2: Here is the message: OK.
No.3: Here is the message: Hello!
No.4: Here is the message: It is great.
No.5: Here is the message: This is SimpleClient.

```
```
# Terminal 2
>>> ./Client 127.0.0.1 7001
The message is "This is a simple client.".
The message is "Thank you!".
The length of the message is 24
The length of the message is 10
The message is "OK.".
The message is "Hello!".
The message is "It is great.".
The length of the message is 3
The length of the message is 6
The length of the message is 12
>>> ./SimpleClient 127.0.0.1 7001
Please enter the message: This is SimpleClient.
The length of the message is 22
```
Client send random messages selected from a string array.
We can see the interesting sequence of the replies, it is because the multi processing, Client creates many child processes to behave like many clients.

---

Author: irmo

Reference: 
> [Socket Tutorial](http://www.linuxhowtos.org/C_C++/socket.htm)

Thanks to: qzane

2015.10
