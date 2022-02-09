#include "fs/operations.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void *write(void *args) {
    
    char *str = (char *)args;

    int f = tfs_open("/somefile", TFS_O_CREAT);

    ssize_t written = tfs_write(f, str, strlen(str));

    return (void *) written;
}

int main() {
    pthread_t threads[2];
    int fhandle;
    char str[2][41] = {"WfmfaDZl8nReZMz5CAIbmX4WBkUWx1D5WGaq7lrd",
                          "DkMdX8PsJ4habsD3W4KoaekJbCthh1llueSDZfia"};


    assert(tfs_init() != -1);
           
    void *status1 = NULL;
    void *status2 = NULL;
    
    pthread_create(&threads[0], NULL, write, &str[0]);
    pthread_create(&threads[1], NULL, write, &str[1]);

    pthread_join(threads[0], &status1);
    pthread_join(threads[1], &status2);


    assert((ssize_t)status1 == strlen(str[0]) || (ssize_t)status1 == strlen(str[1]));
    assert((ssize_t)status2 == strlen(str[0]) || (ssize_t)status2 == strlen(str[1]));

    fhandle = tfs_open("/somefile", TFS_O_CREAT);

    assert(fhandle != -1);

    char buffer[strlen(str[0])+1];

    ssize_t read = tfs_read(fhandle, buffer, strlen(str[0]));

    buffer[strlen(str[0])] = '\0';

    assert((size_t)read == strlen(buffer));

    assert(strcmp(str[0], buffer) == 0 || strcmp(str[1], buffer) == 0);


    printf("Multiple open and write successful\n");
    return 0;
}