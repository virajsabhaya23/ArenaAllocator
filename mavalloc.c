/*
  GROUP NAME : GoScooters
  
  MEMBERS : Kennedy Mosoti (ID : 1001596311)
            Viraj Sabhaya  (ID : 1001828871)
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
  void* start_address;
  size_t size;
  struct memory_node* next;
  struct memory_node* prev;
} memory_node;

memory_node* head_of_memory = NULL;
memory_node* previous_node = NULL;
size_t max_size = 0;
void* start_address = NULL;
enum ALGORITHM allocation_algorithm = FIRST_FIT;

void print_memory( );

memory_node* newNode(enum TYPE mem_type, void* address, size_t size, memory_node* next, memory_node* prev){

  memory_node* new_node = (memory_node*)malloc(sizeof(memory_node));
  new_node -> node_type = mem_type;
  new_node -> start_address = address;
  new_node -> size = size;
  new_node -> next = next;
  new_node -> prev = prev;

  return new_node;
}

// void freeNode(memory_node){
  
// }

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  start_address = malloc(ALIGN4(size));
  max_size = ALIGN4(size);
  allocation_algorithm = algorithm;

  head_of_memory = newNode(FREE, start_address, size, NULL, NULL);
  previous_node = head_of_memory;

  return 0;
}

void mavalloc_destroy()
{
  free(start_address);

  memory_node* target;
  //Iterative loop that goes through the list deleting each node until the end
  while(head_of_memory != NULL){
    target = head_of_memory;
    head_of_memory = head_of_memory->next;
    free(target);
  }
  
  return;
}

void * mavalloc_alloc( size_t size )
{  
  memory_node* node = NULL;
  int size_mem = mavalloc_size();
  if(allocation_algorithm != NEXT_FIT){
      node = head_of_memory;
  }
  else if(allocation_algorithm == NEXT_FIT){
      node = previous_node;
  }
  else{
      printf("Error: Unknown Allocation Algorithm!\n");
      exit(0);
  }

  size_t aligned_size = ALIGN4(size);
  

  if(aligned_size == max_size){
    node->node_type == USED;
    //printf("\nequal? %d", aligned_size == max_size);
    return node;
  }
  else if(allocation_algorithm == FIRST_FIT)
  {
      while(node != NULL)
      {    
          if(node->size >= aligned_size && node->node_type==FREE)
          {
              size_t leftover_size = 0;

              node->node_type = USED;
              leftover_size = node->size - aligned_size;
              node->size = aligned_size;
              
              if(leftover_size > 0)
              {
                  memory_node* previous_next = node->next;
                  memory_node* previous_prev = node->prev;
                  memory_node* leftover_node = newNode(
                                    FREE, 
                                    (size_t*)node->start_address+aligned_size, 
                                    leftover_size,
                                    previous_next, 
                                    previous_prev
                                    );
                  node->next = leftover_node;
              }
              previous_node = node;
              return (void*)node->start_address;
          }
          node = node->next;
      }
  }

  else if(allocation_algorithm == NEXT_FIT)
  {
    if(node->next == NULL){
      node = head_of_memory;
    }
    while(node != NULL)
      {    
          if(node->size >= aligned_size && node->node_type==FREE)
          {
              size_t leftover_size = 0;

              node->node_type = USED;
              leftover_size = node->size - aligned_size;
              node->size = aligned_size;
              
              if(leftover_size > 0)
              {
                  memory_node* previous_next = node->next;
                  memory_node* previous_prev = node->prev;
                  memory_node* leftover_node = newNode(
                                    FREE, 
                                    (size_t*)node->start_address+aligned_size, 
                                    leftover_size,
                                    previous_next, 
                                    previous_prev
                                    );
                  node->next = leftover_node;
              }
              previous_node = node;
              return (void*)node->start_address;
          }
          node = node->next;
      }

  }

  else if(allocation_algorithm == WORST_FIT)
  {
    while(node != NULL)
      {    
          if(node->size >= aligned_size && node->node_type==FREE)
          {
              size_t leftover_size = 0;

              node->node_type = USED;
              leftover_size = node->size - aligned_size;
              node->size = aligned_size;
              
              if(leftover_size > 0)
              {
                  memory_node* previous_next = node->next;
                  memory_node* previous_prev = node->prev;
                  memory_node* leftover_node = newNode(
                                    FREE, 
                                    (size_t*)node->start_address+aligned_size, 
                                    leftover_size,
                                    previous_next, 
                                    previous_prev
                                    );
                  node->next = leftover_node;
              }
              previous_node = node;
              return (void*)node->start_address;
          }
          node = node->next;
      }
  }

  memory_node* best_node = NULL;
  size_t min_size = max_size;
  if(allocation_algorithm == BEST_FIT)
  {
    //printf("\nequal? %d", aligned_size == max_size);
    int size_mem = mavalloc_size();
    while(node != NULL)
    {
      size_t size_check = node -> size - size;

      if(node -> node_type == FREE && size_check< min_size && node->size >= aligned_size)
      {
        best_node = node;
        min_size = node -> size - size;
      }
      node = node -> next;
    }
    node = head_of_memory;
    while(node != NULL)
      {    
          if(node == best_node)
          {
              size_t leftover_size = 0;

              node->node_type = USED;
              leftover_size = node->size - aligned_size;
              node->size = aligned_size;
              
              if(leftover_size > 0)
              {
                  memory_node* previous_next = node->next;
                  memory_node* previous_prev = node->prev;
                  memory_node* leftover_node = newNode(
                                    FREE, 
                                    (size_t*)node->start_address+aligned_size, 
                                    leftover_size,
                                    previous_next, 
                                    previous_prev
                                    );
                  node->next = leftover_node;
              }
              previous_node = node;
              return (void*)node->start_address;
          }
          node = node->next;
      }
    return NULL;
  }
  


  return NULL;
}

void mavalloc_free( void * ptr )
{
  memory_node* target = head_of_memory;

  //Find the block address in linked list and free it
  while(target != NULL){
    if(target->start_address == ptr )
    {
      target->node_type = FREE;
      break;
    }
    target = target->next;
  }

  //Reset target for another loop
  target = head_of_memory;

  //
  while(target != NULL)
  {
    //If the next node is free, this block adds the sizes and deletes the next node.
    if(target->next && target->node_type == FREE && target->next ->node_type == FREE  )
    {
      memory_node* previous = target->next;
      target->size = target->size + target -> next->size;
      target->next = target->next->next;
      free(previous);
      //Ensures that if a recently coelesced hole can be coelesced to another subsequent hole
      continue;
    }
    target = target -> next;
  }
  return;
}

int mavalloc_size( )
{
  int number_of_nodes = 0;

  //Iterative loop that counts each node untill it hits the end of the list
  memory_node* head = head_of_memory;
  while(head != NULL)
  {
    head = head->next;
    number_of_nodes++;
  }
  //printf("\nSize = %d\n",number_of_nodes);
  //print_memory();
  return number_of_nodes;
}

void print_memory( )
{
  int node_counter = 0;
  //Iterative loop that counts each node untill it hits the end of the list
  memory_node* head = head_of_memory;
  //Basically visuallizes the state of the arena and it's holes
  printf("\nArena Size: %zu\n", max_size);
  while(head != NULL)
  {
    printf("\n---------\nNode[%d]: "
            "Address = %p, "
            "size = %zu, "
            "node_type = %d, "
            "start->%p, "
            "next->%p",
            node_counter, head, head->size, head->node_type, head->start_address, head->next
    );
    head = head->next;
    node_counter++;
  }

  return;
}

