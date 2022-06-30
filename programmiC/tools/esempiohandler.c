#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdlib.h>

struct  sigaction act;

static void sig_handler(int signo, siginfo_t *info, void *ctx) {
  int sender;
  sender=(int)info->si_pid; // retrieve pid of sender
  

  // ending signal
  if (signo==SIGTERM || signo==SIGINT) { 
    printf("\n SIGTERM e SIGNINT\n");
    exit(0);
  } else {
    switch (signo) { // update counter based on signal received
      case SIGUSR1: 
        
    printf("\n SIGUSR 1\n");
        break;
      case SIGUSR2: 
        printf("\n SIGUSR 2\n");
        break;
    };
  };
};

void set_handler() {
  act.sa_flags = SA_SIGINFO;           // retrieve additional info when signal arrives (as pid of sender)
  act.sa_sigaction = &sig_handler;
  sigaction(SIGUSR1, &act, 0);
  sigaction(SIGUSR2, &act, 0);
  sigaction(SIGTERM, &act, 0);
  sigaction(SIGINT , &act, 0);
}


int main(void) {
  
  set_handler();                                // initialize handling of signals
  
  printf("[main: #%d]\n", getpid());            // dump own pid
  
  while(1){
    pause();
  }
  
  return 0;
}

