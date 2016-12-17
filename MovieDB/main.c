/**************************************************//**
 * @file   main.c                                     *
 * @author Foivos S. Zakkak <zakkak@csd.uoc.gr>       *
 *                                                    *
 * @brief Main function for the needs of cs-240a 2014 *
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "moviedb.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


int register_user(int uid) { //prepend
  if(users_list==NULL)
  {
    users_sentinel=malloc(sizeof(struct user));
    if(!users_sentinel) {puts("Out of memory"); return 0;}
    users_sentinel->uid=-1;
    users_sentinel->history=NULL;
    users_sentinel->favorites=NULL;
    users_sentinel->next=NULL;
    users_list=malloc(sizeof(struct user));
    if(!users_list) {puts("Out of memory"); return 0;}
    users_list->uid=uid;
    users_list->history=NULL;
    users_list->favorites=NULL;
    users_list->next=users_sentinel;
    printf("Users = %d , \n",users_list->uid);
    return 1;
  }
  struct user *new_user=malloc(sizeof(struct user));
  if(!new_user) {puts("Out of memory"); return 0;}
  new_user->uid=uid;
  new_user->next=users_list;
  new_user->history=NULL;
  new_user->favorites=NULL;
  users_list=new_user;
  struct user *tmp=users_list;
  printf("Users = ");
  while(tmp->uid!=-1)
  {
      printf("%d , ",tmp->uid);
      tmp=tmp->next;
  }
  printf("\n");
  return 1;
}


int unregister_user(int uid) {
 struct user *currP;
 struct user *prevP=NULL;
  for (currP = users_list;currP != NULL;prevP = currP, currP = currP->next) 
  {
    if (currP->uid==uid) {  
      if (prevP == NULL) 
      {
        users_list = currP->next;
      } 
      else 
      {
        prevP->next = currP->next;
      }
      free(currP->history);
      free(currP->favorites);
      free(currP);
      break;
    }
  }
 struct user* print = users_list;
 printf("Users: \n");
 while (print !=users_sentinel)
 {
   printf(" <%d> ",print->uid);
   print=print->next;
 }
  printf("\n");
  return 1;
}
  

int add_new_movie(int mid, int category, int year){
    int i=0;
    struct movie* current;
    struct movie* new_node=malloc(sizeof(struct movie));
    if(!new_node) {puts("Out of memory"); return 0;}
    new_node->mid=mid;
    new_node->category=category;
    new_node->year=year;
    if (Movie_categories[5]== NULL || Movie_categories[5]->mid >= new_node->mid)
    {
        new_node->next = Movie_categories[5];
        Movie_categories[5]= new_node;
    }
    else
    {
        current = Movie_categories[5];
        while (current->next!=NULL && current->next->mid < new_node->mid)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }
    for(;i<6;i++)
    {
      printf("category[%d]= ",i);
      struct movie* tmp=Movie_categories[i];
      while(tmp!=NULL)
      {
        printf("%d ",tmp->mid);
        tmp=tmp->next;
      }
      printf("\n");
      
    }
    return 1;
}

void sort_categories(int category,int year,int mid){

      struct movie* current;
      struct movie* new_node=malloc(sizeof(struct movie));
      if(!new_node) {puts("Out of memory"); return;}
      new_node->mid=mid;
      new_node->category=category;
      new_node->year=year;
      if (Movie_categories[category]== NULL || Movie_categories[category]->mid >= new_node->mid)
      {
        new_node->next = Movie_categories[category];
        Movie_categories[category]= new_node;
      }
      else
      {
        current = Movie_categories[category];
        while (current->next!=NULL && current->next->mid < new_node->mid)
        {
              current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
      }
    
  return;  
}

int categorize_movies() {
  struct movie *head=Movie_categories[5];
  while (head->next!=NULL)
  {
    if(head->category==0)
      sort_categories(0,head->year,head->mid);
    else if(head->category==1)
      sort_categories(1,head->year,head->mid);
    else if(head->category==2)
      sort_categories(2,head->year,head->mid);
    else if(head->category==3)
      sort_categories(3,head->year,head->mid);
    else if(head->category==4)
      sort_categories(4,head->year,head->mid);
    head=head->next;
  }
print_movies();
  return 1;

}

int rate_movie(int uid, int mid, int score) {
      int flag=0;
      int mflag=0;
      struct movie *search=Movie_categories[5];
      struct user *userfound=malloc(sizeof(struct user));
      if(!userfound) {puts("Out of memory"); return 0;}
      struct user *tmp=users_list;
      while (tmp!=users_sentinel)
      {
        if (tmp->uid==uid)
        {
          userfound=tmp;
          flag=1;
        }
        tmp=tmp->next;
      }
      if (flag==0){puts("User not found"); return 0;}
      
      while (search!=NULL)
      {
        if(search->mid==mid)
        {
            mflag=1;
            struct user_movie* current;
            struct user_movie* new_node=malloc(sizeof(struct user_movie));
            if(!new_node) {puts("Out of memory"); return 0;}
            new_node->mid=search->mid;
            new_node->category=search->category;
            new_node->score=score;
            if (userfound->history== NULL || userfound->history->score <= new_node->score)
            {
              new_node->prev = NULL;
              new_node->next = userfound->history;
              userfound->history= new_node;
            }
            else
            {
              current = userfound->history;
              while (current->next!=NULL && current->next->score > new_node->score)
              {
	    current = current->next;
              }
              new_node->prev = current;
              new_node->next = current->next;
              current->next = new_node;
            }
        }
        search=search->next;
      }
      if (mflag==0){puts("Movie not found"); return 0;}
      printf("HISTORY : \n");
      struct user_movie *head=userfound->history;
        while (head!=NULL)
        {
          printf("mid :%d category: %d score: %d \n",head->mid,head->category,head->score);
          head=head->next;
        }
    return 1;
}

int identify_favorite_movies() {
    struct user *tmp=users_list;
    while (tmp!=users_sentinel)
    {
      struct user_movie *searchistory=tmp->history;
      while (searchistory!=NULL)
      {
      if (searchistory->score>=7)
        {
          struct user_movie* current;
          struct user_movie* new_node=malloc(sizeof(struct user_movie));
          if(!new_node) {puts("Out of memory"); return 0;}
          new_node->mid=searchistory->mid;
          new_node->category=searchistory->category;
          new_node->score=searchistory->score;
          if (tmp->favorites== NULL || tmp->favorites->score < new_node->score)
          {
            new_node->prev=NULL;
            new_node->next = tmp->favorites;
            tmp->favorites= new_node;
          }
          else
          {
            current = tmp->favorites;
            while (current->next!=NULL && current->next->score > new_node->score)
            {
	  current = current->next;
            }
            new_node->next = current->next;
            current->next = new_node;
          }
        }
        searchistory=searchistory->next;
      }
      tmp=tmp->next;
    } 
    printf("FAVORITES: \n");
    struct user *manwlis=users_list;
    while (manwlis->next!=NULL){
      struct user_movie *head=manwlis->favorites;
      printf("uid :%d \n",manwlis->uid);
        while (head!=NULL)
          {
            printf("<mid: %d, score: %d> ",head->mid,head->score);
            head=head->next;
          }
          printf("\n");
          manwlis=manwlis->next;
    }

      return 1;
}


int suggest_movie(int uid, int mid) {
  struct user *tmp=users_list;
  
  while (tmp!=users_sentinel)
  {
    
    if (tmp->uid==uid)
    {  
     
      struct user_movie *searchfavs=tmp->favorites;
      while (searchfavs!=NULL)
      {
        if (searchfavs->mid==mid)
        {
          struct user_movie *favsentinel=searchfavs;
          struct user_movie *search2=tmp->favorites;
          while (search2!=favsentinel)
          {
            if (search2->category==searchfavs->category)
            {
              printf("Primary movie id: %d, score: %d , category: %d, \n",searchfavs->mid,searchfavs->score,searchfavs->category);
              printf("Suggest movie id: %d, score: %d , category: %d, \n",search2->mid,search2->score,search2->category);
              return 1;
            }
            search2=search2->next;
          }
        }
      searchfavs=searchfavs->next;
     }
  }
  
  tmp=tmp->next;
}
return 0	;
}

int search_movie(int mid) {
struct movie *search=Movie_categories[5];      
  while(search->next!=NULL)
    {
      if (search->mid==mid)
      {
        printf("category = %d ,year = %d \n",search->category,search->year);
        return 1;
      }
      search=search->next;
    }
    puts("Movie not found");
    return 0;
}


int print_movies(void) {
 int i=0;
for (;i<6;i++)
{
   struct movie *head=Movie_categories[i];
   printf("category : %d \n",i);
   while (head!=NULL)
    {
      printf("mid :%d category: %d year: %d \n",head->mid,head->category,head->year);
      head=head->next;
    }
}
return 1;
}

int print_users(void) {
  struct user *tmp=users_list;
  while(tmp!=users_sentinel)
  {
    struct user_movie *phistory=tmp->history;
    struct user_movie *pfavorites=tmp->favorites;
    printf("user %d \nHISTORY : ",tmp->uid);  
    while(phistory!=NULL)
    {
      printf("<%d> ",phistory->mid);
      phistory=phistory->next;
    }
    printf("\n");
    printf("FAVORITES : ");  
    while(pfavorites!=NULL)
    {
      printf("<%d> ",pfavorites->mid);
      pfavorites=pfavorites->next;
    }
    printf("\n");
    tmp=tmp->next;
  }
  
  return 1;
}




/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if ( argc != 2 ) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Read input file buff-by-buff and handle the events */
	while ( fgets(buff, BUFFER_SIZE, fin) ) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

		/* Comment */
		case '#':
			break;

		/* Register user
		 * R <uid> */
		case 'R':
		{
			int uid;
			sscanf(buff, "%c %d", &event, &uid);

			if ( register_user(uid) ) {
				DPRINT("R succeeded\n");
			} else {
				fprintf(stderr, "R failed\n");
			}

			break;
		}

		/* Unregister user
		 * U <uid> */
		case 'U':
		{
			int uid;
			sscanf(buff, "%c %d", &event, &uid);
			DPRINT("%c %d\n", event, uid);

			if ( unregister_user(uid) ) {
				DPRINT("%c %d succeeded\n", event, uid);
			} else {
				fprintf(stderr, "%c %d failed\n", event, uid);
			}

			break;
		}

		/* Add movie
		 * A <mid> <category> <year> */
		case 'A':
		{
			int mid;
			int category;
			int year;
			sscanf(buff, "%c %d %d %d", &event, &mid, &category, &year);
			DPRINT("%c %d %d %d\n", event, mid, category, year);

			if ( add_new_movie(mid, category, year) ) {
				DPRINT("%c %d %d %d succeeded\n", event, mid, category, year);
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, mid, category, year);
			}

			break;
		}

		/* Categorize movies
		 * C */
		case 'C':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( categorize_movies() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Rate movie
		 * G <uid> <mid> <score>*/
		case 'G':
		{
			int uid;
			int mid;
			int score;
			sscanf(buff, "%c %d %d %d", &event, &uid, &mid, &score);
			DPRINT("%c %d %d %d\n", event, uid, mid, score);

			if ( rate_movie(uid, mid, score) ) {
				DPRINT("%c %d %d %d succeeded\n", event, uid, mid, score);
				
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, uid, mid, score);
			}
			
			break;
		}

		/* Identify favorites
		 * F */
		case 'F':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( identify_favorite_movies() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Trigger an orphan planet cluster split
		 * C <cid1> <cid2> <cid3> */
		case 'S':
		{
			int uid;
			int mid;
			sscanf(buff, "%c %d %d\n", &event, &uid, &mid);
			DPRINT("%c %d %d\n", event, uid, mid);

			if ( suggest_movie(uid, mid) ) {
				DPRINT("%c %d %d succeeded\n", event, uid, mid);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, uid, mid);
			}

			break;
		}

		/* Search movie
		 * I <mid> */
		case 'I':
		{
			int mid;
			sscanf(buff, "%c %d", &event, &mid);
			DPRINT("%c %d\n", event, mid);

			if ( search_movie(mid) ) {
				DPRINT("%c %d succeeded\n", event, mid);
			} else {
				fprintf(stderr, "%c %d failed\n", event, mid);
			}

			break;
		}

		/* Print movies
		 * M */
		case 'M':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_movies() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Print users
		 * P */
		case 'P':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_users() ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}

		/* Empty line */
		case '\n':
			break;

		/* Ignore everything else */
		default:
			DPRINT("Ignoring buff: %s \n", buff);

			break;
		}
	}

	return (EXIT_SUCCESS);
}
