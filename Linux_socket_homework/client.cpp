#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#define PORT 8080

int main(int argc, char const* argv[])
{
	int status, valread, client_fd;
	sockaddr_in serv_addr{};
	std::string massage;
	char buffer[1024] = { 0 };
	if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		std::cerr << "Socket creation error \n";
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)
		<= 0) {
        std::cerr <<"Invalid address/ Address not supported \n";
		return -1;
	}

	if ((status = connect(client_fd, (struct sockaddr*)&serv_addr,
				sizeof(serv_addr))) < 0) {
        std::cerr << "Connection Failed \n";
		return -1;
	}
    int action, a, b;
    while (true) {
        std::cout << "add: 0\n";
        std::cout << "diff: 1\n";
        std::cout << "mul: 2\n";
        std::cout << "div: 3\n";
        std::cin >> action;
        std::cout << "write nums\n";
        std::cin >> a >> b;
        massage = std::to_string(action) + std::to_string(a) + std::to_string(b);
        send(client_fd, massage.c_str(), strlen(massage.c_str()), 0);
        std::cout << "message sent\n";
        valread = read(client_fd, buffer, 1024);
        std::cout << "result: " << buffer <<  std::endl;
    }

	close(client_fd);
	return 0;
}
