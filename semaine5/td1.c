#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX 99

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[MAX + 1];
} message;

int main()
{
    key_t key;
    int msgid;
    pid_t pid;

    // ftok to generate unique key
    key = ftok("TD1.c", 65);
    pid = fork();
    if (pid == 0)
    {
        int k;
        int total = 0;
        // msgget creates a message queue
        //  or connect at an existing message queue
        // and returns identifier
        msgid = msgget(key, 0666 | IPC_CREAT);
        do
        {
            // msgrcv to receive message
            msgrcv(msgid, &message, sizeof(message), 1, 0);
            k = 0;
            while(message.mesg_text[k] != 10 && message.mesg_text[k] != 0)
            {
                k++;
            }
            total += k;
            if(message.mesg_text[0] != 10)
                printf("\nData Received is : %s \n", 
                            message.mesg_text);
        } while (message.mesg_text[0] != 10);
        printf("Amont of chars: %d \n", total);
        
    } else {
        msgid = msgget(key, 0666 | IPC_CREAT);
        message.mesg_type = 1;
        do
        {
            printf("Enter your message, empty for exit : ");
            fgets(message.mesg_text,MAX,stdin);
            // msgsnd to send message
            msgsnd(msgid, &message, sizeof(message), 0);
        // 10 is the ending methode of fgets
        } while (message.mesg_text[0] != 10);
        wait(NULL);
        printf("END\n");
        // to destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}
