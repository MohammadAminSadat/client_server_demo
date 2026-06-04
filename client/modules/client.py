import socket


class Client:
    def __init__(self, host: str = "127.0.0.1", port: int = 12345):
        self.host = host
        self.port = port
        self._socket = None

    def connect(self):
        self._socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self._socket.connect((self.host, self.port))
        print(f"Connected to {self.host}:{self.port}")

    def send(self, message: str) -> str:
        if not self._socket:
            raise RuntimeError("Not connected. Call connect() first.")

        self._socket.sendall((message + "\n").encode())
        print(f"Sent:     {message}")

        reply = b""
        while not reply.endswith(b"\n"):
            chunk = self._socket.recv(1024)
            if not chunk:
                break
            reply += chunk

        response = reply.decode().strip()
        print(f"Received: {response}")
        return response

    def disconnect(self):
        if self._socket:
            self._socket.close()
            self._socket = None
            print("Disconnected.")

    def __enter__(self):
        self.connect()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.disconnect()
