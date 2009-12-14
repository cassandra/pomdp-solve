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
 *    $RCSfile: template-test.c,v $
 *    $Source: /u/cvs/proj/pomdp-solve/testing/template-test.c,v $
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


#include <double-vector.h>

#include "global-test.h"

/************************************************************/
int
TEST_DV_zzz( zzz )
{

  int error_count = 0;
  char msg[1024];

  if ( zzz )
    {
	 sprintf( msg, "Size %d list failed", t_list_size );
	 Error( msg );
    }

  return error_count;

} /* TEST_DV_zzz */

/************************************************************/
int
DV_unitTest()
{
  int error_count = 0;

  StartUnitTest( "double-vector.c" );

  error_count += TEST_DV_zzz();

  EndUnitTest( "double-vector.c" );

  return error_count;

} /* DV_unitTests */
