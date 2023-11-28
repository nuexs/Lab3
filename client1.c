/* chat_client1.c*/
#define SEND "P1" 
#define RECV "P2"

#include "chat.h"
#include <pthread.h>

int main(int argc, char **argv)
{
    pthread_t sendThread, recvThread;
    struct mq_attr attr;
    int ret, status = 0;
    char namebuf[10];

    sprintf(namebuf, "%d.txt", getpid());
    pthread_mutex_init(&mutex, NULL);

    attr.mq_maxmsg = MAX_MSG;
    attr.mq_msgsize = MSG_SIZE;

    mq1 = mq_open(MQ_1, O_CREAT | O_RDWR, 0666, &attr);
    mq2 = mq_open(MQ_2, O_CREAT | O_RDWR, 0666, &attr);

    fd = open(namebuf, O_WRONLY | O_CREAT, 0666);
    if ((mq1 == (mqd_t)-1) || (mq2 == (mqd_t)-1))
    {
        perror("[ Error ] open message queue error");
        exit(0);
    }
    /* 송신 스레드 생성 */
    ret = pthread_create(&sendThread, NULL, send_thread, (void *)&mq1);
    if (ret < 0)
    {
        perror("[ Error ] thread create error : ");
        exit(0);
    }
    /* 수신 스레드 생성 */
    ret = pthread_create(&recvThread, NULL, recv_thread, (void *)&mq2);
    if (ret < 0)
    {
        perror("[ Error ] thread create error : ");
        exit(0);
    }

    printf("------- System : The chat has started. -------\n");
    strcpy(send_buf, "/s");
    mq_send(mq1, send_buf, strlen(send_buf), 0);

    pthread_join(sendThread, (void **)&status);
    pthread_join(recvThread, (void **)&status);

    return 0;
}
