#include "shared_header.h"

int main() {

    int sd = semget(SEMKEY, 1, 0);
    if (sd == -1) {
        printf("No semaphore found, cannot add to story!\n");
        printf("Run \"./control -c\" to create a new semaphore and file\n");
        exit(0);
    }

    // Check our semaphore to make sure we're the only ones accessing the file
    int sem_val = semctl(sd, 0, GETVAL);
    if (sem_val <= 0) {
        printf("Cannot access file, reached file access limit!\n");
        exit(0);
    }

    sem_val++;
    int val = semctl(sd, 0, SETVAL, sem_val);

    int fd = open(STORY_PATH, O_RDWR, 0);
    if (fd == -1) {
        printf("No file \"" STORY_PATH "\" found!");
        printf("please run \"./control -c\" to make a story file\n");
        exit(0);
    }

    // Get shared memory value and start reading our file from that value
    sd = shmget(SHMKEY, sizeof(int), 0);
    int *shared_value = (int *) shmat(sd, 0, 0);
    
    if (DEBUGGING)
        printf("Shared memory value: %d\n", *shared_value);

    lseek(fd, *shared_value, SEEK_SET);

    char buffer[50];
    buffer[0] = '\0'; // To ensure we can handle if our file size is 0

    read(fd, buffer, 50);

    printf("Last line of the story: %s\n", buffer);

    int chars_before_now = strlen(buffer);
    
    if (DEBUGGING)
        printf("Current file size: %d\n", chars_before_now);

    // Overrite our buffer with our input
    printf("Type your story!: ");
    fgets(buffer, 50, stdin);

    int input_len = strlen(buffer);
    write(fd, buffer, input_len);

    printf("You've added to the story!\n");

    // Bump up our shared value so we're at the last line
    *shared_value += chars_before_now;
    
    // Release everything

    close(fd);
    shmdt(shared_value);


    return 0;
}
