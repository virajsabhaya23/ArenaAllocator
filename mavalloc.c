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

typedef struct memory_nodes {
  char node_type;
  int start;
  int length;
  struct memory_nodes* next;
} memory_nodes;



int mavalloc_init( size_t size, enum ALGORITHM algorithm )
{
  //You can't allocate a size less than 0, code must break in this mistake
  if( size < 0){
    return -1;
  }
  memory_nodes* new_node = (memory_nodes*)malloc(sizeof(memory_nodes));
  
  /*
  Can't continue with program if the main initialization of the linked list failed, 
  code must break
  */
  if(new_node){
    return -1;
  }
  return 0;
}


void mavalloc_destroy( )
{
  return;
}

void * mavalloc_alloc( size_t size )
{
  // only return NULL on failure
  return NULL;
}

void mavalloc_free( void * ptr )
{
  return;
}

int mavalloc_size( )
{
  int number_of_nodes = 0;

  return number_of_nodes;
}
