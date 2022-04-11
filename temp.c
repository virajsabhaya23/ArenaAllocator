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
void* start_address = NULL;
enum ALGORITHM allocation_algorithm = FIRST_FIT;

memory_node* newNode(enum TYPE mem_type, void* address, size_t size, memory_node* next, memory_node* prev){

  memory_node* new_node = (memory_node*)malloc(sizeof(memory_node));
  new_node -> node_type = mem_type;
  new_node -> start_address = address;
  new_node -> size = size;
  new_node -> next = next;
  new_node -> prev = prev;

  return new_node;
}

int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  start_address = malloc(ALIGN4(size));
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
  size_t requested_size = ALIGN4(size);
  
  memory_node* node = NULL;

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

  if(allocation_algorithm == FIRST_FIT){
      while(node != NULL){
          
          if(node->size >= aligned_size && node->node_type==FREE){
              size_t leftover_size = 0;

              node->node_type = USED;
              leftover_size = node->size - aligned_size;
              node->size = aligned_size;

              if(leftover_size > 0){
                  memory_node* previous_next = node->next;
                  memory_node* previous_prev = node->prev;
                  memory_node* leftover_node = newNode(
                                    FREE, 
                                    (size_t*)node->start_address+aligned_size, 
                                    leftover_size,
                                    previous_next, 
                                    previous_prev);
                  node->next = leftover_node;
              }
              previous_node = node;
              return (void*)node->start_address;
          }

          node = node->next;
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

  else if(allocation_algorithm == BEST_FIT)
  {
    memory_node* best_node = NULL;
    size_t min_size = aligned_size;
    //printf("\nAllocating %zu to Min_size: %zu\n", aligned_size,min_size);
    while(node != NULL){
      if(node->node_type == FREE && node->size >= aligned_size && node->size <= min_size){
        min_size = node->size;
      }
      node = node->next;
    }
    //printf("\nAllocating %zu to Min_size: %zu\n", aligned_size,min_size);
    node = head_of_memory;
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
  }
  else return NULL;
}

void mavalloc_free( void * ptr )
{
  if(ptr==NULL) return;
  
  memory_node* curr = head_of_memory;
  memory_node* target = NULL;
  
  while(curr != NULL){
      if(curr->start_address == ptr){
        target = curr;
        target->node_type = FREE;
        break;
      }
      curr = curr->next;
  }

  if(target->prev != NULL && target->prev->node_type == FREE){
    memory_node* previous_temp = target->prev;
    if(previous_temp->prev != NULL ){
      previous_temp->size = previous_temp->size + target->size;
      previous_temp->next = target->next;
      //free(target);
      target = previous_temp;
    }
    printf("\n");
  }
  if(target->next != NULL && target->next->node_type == FREE){
    target->size = target->size + target->next->size;
    target->next = target->next->next;
    //free(target->next);
  }

  //print_memory();
  return;
}

int mavalloc_size( )
{
  int number_of_nodes = 0;

  //Iterative loop that counts each node untill it hits the end of the list
  memory_node* head = head_of_memory;
  while(head != NULL){
    head = head->next;
    number_of_nodes++;
  }

  return number_of_nodes;
}
