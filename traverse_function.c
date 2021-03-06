#include <stddef.h>
#include<linux/slab.h>
//#include <linux/vmalloc.h>
#include "traverse_function.h"
//#include <linux/module.h>

int inside_the_page(mchunkptr p ,size_t VpageNO) 
{
  // return true if the chunk is inside the virtual page
  if (((int64_t)p >> 12) == VpageNO)
  {
    return 1;
  } else 
  {
    return 0;
  }
}

void add_free_chunk(free_chunk_info_ptr *free_chunk_info_head_ptr, free_chunk_info_ptr *free_chunk_info_current_ptr, mchunkptr *p_ptr)
{
  if (free_chunk_info_head_ptr == NULL)
  {
    //*free_chunk_info_head_ptr = *free_chunk_info_current_ptr = (free_chunk_info_ptr)kmalloc(sizeof(struct free_chunk_info), GFP_KERNEL);
    //*free_chunk_info_head_ptr = *free_chunk_info_current_ptr = (free_chunk_info_ptr)malloc(sizeof(free_chunk_info_t));
  } else
  {
    //(*free_chunk_info_current_ptr)->next = (free_chunk_info_ptr)kmalloc(sizeof(struct free_chunk_info), GFP_KERNEL);
    //(*free_chunk_info_current_ptr)->next = (free_chunk_info_ptr)malloc(sizeof(free_chunk_info_t));
    //(*free_chunk_info_current_ptr) = (*free_chunk_info_current_ptr)->next;
  }
  (*free_chunk_info_current_ptr)->start = *p_ptr;
  (*free_chunk_info_current_ptr)->len = chunksize(*p_ptr); // size of the data chunk (including header and mem)
  //(*free_chunk_info_current_ptr)->next = NULL;
}

/* 
   get the free chunk information in current virtual page, VpageNO.
   input: 
    arena_start_ptr: pointer to the arena containing the virtual page VpageNO.
    VpageNO: Virtual Page NO
    len: number of the free chunks 
   output:
    pointer to the free chunk list 
 */

free_chunk_info_ptr traverse (void* arena_start_ptr, size_t VpageNO, size_t* len) 
{
  mstate av = (mstate)arena_start_ptr;
  free_chunk_info_ptr free_chunk_info_head = NULL;
  free_chunk_info_ptr free_chunk_info_current = NULL;
  mchunkptr p, b;
  mchunkptr top_addr = av->top;
  int i;

  // traverse fast bins 
  for (i = 0; i < NFASTBINS; ++i) { 
    for (p = fastbin (av, i); p != 0; p = p->fd) 
    {
      if (inside_the_page(p, VpageNO))
      {
        add_free_chunk(&free_chunk_info_head, &free_chunk_info_current, &p);
      }
    }
  }

  // traverse regular bins 
  for (i = 1; i < NBINS; ++i) {
    b = bin_at (av, i); //
    for (p = (b)->bk; p != b; p = p->bk) { 
      //address: p  size: chunksize(p)
      if (inside_the_page(p, VpageNO))
      {
        add_free_chunk(&free_chunk_info_head, &free_chunk_info_current, &p);
      }
    }
  }

  //get top chunk
  if (inside_the_page(top_addr, VpageNO))
  {
      add_free_chunk(&free_chunk_info_head, &free_chunk_info_current, &top_addr);
  }
  return free_chunk_info_head;
}

