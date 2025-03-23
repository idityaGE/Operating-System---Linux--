#include <pthread.h>
#include <stdio.h>

int counter = 0;

void *incrementCounter(void *arg) {
  for (int i = 0; i < 1000000; i++) {
    counter++;  // Race condition
  }
  return NULL;
}

int main() {
  pthread_t t1, t2;

  pthread_create(&t1, NULL, incrementCounter, NULL);
  pthread_create(&t2, NULL, incrementCounter, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Final Counter: %d\n", counter);  // Incorrect result due to race condition
  return 0;
}


// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./rc
// Final Counter: 1575272
// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./rc
// Final Counter: 1515162
// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./rc
// Final Counter: 1315314
// adii@Virus:/mnt/d/Github/Operating System [ Linux ]/System Calls/thread$ ./rc
// Final Counter: 1018100
