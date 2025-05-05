#include "../includes/Server.hpp"

bool Server::_isRunning = false;

Server::Server(std::string port, std::string password) {
    this->_fd = -1;
    this->_port = validatePort(port);
    this->_password = validatePassword(password);
}

Server::~Server() { }

// Getters
int const& Server::getFd(void) { return this->_fd; }

int const& Server::getPort(void) { return this->_port; }

std::string const& Server::getPassword(void) { return this->_password; }

// Setters
void Server::setFd(int fd) { this->_fd = fd; }

void Server::setIsRunning(bool isRunning) { this->_isRunning = isRunning; }

// Check validity of the port and password
int Server::validatePort(std::string port) {
    if (port.empty()) {
        throw std::invalid_argument("Port is empty");
    }
    for (size_t i = 0; i < port.size(); i++) {
        if (!isdigit(port[i])) {
            throw std::invalid_argument("Port is not a number");
        }
    }
    int portNumber = std::stoi(port);
    /*
        Ports 0 à 1023 : well-known ports, reserved for system services (HTTP, SSH, FTP…)
        Ports 1024 à 49151 : registered ports, used for applications or standard services
        Ports 49152 à 65535 : dynamic/private ports, often used for temporary connexions (clients)
    */
    if (portNumber < 0 || portNumber > 65535) {
        throw std::invalid_argument("Port is out of range");
    }
    if (portNumber >= 0 && portNumber <= 1023) {
        std::cerr << ITALICRED << "Warning: Port " << portNumber << " is a well-known port. "
                  << "It may conflict with system services or require admin rights." << DEF << std::endl;
    }
    return portNumber;

}

std::string Server::validatePassword(std::string password) {
    if (password.empty()) {
        throw std::invalid_argument("Password cannot be empty.");
    }
    if (password.length() > 20) {
        throw std::invalid_argument("Password cannot be longer than 20 characters.");
    }
    for (size_t i = 0; i < password.length(); i++) {
        if (isspace(password[i])) {
            throw std::invalid_argument("Password cannot contain a space.");
        }
    }
    return password;
}

void Server::setServerSocket(int socketFd) {
    int opt = 1;
    int port = this->getPort();

    // Reuse the same local port (rebind the port after a crash or restart)
    if(setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw(std::runtime_error("failed to set option (SO_REUSEADDR) on socket"));
    // Keep-alive mechanism for the socket (detect dead connections)
    setsockopt(socketFd, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));

    // IPv4 socket address structure
    sockaddr_in serverAddr;
    // Initializes the serverAddr structure to all zeros
    memset(&serverAddr, 0, sizeof(serverAddr));
    // Address family, AF_INET = IPv4
    serverAddr.sin_family = AF_INET;
    // Set the port (converted to network byte order)
    serverAddr.sin_port = htons(port);
    // Bind to all available interfaces
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Calls like accept(), recv(), send(), will not block your program even with no data
    if (fcntl(socketFd, F_SETFL, O_NONBLOCK) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to set socket to non-blocking mode");
    }
    // Bind the socket to the address and port on the local machine 
    if (bind(socketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to bind socket");
    }
    // Prepares the server socket to listen for incoming connections
    if (listen(socketFd, SOMAXCONN) < 0) {
        close(socketFd);
        throw std::runtime_error("Failed to listen on socket");
    }
}

// Launcher the server
void Server::launch(void) {
    this->setFd(socket(AF_INET, SOCK_STREAM, 0));
    int fd = this->getFd();
    if (fd < 0) {
        throw std::runtime_error("Failed to create socket");
    }
    this->setServerSocket(fd);
    this->setIsRunning(true);
}