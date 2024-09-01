/**
 * @file server.h
 * @author Robledo, Valentín
 * @brief 
 * @version 1.0
 * @date March 2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

struct
{
    u_int8_t server_fifo_status;
    u_int8_t server_shm_status;
    u_int8_t server_msq_status;
    u_int8_t reserved;
} flags;

struct 
{
    int fifo_counter;
    int shm_counter;
    int msg_counter;
    int reserved;
} counters;

typedef enum
{
    SERVER_TAKEN,
    SERVER_FREE
} type_server_status;

struct mesg_buffer {
    long mesg_type;
    char mesg_text[1024];
} message;

sigset_t set;
siginfo_t info;
struct sigaction sig;
union sigval value;
int client_to_server;
int shmid;
int msgid;
char* mem_data;

void sig_config();
void server_init();
void read_fifo();
void read_shared_memory();
void read_message_queue();
void save_date();
void print_data();
void print_data_exe();
void server_logic(siginfo_t *info);

#endif // __SERVER_H__