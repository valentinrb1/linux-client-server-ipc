/**
 * @file clients.h
 * @author Robledo, Valentín
 * @brief 
 * @version 1.0
 * @date March 2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __CLIENTS_H__
#define __CLIENTS_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

struct mesg_buffer {
    long mesg_type;
    char mesg_text[1024];
} message;

struct sigaction sig_server;
char buffer_server_id[40];
pid_t server_id;
union sigval value;
int shmid;
int msgid;

void sig_handler();
void client_init(char* argv[]);
void write_fifo();
void write_shared_memory();
void write_message_queue();

#endif //__CLIENTS_H__