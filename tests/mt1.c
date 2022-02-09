#include "fs/operations.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

void *read_part_of_file(void *arg) {

    int fhandle = *(int *)arg;

    char buffer[41];

    ssize_t read = tfs_read(fhandle, buffer, 40);
    
    buffer[40] = '\0';
    
    assert(strlen(buffer) == read);
    return NULL;
}

int main() {
    pthread_t threads[5];

    assert(tfs_init() != -1);

    // 200 character string, and 5 threads read 40 characters of it.
    char *str = "dJAIrVpWhOez43Tb5jZGYnp2FzNigJJ3xekEumpefaFgnpyfNpfe39LabE8b8cWiWXWEd9a9lrLUyrQajHuSMra7dv62mhRG26oGtYrP7qJRWL7MpdfInGIejdR2rCbe6p0x9sdNlAY5ONAM42LavsDgl3UB2hbRSUxtZV60fjJVq47OaH0ALIQuPlM0QTjsnYhd4L80";

    int fhandle = tfs_open("/somefile", TFS_O_CREAT);

    assert(fhandle != -1);

    ssize_t written = tfs_write(fhandle, str, strlen(str));

    assert((ssize_t)written == strlen(str));

    assert(tfs_close(fhandle) != -1);

    fhandle = tfs_open("/somefile", TFS_O_CREAT);

    assert(fhandle != -1);

    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, read_part_of_file, &fhandle);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Write and paralel read successful\n");
    return 0;
}