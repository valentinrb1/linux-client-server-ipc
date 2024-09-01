/**
 * @file server.c
 * @author Robledo, Valentín
 * @brief 
 * @version 1.0
 * @date March 2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/server.h"

char buffer_fifo[40] = "";

int main()
{
    server_init();
    sig_config();

    while(1){}

    return 0;
}

void sig_handler(int signum, siginfo_t *info, void *ucontext)
{
    (void)ucontext;

    if(signum == SIGUSR1)
        server_logic(info);

    return;
}

void server_logic(siginfo_t *info)
{
    pid_t pid_client = info->si_pid;

    switch (info->si_value.sival_int)
    {
    case 0:
        if(flags.server_fifo_status == SERVER_FREE)
        {
            flags.server_fifo_status = SERVER_TAKEN;
            kill(pid_client, SIGUSR1);
            read_fifo(pid_client);
        }
        else if(flags.server_fifo_status == SERVER_TAKEN)
            kill(pid_client, SIGUSR2);
        break;
    case 1:
        if(flags.server_shm_status == SERVER_FREE)
        {
            flags.server_shm_status = SERVER_TAKEN;
            kill(pid_client, SIGUSR1);
        }
        else if(flags.server_shm_status == SERVER_TAKEN)
            kill(pid_client, SIGUSR2);
        break;
    case 2:
        if(flags.server_msq_status == SERVER_FREE)
        {
            flags.server_msq_status = SERVER_TAKEN;
            kill(pid_client, SIGUSR1);
        }
        else if(flags.server_msq_status == SERVER_TAKEN)
            kill(pid_client, SIGUSR2);
        break;
    case 3:
        read_shared_memory();
        break;
    case 4:
        read_message_queue();
        break;
    default:
        break;
    }
}

void read_fifo()
{
    client_to_server = open("/tmp/client_to_server", O_RDONLY);
    read(client_to_server, buffer_fifo, sizeof(buffer_fifo));

    close(client_to_server);

    flags.server_fifo_status = SERVER_FREE;
    counters.fifo_counter++;

    print_data_exe();
}

void read_shared_memory()
{
    mem_data = (char*)shmat(shmid,(void*)0,0);

    print_data_exe();

    flags.server_shm_status = SERVER_FREE;  
    counters.shm_counter++;
}

void read_message_queue()
{
    msgrcv(msgid, &message, sizeof(message.mesg_text), 1, 0);
    
    flags.server_msq_status = SERVER_FREE;
    counters.msg_counter++;

    print_data_exe();
}

void print_data_exe()
{
    int total_messages = counters.fifo_counter+counters.msg_counter+counters.shm_counter;

    printf("\033[2J\033[1;1H");
    printf("Número de mensajes, cliente tipo A (FIFO): %d\n", counters.fifo_counter);
    printf("Número de mensajes, cliente tipo B (Shared Memory): %d\n", counters.shm_counter);
    printf("Número de mensajes, cliente tipo C (Message Queue): %d\n\n", counters.msg_counter);
    printf("Mensaje Actual, cliente tipo A (FIFO): %s\n", buffer_fifo);
    printf("Mensaje Actual, cliente tipo B (Shared Memory): %s\n", mem_data);
    printf("Mensaje Actual, cliente tipo C (Message Queue): %s\n\n", message.mesg_text);
    printf("Total de mensajes: %d\n", total_messages);
}

void save_data()
{
    FILE* data;

    int total_messages = counters.fifo_counter+counters.msg_counter+counters.shm_counter;

    data = fopen("./files/data.txt", "w");
    if(data == NULL){
        perror("Error al abrir el archivo data, ubicarse en el directorio principal de trabajo\n");
        exit(EXIT_FAILURE);
    }

    printf("\033[2J\033[1;1H");
    fprintf(data, "Número de mensajes tipo A (FIFO): %d\n", counters.fifo_counter);
    fprintf(data, "Número de mensajes tipo B (Shared Memory): %d\n", counters.shm_counter);
    fprintf(data, "Número de mensajes tipo C (Message Queue): %d\n\n", counters.msg_counter);
    fprintf(data, "Total de mensajes: %d\n", total_messages);

    fclose(data);
}

void print_data()
{
    FILE* data;
    char character;

    data = fopen("./files/data.txt", "r");
    if(data == NULL)
    {
        perror("Error al abrir el archivo data, ubicarse en el directorio principal de trabajo\n");
        exit(EXIT_FAILURE);
    }

    while((character = (char)(fgetc(data))) != EOF)
    {
        printf("%c", character);
    }

    fclose(data);
}

void sigint_handler()
{
    save_data();
    print_data();

    sigset_t signal_set;
    sigemptyset(&signal_set);
    sigprocmask(SIG_SETMASK, &signal_set, NULL);

    close(client_to_server);
    unlink("/tmp/client_to_server");

    shmdt(mem_data);
    shmctl(shmid,IPC_RMID,NULL);

    msgctl(msgid, IPC_RMID, NULL);

    exit(0);
}

void server_init()
{
    FILE* file_server_id;

    file_server_id = fopen("./files/server_id.txt","w");
    if (file_server_id == NULL) {
        perror("Error al abrir el archivo server_id, ubicarse en el directorio principal de trabajo\n");
        exit(EXIT_FAILURE);
    }

    fprintf(file_server_id, "%d", getpid());
    fclose(file_server_id);
    
    flags.server_fifo_status = SERVER_FREE;
    flags.server_shm_status = SERVER_FREE;
    flags.server_msq_status = SERVER_FREE;

    counters.fifo_counter = 0;
    counters.shm_counter = 0;
    counters.msg_counter = 0;

    char* fifo = "/tmp/client_to_server";
    mkfifo(fifo, 0666);

    key_t key = ftok("/bin", 65);
    
    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    msgid = msgget(key, 0666 | IPC_CREAT);
}

void sig_config()
{
    sig.sa_sigaction = sig_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_SIGINFO;

    sigaction(SIGUSR1, &sig, NULL);
    signal(SIGINT, sigint_handler);
    signal(SIGHUP, sigint_handler);
    signal(SIGTERM, sigint_handler);
}