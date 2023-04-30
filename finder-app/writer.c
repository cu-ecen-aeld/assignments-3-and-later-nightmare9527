#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>


int main(int argc, char *argv[]){
  int openfile;
  
  openlog(NULL,0,LOG_USER);
  if(argc != 3){
   syslog(LOG_ERR, "Invalid number of arguments, please enter 2 arguments, path to file AND text in this file\n");
   return 1;
  }
  
  char *path=argv[1];
  char *text=argv[2];
  
  openfile=open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
  
  if(openfile == -1){
   syslog(LOG_ERR, "error opening file %s %s\n", path, strerror( errno ));
   return 1;
   }
   
  ssize_t writecheck = write(openfile, text, strlen(text));
  if(writecheck == -1){
    syslog(LOG_ERR, "fail to write file %s %s\n", text, strerror( errno ));
    return 1;
    }
   
   syslog(LOG_DEBUG, "Writing %s to %s\n", text, path);
   close(openfile);
   
   return 0;
   }
    
