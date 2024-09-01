Sistemas Operativos II - Laboratorio I IPC
# Client-Server using Inter-Process Communication

## Date - 2023

Inter-process communication system using a client-server architecture.

### Authors:
- **Robledo, Valentín**

## How to clone this repository?
You can clone this repository to any directory you want using the following command:

```console
git clone https://github.com/valentinrb1/linux-client-server-ipc.git
```

## How to use?
First, we move to the project folder.

```console
cd linux-client-server-sockets
```

Inside the project directory we are going to create a folder called *build*.

```console
mkdir build
```

We move to that folder.
```console
cd build
```

We run the *cmake* command inside the *build* folder.
```console
cmake ..
```

We run the *make* command, to obtain the binary files that are saved inside the *bin* directory.
```console
make
```

To run the server program.
```console
./bin/server
```

To run the clients. Move to the **bash** directory and run the testing script through the command:
```console
./test.sh <number of clients>
```

## Program logic
#### Client A (FIFO) - Server
In the case of client A or FIFO type, when a client decides to connect to the server it sends a SIGUSR1 type signal plus a token, in this case 0, the server checks if it is being used or not and informs the client with another signal, SIGUSR1 if it is available and SIGUSR2 if it is busy. If it is busy, the client waits a "random" time between 1 and 5 seconds and sends a message sending request again, otherwise, if the server is available, both the client and the server proceed to open the FIFO and the message is sent.

#### Client B (Shared Memory) - Server
For client B or Shared Memory type, the same logic is used, when the client wants to connect to the server it sends a SIGUSR1 signal plus a token, in this case 1, the server checks if it is being used or if it is free and notifies the client with the SIGUSR1 signal if it is available or SIGUSR2 if it is busy. In the case of the second, the client waits a "random" time between 1 and 5 seconds and sends a message sending request again. Otherwise, the client writes the message and notifies the server to read this message using the SIGUSR1 signal but with a token of value 3, and thus we complete the sending of the message.

#### Client C (Message Queue) - Server
In this case the logic is very similar to client B.

#### Server - Save Data
At the end of the server, the number of messages sent by each type of client and the total number of messages are saved and saved in a text file called data.txt
