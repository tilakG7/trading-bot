#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>



const int BUFFER_SIZE = 4096;

int main() {
    
    // URL and port
    const char *url = "https://testnet.binance.vision";
    const char *port = "80";

    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Error: Could not create socket\n";
        return 1;
    }

    // Resolve the URL
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(url, port, &hints, &res);
    if (status != 0) {
        std::cerr << "Error: getaddrinfo failed: " << gai_strerror(status) << std::endl;
        close(clientSocket);
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, res->ai_addr, res->ai_addrlen) == -1) {
        std::cerr << "Error: Connection failed\n";
        close(clientSocket);
        freeaddrinfo(res);
        return 1;
    }

    // Send HTTP request
    const char *request = "GET / HTTP/1.1\r\nHost: https://testnet.binance.vision\r\nConnection: close\r\n\r\n";
    if (send(clientSocket, request, strlen(request), 0) == -1) {
        std::cerr << "Error: Send failed\n";
        close(clientSocket);
        freeaddrinfo(res);
        return 1;
    }

    // Receive response
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    while ((bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }

    if (bytesReceived == -1) {
        std::cerr << "Error: Receive failed\n";
    }

    // Close socket and free address info
    close(clientSocket);
    freeaddrinfo(res);

    return 0;
}
