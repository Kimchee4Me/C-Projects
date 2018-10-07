/*Esther Malone emalone3 */
/*113588334*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <ctype.h>
#include <sys/types.h>
#include <sysexits.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "command.h"
#include "executor.h"

#define SUCCESS 0
#define FAILURE -1
/*static void print_tree(struct tree *t);*/

int aux_execute(struct tree *t) {
  int pipe_fd[2];
  pid_t pid, pid2;

  pid_t child_one, child_two;
  int fd, status;

  if(t == NULL) {
    return FAILURE;
  }
  /*Check the conjunction */
  if(t -> conjunction == NONE) {
    if(strcmp(t -> argv[0], "exit") == 0) {
      exit(0);
    }

    if(strcmp(t -> argv[0], "cd") == 0) {
      if(t -> argv[1] == NULL) {
	/*If there is no argument */
	if(chdir(getenv("HOME")) == -1) {
	  return FAILURE;
	}
	return SUCCESS;
      }
      /*If it fails*/
      if(chdir(t -> argv[1]) == -1) {
	return FAILURE;
      }
      else {
	return SUCCESS;
      }
    }

    else {
      /* Check for redirection */
      if(t -> input != NULL) {
	if((fd = open(t -> input, O_RDONLY)) < 0) {
	  perror("File opening failed");
	}
	if(dup2(fd, STDIN_FILENO) < 0) {
	  perror("dup2 failed");
	}
      }

      if(t-> output != NULL) {
	if((fd = open(t -> output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0) {
	  perror("File opening failed");
	}
	if(dup2(fd, STDOUT_FILENO) < 0) {
	  perror("dup2 failed");
	}
      }
  
      
      /*fork*/
      pid = fork();
      /*parent*/

      if(pid) {
	/*get return status*/
	wait(&status);
	if (WIFEXITED(status)) {
	  if(WEXITSTATUS(status) == 0) {
	    return SUCCESS;
	  }
	  else {
	    return FAILURE;
	  }
	}
      }

      else {
	if(execvp(t -> argv[0], t -> argv) == -1) {
	  fprintf(stderr,"Failed to execute %s\n", t -> argv[0]);
	  exit(-1);
	}
      }
    }
  }

  if(t -> conjunction == AND) {
    if(aux_execute(t -> left) == SUCCESS) {
      aux_execute(t -> right);
    }
  }

  if(t -> conjunction == PIPE) {
    /* Before the fork, we need to create the pipe */
    if (pipe(pipe_fd) < 0) {
      perror( "pipe error");
    }

    if ((child_one = fork()) < 0) {
      perror( "fork error");
    }

    if (child_one == 0)  { /* CHILD #1's code  */
      if(t -> left -> output != NULL) {
	fprintf(stdout, "Ambiguous output redirect.\n");
	exit(0);
      }
      close(pipe_fd[0]); /* we don't need pipe's read end */
      
      /* Redirecting standard output to pipe write end */
      if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
	perror("dup2 error");
      }
      /* Releasing resource */
      close(pipe_fd[1]);
     
      aux_execute(t -> left);
      exit(0);
    }  else { /* parent's code */

      /* Creating second child */
      if ((child_two = fork()) < 0) {
	perror("fork error");
      }

      if (child_two == 0)  { /* CHILD #2's  */
	if(t -> right -> input != NULL) {
	  fprintf(stdout,"Ambiguous input redirect.\n");
	  exit(0);
	}
	close(pipe_fd[1]); /* we don't need pipe's write end */
	
	if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
	  perror( "dup2 error");
	}
	/* Releasing resource */
	close(pipe_fd[0]);

    
	aux_execute(t -> right);
	exit(0);
      } else {
      
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	/* Reaping each child */
	wait(NULL);
	wait(NULL);
      }
    }
    
  }

  if(t -> conjunction == SUBSHELL) {
    pid2 = fork();

    if(pid2 > 0) {
      /*get return status*/
      wait(&status);
      if (WIFEXITED(status)) {
	if(WEXITSTATUS(status) == 0) {
	  return SUCCESS;
	}
	else {
	  return FAILURE;
	}
      }
    }
    else {
      aux_execute(t -> left);
      exit(0);
    }
  }
   
  /* print_tree(t); */ 
  
  return 0;
}

int execute(struct tree *t) {
  aux_execute(t);
  return 0;
}
/*
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}

*/
