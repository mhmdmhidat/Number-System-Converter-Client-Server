# Number-System-Converter-Client-Server
Simple number system converter using TCP socket programming in C with client and concurrent server programs.
This is a simple TCP socket program in C to implement a pair of client and concurrent server programs.
Client-side:
The client asks the user to input the following (with appropriate messages guiding the user) 
1. from_base: the current base of the number ( supported bases: 2, 10, 16) 
2. number: the number to be converted 
3. to_base: the base to which the number need to be converted.
client, after checking the input for any possible errors, sends a request to the server as a string using the following format: number:from_base:to_base
and then waits for the server reply. Once the reply from the server arrives, the client displays the result to the user (with appropriate formatting)
and asks the user for another round of input.
the connection between the client and server stays open, so that the client can repeat the operation again until the user ask for termination by entering “Ctrl+\”. 
The client uses command line arguments to read the IP and port number of the server, and leave it for the kernel to assign its local socket address.
I chose 44008 as Port number in my program.

Server_side:
the server (parent) creates a child for each request of the client (server prints the port and IP address of the client). 
Then it waits for the child to finish processing client’s request and prints the process ID of the terminated child. 
The server will wait for clients’ requests in the specified format as explained above, perform the conversion, sends the result back to the client.
The server is required to support the following conversions: 
1. From binary to decimal 
2. From decimal to binary 
3. From binary to hexadecimal 
4. From hexadecimal to binary 
5. From decimal to Hexadecimal 
6. From hexadecimal to decimal 
If the server receives an unsupported conversion request, it should report back the request that caused the error to the client with the error message
“Unsupported conversion”. The client needs to display that for the user. 
In addition, the server is required to display the IP and the port number of the client together with the request itself. 
Note that the server uses command line arguments to indicate the port it will listen to.
The connection between client and server stays open, so that the client can select an operation from the menu.
when the user sends the client program the SIGQUIT signal (Ctrl+\). The handler will print a notifying message (for example, “Good bye”) before the client exits.
in all cases, the parent server is kept running.
