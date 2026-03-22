# Crilly 🦐

**Crilly** is a lightweight, standalone C++ wrapper for **Asio**. It simplifies TCP networking by providing high-level `ServerManager` and `ClientManager` classes, handling the boilerplate of socket management, background worker threads, and message framing.

---

## Features

* **Standalone Asio:** No Boost dependency required.
* **Automatic Framing:** Includes built-in Length-Prefix Framing (LPF) to handle TCP stream fragmentation effortlessly.
* **Thread-Safe:** Server-side socket management is handled on a background thread with mutex protection.
* **POD Friendly:** Easily send and receive raw data or POD (Plain Old Data) structs.

---

## Installation

1.  Ensure you have the [Asio](https://think-async.com/Asio/) library (header-only) available in your include path.
2.  Add `Crilly.h` and `Crilly.cpp` to your project.
3.  The header automatically defines `ASIO_STANDALONE` for you.

---

## Quick Start

### 1. Setting up a Server
The server runs a background thread to accept connections and clean up disconnected clients.

```cpp
#include "Crilly.h"
#include <vector>
#include <iostream>

int main() {
    // Starts the server on port 8080
    Crilly::ServerManager server(8080);

    while (true) {
        std::vector<char> buffer;
        std::shared_ptr<asio::ip::tcp::socket> clientSocket;

        // Poll for incoming framed messages
        if (server.readLPFMessage(buffer, clientSocket)) {
            std::string msg(buffer.begin(), buffer.end());
            std::cout << "Received: " << msg << std::endl;

            // Optional: Echo it back
            server.sendLPFMessage(clientSocket, buffer);
        }
    }
    return 0;
}
```


### 2. Setting up a Client
The client provides a synchronous interface for connecting and communicating with the server.

```cpp
#include "Crilly.h"
#include <vector>
#include <iostream>

int main() {
    // Starts the server on port 8080
    Crilly::ServerManager server(8080);

    while (true) {
        std::vector<char> buffer;
        std::shared_ptr<asio::ip::tcp::socket> clientSocket;

        // Poll for incoming framed messages
        if (server.readLPFMessage(buffer, clientSocket)) {
            std::string msg(buffer.begin(), buffer.end());
            std::cout << "Received: " << msg << std::endl;

            // Optional: Echo it back
            server.sendLPFMessage(clientSocket, buffer);
        }
    }
    return 0;
}
```

## Core Concepts

### Length-Prefix Framing (LPF)
TCP is a stream-based protocol, which means data sent in one `write` call might be received in multiple `read` calls (fragmentation). **Crilly** solves this by implementing a simple framing protocol:

1.  **Header:** It first sends a `size_t` representing the total length of the following data.
2.  **Payload:** It then sends the actual content (the "body").
3.  **Reception:** `readLPFMessage` automatically waits until the entire payload defined by the header has arrived before returning it to your application logic.

---

## API Overview

### ServerManager
| Method | Description |
| :--- | :--- |
| `readLPFMessage` | Scans all connected sockets. If a full framed message is available, it populates the buffer and identifies the sender. |
| `sendLPFMessage` | Prefixes the data with a size header and sends the full packet to a specific socket. |
| `writeData` | A template method to send a POD (Plain Old Data) struct directly. |
| `writeRaw` | Sends a raw pointer and size without any framing headers. |

### ClientManager
| Method | Description |
| :--- | :--- |
| `isConnected` | Returns a boolean indicating if the client's socket is currently open. |
| `sendLPFMessage` | Sends a framed message (header + payload) to the connected server. |
| `readLPFMessage` | Checks if a full framed message has arrived from the server and reads it into a vector. |
| `writeRaw` | Sends raw data to the server without adding a length-prefix header. |