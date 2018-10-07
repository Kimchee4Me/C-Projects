/*Esther Malone 113588334 emalone3 */
/*This program create a calendear in which a user can ad events with details on certain dates*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "event.h"
#include "calendar.h"

/*Initialize the calendar based on parameters */
int init_calendar(const char *name, int days, int (*comp_func) (const void *ptr1, const void *ptr2), void (*free_info_func) (void *ptr), Calendar ** calendar) {
  char *new_name;
  int fail_alloc = 0;
  if(calendar != NULL && name != NULL && days > 0) {
    new_name = malloc(strlen(name) + 1);

    /*check if allocation fails */
    if(new_name == NULL) {
      fail_alloc = 1;
    }
    
    (*calendar) = malloc(sizeof(Calendar));
    if((*calendar) == NULL) {
      fail_alloc = 1;
    }
    
    (*calendar) -> events = calloc(days, sizeof(Event));
    if((*calendar) -> events == NULL) {
      fail_alloc = 1;
    }

    /*If any allocations failed*/
    if(fail_alloc) {
      return FAILURE;
    }
    
    strcpy(new_name, name);
    
    (*calendar) -> name = new_name;
    (*calendar) -> comp_func = comp_func;
    (*calendar) -> free_info_func = free_info_func;
    (*calendar) -> days = days;
    (*calendar) -> total_events = 0;
    
    return SUCCESS;
  }

  else return FAILURE;
}

/*Print the contents of the calendar */
int print_calendar(Calendar *calendar, FILE *output_stream, int print_all) {
  int i = 0;
  Event *curr;

  if(calendar != NULL && output_stream != NULL) {
    /*If print all is true*/
    if(print_all) {
      fprintf(output_stream, "Calendar's Name: \"%s\"\n", calendar -> name);
      fprintf(output_stream, "Days: %d\n", calendar -> days);
      fprintf(output_stream, "Total Events: %d\n", calendar -> total_events);
    }

    /*Print all events*/
    fprintf(output_stream, "\n**** Events ****\n");
    if(calendar -> total_events > 0) {
      curr = calendar -> events[i];

      while(i < calendar -> days) {
	fprintf(output_stream, "Day %d\n", i + 1);

	curr = calendar -> events[i];
	while(curr != NULL) {
	  fprintf(output_stream, "Event's Name: \"%s\", Start_time: %d, Duration: %d\n", curr -> name, curr -> start_time, curr -> duration_minutes);
	  curr = curr -> next;
	}
	i++;
      }
    }  
    return SUCCESS;
  }

  else return FAILURE;
}

/*Find an event by name in the calendar*/
int find_event(Calendar *calendar, const char *name, Event **event) {
  Event *curr, *temp;
  int found = 0;
  int i = 0;
  
  if(calendar != NULL && name != NULL) {
    curr = calendar -> events[i];
    while(i < calendar -> days) {

      curr = calendar -> events[i];
      
      while(curr != NULL) {
	if(strcmp(curr -> name,  name) == 0) {
	  found = 1;
	  temp = curr;
	}	
	curr = curr -> next;
      }
      i++;
    }

    if(found) {
      /*If it is found and event is not null return the event */
      if((*event) != NULL) {
	(*event) = temp;
      }
      return SUCCESS;
    } else return FAILURE;
  }
  else return FAILURE;
}
/*add an event to the calendar onto a specific day*/
int add_event(Calendar *calendar, const char *name, int start_time, int duration_minutes, void *info, int day) {
  Event *curr = NULL, *prev = NULL, *temp = NULL, *new_event = NULL;
  char *new_name;
  int fail_alloc = 0;
  
  if(calendar != NULL && name != NULL && day >= 1 && duration_minutes > 0 && start_time >= 0 && start_time <= 2400) {

    /*check if the name is already in the calendar*/
    if(find_event(calendar, name, &temp) == FAILURE) {
      new_name = malloc(strlen(name) + 1);

      if(new_name == NULL) {
	  fail_alloc = 1;
      }
      strcpy(new_name, name);

      curr = calendar -> events[day - 1];

      new_event = malloc(sizeof(*new_event));

      /*check allocation*/
      if(new_event == NULL) {
	fail_alloc = 1;
      }

      if(fail_alloc) {
        return FAILURE;
      }

      /*Initialize the new event*/
      new_event -> name = new_name;
      new_event -> start_time = start_time;
      new_event -> duration_minutes = duration_minutes;
      new_event -> info = info;
	

      /*Iterate to find where to insert*/
      while(curr != NULL && calendar -> comp_func(curr, new_event) < 0) {
	prev = curr;
        curr = curr -> next;
      }

      /*If it will be the head of the linked list*/
      if(prev == NULL) {
        new_event -> next = curr;
        calendar -> events[day - 1] = new_event;
      }

      else {
        new_event -> next = curr;
        prev -> next = new_event;
      }

      /*Increase number of events*/
      calendar -> total_events++;

      return SUCCESS;
    }
  }
  return FAILURE;
}

/*Find an event in the Calendar in a day*/
int find_event_in_day(Calendar *calendar, const char *name, int day, Event **event) {
  Event *curr;
 
  if(calendar != NULL && name != NULL && day > 0 && day < calendar -> days) {
    curr = calendar -> events[day -1];

    while(curr != NULL && strcmp(curr -> name, name) != 0) {
      curr = curr -> next;
    }

    if(curr != NULL) {
      (*event) = curr;
      return SUCCESS;
    }
  }
  return FAILURE;
}

/*remove an event */
int remove_event(Calendar *calendar, const char *name) {
  Event *curr = NULL, *prev = NULL;
  int i = 0, found = 0;

  if(calendar != NULL && name != NULL) {
    do{
      curr = calendar -> events[i];

      /*If it is the first element of the linkled list */

      if(curr != NULL && strcmp(curr -> name, name) == 0) {
	calendar -> events[i] = curr -> next; 

	if(calendar -> free_info_func != NULL && curr -> info != NULL) {
	  calendar -> free_info_func(curr -> info);
	}
	else {
	  free(curr -> info);
	}
	
	free(curr -> name);
	free(curr);
	curr = NULL;
	calendar -> total_events--;
	return SUCCESS;
      }
      
      curr = calendar -> events[i];

      /*traverse*/
      while(curr != NULL) {
	if(strcmp(curr -> name, name) == 0) {
	  found = 1;
	  break;
	}
	prev = curr;
	curr = curr -> next;
      }
      if(found) {
	break;
      }
      i++;
    } while( i < calendar -> days);

    if(curr == NULL) {
      return FAILURE;
    }

    prev -> next = curr -> next;
    /*
    if(curr -> info != NULL  && calendar -> free_info_func != NULL) {
      calendar -> free_info_func(curr -> info);
    }

    else {
      free(curr -> info);
    }
    */
    free(curr -> name);
    free(curr);
    curr = NULL;
    calendar -> total_events--;
    return SUCCESS;
  }
  return FAILURE;
}

/*get the info of an event and return it*/
void *get_event_info(Calendar *calendar, const char *name) {
  Event *curr, *temp;
  int i = 0;
 
  curr = calendar -> events[i];
  while(i < calendar -> days) {
    curr = calendar -> events[i];
    while(curr != NULL) {
      if(strcmp(name, curr -> name) == 0) {
        temp = curr;
      }
      curr = curr -> next;
    }
    i++;
  }
  if(temp != NULL) { 
    return temp -> info;
  }
  return NULL;
}

/*Clear the whole calendar of events*/
int clear_calendar(Calendar *calendar) {
  Event *curr, *prev = NULL;
  int i = 0;

  if(calendar != NULL) {
    curr = calendar -> events[i];
    while(i < calendar -> days) {
      while(curr!= NULL) {
	prev = curr -> next;
	if(calendar ->free_info_func != NULL && curr -> info != NULL) {
	  calendar -> free_info_func(curr -> info);
	}
	free(curr -> name);
	free(curr);
	curr = prev;	
      }
      calendar -> events[i] = NULL;
      i++;
    }
    calendar -> total_events = 0;
    return SUCCESS;
  }
  else return FAILURE;
}

/*Clear a day of events*/
int clear_day(Calendar *calendar, int day) {
  Event *curr, *prev = NULL;

  if(calendar != NULL && day >= 1 && day < calendar -> days) {
    curr = calendar -> events[day -1];
    while(curr != NULL) {
      prev = curr -> next;
      if(calendar ->free_info_func != NULL && curr -> info != NULL) {
	calendar -> free_info_func(curr -> info);
      }
      free(curr -> name);
      free(curr);
      curr = prev;
      calendar -> total_events--;
    }

    calendar -> events[day -1] = NULL;
    return SUCCESS;
  }
  else return FAILURE;
}

/*Destroy the calendar*/
int destroy_calendar(Calendar *calendar) {
  int i = 0;
  Event *curr, *prev;
  curr = calendar -> events[0];
  while(i < calendar -> days) {
    curr = calendar -> events[i];
    while(curr != NULL) {
      prev = curr -> next;
      if(calendar ->free_info_func != NULL && curr -> info != NULL) {
	calendar -> free_info_func(curr -> info);
      }
      free(curr -> name);
      free(curr);
      curr = prev;
    }
    i++;
  }
  free(calendar -> events);
  free(calendar -> name);
  free(calendar);
  calendar = NULL;
  return SUCCESS;
}

 

