/*Esther Malone emalone3 113588334 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sysexits.h>
#include <ctype.h>
#include "document.h"

#define MAX_LEN 1024
int main(int argc, char *argv[]) {
  
  Document doc;
  const char *doc_name = "main_document";
  FILE *input;
  int quotes[4];
  char new_line[MAX_LEN], line[MAX_LEN], command[MAX_LEN/4],
    target[MAX_LEN], extra[MAX_LEN];
  int standard = 0, check = 0, i = 0, j = 0, nl_check = 0,
    paragraph_number = 0, line_number = 0, total = 0, result = 0;
  char c_paragraph_number, c_line_number;
  init_document(&doc, doc_name);


  /*Check if a file is provided */
  if(argc == 1) {
    input = stdin;
    standard = 1;
  }
  else if( argc == 2) {
    input = fopen(argv[1], "r");    
  }
  else {
    fprintf(stderr, "Usage: a.out\n");
    return EX_USAGE;
  }
  
  fgets(line, MAX_LEN + 1, input);

  /* read until end of input or quit or exit */
  while(!feof(input) && strcmp(line, "quit") != 0
	&& strcmp(line, "exit") != 0) {
    if(standard == 1) {
      printf("> ");
    }

    /*retrieve command */
    check = sscanf(line, "%s", command);

    /*add_paragraph_after */
    if(strcmp(command, "add_paragraph_after") == 0 && check == 1) {
      total = sscanf(line, "%s %d %s", command, &paragraph_number, extra);
      sscanf(line, "%s %c", command, &c_paragraph_number);
      if(total == 2 && isdigit(c_paragraph_number) && paragraph_number >= 0) {
	result = add_paragraph_after(&doc, paragraph_number);
     
	if(result == FAILURE) {
	printf("add_paragraph_after failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
    }

    /*add_line_after */
    if (strcmp(command, "add_line_after") == 0 && check == 1) {
      total = sscanf(line, "%s %d %d * %s", command, &paragraph_number,
		     &line_number, new_line);
      sscanf(line, "%s %c %c * %s", command, &c_paragraph_number,
	     &c_line_number, new_line);

      if(total == 4 && isdigit(c_paragraph_number) && isdigit(c_line_number)
	 && paragraph_number >0 && line_number >=0) {
	for (i = 0; i < MAX_LEN; i++) {
	  if(line[i] == '*') {
	    nl_check = 1;
	    continue;
	  }
	  if (nl_check == 1) {  
	    strncpy(new_line, &line[i], strlen(&line[i])-1);
	    new_line[strlen(new_line)] = '\0';
	    break;
	  }
	}
       
	result = add_line_after(&doc, paragraph_number, line_number, new_line);
	if (result == FAILURE) {
	  printf("add_line_after failed\n");
	}
	nl_check = 0;
      } else {
	printf("Invalid Command\n");
      }
    }

    /*append_line*/
    if (strcmp(command, "append_line") == 0 && check == 1) {
      total = sscanf(line, "%s %d * %s", command, &paragraph_number, new_line);
      sscanf(line, "%s %c * %s", command, &c_paragraph_number, new_line);

      if (total == 3 && isdigit(c_paragraph_number) && paragraph_number > 0) { 
	for (i = 0; i < MAX_LEN; i++)  {

	  if (line[i] == '*') {
	    nl_check = 1;
	    continue;
	  }
	  if (nl_check == 1) {
	    strncpy(new_line, &line[i], strlen(&line[i]) - 1 );
	    break;
	  }
	}

	result = append_line(&doc, paragraph_number, new_line);
	nl_check = 0;

	if (result == FAILURE) {
	  printf("append_line failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
    }

    /*print document */
    if (strcmp(command, "print_document") == 0 && check == 1) {
      total = sscanf(line, "%s %s", command, extra);

      if (total == 1) {
	print_document(&doc);
      } else {
	printf("Invalid Command\n");
      }
    } 

    /*remove line */
    if (strcmp(command, "remove_line") == 0 && check == 1) {

      total = sscanf(line, "%s %d %d %s", command, &paragraph_number,
		     &line_number, extra);
      sscanf(line, "%s %c %c", command, &c_paragraph_number, &c_line_number);

      if (total == 3 && isdigit(c_paragraph_number) && isdigit(c_line_number)
	 && paragraph_number > 0 && line_number > 0) {

	result = remove_line(&doc, paragraph_number, line_number);

	if (result == FAILURE) {
	  printf("remove_line failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
								   
    }

    /*load_file */
    if (strcmp(command, "load_file") == 0 && check == 1) {
      total = sscanf(line, "%s %s %s", command, new_line, extra);
      if (total == 2) {
	result = load_file(&doc, new_line);
	if (result == FAILURE) {
	  printf("load_file failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
    }
   

    /*replace_text */
    if (strcmp(command, "replace_text") == 0 && check == 1) {
      j = 0;

      for (i = 0; i < strlen(line); i++) {
	if (line[i] == '"') {
	  quotes[j] = i;
	  j++;
	}
      }
      
      if(j == 4) {
	strncpy(target, &line[quotes[0] + 1], quotes[1] - quotes[0]-1);
	strncpy(extra, &line[quotes[2] + 1], quotes[3] - quotes[2] - 1);

	result = replace_text(&doc, target, extra);

	if (result == FAILURE) {
	  printf("replace_text failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
    }


    /*highlight_text*/
    if (strcmp(command, "highlight_text") == 0 && check == 1) {

      new_line[0] = '\0';
      j = 0;

      for (i = 0; i < strlen(line); i++) {
	if (line[i] == '"') {
	  quotes[j] = i;
	  j++;
	}
      }

      if(j == 2) {
	strncpy(target, &line[quotes[0] + 1], quotes[1] - quotes[0]-1);
	highlight_text(&doc, target);
      } else {
	printf("Invalid Command");
      }
    }
    
    /*remove_text*/
    if (strcmp(command, "remove_text") == 0 && check == 1) {
      j = 0;
      for(i = 0; i < strlen(line); i++) {
	if(line[i] == '"'){
	  quotes[j] = i;
	  j++;
	}
      }
      if (j == 2) {

	strncpy(target, &line[quotes[0] +1], quotes[1] - quotes[0] - 1);
	remove_text(&doc, target);

      } else {
	printf("Invalid Command\n");
      }
    }
    
    /*save_document*/
    if (strcmp(command, "save_document") == 0 && check == 1) {
      total = sscanf(line, "%s %s %s", command, target, extra);
      if(total == 2) {
	result = save_document(&doc, target);
	if(result == FAILURE) {
	  printf("save_document failed\n");
	}
      } else {
	printf("Invalid Command\n");
      }
    }

    /*reset_document*/
    if (strcmp(command, "reset_document") == 0 && check == 1) {

      total = sscanf(line, "%s %s", command, extra);

      if(total == 1) {
	reset_document(&doc);
      }
    }

    /*quit*/
    if(strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {

      total = sscanf(line, "%s %s", command, extra);

      if(total != 1) {
	printf("Invalid Command\n");
      }
    }

    fgets(line, MAX_LEN + 1, input);
  } 


  fclose(input);
  return 0;
}
