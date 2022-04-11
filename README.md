# Arena Allocator

### <u>Description</u> : 
- In the glibc <code>malloc()</code> implementation <code>malloc()</code> either invokes either <code>brk()</code> or <code>mmap ()</code>
syscall to obtain memory. 

- While this implementation works fine for most applications applications with performance 
needs, e.g. games, aircraft, spacecraft, etc. can not afford to make a system call every time 
an allocation is needed.  In these cases a large memory pool or arena is allocated on 
application startup.  The application then manages memory on its own by implementing 
its own allocators to handle requests.  

 - <i>This code implements a memory arena allocator that supports First Fit, Best Fit, Worst Fit 
and Next Fit algorithms.</i>  

--- 

### <u>Memory Allocator API</u> : 
<i>Contains the following four functions</i> : 
- <b><code>int mavalloc_init( size_t size, enum ALGORITHM algorithm )</code></b>
  - This function will use malloc to allocate a pool of memory that is size bytes big. If 
the size parameter is less than zero then return -1.  If allocation fails return -1.  If the 
allocation succeeds return 0.  This is the only <code>malloc()</code> your code will call. 
  - Size must be 4-byte aligned.  You are provided a <mark><span style="background-color:#330066">macro ALIGN4</mark> to perform this alignment. 
  - The second parameter will set which algorithm you will use to allocate memory from 
your pool.  The enumerated value is:<br>
  - ```
    enum ALGORITHM 
    { 
    FIRST_FIT = 0, 
    NEXT_FIT, 
    BEST_FIT, 
    WORST_FIT 
    };
    ``` 
- <b><code>void *mavalloc_alloc( size_t size )</code></b>
  - This function will allocate size bytes from your preallocated memory arena using the heap allocation algorithm that was specified during <code>mavalloc_init</code>.  This function returns a pointer to the memory on success and NULL on failure. 
- <b><code>void mavalloc_free( void * pointer )</code></b>
  - This function will free the block pointed to by the pointer back to your preallocated memory arena.  This function returns no value.  If there are two consecutive blocks 
free then combine (coalesce) them.
- <b><code>void mavalloc_destroy( )</code></b>
  - This function will free the allocated arena and empty the linked list 
- <b><code>int mavalloc_size( )</code></b> 
  - This function will return the number of nodes in the memory area 