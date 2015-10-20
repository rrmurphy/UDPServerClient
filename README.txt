UDP Server

UDPServerDisplay.c
This program is a datagram (UDP) server installed at a port # provided by the user
through the command line. This program displays a received datagram one byte at a time in hexadecimal.

How to use it?

1) Compile UDP Server:
	$ java ServerUDP.java
2) Run UDP Server:
	$ java ServerUDP <port number>
3) Compile UDP Client:
	$ g++ -o ClientUDP.out ClientUDP.c
4) Run UDP Client:
	$ ./ClientUDP.out <hostname> <port number> <operation> <message>
