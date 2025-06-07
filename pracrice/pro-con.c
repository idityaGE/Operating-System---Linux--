#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// #include <sys/wait.h>

typedef struct {
  int count;
  int in;
  int out;
  int buffer[100];
  pthread_mutex_t mutex;
  pthread_cond_t notFull;
  pthread_cond_t notEmpty;
} data;

void handle_abr(int sig) {
}

int main() {
  pid_t pid;
  int fd[2];
  int buffer[100];
  pipe(fd);
  char *temp = "/tmp/fifobd";
  mkfifo(temp, 0666);
  int fd2;
  fd2 = open(temp, O_RDONLY);

  FILE *fp;
  fp = popen("ls -la", 'r');
  write(fd[1], "Helo", strlen("Helo") + 1);
  close(fd2);
  pclose(fp);
  sprintf(buffer, "&d", 2);
  fgets(buffer, sizeof(buffer), fp);

  pause();
  raise(SIGINT);
  fflush(stdout);
  kill(pid, SIGABRT);
  int signal;
  wait(&signal);

  execl("./q2_1", "q2_1", NULL);

  waitpid(pid, &signal, 0);

  data daa = {};
  pthread_t producer;
  pthread_t consumer;

  char cmd[50];
  sprintf(cmd, "ps -p %d -o pid,cmd", pid);
  system(cmd);

  signal(SIGABRT, SIG_IGN);
  pthread_cond_init(&daa, NULL, produce, &id);

  pthread_mutex_init(&daa.mutex, NULL);
  
  pthread_join(producer, NULL);

  pthread_mutex_destroy(&daa.mutex);


  return 0;
}
