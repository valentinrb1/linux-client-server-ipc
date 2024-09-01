Sistemas Operativos II - Laboratorio I IPC
###  Ingeniería en Computación - FCEFyN - UNC - 2023
# Laboratorio 1 - Interprocess Communication

## Introduction
IPC mechanisms allow processes to exchange information. We have seen in SOI that they can be classified into two large groups, those of data transfer (pipe, FIFO, message queue, etc.) and those of shared memory (shared memory, memory mapping, etc.). All this set of mechanisms are extremely useful in any project and is what motivates this work.

## Objective
The objective of this practical work is for the student to be able to design and implement a software that makes use of the IPC mechanisms provided by the Operating System, implementing what has been seen in the theoretical and practical sessions and making use of all the knowledge acquired in Software Engineering and Operating Systems I.

## Development
The student is asked to design, implement and test a set of applications (developed in C language) of a client-server architecture, where:

### Clients
- Three types of clients must be implemented (A, B and C).
- Which can be instantiated a number N of times.
- Each type of client must be defined by the IPC mechanism it uses to communicate with the server.
- The IPC mechanism to be used is the one seen in SOI.
- Each client must be able to send a message (string)

### Server
- The server must be able to serve all types of clients. Each client can have more than one instance.
- The server must show the amount of data for each of its protocols and the total. This data must persist in a file
- The server cannot have hardcoded addresses, routes, ports, etc. Everything must be supplied by parameter to the server or by environment variables.

## Correctness Criteria
- The code must be compiled with the compilation flags:
-Wall -Pedantic -Werror -Wextra -Wconversion -std=gnu11
- Correct memory management.
- Divide the code into modules judiciously.
- Code style.
- Error handling
- The code must not contain errors or warnings.
- The code must not contain cppcheck errors.

## Delivery

The delivery is made through the GitHub repository and the completion of the same must be demonstrated by a correct use of the [Git workflow][workflow]. The repository must provide the source files and any other files associated with the compilation, CMAKE project files and the correctly documented code. It must not contain any files associated with IDE projects and it must be assumed that the project can be compiled and run by a `tty` in a Linux distribution with the typical development tools. A report must also be delivered (which can be in MD in the repo) explaining step by step the development, including graphics of the proposed solution design, justifying at all times what was implemented.

## Evaluation
This practical work is individual and must be delivered before 14:59ART on April 23, 2023, leaving it recorded in the LEV with the report file. It will be corrected and then a date will be coordinated for the oral defense of the same.
