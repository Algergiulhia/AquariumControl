#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <syslog.h>
#include <sqlite3.h>

#include "msgqueue.h"
#include "mqHandler.h"
#include "Database.h"

void daemonImp();
int createDaemon();

int main(int argc, char *argv[]) {
    if(createDaemon() != 0){
        syslog(LOG_ERR, "Daemon creation failed");
        exit(EXIT_SUCCESS);
    }
    
    return 0;
}

int createDaemon() {
    pid_t pid;      // process ID
    pid_t sid;      // session ID

    // initialize syslog
    openlog("AqCtrlDaemon", LOG_PID, LOG_DAEMON);

    // create a new process
    pid = fork();
    
    // check for errors during fork
    if (pid < 0){
        syslog(LOG_ERR, "fork Failed"); 
        exit(EXIT_FAILURE);
    }

    // exit the parent process if fork successful
    if (pid > 0)
        exit(EXIT_SUCCESS);

    // create a new session
    sid = setsid();
    
    // check for errors during setsid
    if (sid < 0){
        syslog(LOG_ERR, "setsid failed"); 
        exit(EXIT_FAILURE);
    }

    // set full permissions over newly created files
    umask(0);

    // close the standard streams file descriptors
    // detach from the terminal
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    // change to the root directory and verify its success
    if (chdir("/") < 0){
        syslog(LOG_ERR, "chdir failed"); 
        exit(EXIT_FAILURE);
    }

    // START DAEMON CODE HERE -----------------------------------------------------------------

    syslog(LOG_INFO, "Daemon Created Successfully");
    daemonImp();

    // END DAEMON CODE HERE -------------------------------------------------------------------

    // close resources, cleanup and exit
    closelog();
    exit(EXIT_SUCCESS);
}

/**
 * @brief main daemon function
 * 
 * create all message queues for the program
 * reads the mq "/mqueueToDaemon" infinitey and handles its content
 */
void daemonImp() {  
    mqInitALL();

    while(true) {
        char *msgcontent = readMsgQueue("/mqueueToDaemon");
        if (msgcontent != NULL) {
            HandleMQueue(msgcontent);
            syslog(LOG_INFO, "READ: %s", msgcontent); 
        }   
        else
            syslog(LOG_INFO, "NOT READ");
    }
}

