# asio-demo

A client-server example using a **C++ TCP server** (standalone Asio, CMake) and a **Python TCP client** (standard library). Built as a learning project for async networking fundamentals.

---

## Overview

The server accepts multiple concurrent TCP connections using Asio's async I/O model. For each message the client sends (terminated by `\n`), the server echoes it back with a `SERVER ECHO:` prefix. The Python client connects, sends four messages in sequence, prints each reply, then disconnects.

``` bash
Python client  ──── TCP (port 12345) ────  C++ Asio server
  main.py                                     main.cpp
```

---

## Project structure

```bash
asio-demo/
├── server/
│   ├── CMakeLists.txt          # Root build configuration
│   ├── main.cpp                # Entry point — starts the server
│   ├── external/
│   │   └── asio/               # Standalone Asio headers (no Boost)
│   │       ├── asio.hpp
│   │       └── asio/
│   └── libs/
│       ├── CMakeLists.txt      # Adds session and server_src subdirectories
│       ├── session/
│       │   ├── CMakeLists.txt
│       │   ├── inc/
│       │   │   └── session.hpp
│       │   └── session.cpp     # Session class — one per connected client
│       └── server_src/
│           ├── CMakeLists.txt
│           ├── inc/
│           │   └── server_src.hpp
│           └── server_src.cpp  # Server class — accepts connections
└── client/
    ├── main.py                 # Entry point — sends messages to the server
    └── modules/
        ├── __init__.py         # Re-exports Client for clean imports
        └── client.py           # Client class — manages the TCP connection
```

---

## Prerequisites

| Tool | Minimum version |
|------|----------------|
| CMake | 3.16 |
| C++ compiler | GCC 14 |
| Python | 3.12 |
| Git | any recent version |

No Boost installation required — Asio is used in standalone mode.

---

## Setup

### 1. Clone the Asio headers

From the `server/` directory:

```bash
git clone --depth=1 https://github.com/chriskohlhoff/asio.git asio-src
cp -r asio-src/asio/include/asio* external/asio/
```

### 2. Build the server

```bash
cd server
mkdir build && cd build
cmake ..
cmake --build .
```

This produces the `server` binary (or `server.exe` on Windows) inside `build/`.

---

## Running

Open two terminals from the project root.

**Terminal 1 — start the server:**

```bash
./server/build/server
# Server listening on port 12345...
```

**Terminal 2 — run the Python client:**

```bash
cd client
python3 main.py
```

### Expected output

**Python client:**

```bash
Connected to 127.0.0.1:12345
Sent:     Hello from Python!
Received: SERVER ECHO: Hello from Python!

Sent:     How are you, server?
Received: SERVER ECHO: How are you, server?

Sent:     This is message three.
Received: SERVER ECHO: This is message three.

Sent:     Goodbye!
Received: SERVER ECHO: Goodbye!
Disconnected.
```

**C++ server:**

```bash
Server listening on port 12345...
New connection from 127.0.0.1:XXXXX
[client] Hello from Python!
[client] How are you, server?
[client] This is message three.
[client] Goodbye!
Client disconnected.
```

---

## How it works

### C++ server

The server is split into two classes, each in its own library target:

- **`Session`** (`libs/session/`) — manages one client connection. Uses `async_read_until('\n')` to receive messages and `async_write` to send replies. Lifetime is managed by `shared_ptr` so the session stays alive for the full duration of its async chain.
- **`Server`** (`libs/server_src/`) — owns a `tcp::acceptor` and continuously calls `async_accept()`. Each accepted socket is handed to a new `Session`.

`main.cpp` creates an `asio::io_context` (the event loop), constructs a `Server`, and calls `io.run()` which blocks and dispatches all async callbacks. Because every operation is non-blocking and callback-driven, the single-threaded server handles many simultaneous clients without spawning threads.

### CMake structure

A shared `asio_config` interface library defined in the root `CMakeLists.txt` carries all the Asio settings (include path, compile definitions, Windows socket libraries). Every target links to it and inherits everything automatically — no repeated definitions across `CMakeLists.txt` files.

```
asio_config (INTERFACE)
    └── session      (links asio_config PUBLIC)
        └── server_src   (links session PUBLIC)
            └── server       (links server_src PRIVATE)
```

### Python client

The client is structured as a package under `modules/`:

- **`Client`** (`modules/client.py`) — wraps a `socket.socket`, exposes `connect()`, `send()`, and `disconnect()`. Also implements `__enter__` / `__exit__` so it can be used as a context manager, guaranteeing the socket is closed even if an exception is raised.
- **`modules/__init__.py`** — re-exports `Client` so `main.py` imports from the package name, not the internal file layout.

`main.py` uses `Client` as a context manager via `with Client(...) as client:`, keeping the entry point free of any connection lifecycle boilerplate.

---

## Protocol

The wire protocol is intentionally minimal:

- **Framing:** newline-delimited (`\n`) UTF-8 text
- **Request:** any string + `\n`
- **Response:** `SERVER ECHO: <original string>\n`

This makes it easy to test with `nc` (netcat) as well:

```bash
echo "hello" | nc 127.0.0.1 12345
```

---

## License

This project is provided as a learning example with no license restrictions. Do whatever you like with it.