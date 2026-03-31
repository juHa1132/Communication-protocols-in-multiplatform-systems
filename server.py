import socket
import subprocess

def error(msg):
    print(msg)
    exit(1)

def main():
    # Sprawdzanie, czy podano port jako argument
    import sys
    if len(sys.argv) < 2:
        print("ERROR, no port provided")
        exit(1)

    # Ustawienia serwera
    portno = int(sys.argv[1])
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    try:
        sockfd.bind(('', portno))
    except socket.error as msg:
        error(f"ERROR on binding: {msg}")

    sockfd.listen(5)
    print(f"Serwer nasłuchuje na porcie {portno}")

    while True:
        newsockfd, addr = sockfd.accept()
        print(f"Połączono z {addr}")

        buffer = newsockfd.recv(256).decode('utf-8').strip()
        print(f"Here is the message: {buffer}")

        # Sprawdzanie, czy wiadomość zawiera "red"
        # if buffer == "red":
        #     try:
        #         result = subprocess.run(["python3", "/home/student/led_on.py"], 
        #                                 stdout=subprocess.PIPE, 
        #                                 stderr=subprocess.PIPE)
        #         if result.returncode != 0:
        #             print(f"Error executing system command: {result.stderr.decode('utf-8')}")
        #         else:
        #             print("Warunek spełniony")
        #     except Exception as e:
        #         print(f"Exception occurred: {e}")
        # else:
        #     print("Warunek nie jest spełniony")

        

        newsockfd.sendall(b"I got your message")
        newsockfd.close()

if __name__ == "__main__":
    main()
