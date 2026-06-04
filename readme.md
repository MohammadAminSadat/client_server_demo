# client server demo

A minimal client-server example using a **C++ TCP server** (standalone Asio, CMake) and a **Python TCP client** (standard library). Built as a learning project for async networking fundamentals.

---

## Overview

The server accepts multiple concurrent TCP connections using Asio's async I/O model. For each message the client sends (terminated by `\n`), the server echoes it back with a `SERVER ECHO:` prefix. The Python client connects, sends four messages in sequence, prints each reply, then disconnects.

``` bash
Python client  ──── TCP (port 12345) ────  C++ Asio server
  client.py                                   main.cpp
```

---

## Project structure

```bash
asio-demo/
├── CMakeLists.txt      # Build configuration
├── main.cpp            # C++ async TCP server
├── client.py           # Python TCP client
└── asio/               # Standalone Asio headers (no Boost)
    ├── asio.hpp
    └── asio/
```

---

## Prerequisites

| Tool | Minimum version |
|------|----------------|
| CMake | 4.1 |
| C++ compiler | GCC 14 |
| Python | 3.12 |
| Git | any recent version |

No Boost installation required — Asio is used in standalone mode.

---

## Setup

### 1. Clone the Asio headers

From the project root:

```bash
git clone --depth=1 https://github.com/chriskohlhoff/asio.git asio-src
cp -r asio-src/asio/include/asio* asio/
```

### 2. Build the server

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

This produces the `server` binary inside `build/`.

---

## Running

Open two terminals from the project root.

**Terminal 1 — start the server:**

```bash
./build/server
# Server listening on port 12345...
```

**Terminal 2 — run the Python client:**

```bash
python3 client.py
```

### Expected output

**Python client:**

```
Connected to 127.0.0.1:12345
Sent:     Hello from Python!
Received: SERVER ECHO: Hello from Python!

Sent:     How are you, server?
Received: SERVER ECHO: How are you, server?

Sent:     This is message three.
Received: SERVER ECHO: This is message three.

Sent:     Goodbye!
Received: SERVER ECHO: Goodbye!
```

**C++ server:**

```
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

### Server (main.cpp)

The server is built around three concepts:

- **`asio::io_context`** — the event loop that drives all async operations. `io.run()` blocks the main thread and dispatches callbacks as I/O completes.
- **`Server` class** — owns a `tcp::acceptor` and continuously calls `async_accept()`. Each accepted socket is handed to a new `Session`.
- **`Session` class** — manages one client connection. Uses `async_read_until('\n')` to receive messages and `async_write` to send replies. Lifetime is managed by `shared_ptr` so the session stays alive for the full duration of its async chain.

Because every operation is non-blocking and callback-driven, the single-threaded server handles many simultaneous clients without spawning threads.

### Client (client.py)

A straightforward synchronous client using Python's built-in `socket` module. It connects, iterates over a list of messages, sends each one (with a `\n` delimiter), reads until `\n`, and prints the reply.

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
