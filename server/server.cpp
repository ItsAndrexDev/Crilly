#include "Crilly.h"

int main() {
	Crilly::ServerManager serverManager(4444);
    for (;;) {
		if(serverManager.m_sockets.size() > 0) {

			std::vector<char> buffer;
			std::shared_ptr<asio::ip::tcp::socket> socket;
			
            if (serverManager.readLPFMessage(buffer, socket)) {
                std::string data = std::string(buffer.data(), buffer.size());
                std::cout << "Received message: " << data << std::endl;

				if (data == "ping!") {
					std::string response = "pong!";
					std::vector<char> responseBuffer(response.begin(), response.end());
					serverManager.sendLPFMessage(socket, responseBuffer);
				}
            }
		}
	}
    std::cin.get();
	return 0;
}