# SSH-using-SSL


Specifications
The SSH server has at least one argument <server_port>, which specifies the port at which the SSH server accepts connection requests. The port number used in this assignment should be a user-defined number (i.e., a number between 1024 and 65535). As multiple teams will be testing their servers, it is possible that multiple teams end up using the same port number. To prevent this, use a unique port number (e.g., the last 4 digits of your B number).
The SSH client has at least two arguments: <server_domain> and <server_port>.
<server_domain> specifies the server’s domain name. The domain name is the name of the machine on which the server is running. After you log into remote.cs.binghamton.edu, it will show “remote01:~>”, “remote02:~>” etc. If the server runs on remote01, then the domain name of the server is remote01.cs.binghamton.edu. If the server runs on remote02, then the domain name of the server is remote02.cs.binghamton.edu. If you use C/C++, your program needs to convert it to corresponding 32 bit IP address using the gethostbyname(). http://retran.com/beej/gethostbynameman.html
<server_port> specifies the port number of the server. You can add other arguments to the client and the server if needed.
For example, if you use C/C++, the SSH server can be invoked as:
./sshserv 3479
The SSH client can be invoked as :
./sshcli remote01.cs.binghamton.edu 3479
Upon connecting to the server, the client prints ssh >, which allows the user to execute the following commands.
telnet > ls //list files and sub-directories under the current directory
telnet > pwd //print the working directory of the server
telnet > exit // ends the SSH session
If a user enters a command other than the above, then print “Invalid Command”.
 
You need to generate a public key certificate in order to establish the ssl connection. E.g., in C, you can use the following command to generate certificate.
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -days 365 If you use java, you can use keytool to generate the certificate.
To compile your C program, please use the following commands:
gcc -Wall -w -o sslcli sslcli.c -I/usr/local/ssl/include/ -L/usr/local/ssl/lib -lssl -lcrypto gcc -Wall -w -o sslserv sslserv.c -I/usr/local/ssl/include/ -L/usr/local/ssl/lib -lssl -lcrypto
Important Note
You can use any code available on the web for SSL socket programming. However, you must write your own code for the rest part of the assignment (i.e., ls, pwd, exit). You should also generate the certificate by yourself. When you create a certificate, it will ask you for your name.


TO get the Server.c and client.c file email me @ ritikramaiya34@gmail.com
