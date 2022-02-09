#include "fs/operations.h"
#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

void *open_file() {

    int fhandle = tfs_open("/somefile", TFS_O_CREAT);

    printf("%d", fhandle);
    
    assert(fhandle != -1);

    return NULL;
}

int main() {
    pthread_t threads[MAX_OPEN_FILES];

    assert(tfs_init() != -1);

    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        pthread_create(&threads[i], NULL, open_file, NULL);
    }

    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Max limit open test successful\n");
    return 0;
}