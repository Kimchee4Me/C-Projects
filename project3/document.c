/*Esther Malone emalone3 */
/* This program manipulates a document through several tasks
   such as adding paragraphs and lines, removing lines or words
   and replacing and highlighting */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sysexits.h>

#include "document.h"
#include <ctype.h>

#define MAX_LEN 1024
/*initializes an empty document*/
int
init_document (Document * doc, const char *name) {
  if (name != NULL && doc != NULL && strlen (name) <= MAX_STR_SIZE){
    doc-> number_of_paragraphs = 0;
    strcpy (doc-> name, name);
    return SUCCESS;
  }
  return FAILURE;
}

/*resets a document to be empty */
int
reset_document (Document * doc) {
  if (doc != NULL) {
    doc-> number_of_paragraphs = 0;
    return SUCCESS;
  }
  return FAILURE;
}

/*prints out document */
int
print_document (Document * doc) {
  /*used for iteration */
  int i = 0, j = 0;

  /*check if doc is null*/
  if (doc != NULL) {
    printf ("Document name: \"%s\"\n", doc->name);
    printf ("Number of Paragraphs: %d", doc->number_of_paragraphs);

    for (i = 0; i <= doc->number_of_paragraphs; i++) {
        for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
            printf ("%s\n", doc-> paragraphs[i].lines[j]);
        }
        printf ("\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}

/*add a paragraph after a certain paragraph, or to the 
  beggining of the document */
int
add_paragraph_after (Document * doc, int paragraph_number) {
  int i = 0;
  int np = doc -> number_of_paragraphs;
  
  if (doc != NULL && np <= MAX_PARAGRAPHS
      && paragraph_number <= np) {
    /*If it is empty */
      if (doc-> number_of_paragraphs == 0) {
	  doc-> number_of_paragraphs++;
      }
      else {
	/*Shift all paragraphs down to insert new paragraph */
	  for (i = doc-> number_of_paragraphs; i > paragraph_number; i--) {
	      doc-> paragraphs[i] = doc-> paragraphs[i - 1];
	  }
	  doc-> number_of_paragraphs++;
      }
      return SUCCESS;
  }
  return FAILURE;
}

/*add a line after a particular line */
int
add_line_after (Document * doc, int paragraph_number, int line_number,
		const char *new_line) {
  int i = 0;
  /*variable to check number if number of lines is filled */
  int np = doc -> number_of_paragraphs;
  int check = doc-> paragraphs[paragraph_number].number_of_lines;
  
  if (doc != NULL && np <= MAX_PARAGRAPHS && check <= MAX_PARAGRAPH_LINES
      && new_line != NULL
     && check <= MAX_PARAGRAPH_LINES
      && line_number <= check && paragraph_number <= np) {

    /*check if there are no lines */
      if (doc-> paragraphs[paragraph_number].number_of_lines == 0) {
	  strcpy (doc-> paragraphs[paragraph_number].lines[line_number],
		  new_line);
	  doc-> paragraphs[paragraph_number].number_of_lines++;
      }

      else {
	/*shift lines down and copy in new line */
	  for (i = doc-> paragraphs[paragraph_number].number_of_lines;
	       i > line_number; i--) {
	      strcpy (doc->paragraphs[paragraph_number].lines[i],
		      doc->paragraphs[paragraph_number].lines[i - 1]);
	  }
	  strcpy (doc->paragraphs[paragraph_number].lines[line_number],
		  new_line);
	  /*increment number of lines */
	  doc->paragraphs[paragraph_number].number_of_lines++;
      }
      return SUCCESS;
  }
  return FAILURE;
}

/*get the number of lines in a paragraph */
int
get_number_lines_paragraph (Document * doc, int paragraph_number,
			    int *number_of_lines) {
  int check = doc -> number_of_paragraphs;
  if (doc != NULL && number_of_lines != NULL
      && paragraph_number <= check) {
      *number_of_lines = doc-> paragraphs[paragraph_number].number_of_lines;
      return SUCCESS;
  }
  return FAILURE;
}

/*append a line to the end of a paragraph */
int
append_line (Document * doc, int paragraph_number, const char *new_line) {
  int check = doc-> number_of_paragraphs;
  /*check if document is null */
  if (doc != NULL && paragraph_number <= check && new_line != NULL
     && check <= MAX_PARAGRAPH_LINES) {
      /* copy string to the end of the paragraph */
	strcpy (doc->paragraphs[paragraph_number].
		lines[doc->paragraphs[paragraph_number].number_of_lines],
		new_line);
	doc->paragraphs[paragraph_number].number_of_lines++;
	return SUCCESS;
    
  }
  return FAILURE;
}

/*remove a line from a paragraph */
int
remove_line (Document * doc, int paragraph_number, int line_number) {
  int i = 0;
  int np = doc -> number_of_paragraphs;
  int nl = doc -> paragraphs[paragraph_number].number_of_lines;

  if (doc != NULL && paragraph_number <= np && line_number <= nl) {
    /*remove by shifting all elements left over the removed item */

    for (i = line_number - 1;
	   i < doc->paragraphs[paragraph_number].number_of_lines; i++) {
              strcpy (doc->paragraphs[paragraph_number].lines[i],
	      doc->paragraphs[paragraph_number].lines[i + 1]);
      }

    doc->paragraphs[paragraph_number].number_of_lines--;
    return SUCCESS;
  }
  return FAILURE;
}

/*load data into a document */
int
load_document (Document * doc, char data[][MAX_STR_SIZE + 1], int data_lines) {
  int i = 0, j = 0;

  if (doc != NULL && data != NULL && data_lines != 0) {
      doc->number_of_paragraphs++;
      for (i = 0; i < data_lines; i++) {
	/*check to see if you need to add a new paragraph */
	if (strcmp (data[i], "") == 0) {
	      doc->number_of_paragraphs++;
	      j = 0;
	      i++;
	}

	/*add lines to current paragraph */
	strcpy (doc->paragraphs[doc->number_of_paragraphs].lines[j],
		  data[i]);
	doc->paragraphs[doc->number_of_paragraphs].number_of_lines++;
	j++;
      }
      return SUCCESS;
  }
  return FAILURE;
}

/*replace all occurences of a word with a replacement */
int
replace_text (Document * doc, const char *target, const char *replacement){
  char *occurence;
  /*temporary array to alter and then copy to document */
  char temp[MAX_STR_SIZE] = { '\0' };
  /*pointer that will move for copying points, starts at the 
beggining of temp */
  char *check = &temp[0];
  /*a pointer pointing at current line during iteration */
  const char *tmp;
  /*control is to check for while loop status */
  int i = 0, j = 0, control = 1;
  int target_len, replace_len;
  target_len = strlen (target);
  replace_len = strlen (replacement);

  if (doc != NULL && target != NULL && replacement != NULL) {
      for (i = 0; i <= doc->number_of_paragraphs; i++) {
	  control = 1;
	  for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
	      control = 1;

	      /*reset the temp array for each iteration to all nulls */
	      memset (&temp[0], 0, sizeof (temp));
	      check = &temp[0];
	      
	      /*get current line */
	      tmp = doc->paragraphs[i].lines[j];

	      while (control != 0){
		/*find first occurence of the target */
		occurence = strstr (tmp, target);

		if (occurence != NULL) {
	       /*copy the beggining of the line until occurence into temp */
		  memcpy (check, tmp, occurence - tmp);
		  check += occurence - tmp;
		    /*Then copy the replacement in */
		  memcpy (check, replacement, replace_len);
		  check += replace_len;

	          tmp = occurence + target_len;
	        }
	        else {
		  /*if there are no more occurences*/
		  /*copy the rest of the line */
		  strcpy (check, tmp);
	          control = 0;
		}
	      }
	      /*copy into document */
	      strcpy (doc->paragraphs[i].lines[j], temp);
	  }

      }
      return SUCCESS;
  }

  return FAILURE;
}
/*highlight a word in the document */
int
highlight_text (Document * doc, const char *target) {
  char replace[MAX_STR_SIZE];
  if (doc != NULL && target != NULL) {
 /*create a target witht he highlights to replace the target */
    strcpy (replace, HIGHLIGHT_START_STR);
    strcat (replace, target);
    strcat (replace, HIGHLIGHT_END_STR);

    replace_text (doc, target, replace);
    return SUCCESS;
  }
  return FAILURE;
}

/*remove all occurences of a word */
int
remove_text (Document * doc, const char *target){
  char *occurence;
  char temp[MAX_STR_SIZE] = { '\0' };
  char *check = &temp[0];
  const char *tmp;
  int i = 0, j = 0, control = 1;
  int target_len;

  target_len = strlen (target);


  if (doc != NULL && target != NULL) {
      for (i = 0; i <= doc->number_of_paragraphs; i++) {
	  control = 1;
	  for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
	      control = 1;
	      memset (&temp[0], 0, sizeof (temp));
	      check = &temp[0];

	      tmp = doc->paragraphs[i].lines[j];

	      while (control != 0) {
		  occurence = strstr (tmp, target);

		  if (occurence != NULL) {
		    /*copy the begginging without target*/
		    memcpy (check, tmp, occurence - tmp);
		    check += occurence - tmp;
		    tmp = occurence + target_len;

		  }
		  else {
		    /*copy the rest without target */
		    strcpy (check, tmp );
		    control = 0;
		  }
	      }

	      /*copy to the document */
	      strcpy (doc->paragraphs[i].lines[j], temp);
	  }
      }
      
      return SUCCESS;
  }

  return FAILURE;
}

static int get_num_paragraphs(const char *filename) {
  int num = 1, i = 0, space = 0;
  char line[MAX_LEN];
  FILE *input; 
  input = fopen(filename, "r");

  fgets(line, MAX_LEN + 1, input);
  while(!feof(input)) {
    for(i = 0; i < strlen(line) - 1; i++) {
      if(!isspace(line[i])) {
	space = -1;
      }
    }

    if(space == 0) {
      num++;
    }
    space = 0;
    fgets(line, MAX_LEN + 1, input);
  }
  return num;
}
int load_file(Document *doc, const char *filename) {
  int j = 0, i = 0,space = 0, curr = 0, num_paragraphs = 0;
  Paragraph temp;
  char line[MAX_LEN];
 
  FILE *input;
  input = fopen(filename, "r");

  /*check for null paramaters */
  if(doc != NULL && filename !=NULL && input!= NULL) {
    /*shift existing paragraphs down */
    if(doc -> number_of_paragraphs != 0) {
      num_paragraphs = get_num_paragraphs(filename);
   
      for(i = doc -> number_of_paragraphs + num_paragraphs;
	  i > doc -> number_of_paragraphs+1; i--) {
	doc -> paragraphs[i] = doc -> paragraphs[i-num_paragraphs];
	doc -> paragraphs[i-num_paragraphs] = temp;
      }
    }

    doc -> number_of_paragraphs++;
    curr++;
    fgets(line, MAX_LEN + 1, input);

    while(!feof(input)) {
      for(i = 0; i < strlen(line) -1; i++) {
	if(!isspace(line[i])) {
	  space = -1;
	}
      }
      
      if (space  == 0) {
	doc->number_of_paragraphs++;
	curr++;
	j = 0;
      }
      
      /*add lines to current paragraph */
    
      strncpy (doc->paragraphs[curr].lines[j],
	       line, strlen(line) - 1);
      doc->paragraphs[curr].number_of_lines++;      
      j++;
      space = 0;
      fgets(line, MAX_LEN + 1, input);
    }

    return SUCCESS;
  }
  return FAILURE;
}

/*Save the document into a file */
int save_document(Document *doc, const char *filename) {
  int i = 0, j = 0;
  FILE *save;
  save = fopen(filename,"w");
  if(doc != NULL && filename != NULL && save != NULL) {
    for (i = 0; i <= doc->number_of_paragraphs; i++) {
      for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
	fprintf (save, "%s\n", doc-> paragraphs[i].lines[j]);
      }
      fprintf (save, "\n");
    }
    return SUCCESS;
  }
  return FAILURE;
}
