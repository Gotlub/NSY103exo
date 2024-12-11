#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#define MAX 10

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_text[100];
} message;

int main()
{
    int msgid;
    pid_t pid;

    msgid = msgget(IPC_PRIVATE, 0666 | IPC_CREAT);
    pid = fork();
    if (pid == 0)
    {
       
        message.mesg_type = 1;

        printf("Write Data : ");
        fgets(message.mesg_text,MAX,stdin);

        // msgsnd to send message
        msgsnd(msgid, &message, sizeof(message), 0);

        // display the message
        printf("Data send is : %s \n", message.mesg_text);
    } else {
        // msgrcv to receive message
        msgrcv(msgid, &message, sizeof(message), 1, 0);

        // display the message
        printf("Data Received is : %s \n", 
                        message.mesg_text);

        // to destroy the message queue
        msgctl(msgid, IPC_RMID, NULL);
    }
    return 0;
}
