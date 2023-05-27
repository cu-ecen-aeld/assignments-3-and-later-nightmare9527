#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>
#include <errno.h>
#include <stdbool.h>


#define PORT "9000"
#define FILE "/var/tmp/aesdsocketdata"
#define BUF_SIZE 1024

static int socketch=-1;
static int connectch=-1;
static struct addrinfo *servaddr;

void cleanup(){
  if(socketch>-1){
    shutdown(socketch, SHUT_RDWR);
    close(socketch);
  }
  if(connectch>-1){
    shutdown(connectch, SHUT_RDWR);
    close(connectch);
  }
  if(servaddr !=NULL){
    freeaddrinfo(servaddr);
  }
  
  remove(FILE);
  closelog();
  
}
  
   

static void signal_handler(int signal){
  syslog(LOG_INFO, "Signal Caught %d\n\r",signal);
  
  if(signal ==SIGINT){
    cleanup();
  }
  else if(signal==SIGTERM){
    cleanup();
  }
  
  
  exit(0);
  
 }  



int main(int argc, char *argv[]){


    bool bool_daemon=false;
    openlog("aesdsocket program start", 0, LOG_USER);
    
    sig_t ret_val=signal(SIGINT, signal_handler);
    if(ret_val==SIG_ERR){
      syslog(LOG_ERR, "SIGINT error");
      cleanup();
    }
    
    ret_val=signal(SIGTERM, signal_handler);
    if(ret_val==SIG_ERR){
      syslog(LOG_ERR, "SIGTERM error");
      cleanup();
    }
    
    //check daemon bool
    
    if(argc ==2){
      if (strcmp(argv[1], "-d")==0) {
         bool_daemon = true;
      }
      
      else{
         printf("please use -d option for daemon");
         syslog(LOG_ERR, "error:%s\nUse -d option for creating daemon", argv[1]);
         return -1;
      }
      
    }
    
    struct addrinfo addr;

    memset(&addr, 0, sizeof(addr));

    /*initialise server address*/
    addr.ai_family = AF_INET;
    addr.ai_socktype = SOCK_STREAM;
    addr.ai_flags = AI_PASSIVE;
    
    int result = getaddrinfo(NULL, (PORT), &addr, &servaddr);
    if(result!=0){
      syslog(LOG_ERR, "fail to getaddrinfo, error:%s\n", gai_strerror(result));
      cleanup();
      return -1;
    }
    
    
    //create socket
    
    socketch=socket(servaddr->ai_family, SOCK_STREAM, 0);
    if(socketch<0){
      syslog(LOG_ERR, "fail to create socket, error:%d\n", errno);
      cleanup();
      return -1;
      
    }
    
    if (setsockopt(socketch, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0) 
    {
       syslog(LOG_ERR, "set socket options failed with error:%d\n", errno);
       cleanup();
       return -1;
    }
    //Bind
    if (bind(socketch, servaddr->ai_addr, servaddr->ai_addrlen) < 0) 
    {
       syslog(LOG_ERR, "error on binding socket error:%d\n", errno);
       cleanup();
       return -1;
    }
    //listening
    if (listen(socketch,5)) 
    {
       syslog(LOG_ERR, "listening error:%d\n", errno);
       cleanup();
       return -1;
    }
    
    printf("Listening for connections\n\r");

    if (bool_daemon == true) 
    {
       int return_val = daemon(0,0);
       
       if(return_val==-1)
       {
          syslog(LOG_ERR, "daemon create failed\n");
          cleanup();
          return -1;
       }
    }
  while(1){
    
      struct sockaddr_in client;
      socklen_t addr_size = sizeof(client);
      
      connectch = accept(socketch, (struct sockaddr*)&client, &addr_size);
      
      if(connectch<0)
      {
        syslog(LOG_ERR, "new connection error:%s", strerror(errno));
        cleanup();
        return -1;
      }
      
    char *client_ip = inet_ntoa(client.sin_addr);

    syslog(LOG_INFO, "Accepted connection from %s\n\r",client_ip);
    
    char buf[BUF_SIZE];
    
    
    while(1){
      int BytesRead = read(connectch, buf, (BUF_SIZE));
      if (BytesRead < 0) {
          syslog(LOG_ERR, "erroron reding socket:%d\n", errno);
          continue; 
      }
              
      if (BytesRead == 0)
          continue;

      printf("read %d bytes\n\r", BytesRead);
      
      int fd = open(FILE,O_RDWR | O_CREAT | O_APPEND, 0766);

      if (fd < 0)
         syslog(LOG_ERR, "opening file error:%d\n\r", errno);
         
         
      int BytesWritten = write(fd, buf, BytesRead);
      
      if(BytesWritten < 0){
         syslog(LOG_ERR, "writing file error:%d\n\r", errno);
         close(fd);
         continue;
      }
      close(fd);
      printf("wrote %d bytes\n\r", BytesWritten);
      if (strchr(buf, '\n')) {  
        //if we have recieved a newline, break
        break;
        
      }
    }
    
    int read_sum=0;
    
    while(1){
    
      int fd=open(FILE, O_RDWR | O_CREAT | O_APPEND, 0766);
      if(fd<0){
        syslog(LOG_ERR, "file open failed:%d\n", errno);
        printf("error is %d\n\r", errno);
        continue;
      }
      
      lseek(fd, read_sum, SEEK_SET);
      int BytesRead = read(fd, buf, (BUF_SIZE));
      
      
      close(fd);
      
      if(BytesRead < 0){
        syslog(LOG_ERR, "fail to read file:%d\n", errno);
        continue;
      }
      
      if(BytesRead == 0)
          break;
          
      int BytesWritten = write(connectch, buf, BytesRead);
      printf("wrote %d bytes to client\n\r", BytesWritten);
      
      if(BytesWritten < 0){
        printf("errno is %d", errno);

        syslog(LOG_ERR, "error writing to client%d\n", errno);
        continue;
      }
      
      
      read_sum += BytesWritten;
      
    }
    close(connectch);
    connectch=-1;
    syslog(LOG_INFO, "closing client socket\n\r");
  }
  
  cleanup();
  
  return 0;
  
}

