#include "Crilly.h"
#include <thread>
#include <iostream>
#include <cstring>
int main() {
	Crilly::ClientManager clientManager("127.0.0.1",4444);
	std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for connection to establish

	std::string message = "ping!";
	std::vector<char> buffer(message.begin(),message.end());
	clientManager.sendLPFMessage(buffer);

	for (;;) {
		std::vector<char> buffer;
		std::shared_ptr<asio::ip::tcp::socket> socket;

		if (clientManager.readLPFMessage(buffer)) {
			std::string data = std::string(buffer.data(), buffer.size());
			std::cout << "Received response: " << data << std::endl;
			if (data == "pong!")
				std::cout << "Success!\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Avoid busy waiting
	}


	std::cin.get();
	return 0;
}