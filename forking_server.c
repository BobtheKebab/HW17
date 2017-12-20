#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove("luigi");
    exit(0);
  }
}

int main() {

  int from_client;
  while(1){
    from_client = server_setup();
    if(!fork()) {
      subserver(from_client);
    }
  }

  return 0;
}

void subserver(int from_client) {
  char buffer[BUFFER_SIZE];
  int to_client;
  to_client = server_connect(from_client);
  while(1){
    read(from_client, buffer, BUFFER_SIZE);
    printf("[server] received: %s\n", buffer);

    if(!strcmp(buffer, "exit")){
      PRINT("[server] client disconnected");
      close(to_client);
      close(from_client);
      break;
    }

    process(buffer);
    write(to_client, buffer, BUFFER_SIZE);
    printf("[server] sent: %s\n", buffer);
  }
}

void process(char * s) {
  int i;
  for(i = 0; i < strlen(s); i++)
    s[i] = (s[i] + 14) % 94 + 33;
}
