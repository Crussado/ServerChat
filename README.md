# ServerChat
Simple server chat make in c.
# Use
## Server
In server folder use `make` and execute the server file indicating the port where to open it.
## Clients
In Client folder use `make` and each client must execute the client file indicating the ip and port to connect to.

The client can:
- Send any msg to all users.
- Change his nickname with `/nickname new_nickname`.
- Whisper another user with `/msg destination data`.
- Exit the chat with `/exit`.
