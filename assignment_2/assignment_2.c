#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

#define DOCKER_ENV
#define DEBUG

#define MAX_BUFFER_SIZE 1024
#define PRINT_THRESHOLD 50
#define TIMER_INTERVAL 1

unsigned char buffer[MAX_BUFFER_SIZE];
int buffer_size = 0;
pthread_mutex_t lock;

void generate_sensor_data(int);
void *process_data(void *);

int main()
{
  srand(time(NULL));
  pthread_mutex_init(&lock, NULL);

  pthread_t tid;
  pthread_create(&tid, NULL, process_data, NULL);

  struct sigaction sa;
  struct itimerval timer;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = &generate_sensor_data;
  sigaction(SIGALRM, &sa, NULL);
  timer.it_value.tv_sec = TIMER_INTERVAL;
  timer.it_value.tv_usec = 0;
  timer.it_interval.tv_sec = TIMER_INTERVAL;
  timer.it_interval.tv_usec = 0;
  setitimer(ITIMER_REAL, &timer, NULL);

  while (1)
  {
    sleep(1);
  }

  pthread_mutex_destroy(&lock);
  return 0;
}

void generate_sensor_data(int signum)
{
  pthread_mutex_lock(&lock);

  int num_bytes = rand() % 6;
  if (buffer_size + num_bytes >= MAX_BUFFER_SIZE)
  {
    num_bytes = MAX_BUFFER_SIZE - buffer_size;
  }
#ifdef DEBUG
  printf("Generated %d bytes: ", num_bytes);
#endif
  for (int i = 0; i < num_bytes; i++)
  {
    buffer[buffer_size + i] = rand() % 256;
#ifdef DEBUG
    printf("%02X ", buffer[buffer_size + i]);
#endif
  }

  buffer_size += num_bytes;
#ifdef DEBUG
  printf(" Current buffer size: %d\n", buffer_size);
#endif
#ifdef DOCKER_ENV
  fflush(stdout);
#endif

  pthread_mutex_unlock(&lock);
}

void *process_data(void *arg)
{
  while (1)
  {
    sleep(10);

    pthread_mutex_lock(&lock);

    if (buffer_size >= PRINT_THRESHOLD)
    {
#ifdef DEBUG
      printf("Processing latest 50 bytes: ");
#endif
      for (int i = buffer_size - PRINT_THRESHOLD; i < buffer_size; i++)
      {
        printf("%02X ", buffer[i]);
      }
      printf("\n");

#ifdef DOCKER_ENV
      fflush(stdout);
#endif

      buffer_size -= PRINT_THRESHOLD;
    }

    pthread_mutex_unlock(&lock);
  }
  return NULL;
}