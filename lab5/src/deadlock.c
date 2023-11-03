#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


pthread_mutex_t lock1, lock2;
struct monitor {

    // no. of readers
    int rcnt;
 
    // no. of writers
    int wcnt;
 
    // no. of readers waiting
    int waitr;
 
    // no. of writers waiting
    int waitw;
 
    // condition variable to check whether reader can read
    pthread_cond_t canread;
 
    // condition variable to check whether writer can write
    pthread_cond_t canwrite;
 
    // mutex for synchronization
    pthread_mutex_t condlock;
 
public:
    monitor()
    {
        rcnt = 0;
        wcnt = 0;
        waitr = 0;
        waitw = 0;
 
        pthread_cond_init(&canread, NULL);
        pthread_cond_init(&canwrite, NULL);
        pthread_mutex_init(&condlock, NULL);
    }
 
    // mutex provide synchronization so that no other thread
    // can change the value of data
    void beginread(int i)
    {
        pthread_mutex_lock(&condlock);
 
        // if there are active or waiting writers
        if (wcnt == 1 || waitw > 0) {
            // incrementing waiting readers
            waitr++;
 
            // reader suspended
            pthread_cond_wait(&canread, &condlock);
            waitr--;
        }
 
        // else reader reads the resource
        rcnt++;
        cout << "reader " << i << " is reading\n";
        pthread_mutex_unlock(&condlock);
        pthread_cond_broadcast(&canread);
    }
 
    void endread(int i)
    {
 
        // if there are no readers left then writer enters monitor
        pthread_mutex_lock(&condlock);
 
        if (--rcnt == 0)
            pthread_cond_signal(&canwrite);
 
        pthread_mutex_unlock(&condlock);
    }
 
    void beginwrite(int i)
    {
        pthread_mutex_lock(&condlock);
 
        // a writer can enter when there are no active
        // or waiting readers or other writer
        if (wcnt == 1 || rcnt > 0) {
            ++waitw;
            pthread_cond_wait(&canwrite, &condlock);
            --waitw;
        }
        wcnt = 1;
        cout << "writer " << i << " is writing\n";
        pthread_mutex_unlock(&condlock);
    }
 
    void endwrite(int i)
    {
        pthread_mutex_lock(&condlock);
        wcnt = 0;
 
        // if any readers are waiting, threads are unblocked
        if (waitr > 0)
            pthread_cond_signal(&canread);
        else
            pthread_cond_signal(&canwrite);
        pthread_mutex_unlock(&condlock);
    }
 
}
 
// global object of monitor class
M;
 

void *resource1(){

    pthread_mutex_lock(&lock1);

    printf("Job started in resource1..\n");
    sleep(2);

    printf("Trying to get resourc2\n");
    pthread_mutex_lock(&lock2); 
    printf("Aquired resourc2\n");
    pthread_mutex_unlock(&lock2);

    printf("Job finished in resource1..\n");

    pthread_mutex_unlock(&lock1);

    pthread_exit(NULL);

}

void *resource2(){

    pthread_mutex_lock(&lock2);

    printf("Job started in resource2..\n");
    sleep(2);

    printf("Trying to get resourc1\n");
    pthread_mutex_lock(&lock1); 
    printf("Aquired resourc1\n");
    pthread_mutex_unlock(&lock1);

    printf("Job finished in resource2..\n");

    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);

}



int main() {

    pthread_t r[5], w[5];
    int id[5];
    for (int i = 0; i < 5; i++) {
        id[i] = i;
 
        // creating threads which execute reader function
        pthread_create(&r[i], NULL, &resource1, &id[i]);

        // creating threads which execute writer function
        pthread_create(&w[i], NULL, &resource2, &id[i]);
    }
 
    for (int i = 0; i < 5; i++) {
        pthread_join(r[i], NULL);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(w[i], NULL);
    }

}