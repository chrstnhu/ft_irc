#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Server {
    private:
        int _fd;
        int _port;
        std::string _password;
        
        static bool _isRunning;
    
    public:
        Server(std::string port, std::string password);
        ~Server();

        // Getters
        int const& getFd(void);
		int const& getPort(void);
		std::string const& getPassword(void);

        // Setters
        void setFd(int fd);

        // Check the validity of the port and password
        int validatePort(std::string port);
        std::string validatePassword(std::string password);

        // Launch the server
        void launch(void);
        void setServerSocket(int socketFd);
		void setIsRunning(bool isRunning);
}

# endif