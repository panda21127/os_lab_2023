#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main ()
{
    int count=0;
    for(int i=0;i<6;i++){
        pid_t child_pid = fork ();
        if (child_pid == 0) {
            printf("GoodBye my son");
            exit (0);
        }
        else {
            sleep (5);
            count++;
        }
    }
    printf("No, father %d",count);
    return 0;
}
