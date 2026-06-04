from modules import Client


def main():
    messages = [
        "Hello from Python!",
        "How are you, server?",
        "This is message three.",
        "Goodbye!",
    ]

    with Client(host="127.0.0.1", port=12345) as client:
        for message in messages:
            client.send(message)


if __name__ == "__main__":
    main()
