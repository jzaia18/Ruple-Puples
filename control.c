#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define DEBUGGING 1 // Set to or as appropriate
#define SEMKEY 420
#define SHMKEY 421

void create_story() {
  semget(SEMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  shmget(SHMKEY, 1, IPC_CREAT | IPC_EXCL | 0644);
  open("story.txt", O_RDWR | O_CREAT | O_TRUNC);
  printf("Created story\n");
}


void view_story() {
  //TODO
}

void remove_story() {
  int sd = semget(SEMKEY, 1, 0);
  semctl(sd, 0, IPC_RMID);

  //TODO: shm

  remove("story.txt");

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
