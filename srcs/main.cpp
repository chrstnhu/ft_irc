#include "../includes/Includes.hpp"
#include "../includes/Server.hpp"
#include "../includes/Utils.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        printColor("Usage: ./ircserver <port> <password>", BOLDRED);
        return 1;
    }

    std::string port = argv[1];
    std::string password = argv[2];

    try {
        Server server(port, password);
        server.launch();
        server.cleanup();
    } catch (const std::exception &e) {
        printColor("Error: " + std::string(e.what()), BOLDRED);
        return 1;
    }
}
