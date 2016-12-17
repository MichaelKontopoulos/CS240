/*****************************************************
 * @file   main.c                                    *
 * @author Alexandra Barka <barka@csd.uoc.gr>        *
 *                                                   *
 * @brief Main Function for Data Structures (CS240b) *
 * Project: Spring 2016						         *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "Movie.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */
 int hash_function(int userID)
 {
   return(23*userID)%hashtable_size;
  }
/**
 * @brief Creates a new user.
 * Creates a new user with userID as its identification.
 *
 * @param userID The new user's identification
 *
 * @return 0 on success
 *         1 on failure
 */


 int register_user(unsigned int userID){
  int key=hash_function(userID);
  printf("key = %d",key);
  user_t * new_user=malloc(sizeof(user_t));
  user_t * current=malloc(sizeof(user_t));
  new_user->history=NULL;
  new_user->userID=userID;
   if (user_hashtable_p[key]== NULL || user_hashtable_p[key]->userID >= new_user->userID)
      {
        new_user->next = user_hashtable_p[key];
        user_hashtable_p[key]= new_user;
      }
      else
      {
        current = user_hashtable_p[key];
        while (current->next!=NULL && current->next->userID < new_user->userID)
        {
              current = current->next;
        }
        new_user->next = current->next;
        current->next = new_user;
      }
    
   return 1;
 }
 
/**
 * @brief Deletes a user.
 * Deletes a user with userID from the system, along with users' history tree.
 *
 * @param userID The new user's identification
 *
 * @return 0 on success
 *         1 on failure
 */

 int unregister_user(unsigned int userID){
  user_t *currP;
  user_t *prevP=NULL;
  for (currP = user_hashtable_p[hash_function(userID)];currP != NULL;prevP = currP, currP = currP->next) 
  {
    if (currP->userID==userID) {  
      if (prevP == NULL) 
      {
        user_hashtable_p[hash_function(userID)] = currP->next;
      } 
      else 
      {
        prevP->next = currP->next;
      }
      free(currP->history);
      free(currP);
      break;
    }
  }
	 return 1;
 }
 
 /**
 * @brief Add new movie to new release binary tree.
 * Create a node movie and insert it in 'new release' binary tree.
 *
 * @param movieID The new movie identifier
 * @param category The category of the movie
 * @param year The year movie released
 *
 * @return 0 on success
 *         1 on failure
 */
 
 
 
 int search_movie_rate(unsigned int movieID,int score,struct movie **root){
  if(*root==NULL) return 0;
  if((*root)->movieID<movieID)
    search_movie_rate((*root)->movieID,score,&(*root)->leftChild);
  else if((*root)->movieID>movieID)
    search_movie_rate((*root)->movieID,score,&(*root)->rightChild);
  else
  {
    (*root)->watchedCounter++;
    (*root)->sumScore=(*root)->sumScore+score;
    (*root)->medianScore=(*root)->medianScore+((*root)->sumScore/(*root)->watchedCounter);
    printf("watchedCounter = %d \n",(*root)->watchedCounter);
    printf("sumscore =%d \n",(*root)->sumScore);
    return 1;
  }
    
   return 0;
   
 }
 

int add_new_movie(unsigned int movieID, int category, int year,struct movie **root){
  if(*root==NULL)
  { 
      struct movie* tmp=(struct movie*)malloc(sizeof(struct movie));
      tmp->category=category;
      tmp->movieID=movieID;
      tmp->year=year;
      tmp->watchedCounter=0;
      tmp->sumScore=0;
      tmp->medianScore=0;
      tmp->leftChild=NULL;
      tmp->rightChild=NULL;
      //root = (struct movie*)malloc(sizeof(struct movie));
      *root = tmp;      
    return 1;
  }
  else if((*root)->movieID<movieID)
  {    
    return add_new_movie(movieID,category,year,&(*root)->rightChild);
  }
  else
  {
     return add_new_movie(movieID,category,year,&(*root)->leftChild);
  }    
  return 1;
 }
 
 /**
 * @brief Categorize the movies from new release binary tree to the array of categories.
 *
 * @return 0 on success
 *         1 on failure
 */

 void sort_categories(int category,int year,int movieID,int watchedCounter,int sumScore,float medianScore,struct movie **root)
 {
 if(*root==NULL)
  { 
      struct movie* tmp=(struct movie*)malloc(sizeof(struct movie));
      tmp->category=category;
      tmp->movieID=movieID;
      tmp->year=year;
      tmp->watchedCounter=watchedCounter;
      tmp->sumScore=sumScore;
      tmp->medianScore=medianScore;
      tmp->leftChild=NULL;
      tmp->rightChild=NULL;
      //root = (struct movie*)malloc(sizeof(struct movie));
      *root= tmp;      
     
  }
  else if((*root)->movieID<movieID)
  {    
    sort_categories(category,year,movieID,watchedCounter,sumScore,medianScore,&(*root)->rightChild);
  }
  else
  {
     sort_categories(category,year,movieID,watchedCounter,sumScore,medianScore,&(*root)->leftChild);
  }    
  
 }
 
 
 int categorize_movies(movie_t *root){
    if (root==NULL) return 0;
    if(root->category==0)
    sort_categories(0,root->year,root->movieID,root->watchedCounter,root->sumScore,root->medianScore,&categoryArray[0]->movie);
    else if(root->category==1)
    sort_categories(1,root->year,root->movieID,root->watchedCounter,root->sumScore,root->medianScore,&categoryArray[1]->movie);
    else if(root->category==2)
    sort_categories(2,root->year,root->movieID,root->watchedCounter,root->sumScore,root->medianScore,&categoryArray[2]->movie);
    else if(root->category==3)
    sort_categories(3,root->year,root->movieID,root->watchedCounter,root->sumScore,root->medianScore,&categoryArray[3]->movie);
    else if(root->category==4)
    sort_categories(4,root->year,root->movieID,root->watchedCounter,root->sumScore,root->medianScore,&categoryArray[4]->movie);
    categorize_movies(root->leftChild);
    categorize_movies(root->rightChild);
    return 1;
 }
 
 /**
 * @brief User rates the movie with identification movieID with score
 *
 * @param userID The identifier of the user
 * @param movieID The identifier of the movie
 * @param score The score that user rates the movie with id movieID
 *
 * @return 0 on success
 *         1 on failure
 */

 int rate_movie(unsigned int userID, unsigned int movieID, int score){
   int k=0;
   for(;k<5;k++)
   search_movie_rate(movieID,score,&categoryArray[k]->movie);
	 return 1;
 }
 
/**
 * @brief Identify the best rating score movie and cluster all the movies of a category.
 *
 * @param category The category of the movie
 *
 * @return 0 on success
 *         1 on failure
 */

 int cluster_movies(int category){
	 return 1;
 }
 
/**
 * @brief Find the median score that user rates movies.
 *
 * @param userID The identifier of the user
 *
 * @return 0 on success
 *         1 on failure
 */

 int user_stats(unsigned int userID){
	 return 1;
 }
 
/**
 * @brief Search for a movie with identification movieID in a specific category.
 *
 * @param movieID The identifier of the movie
 * @param category The category of the movie
 *
 * @return 0 on success
 *         1 on failure
 */

 int search_movie(unsigned int movieID, int category,struct movie **root){
  if(*root==NULL) return 0;
  if((*root)->movieID<movieID)
    search_movie((*root)->movieID,(*root)->category,&(*root)->leftChild);
  else if((*root)->movieID>movieID)
    search_movie((*root)->movieID,(*root)->category,&(*root)->rightChild);
  else
    printf("MOVIEID VRE8HKE,YEAR = %d ,CATEGORY = %d,MEDIANSCORE = %f \n",(*root)->year,(*root)->category,(*root)->medianScore);
    
   
   return 1;
 }
 
 /**
 * @brief Prints the movies in movies categories array.
 * @return 0 on success
 *         1 on failure
 */

 int print_movies(movie_t *root){
   if(root==NULL) return 0;
  print_movies(root->leftChild);
  printf("MOVIE && CATEGORY %d %d\n",root->movieID,root->category);
  print_movies(root->rightChild);
  return 1;
 }
 
  /**
 * @brief Prints the users hashtable.
 * @return 0 on success
 *         1 on failure
 */

 int print_users(void){
   int m=0;
   for(;m<hashtable_size;m++)
   {
     if(user_hashtable_p[m]!=NULL)
     {
       user_t *tmp=user_hashtable_p[m];
       while (tmp!=NULL)
       {
         printf("USER ID = %d \n",tmp->userID);
         tmp=tmp->next;
       }
     }
   }
   
   return 1;
 }
 
 /**
 * @brief Prints all the system information.
 * @return 0 on success
 *         1 on failure
 */
int print_world(void){
  print_movies(categoryArray[0]->movie);
  print_movies(categoryArray[1]->movie);
  print_movies(categoryArray[2]->movie);
  print_movies(categoryArray[3]->movie);
  print_movies(categoryArray[4]->movie);
  
  int m=0;
  for(;m<hashtable_size;m++)
   {
     if(user_hashtable_p[m]!=NULL)
     {
       user_t *tmp=user_hashtable_p[m];
       while (tmp!=NULL)
       {
         int key=hash_function(tmp->userID);
         printf("USER ID = %d ,USER KEY = %d\n",tmp->userID,key);
         tmp=tmp->next;
       }
     }
   }
   
	 return 1;
 }
 

int          hashtable_size; 	/** The size of the users hashtable (>0) */
unsigned int max_users;         /** The maximum number of registrations (users) */
unsigned int max_id;            /** The maximum user ID */

// This is a very conservative progress on the hashtable. Our purpose
// is to force many rehashes to check the stability of the code.
unsigned int primes_g[160] = {  5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                               41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                               83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                              137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                              191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                              241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                              307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                              367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                              431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                              487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                              563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                              617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                              677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                              751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                              823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                              883, 887, 907, 911, 919, 929, 937, 941, 947, 953};

user_t **user_hashtable_p;	/* The users hashtable. This is an array of chains (pinakas katakermatismoy xrhstwn)*/
movie_t *new_releases;     /* New releases simply-linked binary tree*/
movieCategory_t *categoryArray[5];  /* The categories array (pinakas kathgoriwn)*/


int main(int argc, char** argv)
{
	int k=0;
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
	for (;k<5;k++){
	    categoryArray[k]=(struct movie_category*)malloc(sizeof(struct movie_category*));
	    categoryArray[k]->movie=NULL;
	}
	new_releases=malloc(sizeof(movie_t));
	new_releases=NULL;
	
	/* Read input file buff-by-buff and handle the events */
	while ( fgets(buff, BUFFER_SIZE, fin) ) {

		DPRINT("Event: %s \n", buff);

		switch(buff[0]) {

		/* Comment */
		case '#':
			break;
		/* max_users */
		case '0': {
			sscanf(buff, "%c %u", &event, &max_users);
			DPRINT("max users: %u\n", max_users);
			hashtable_size=max_users*0.75;
			user_hashtable_p=(user_t **)malloc(sizeof(user_t *)*hashtable_size);
			int l=0;
			for(;l<hashtable_size;l++)
			  user_hashtable_p[l]=NULL;
			break;
		}
		/* max_id */
		case '1': {
			sscanf(buff, "%c %u", &event, &max_id);
			DPRINT("max id: %u\n", max_id);
			break;
		}		
		/* Event R : R <userID> - Register user. */
		case 'R':
		{
			unsigned int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);
			if ( register_user(userID) ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event U : U <userID> - Unregister user. */
		case 'U':
		{
			unsigned int userID;
			sscanf(buff, "%c %d", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( unregister_user(userID) ) {
				DPRINT("%c %d succeeded\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event A : A <movieID> <category> <year> - Add new movie. */
		case 'A':
		{
			unsigned movieID;
			int category, year;
			sscanf(buff, "%c %d %d %d", &event, &movieID, &category, &year);
			DPRINT("%c %d %d %d\n", event, movieID, category, year);

			if ( add_new_movie(movieID, category, year,&new_releases) ) {
				DPRINT("%c %d %d %d succeeded\n", event, movieID, category, year);
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, movieID, category, year);
			}

			break;
		}
		/* Event C : C  - Categorize movies. */
		case 'C':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( categorize_movies(new_releases) ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event G : G <userID> <movieID> <score>  - Rate movie */
		case 'G':
		{
			unsigned int userID, movieID;
			int score;
			sscanf(buff, "%c %d %d %d", &event, &userID, &movieID, &score);
			DPRINT("%c %d %d %d\n", event, userID, movieID, score);

			if ( rate_movie(userID, movieID, score) ) {
				DPRINT("%c %d %d %d succeeded\n", event, userID, movieID, score);
			} else {
				fprintf(stderr, "%c %d %d %d failed\n", event, userID, movieID, score);
			}

			break;
		}
		/* Event S : S <category>  - Cluster movies */
		case 'S':
		{
			
			int category;
			sscanf(buff, "%c %d\n", &event, &category);
			DPRINT("%c %d\n", event, category);

			if ( cluster_movies(category) ) {
				DPRINT("%c %d succeeded\n", event, category);
			} else {
				fprintf(stderr, "%c %d failed\n", event, category);
			}

			break;
		}
		/* Event Q : Q <userID> - User statistics */
		case 'Q':
		{
			unsigned int userID;
			sscanf(buff, "%c %d\n", &event, &userID);
			DPRINT("%c %d\n", event, userID);

			if ( user_stats(userID) ) {
				DPRINT("%c %d succeeded\n", event, userID);
			} else {
				fprintf(stderr, "%c %d failed\n", event, userID);
			}

			break;
		}
		/* Event I : I <movieID> <category> - Search movie */
		case 'I':
		{
			unsigned int movieID;
			int category;
			sscanf(buff, "%c %d %d\n", &event, &movieID, &category);
			DPRINT("%c %d %d\n", event, movieID, category);

			if ( search_movie(movieID, category,&categoryArray[category]->movie) ) {
				DPRINT("%c %d %d succeeded\n", event, movieID, category);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, movieID, category);
			}

			break;
		}
		/* Event M : M  - Print movies */
		case 'M':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_movies(categoryArray[0]->movie)&&
			  print_movies(categoryArray[1]->movie)&&
			  print_movies(categoryArray[2]->movie)&&
			  print_movies(categoryArray[3]->movie)&&
			  print_movies(categoryArray[4]->movie) ) {
				DPRINT("%c succeeded\n", event);
			} else {
				fprintf(stderr, "%c failed\n", event);
			}

			break;
		}
		/* Event P : P  - Print users */
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
		/* Event W : W  - Print world */
		case 'W':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_world() ) {
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
