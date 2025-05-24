import socket
import random
import time
import os

SERVER_IP = os.getenv("SERVER_IP", "127.0.0.1")  # Default to localhost if not set
SERVER_PORT = 2152      

# Create a UDP socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    while True:
        # Generate a random buffer size between 1 and 1024 bytes
        buffer_size = random.randint(1, 1024)
        random_data = os.urandom(buffer_size).hex()

        print(f"Sending {buffer_size} bytes of data.")

        client_socket.sendto(random_data.encode(), (SERVER_IP, SERVER_PORT))
        
        # Random sleep between 1 and 5 seconds
        time.sleep(random.randint(1, 5))
except KeyboardInterrupt:
    print("\nClient stopped.")
finally:
    client_socket.close()
