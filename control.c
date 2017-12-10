#include "shared_header.h"


// Removes story, shm, and sem
void create_story() {
  union semun su;
  su.val = 1;
  int sd = semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  semctl(sd, 0, SETVAL, su);

  shmget(SHMKEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);  

  open(STORY_PATH, O_RDWR | O_CREAT | O_TRUNC, 0744);
  printf("Created story\n");
}


// View the story (and shm & sem)
void view_story() {
  if (DEBUGGING) {
    printf("DEBUG INFO:\n");
    int sd = semget(SEMKEY, 1, 0);
    printf("Semaphore value: %d\n", semctl(sd, 0, GETVAL));

    sd = shmget(SHMKEY, sizeof(int), 0);
    int *i = (int *)shmat(sd, 0, SHM_RDONLY);
    printf("Shared mem Value: %d\n\n", *i);
    shmdt(i);
  }

  printf("The story:\n");

  if (!fork())
    execlp("cat", "cat", STORY_PATH, NULL);
}


// Removes story, shm, and sem
void remove_story() {
  view_story(); // Prints out story

  int sd = 0; //Later to be used as sem/shm desc
  wait(&sd);
  sd = semget(SEMKEY, 1, 0);
  semctl(sd, 0, IPC_RMID);
  sd = shmget(SHMKEY, 1, 0);
  shmctl(sd, 0, IPC_RMID);
  remove(STORY_PATH);
  printf("Story removed\n");
}

int main(int argc, char** argv) {
  if (argc<2)
    return 1;

  if (!strcmp(argv[1], "-c"))
    create_story();
  else if (!strcmp(argv[1], "-v"))
    view_story();
  else if (!strcmp(argv[1], "-r"))
    remove_story();

  return 0;
}
