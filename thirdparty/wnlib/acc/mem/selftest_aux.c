/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#include "wnlib.h"

#ifndef WN_WINDOWS
# include <unistd.h>
#endif

#include "wnmem.h"
#include "wnasrt.h"
#include "wnmemb.h"
/* note wnmlc.h is included further down, after really alloc & really free */

#define LO_LENGTH(a) (sizeof(a) / sizeof(a[0]))


local void *lo_really_malloc(size_t size)
{
  return malloc(size);
}
local void lo_really_free(void *p)
{
  free(p);
}

#include "wnmlc.h"


#define LO_SUBSTITUTE_ALLOC_BUF_SIZE  (100 * 1000 * 1000)
static char *lo_substitute_alloc_buf = NULL;
static char *lo_ptr_to_next_alloc = NULL;

local void *lo_substitute_alloc(size_t size)
{
  /* long l; -- unused - bc041220 */
  size_t *ps;
  char *ret = lo_ptr_to_next_alloc + 8;

  wn_assert(sizeof(size_t) <= 8);
  wn_assert(!((long) lo_ptr_to_next_alloc & 7));

  size += (8 - (size & 7)) & 7;
  wn_assert(!(size & 7));

  lo_ptr_to_next_alloc += size + 8;
  wn_assert(lo_ptr_to_next_alloc  <
  /**/		lo_substitute_alloc_buf + LO_SUBSTITUTE_ALLOC_BUF_SIZE);

  ps = (size_t *) (long) ret;

  ps[-1] = size;

  return (void *) ps;
} /* lo_substitute_alloc */


local void lo_substitute_free(ptr p)
{
  size_t *ps;
  long l;
  size_t size;

  l = (long) p;

  wn_assert(!(l & 7));

  wn_assert(p);
  ps = (size_t *) p;
  size = ps[-1];

  wn_assert(size);

  wn_memset((ptr) ((char *) ps - 8), (char) 0xa3, size + 8);
} /* lo_substitute_free */


local void lo_init_substitute_alloc(void)
{
  /*     note this malloc has to come before wnmlc.h is included and
  ** redefines malloc */

  lo_substitute_alloc_buf = (char *)
  /**/			lo_really_malloc(LO_SUBSTITUTE_ALLOC_BUF_SIZE);
  lo_ptr_to_next_alloc = lo_substitute_alloc_buf;
  wn_set_system_memory_alloc_func(&lo_substitute_alloc);
  wn_set_system_memory_free_func( &lo_substitute_free);
} /* lo_init_substitute_alloc */


/* return true iff all bytes in the block of memory are set to c */
local bool lo_memtest(void *mem, char c, int n)
{
  int i;
  char *pmem = (char *) mem;

  for (i = 0;  i < n;  ++i)
  {
    if (pmem[i] != c)
    {
      return FALSE;
    }
  }

  return TRUE;
} /* lo_memtest */


local void test_f_malloc_stuff(void)
{
  ptr mem_array[1000];
  int i;

  wn_assert(wn_mem_used() == 0);
  wn_allmem_verify();

  for(i=0;i<1000;++i)
  {
    mem_array[i] = wn_f_malloc(i);
    wn_allmem_verify();  /* tmp */
  }

  wn_assert(wn_mem_used() > 0);
  wn_allmem_verify();

  for(i=0;i<1000;++i)
  {
    wn_f_free(mem_array[i]);
  }

  wn_assert(wn_mem_used() == 0);
  wn_allmem_verify();
}


local void test_main_stuff(void)
{
  ptr mem_array[1000];
  int i,start_mem_used;

  wn_assert(wn_mem_used() == 0);
  wn_allmem_verify();

  wn_gpmake("general_free");
  wn_gplabel("main_stuff");

  start_mem_used = wn_mem_used();

  for(i=0;i<1000;++i)
  {
    mem_array[i] = wn_alloc(i);
  }

  wn_allmem_verify();

  for(i=0;i<1000;++i)
  {
    wn_free(mem_array[i]);
  }

  wn_assert(wn_mem_used() == start_mem_used);
  wn_allmem_verify();

  wn_gpfree();

  wn_assert(wn_mem_used() == 0);
  wn_allmem_verify();
}


local void test_mem_detailed(void)
{
  ptr mem_array1[1000], mem_array2[1000], mem_array3[1000];
  int start_mem_used;
  int i, j, k;
  char *group_types[] = { "general_free", "no_free",
  /**/			  "general_free 200", "no_free 200" };
# define FREEABLE_GROUP(k)  (!(k & 1))
  wn_memgp child3, child4;

  wn_assert(0 == wn_mem_used());

  for (k = 0;  k < (int) (sizeof(group_types) / sizeof(group_types[0]));  ++k)
  {
    wn_gpmake(group_types[k]);
      wn_gplabel(group_types[k]);
      wn_allmem_verify();

      start_mem_used = wn_mem_used();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 71) % 1000;

	mem_array1[j] = wn_alloc(j);
	wn_memset(mem_array1[j], (char) (j % 253), j);
      }

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 71) % 1000;

	wn_assert(wn_memtest(mem_array1[j], (char) (j % 253), j));
      }

      wn_allmem_verify();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 29) % 1000;

	mem_array2[j] = wn_alloc(j);
	wn_memset(mem_array2[j], (char) (j % 221), j);
      }

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 29) % 1000;

	wn_assert(wn_memtest(mem_array2[j], (char) (j % 221), j));
      }

      wn_allmem_verify();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 31) % 1000;

	wn_free(mem_array1[j]);
	mem_array1[j] = NULL;
      }

      wn_allmem_verify();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 29) % 1000;

	wn_assert(!mem_array1[j]);
	mem_array1[j] = wn_alloc(j);
	wn_memset(mem_array1[j], (char) (j % 193), j);
      }

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 29) % 1000;

	wn_assert(wn_memtest(mem_array1[j], (char) (j % 193), j));
      }

      wn_allmem_verify();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 53) % 1000;

	wn_free(mem_array1[j]);
	mem_array1[j] = NULL;
      }

      wn_allmem_verify();

      for (i = 0;  i < 1000;  ++i)
      {
	j = (i * 7) % 1000;

	wn_free(mem_array2[j]);
	mem_array2[j] = NULL;
      }

      wn_allmem_verify();

      wn_assert(!FREEABLE_GROUP(k)  ||  start_mem_used == wn_mem_used());

      /* child group */
      wn_gpmake("no_free");
        wn_gplabel("child");
        wn_allmem_verify();

	for (i = 0;  i < 1000;  ++i)
	{
	  j = (i * 29) % 1000;

	  wn_assert(!mem_array1[j]);
	  mem_array1[j] = wn_alloc(j);
	  wn_memset(mem_array1[j], (char) (j % 157), j);
	}

	for (i = 0;  i < 1000;  ++i)
	{
	  j = (i * 29) % 1000;

	  wn_assert(wn_memtest(mem_array1[j], (char) (j % 157), j));
	}

	wn_allmem_verify();

	/* we really oughta test zalloc */
	{
	  char *pc;

	  pc = (char *) wn_zalloc(32);
	  wn_assert(wn_memtest((ptr) pc, 0, 32));
	  for (i = 0;  i < 32;  ++i)
	  {
	    wn_assert(!pc[i]);
	  }
	}

      wn_gppop();
      wn_allmem_verify();

      /* second child group */
      wn_gpmake("no_free");
        wn_gplabel("child2");
        wn_allmem_verify();

	for (i = 0;  i < 1000;  ++i)
	{
	  j = (i * 37) % 1000;

	  mem_array3[j] = wn_alloc(j);
	  wn_memset(mem_array3[j], j % 151, j);
	}

	for (i = 0;  i < 1000;  ++i)
	{
	  wn_memtest(mem_array3[i], i % 151, i);
	}

      wn_gppop();

      wn_gpmake("no_free");
        wn_gplabel("child1");
	child3 = wn_curgp();

	wn_alloc(10000);
      wn_gppop();

      wn_gpmake("no_free");
        wn_gplabel("child1");
	child4 = wn_curgp();

	wn_alloc(10000);
      wn_gppop();

      wn_gppush(child4);
      wn_gpfree();

      wn_gppush(child3);
      wn_gpfree();

    /* free the parent */
    wn_gpfree();

    wn_allmem_verify();

    wn_assert(0 == wn_mem_used());
  } /* for k */
} /* test_mem_detailed */


static void lo_test_rand(void)
{
  ptr array[1000];
  int  lengths[1000], vals[1000];
  /* int size, block_count; - unused - bc041220 */
  int i, j;

  memset(array,   0, sizeof(array));
  memset(lengths, 0, sizeof(lengths));
  memset(vals,    0, sizeof(vals));

  wn_gpmake("general_free");
    wn_gplabel("rand_test_group");

    wn_allmem_verify();

    for (i = 0;  i < 10000;  ++i)
    {
      j = rand() % LO_LENGTH(array);
      j = abs(j);

      if (!array[j])
      {
        lengths[j] = rand() % (LO_LENGTH(lengths) / 2);
        lengths[j] = abs(lengths[j]);

	vals[j]    = rand() & 0xff;

        array[j] = wn_alloc(lengths[j]);
        memset(array[j], vals[j], lengths[j]);
      }
      else
      {
        lo_memtest(array[j], vals[j], lengths[j]);
        wn_free(array[j]);
        array[j] = NULL;
        lengths[j] = 0;
      }

      if (!(i & 0xf))
      {
	wn_allmem_verify();
      }
    }

    wn_allmem_verify();

    for (i = LO_LENGTH(array)-1;  i >= 0;  --i)
    {
      if (array[i])
      {
        lo_memtest(array[i], vals[i], lengths[i]);
        wn_free(array[i]);
        array[i] = NULL;
        lengths[i] = 0;
      }

      if (!(i & 0xf))
      {
	wn_allmem_verify();
      }
    }

    wn_allmem_verify();

  wn_gpfree();

  wn_allmem_verify();
} /* lo_test_rand */


/* deliberately leak some memory to see if purify catches us */
void lo_test_leaks(void)
{
  printf("In lo_test_leaks(), called by main()\n\n");

  printf("In default group\n");

  printf("Leaking 40 bytes at line %d\n", __LINE__);	(void) wn_alloc(40);
  printf("Leaking 50 bytes at line %d\n", __LINE__);	(void) wn_alloc(50);
  printf("Leaking 60 bytes at line %d\n", __LINE__);	(void) wn_alloc(60);
  printf("Leaking 70 bytes at line %d\n", __LINE__);	(void) wn_alloc(70);

  printf("In group alpha\n");
  wn_gpmake("general_free");

    printf("Leaking 100 bytes at line %d\n", __LINE__);	(void) wn_alloc(100);
    printf("Leaking 110 bytes at line %d\n", __LINE__);	(void) wn_alloc(110);
    printf("Leaking 120 bytes at line %d\n", __LINE__);	(void) wn_alloc(120);
    printf("Leaking 130 bytes at line %d\n", __LINE__);	(void) wn_alloc(130);
  wn_gppop();	/* not free */
} /* lo_test_leaks */


static void lo_test_free_measurement(void) {
  void *p1, *p2;

  wn_gpmake("general_free");
    p1 = wn_alloc(1000);
    p2 = wn_alloc(1000);

    wn_free(p1);
    wn_free(p2);

    wn_assert(2000 == wn_amount_of_free_memory_in_group(wn_curgp()));
  wn_gpfree();
  wn_gpmake("general_free");
    p1 = wn_alloc(30);
    p2 = wn_alloc(30);

    wn_free(p1);
    wn_free(p2);

    wn_assert(60  <= wn_amount_of_free_memory_in_group(wn_curgp()));
    wn_assert(200 >= wn_amount_of_free_memory_in_group(wn_curgp()));
  wn_gpfree();
} /* lo_test_free_measurement */


int main(int argc, char **argv)
{
  int i;
  bool fill, pad, mbtree, substitute, leak;
  extern void wn_mbtree_test(void);

  fill =
  pad  =
  mbtree =
  substitute =
  leak = FALSE;

  for (i = 1;  i < argc;  ++i)
  {
    if (!strcmp("-fill", argv[i]))
    {
      wn_gp_fill();
      fill = TRUE;
    }
    else if (!strcmp("-pad", argv[i]))
    {
      wn_gp_pad();
      pad = TRUE;
    }
    else if (!strcmp("-mbtree", argv[i]))
    {
      mbtree = TRUE;
    }
    else if (!strcmp("-substitute", argv[i]))
    {
      lo_init_substitute_alloc();
      substitute = TRUE;
    }
    else if (!strcmp("-leak", argv[i]))
    {
      leak = TRUE;
    }
    else
    {
      fprintf(stderr, "Unrecognized option \"%s\"\n", argv[i]);
      wn_assert_notreached();
    }
  }

  if (mbtree)
  {
# if !defined(WN_NO_BTREE_TEST)
    wn_mbtree_test();
# endif

    return 0;
  }


  if (leak)
  {
    lo_test_leaks();
    return 0;
  }

  fprintf(stderr, "testing memory stuff%s%s%s ...\n", fill ? " -fill" : "",
  /**/		pad  ? " -pad"  : "", substitute ? " -substitute" : "");

  test_f_malloc_stuff();
  test_main_stuff();
  test_mem_detailed();
  lo_test_rand();
  if (!pad) {
    lo_test_free_measurement();
  }

  if (getenv("WNLIB_SELFTEST_PROFILE_GROUPS_LEFT"))
  {
    wn_print_all_groups_mem_used();
  }

  if (lo_substitute_alloc_buf)
  {
    lo_really_free(lo_substitute_alloc_buf);
    lo_substitute_alloc_buf = NULL;
  }

  fprintf(stderr, "  ok!!!!!!\n");

  return(0);
}
