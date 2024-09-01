/**
 * @file clients.c
 * @author Robledo, Valentín
 * @brief 
 * @version 1.0
 * @date March 2023
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../inc/clients.h"

int flag = 0;
int n = 0;

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("Número de argumentos inválido.");
        exit(1);
    }

    client_init(argv);

    while(1)
    {
        if(!flag)
        {
            if(!strcmp(argv[1], "A"))
            {
                value.sival_int = 0;
                if(sigqueue(server_id, SIGUSR1, value) == -1)
                {
                    perror("Error al enviar la señal de cliente tipo A al servidor");
                    exit(EXIT_FAILURE);     
                }
            }
            else if(!strcmp(argv[1], "B"))
            {
                value.sival_int = 1;
                if(sigqueue(server_id, SIGUSR1, value) == -1)
                {
                    perror("Error al enviar la señal de cliente tipo B al servidor");
                    exit(EXIT_FAILURE);     
                }
            }
            else if(!strcmp(argv[1], "C"))
            {
                value.sival_int = 2;
                if(sigqueue(server_id, SIGUSR1, value) == -1)
                {
                    perror("Error al enviar la señal de cliente tipo C al servidor");
                    exit(EXIT_FAILURE);     
                }
            }

            flag = 1;
        }
    }

    return 0;
}

void write_fifo()
{
    int client_to_server;
    char message[11];
    sprintf(message, "%d", n);

    client_to_server = open("/tmp/client_to_server", O_WRONLY);
    write(client_to_server, message, sizeof(message));

    close(client_to_server);
    
    sleep((unsigned int)rand() % 2 + 1);
    n++; 
    flag = 0;
}

void write_shared_memory()
{
    char mem_message[11];
    sprintf(mem_message, "%d", n);

    char *str = (char*)shmat(shmid,(void*)0,0);
    
    strcpy(str, mem_message);
    
    value.sival_int = 3;
    sigqueue(server_id, SIGUSR1, value); //Le aviso al servidor que lea.

    sleep((unsigned int)rand() % 2 + 1);
    n++;
    flag = 0;
}

void write_message_queue()
{
    char msq_message[11];
    sprintf(msq_message, "%d", n);
    strcpy(message.mesg_text, msq_message);

    msgsnd(msgid, &message, strlen(message.mesg_text) + 1, 0);

    value.sival_int = 4;
    sigqueue(server_id, SIGUSR1, value); //Le aviso al servidor que lea.
    
    sleep((unsigned int)rand() % 2 + 1);
    n++;
    flag = 0;
}

void sig_handler(int signum)
{
    if(signum == SIGUSR1)
    {   
        if(value.sival_int == 0)
            write_fifo();
        else if(value.sival_int == 1)
            write_shared_memory();
        else if(value.sival_int == 2)
            write_message_queue();
        else
        {
            printf("Cliente inválido");
            exit(EXIT_FAILURE);
        }
    }
    else if(signum == SIGUSR2)
    {
        sleep(1 + (unsigned int)rand() % 2);
        sigqueue(server_id, SIGUSR1, value);
    }
}

void client_init(char* argv[])
{
    FILE* file_server_id;

    file_server_id = fopen(argv[2], "r");
    if (file_server_id == NULL) {
        perror("\nError al abrir el archivo server_id, ubicarse en el directorio principal de trabajo\n");
        exit(EXIT_FAILURE);
    }
    if (fgets(buffer_server_id, sizeof(buffer_server_id), file_server_id) == NULL)
    {
        printf("El archivo está vacío.\n");
        exit(EXIT_FAILURE);
    }
    server_id = atoi(buffer_server_id);

    key_t key = ftok("/bin", 65);
    
    if(!strcmp(argv[1], "B"))
    {
        shmid = shmget(key,1024, IPC_CREAT);
        if(shmid == -1)
        {
            perror("\nError al obtener un espacio de memoria clienteB\n");
            printf("Error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }
    else if(!strcmp(argv[1], "C"))
    {
        message.mesg_type = 1;
        msgid = msgget(key, 0666);
        if(msgid == -1)
        {
            perror("\nError al obtener una cola de mensajes clienteC\n");
            printf("Error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
    }

    signal(SIGUSR1, sig_handler);
    signal(SIGUSR2, sig_handler);
}