#! /bin/sh


case "$1" in

  start)
        echo "AESD Socket Server Started"
        start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -- -d
        ;;
  
  
  stop)
        echo "AESD Socket Server Stopped"
        start-stop-daemon -K -n aesdsocket --signal TERM
        ;;
        
        *)
          echo "COMMAND USAGE: $0 {start|stop}"
          
  exit 1
  
 esac
 
exit 0
