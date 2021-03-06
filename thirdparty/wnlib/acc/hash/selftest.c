/****************************************************************************

COPYRIGHT NOTICE:

  The source code in this directory is provided free of
  charge to anyone who wants it.  It is in the public domain
  and therefore may be used by anybody for any purpose.  It
  is provided "AS IS" with no warranty of any kind
  whatsoever.  For further details see the README files in
  the wnlib parent directory.

AUTHOR:

  Will Naylor

****************************************************************************/
#include <stdlib.h>
#include <stdio.h>

#include "wnlib.h"
#include "wnasrt.h"
#include "wnmemb.h"

#include "wnhtbl.h"


#if 0
/* int hash tables deprecated */
local void lo_old_hash_test(void)
{
  wn_htab table;
  int i /*,j*/;
  int pad_before;
  int num;
  int pad_after;
  int ugly_int;
  bool success;

  wn_memset(&ugly_int, (char) 0x81, sizeof(ugly_int));
  pad_before = pad_after = ugly_int;

  wn_gpmake("no_free");

    wn_mkinthtab(&table);

    for(i=0;i<10000;++i)
    {
      success = wn_hins((ptr)(i+1),table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(!(success));
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=5000;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=2500;i<7500;++i)
    {
      success = wn_hfins((ptr)i,table,(ptr)i);
      wn_assert(success);
    }

    for(i=2500;i<7500;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=0;i<2500;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=7500;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    wn_hverify(table);

    for(i=2500;i<10000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    wn_assert(wn_hcount(table) == 0);

    for(i=0;i<10000;++i)
    {
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
    }

  wn_gpfree();
} /* lo_old_hash_test */


local void lo_new_hash_test(void)
{
  wn_htab table;
  int i/*,j*/;
  int pad_before;
  int num;
  int pad_after;
  int ugly_int;
  bool success;

  wn_memset(&ugly_int, (char) 0x81, sizeof(ugly_int));
  pad_before = pad_after = ugly_int;

  wn_gpmake("no_free");

    wn_mkinthtab(&table);

    for(i=0;i<10000;i+=10)
    {
      success = wn_hins((ptr)(i+3),table,(ptr)i);
      wn_assert(success);
    }

    wn_assert(wn_hcount(table) == 1000);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      if (i % 10)
      {
	wn_assert(!success);
      }
      else
      {
        wn_assert(success);
	wn_assert(num == i+3);
      }
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    wn_hverify(table);

    for(i=0;i<10000;i+=25)
    {
      success = wn_hdel(table,(ptr)i);
      if (i % 50)
      {
        wn_assert(!success);
      }
      else
      {
        wn_assert(success);
      }
    }

    wn_assert(wn_hcount(table) == 800);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      success = wn_hfins((ptr)(i+5),table,(ptr)i);
      wn_assert(success);
    }

    wn_assert(wn_hcount(table) == 10000);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+5);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    wn_hverify(table);

    for(i=0;i<10000;i+=3)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success == !!(i % 3));
      wn_assert(!success || num == i+5);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=0;i<10000-1;i+=3)
    {
      success = wn_hdel(table,(ptr)(i+1));
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success == !((i-2) % 3));
      wn_assert(!success || num == i+5);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    for(i=0;i<10000-2;i+=3)
    {
      success = wn_hdel(table,(ptr)(i+2));
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_int;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!success);
      wn_assert(pad_before == ugly_int  &&  pad_after == ugly_int);
    }

    wn_assert(wn_hcount(table) == 0);
    wn_hverify(table);

  wn_gpfree();
} /* lo_new_hash_test */
#endif /* 0 */


local void lo_old_long_hash_test(void)
{
  wn_htab table;
  int i/*,j*/;
  long pad_before;
  long num;
  long pad_after;
  long ugly_long;
  bool success;

  wn_memset(&ugly_long, (char) 0x81, sizeof(ugly_long));
  pad_before = pad_after = ugly_long;

  wn_gpmake("no_free");

    wn_mklonghtab(&table);

    for(i=0;i<10000;++i)
    {
      success = wn_hins((ptr)(i+1),table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(!(success));
    }

    wn_hverify(table);

    for(i=0;i<5000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=5000;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=2500;i<7500;++i)
    {
      success = wn_hfins((ptr)i,table,(ptr)i);
      wn_assert(success);
    }

    for(i=2500;i<7500;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=0;i<2500;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=7500;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+1);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    wn_hverify(table);

    for(i=2500;i<10000;++i)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    wn_hverify(table);

    wn_assert(wn_hcount(table) == 0);

    for(i=0;i<10000;++i)
    {
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!(success));
    }

  wn_gpfree();
} /* lo_old_long_hash_test */


local void lo_new_long_hash_test(void)
{
  wn_htab table;
  int i/*,j*/;
  long pad_before;
  long num;
  long pad_after;
  long ugly_long;
  bool success;

  wn_memset(&ugly_long, (char) 0x81, sizeof(ugly_long));
  pad_before = pad_after = ugly_long;

  wn_gpmake("no_free");

    wn_mklonghtab(&table);

    for(i=0;i<10000;i+=10)
    {
      success = wn_hins((ptr)(i+3),table,(ptr)i);
      wn_assert(success);
    }

    wn_assert(wn_hcount(table) == 1000);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      if (i % 10)
      {
	wn_assert(!success);
      }
      else
      {
        wn_assert(success);
	wn_assert(num == i+3);
      }
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    wn_hverify(table);

    for(i=0;i<10000;i+=25)
    {
      success = wn_hdel(table,(ptr)i);
      if (i % 50)
      {
        wn_assert(!success);
      }
      else
      {
        wn_assert(success);
      }
    }

    wn_assert(wn_hcount(table) == 800);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      success = wn_hfins((ptr)(i+5),table,(ptr)i);
      wn_assert(success);
    }

    wn_assert(wn_hcount(table) == 10000);
    wn_hverify(table);

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success);
      wn_assert(num == i+5);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    wn_hverify(table);

    for(i=0;i<10000;i+=3)
    {
      success = wn_hdel(table,(ptr)i);
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success == !!(i % 3));
      wn_assert(!success || num == i+5);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=0;i<10000-1;i+=3)
    {
      success = wn_hdel(table,(ptr)(i+1));
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(success == !((i-2) % 3));
      wn_assert(!success || num == i+5);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    for(i=0;i<10000-2;i+=3)
    {
      success = wn_hdel(table,(ptr)(i+2));
      wn_assert(success);
    }

    for(i=0;i<10000;++i)
    {
      num = ugly_long;
      success = wn_hget((ptr *)&num,table,(ptr)i);
      wn_assert(!success);
      wn_assert(pad_before == ugly_long  &&  pad_after == ugly_long);
    }

    wn_assert(wn_hcount(table) == 0);
    wn_hverify(table);

  wn_gpfree();
} /* lo_new_long_hash_test */


local void lo_read_dict(void)
{
  FILE *fp;
  char buffer[1000];
  char *filename;
  wn_htab table;
  long int num;
  int sts;

  wn_gpmake("no_free");

    wn_mkstrhtab(&table);

    filename = getenv("WN_HASH_DICT");
    if (!filename) {
#     if defined(linux) || defined(__linux__)
	filename = "/usr/share/dict/words";
#     else 
	filename = "/usr/dict/words";
#     endif
    }	

    fp = fopen(filename, "r");
    if(fp == NULL)
    {
      printf("warning: cannot find dictionary file <%s>,\n",filename);
      printf("    skipping hash dictionary test\n",filename);
      printf("    If you know where a dictionary is, you can use it by\n");
      printf("    setting environment variable $WN_HASH_DICT to it.\n");
      goto finish;
    }

    do
    {
      sts = fscanf(fp, "%s", buffer);

      (void) wn_hins((ptr)0, table, (ptr) buffer);
    } while (sts == 1);

    fclose(fp);

    /*
    wn_assert(wn_hcount(table) > 30000);
    */

    if(wn_hcount(table) <= 20000)
    {
      printf("warning: dictionary contains less than 20000 words\n");
    }
    wn_hverify(table);

    wn_assert(wn_hget((ptr *)&num, table, (ptr) "here"));
    wn_assert(wn_hget((ptr *)&num, table, (ptr) "there"));
    wn_assert(wn_hget((ptr *)&num, table, (ptr) "and"));
    wn_assert(wn_hget((ptr *)&num, table, (ptr) "everywhere"));
    wn_assert(!wn_hget((ptr *)&num, table, (ptr) "missspelled"));
    wn_assert(!wn_hget((ptr *)&num, table, (ptr) "wwwords"));

finish:
  wn_gpfree();
} /* lo_read_dict */


int main(void)
{
  /* bool ints_bad = FALSE; */

  printf("testing hash...\n");

  /* hash table keys won't work if this is not true */
  wn_assert(sizeof(long int) <= sizeof(ptr));

  /*     "wn_hget(&ll, .." where ll is a long will not work right if this
  ** is not true */
  wn_assert(sizeof(long int) == sizeof(ptr));

  lo_old_long_hash_test();
  lo_new_long_hash_test();
  lo_read_dict();

#if 0
  /* int hash tables deprecated */

  if (sizeof(int) != sizeof(ptr))
  {
    fprintf(stderr, "    sizeof(int) != sizeof(ptr), don't expect test code\n"
    /**/ "    for int hash tables to work, set env var $WN_TEST_HASH_INTS\n"
    /**/ "    to run hash int tests\n"
    /**/ "        Note wn_mkinthtab() works, but just by calling\n"
    /**/ "    wn_mklonghtab() and the data values it stores are longs,\n"
    /**/ "    not ints\n");
    ints_bad = TRUE;
  }
  if (!ints_bad  ||  getenv("WN_TEST_HASH_INTS")) {
    lo_old_hash_test();
    lo_new_hash_test();
  }
#endif /* 0 */

  printf("  ok!!!!!!\n");

  return 0;
} /* main */
