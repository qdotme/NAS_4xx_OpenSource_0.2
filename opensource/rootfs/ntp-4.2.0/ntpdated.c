#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main(int argc, char *argv[]) {
  FILE *fp;
  char ntpdate_prog[64], ntpdate_conf[64];
  char buffer[512], ntpdate_cmd[512];
  long sleep_time;
  struct stat sbuf;

  /* Our process ID and Session ID */
  pid_t pid, sid;

  sleep_time = 86400; // seconds
  strcpy(ntpdate_prog, "/usr/bin/ntpdate");
  strcpy(ntpdate_conf, "/etc/ntpdate.conf");

  if (argc == 2) {
    sleep_time = atol(argv[1]);
  } else if (argc > 2) {
    exit(EXIT_FAILURE);
  }

  if (stat(ntpdate_prog,&sbuf) == 0) {
    if((sbuf.st_mode & S_IFREG) != S_IFREG || (sbuf.st_mode & S_IXUSR) != S_IXUSR) {
      fprintf(stderr, "%s was not found\n", ntpdate_prog);
      exit(EXIT_FAILURE);
    }
  } else {
    sprintf(buffer, "stat(\"%s\")", ntpdate_prog);
    perror(buffer);
    exit(EXIT_FAILURE);
  }

  if((fp = fopen(ntpdate_conf, "r")) == NULL) { // config file not found
    sprintf(buffer, "fopen(\"%s\")", ntpdate_conf);
    perror(buffer);
    exit(EXIT_FAILURE);
  } else {
    bzero(buffer, sizeof(buffer) - 1);
    fread(buffer, sizeof(buffer) - 1, 1, fp);
    fclose(fp);

    if (isprint(buffer[0]) != 0) {
      sprintf(ntpdate_cmd, "%s %s", ntpdate_prog, buffer);
    } else {
      exit(EXIT_FAILURE);
    }
  }


  /* Fork off the parent process */
  pid = fork();
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }
  /* If we got a good PID, then
     we can exit the parent process. */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  /* Change the file mode mask */
  umask(0);

  /* Create a new SID for the child process */
  sid = setsid();
  if (sid < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }

  /* Change the current working directory */
  if ((chdir("/")) < 0) {
    /* Log the failure */
    exit(EXIT_FAILURE);
  }

  /* Close out the standard file descriptors */
  //close(STDIN_FILENO);
  //close(STDOUT_FILENO);
  //close(STDERR_FILENO);

  /* Daemon-specific initialization goes here */
  while (1) {
     system(ntpdate_cmd);
     sleep(sleep_time);
  }

  exit(EXIT_SUCCESS);
}
