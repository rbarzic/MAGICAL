/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Bill Chapman

****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "wnlib.h"
#include "wnasrt.h"
#include "wnmem.h"

#include "wnsll.h"


#define LENGTH(a) (sizeof(a)/sizeof((a)[0]))
#define CHECK_LIST(list, array) \
  check_list((list), (array), LENGTH(array), __LINE__)

local void check_list(wn_sll list, int *array, int array_len,
/**/						      int line_number)
{
  wn_sll sll;
  bool success = TRUE;
  int i;

  for (success = TRUE, sll = list, i = 0;  success  &&  sll  &&  i < array_len;
  /**/						sll = sll->next, ++i)
  {
    if ((long) sll->contents != array[i])
    {
      success = FALSE;
  } }

  /* check for different length lists that match as far as the short one */
  if (sll  ||  i != array_len)
  {
    success = FALSE;
  }

  if (!success)
  {
    fprintf(stderr, "CHECK_LIST failed on line %d of %s:\n", line_number,
    /**/							__FILE__);
    fprintf(stderr, "array: ");
    for (i = 0;  i < array_len;  ++i)
    {
      fprintf(stderr, "%d, ", array[i]);
    }
    fprintf(stderr, "\nlist:  ");
    for (sll = list;  sll;  sll = sll->next)
    {
      fprintf(stderr, "%ld, ", (long) sll->contents);
    }
    fprintf(stderr, "\n");

    /* no need to bomb out, just continue with test */
  }
} /* check_list_failed */


int main(void)
{
  wn_sll list, sll, sll2;
  long ll;
  int i;

  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  printf("testing lists ...\n");

  wn_gpmake("general_free");

    list = NULL;
    wn_sllins(&list, (ptr) 5);
    wn_sllins(&list, (ptr) 4);
    wn_sllins(&list, (ptr) 3);

    {
      int a[] = { 3, 4, 5 };
      CHECK_LIST(list, a);
    }

    wn_mksll(&sll);
    sll->contents = (ptr) 2;

    wn_slleins(&list, sll);

    {
      int a[] = { 2, 3, 4, 5 };
      CHECK_LIST(list, a);
    }

    wn_slldel(&list, (ptr) 3);

    {
      int a[] = { 2, 4, 5 };
      CHECK_LIST(list, a);
    }

    wn_sllins(&list->next, (ptr) 3);

    {
      int a[] = { 2, 3, 4, 5 };
      CHECK_LIST(list, a);
    }

    wn_slledel(&list->next->next);

    {
      int a[] = { 2, 3, 5 };
      CHECK_LIST(list, a);
    }

    wn_slleunlink(&list->next);

    {
      int a[] = { 2, 5 };
      CHECK_LIST(list, a);
    }

    wn_sllins(&list->next, (ptr) 4);
    wn_sllins(&list->next, (ptr) 3);

    {
      int a[] = { 2, 3, 4, 5 };
      CHECK_LIST(list, a);
    }

    wn_sllindex((ptr *) &ll, list, 2);
    wn_assert(4 == ll);

    wn_slleindex(&sll, list, 2);
    
    {
      int a[] = { 4, 5 };
      CHECK_LIST(sll, a);
    }

    wn_sllget_index(&i, list, (ptr) 4);
    wn_assert(2 == i);

    wn_sllrev(&list);

    {
      int a[] = { 5, 4, 3, 2 };
      CHECK_LIST(list, a);
    }

    sll = NULL;
    wn_sllins(&sll, (ptr) 6);
    wn_sllins(&sll, (ptr) 7);
    wn_sllins(&sll, (ptr) 8);

    wn_sllcpy(&sll2, sll);

    wn_sllcat(&sll, list);

    {
      int a[] = { 8, 7, 6, 5, 4, 3, 2 };
      CHECK_LIST(sll, a);
    }
    {
      int a[] = { 8, 7, 6 };
      CHECK_LIST(sll2, a);
    }
    {
      int a[] = { 5, 4, 3, 2 };
      CHECK_LIST(list, a);
    }

    sll = NULL;
    wn_sllins(&sll, (ptr) 6);
    wn_sllins(&sll, (ptr) 7);

    wn_sllins_sll(&list, sll);

    wn_assert(list == sll);
    {
      int a[] = { 7, 6, 5, 4, 3, 2 };
      CHECK_LIST(list, a);
    }

    wn_slllast(&sll, list);
    {
      int a[] = { 2 };
      CHECK_LIST(sll, a);
    }

    wn_assert(6 == wn_sllcount(list));

  wn_gpfree();

  printf("  ok!!!!!!\n");

  return(0);
} /* main */
