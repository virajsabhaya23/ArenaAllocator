/*
  GROUP NAME : GoScooters
  MEMBERS : Kennedy Mosoti (ID : )
            Viraj Sabhaya (ID : 1001828871)
*/

// The MIT License (MIT)
// 
// Copyright (c) 2022 Trevor Bakker 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/*
PARTITION SCHEMES

1. FIRST_FIT - Partition is allocated which is first sufficient from the beggining of memory list
2. BEST_FIT - Allocates the process to the partition which the first smallest sufficient available
3. NEXT_FIT - Looks for the next partition after the previous allocation success
4. WORST_FIT - Allocate the process to the parition which is the largest sufficient available
*/

#include "mavalloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

enum TYPE {
  FREE = 0,
  USED
};

typedef struct memory_node {
  enum TYPE node_type;
  int start;
  size_t size;
  void* arena;
  struct memory_node* next;
  struct memory_node* prev;
} memory_node;

memory_node* memory_list = NULL;
memory_node* previous_node = NULL;

void *arena;
enum ALGORITHM allocation_algorithm = FIRST_FIT;

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  arena = malloc(ALIGN4(size));
  allocation_algorithm = algorithm;

  if( size < 0){
    return -1;
  }
  /*
  Can't continue with program if the main initialization of the linked list failed, 
  code must break
  */
  memory_list = (memory_node*)malloc(sizeof(memory_node));
  if(memory_list){
    return -1;
  }

  //Initializing the header of the list where the first node is the max size asked for
  memory_list -> arena = arena;
  memory_list -> node_type = FREE;
  memory_list -> start = 0;
  memory_list -> size = ALIGN4( size );
  memory_list -> next = NULL;
  memory_list -> prev = NULL;

  previous_node = memory_list;
  
  return 0;
}


void mavalloc_destroy()
{
  free(arena);

  void* target;
  //Iterative loop that goes through the list deleting each node until the end
  while(memory_list != NULL){
    target = memory_list;
    memory_list = memory_list->next;
    free(target);
  }
  
  return;
}

void * mavalloc_alloc( size_t size )
{
  memory_node* node = NULL;

  if( allocation_algorithm != NEXT_FIT )
  { 
    node = memory_list;
  }
  else if ( allocation_algorithm == NEXT_FIT )
  {
    node = previous_node;
  }
  else
  {
    printf("ERROR: Unknown allocation algorithm!\n");
    exit(0);
  }

  size_t aligned_size = ALIGN4( size );
  
  if(allocation_algorithm == FIRST_FIT){
    //Allocation Scheme FIRST_FIT Algorithm
    while( node )
    {
      if( node -> size >= aligned_size  && node -> node_type == FREE )
      {
        int leftover_size = 0;
  
        node -> node_type  = USED;
        leftover_size = node -> size - aligned_size;
        node -> size =  aligned_size;
  
        if( leftover_size > 0 )
        {
          memory_node * previous_next = node -> next;
          memory_node * leftover_node = ( memory_node* ) malloc ( sizeof( memory_node));
  
          leftover_node -> arena = node->arena + size;
          leftover_node -> node_type  = FREE;
          leftover_node -> size  = leftover_size;
          leftover_node -> next  = previous_next;
  
          node -> next = leftover_node;
        }
        previous_node = node;
        return ( void * ) node -> arena;
      }
      node = node -> next;
    }
  }
  // else if(allocation_algorithm == BEST_FIT){
  //   //Allocation Scheme BEST_FIT Algorithm
  // }
  else if(allocation_algorithm == NEXT_FIT){
    //Allocation Scheme NEXT_FIT Algorithm
  }
  else if(allocation_algorithm == WORST_FIT){
    //Allocation Scheme WORST_FIT Algorithm
  }
  return NULL;
}

void mavalloc_free( void * ptr )
{
  //Casts void pointer to memory_node type
  memory_node* target = (memory_node*)ptr;

  //Checking if the passed pointer is null
  if(target == NULL){
    return;
  }

  //Checking if the node has already been freed
  if(target->node_type == FREE){
    return;
  }

  /*
  Loop through memory_list looking for the matching node. Program will swap the nodetype 
  to free and coelesce the next free node next to it and append it to it's size and delete 
  the succeding node.
  */
  memory_node* curr = memory_list;
  while(curr!=NULL){
    if(curr->arena == ptr){
      curr->node_type = FREE;
      if(curr->next!= NULL && curr->next->node_type == FREE){
        curr->size += curr->next->size;
        curr->next = curr->next->next;
        free(curr->next);
      }
      break;
    }
    //prev = curr;
    curr = curr->next;
  }

  return;
}

int mavalloc_size( )
{
  int number_of_nodes = 0;

  //Iterative loop that counts each node untill it hits the end of the list
  memory_node* head = memory_list;
  while(head != NULL){
    head = head->next;
    number_of_nodes++;
  }

  return number_of_nodes;
}


// int first_fit(){
  
// }
// int next_fit(){
  
// }
// int best_fit(){
  
// }
// int worst_fit(){
  
// }

