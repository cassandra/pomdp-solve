/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    params.c
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    July, 1998
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
 *    $RCSfile: global-test.h,v $
 *    $Source: /u/cvs/proj/pomdp-solve/testing/global-test.h,v $
 *    $Revision: 1.1 $
 *    $Date: 2005/02/02 23:21:31 $
 *  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
 *    2005 Anthony R. Cassandra
 *
 *    All Rights Reserved
 *                          
 *    Permission to use, copy, modify, and distribute this software and its
 *    documentation for any purpose other than its incorporation into a
 *    commercial product is hereby granted without fee, provided that the
 *    above copyright notice appear in all copies and that both that
 *    copyright notice and this permission notice appear in supporting
 *    documentation.
 * 
 *    ANTHONY CASSANDRA DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 *    INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR ANY
 *    PARTICULAR PURPOSE.  IN NO EVENT SHALL ANTHONY CASSANDRA BE LIABLE FOR
 *    ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *  </COPYRIGHT>
 *
 *</SOURCE_HEADER>
 */


#define StartUnitTest(NAME) printf( "----- Unit test for: %s\n", NAME )
  
#define EndUnitTest(NAME) { \
  if ( error_count < 1 ) \
     printf( "  PASSED\n" ); \
  else \
     printf( "  FAILED with %d errors.\n", error_count ); \
  }

#define Error(MSG) { \
  printf( "  ** ERROR ** File: %s function: %s, Line: %d\n     ", \
          __FILE__, __PRETTY_FUNCTION__, __LINE__ ); \
  fputs( MSG, stdout );							   \
  fputs( "\n", stdout );								   \
  error_count++; }



