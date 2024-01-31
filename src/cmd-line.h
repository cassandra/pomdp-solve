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

#ifndef CMD_LINE_H
#define CMD_LINE_H

/*******************************************************************/
/**************             CONSTANTS               ****************/
/*******************************************************************/

/* This defines the largest allowed word that can appear on the
   command line. */ 
#define MAX_CMD_ARG_LENGTH            100

/*******************************************************************/
/**************             TYPEDEFS                ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL VARIABLES            ****************/
/*******************************************************************/

/*******************************************************************/
/**************       EXTERNAL FUNCTIONS            ****************/
/*******************************************************************/

/* Parses the command line looking for an occurence of the string
   'arg_str'.  If there is a string, it checks the range of the number
   it marks its position in the mark_arg array.  Returns the argument
   nuber if successful and zero otherwise.  */
extern int getFlagParam( int argc, char **argv, char *arg_str, 
                  int *mark_arg );

/* Parses the command line looking for an occurence of the string
   'arg_str'.  If there is no string that follows it, then the
   argument is not incremented in the mark_arg array.  If there is a
   string, it checks the range of the number it marks its position in
   the mark_arg array.  Returns the argument nuber if successful and
   zero otherwise.  */
extern int getStringParam( int argc, char **argv, char *arg_str, 
                    int *mark_arg, char *value );

/* Does a getStringParam and then converts the string into a int.  It
  then checks it against the min and max values.  If the min and max
  values are both zero, then any number is allowed.  */
extern int getIntParam( int argc, char **argv, 
                    char *arg_str, int *mark_arg,
                    int *value, int min, int max  );

/* Does a getStringParam and then converts the string into a double.
  It then checks it against the min and max values.  If the min and
  max values are both zero, then any number is allowed.  Note that we
  do not want to set the 'value' parameter until we know that we have
  a valid value.  We want to leave the current value alone, so that if
  it is initially set to some default value, then we will get the
  default if we do not find that it should be overridden.  */
extern int getDoubleParam( int argc, char **argv, 
                    char *arg_str, int *mark_arg,
                    double *value, double min, double max  );

/* Parses the command line looking for an occurence of the string
   'arg_str'.  If there is no string that follows it, then the
   argument is not incremented in the mark_arg array.  If there is a
   string, it checks the range of the number it marks its position in
   the mark_arg array.  Returns the argument nuber if successful and
   zero otherwise.  */
extern int getStringParamValidate( int argc, char **argv, char *arg_str, 
                            int *mark_arg, int *valid_match,
                            char **valid_str, int num_valid_str );
     
/* Displays a list of enumerated command line options using comma
  separation and inserting newlines where appropriate.  */
extern void showUsageEnumType( FILE *file,
                               char *option_str, 
                               int num_str, 
                               char **str );

#endif






