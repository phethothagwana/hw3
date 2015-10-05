/*
 * mm_alloc.c
 *
 * Stub implementations of the mm_* routines. Remove this comment and provide
 * a summary of your allocator's design here.
 */
#include "mm_alloc.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
/* Your final implementation should comment out this macro. */
// #define MM_USE_STUBS 
#define ASSERT     assert
#define align4(x)  (((((x)-1)>>2)<<2)+4)
#define TRUE 1
#define FALSE 0
                                                    
s_block_ptr base = NULL;            
s_block_ptr last;       
static s_block_ptr find_block(size_t size)    
{  
  s_block_ptr r = base;
  for (; r != NULL; r = r->next)            
   {
      last = r;
      if (r->free && r->size >= size)         
          return r;                           
   }
  return NULL;                                
}

static s_block_ptr extend_heap(size_t s)                
{    
    
    s_block_ptr new_block;                    
    new_block = (s_block_ptr)sbrk(0); 
      int sb = (int)sbrk(s + S_BLOCK_SIZE);                         
    if (sb < 0)      
       {                                                       
        return (NULL);
      }                                                         
    new_block->size = s;                                  
    new_block->ptr  = new_block->data;                       
    new_block->free = TRUE;                                 
     
    if (base == NULL)                      
    {
        new_block->prev = new_block->prev = NULL; 
        base = new_block;                         
    }
    else 
    {
        ASSERT(last->next == NULL);               
        new_block->next = NULL;                    
        new_block->prev = last;                    
        last->next = new_block;                   
    }
    printf("%x %x %d\n", new_block, new_block->ptr, new_block->size); 
    return new_block;                             
}

void split_block(s_block_ptr r, size_t new_size)
{
    s_block_ptr new_block = NULL;              
    if (r->size >= new_size + S_BLOCK_SIZE + 4)
    {
        
        r->size = new_size;  

        
        new_block = (s_block_ptr)(r->data + new_size);
        new_block->size = r->size - new_size - S_BLOCK_SIZE;
        new_block->ptr = new_block->data;
        new_block->free = TRUE;

        
        new_block->next = r->next;
        new_block->prev = r;
        if (r->next)
            r->next->prev = new_block;
        r->next = new_block;
    }

    
}

static s_block_ptr fusion_block(s_block_ptr is)
{
    ASSERT(is->free == TRUE); 


    if (is->next && is->next->free) 
    {
      is->size = is->size + BLOCK_SIZE + is->next->size;
      if (is->next->next)
        is->next->next->prev = is;
      is->next = is->next->next; 		
    }

    return (is);
}


static s_block_ptr get_block(void *r)
{
    
    char *tmp;
    tmp = (char*)r;
    return (s_block_ptr)(tmp - BLOCK_SIZE);
}

static int is_valid_block_addr (void *r)
{
   s_block_ptr is = get_block(r);
   if (base)
   {
     if(r > base && r < sbrk(0))
	return is->ptr == r;
   }

   return FALSE;
}

static void copy_block(s_block_ptr src, s_block_ptr dst)
{
  int *sdata, *ddata; 
  size_t i;
  sdata = src->ptr;
  ddata = dst->ptr;
  for (i = 0; i * 4 < src->size && i * 4 < dst->size; i++)
    ddata[i] = sdata[i];
}


void* mm_malloc(size_t size)
{
#ifdef MM_USE_STUBS
    return calloc(1, size);
#else
 
    size_t s = align4(size);    
    s_block_ptr is;

    
    if (base == NULL) 
    {
       is = extend_heap(s);
       if (is == NULL)
          return NULL;
       base = is;
    }
    
    else 
    {
       
       is = find_block(s);
       if (is == NULL)
        {
          is = extend_heap(s);
          if (is == NULL)
            return NULL;
        }
       else
        {
          if (is->size - s >= S_BLOCK_SIZE + 4)
            split_block(is, s);
        }
    }
        
    is->free = FALSE;
    return is->ptr;
#endif
}

void* mm_realloc(void* ptr, size_t size)
{
#ifdef MM_USE_STUBS
    return realloc(ptr, size);
#else
    size_t s;
    void *newp;
    s_block_ptr is, new;
    
    if (ptr == NULL)
        return mm_malloc(size);
    
    if ((is=get_block(ptr)) != NULL)
    {
        s = align4(size);
        if (is->size >= s)
         {
           
           if (is->size - s >= (BLOCK_SIZE + 4))
             split_block(is, s);
         }
    }
    else 
    {
       
        if (is->next && is->next->free
            && (is->size + BLOCK_SIZE + is->next->size) >= s)
        {
           fusion_block(is);
           if (is->size - s >= BLOCK_SIZE + 4)
             split_block(is, s);
        }
        else
        {
           
           newp = mm_malloc(s);
           if (newp == NULL)
               return NULL;
           new = get_block(newp);
           copy_block(is, new);
           mm_free(is);
           return (newp);  
        }
        return (is);
    } 
    return (NULL);
#endif
}
void mm_free(void* ptr)
{
#ifdef MM_USE_STUBS
    free(ptr);
#else
    
    s_block_ptr is;
    
    if ((is=get_block(ptr)) != NULL)
    {
       is->free = TRUE;  
       
       if(is->prev && is->prev->free)
         fusion_block(is->prev);
      
       if(is->next)
         fusion_block(is);
       
       else
       {
          
          if (is->prev == NULL)
              base = NULL;
         
          else
              is->prev->next = NULL;     
          brk(is);
       }
    }
    
#endif
}
