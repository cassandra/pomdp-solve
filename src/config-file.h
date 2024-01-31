/*
 * pomdp-solve
 *
 * Copyright © 1998-2003, Anthony R. Cassandra
 * Copyright © 1994-1997, Brown University
 *
 * Licensed under the Creative Commons Attribution-NonCommercial 4.0 
 * International License (CC BY-NC 4.0).
 *
 * See the LICENSE.txt file for details.
 */

/*
  Module: CF

  A very simple configuration file reader and writer. Uses an
  associative array to store the key-value pairs.
 */

#ifndef CONFIG_FILE_H
#define CONFIG_FILE_H

#include "associative-array.h"

/*******************************************************/
/* Private Data */
/*******************************************************/

#define MAX_CFG_FILE_STRING_LEN     100
#define MAX_CFG_FILE_PARAMS         256

/* Parse sates for parsing configuration file. See CF_fscanf(). */

#define PARSE_IN_ATTRIBUTE          0
#define PARSE_IN_COMMENT            1 
#define PARSE_IN_VALUE              2 
#define PARSE_POST_ATTRIBUTE        3       
#define PARSE_POST_VALUE            4   
#define PARSE_PRE_ATTRIBUTE         5      
#define PARSE_PRE_VALUE             6  
#define PARSE_IN_ERROR              7 

typedef struct ConfigFileStruct* ConfigFile;
struct ConfigFileStruct {

  int error_count;

  AssocArray params;

};

/*******************************************************/
/* Public Data */
/*******************************************************/

extern ConfigFile  
CF_new( );

extern void  
CF_delete( ConfigFile );

extern int
CF_isValid( ConfigFile );

extern int
CF_addParam( ConfigFile, char*, char* );

extern ConfigFile 
CF_fscanf( FILE* );

extern ConfigFile 
CF_read( char* );

extern int
CF_fprintf( ConfigFile, FILE* );

extern int 
CF_printf( ConfigFile );

extern int 
CF_write( ConfigFile, char* );

#endif
