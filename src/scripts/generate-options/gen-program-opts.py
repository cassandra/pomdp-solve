#!/usr/bin/python

import sys
import os
import errno
import re
import getopt
import string

import SimpleXML

# Globals
#
gDEBUG = 0
gVersion = "1.0"

gFilePrefix = "test"
gPrefixStr = "opt"
gProgName = "gen-program-opts.py"
gDate = "February 2005"

gCCommentLine = "/*******************************************************/"

############################################################
#
def DEBUG( msg ):
    if gDEBUG:
        print "DEBUG: %s" % msg

############################################################
#
def DIE( msg ):
     print "ERROR: %s" % msg
     sys.exit( 1 )
     
############################################################
#
def zzz( msg ):
     return

############################################################
#
class ParamNode:

     """Class that captures information about a particular piece of
     a program option/parameter."""

     def __init__( this,
                   param_id,
                   category,
                   class_value,
                   type,
                   units,
                   valid,
                   default,
                   required,
                   varname,
                   depends_param,
                   depends_value,
                   summary,
                   repeatable,
                   description ):

          this.param_id = param_id
          this.category = category
          this.class_value = class_value
          this.type = type
          this.units = units
          this.valid_str = valid
          this.default = default
          this.required = required
          this.varname = varname
          this.depends_param = depends_param
          this.depends_value = depends_value
          this.summary = summary
          this.repeatable = repeatable
          this.description = description

          this.checkValidity()

          if not this.isValid():
               return

     ############################################################
     #
     def setInvalid( this, msg ):

          """Sets this node to be invalid and also adds the parameter
          message t a list of messages about why this is invalid."""
          
          this.invalid_msg.append( msg )
          
     ############################################################
     #
     def isReservedWord( this, str ):

          """Checks to see if the parameter ID wil conflict with other
          syntax. """

          # The C reserved words
          #
          reserved = [ "auto", "break", "case", "char", "const",
                       "continue", "default", "do", "double", "else",
                       "enum", "extern", "float", "for", "goto", "if",
                       "int", "long", "register", "return", "short",
                       "signed", "sizeof", "static", "struct",
                       "switch", "typedef", "union", "unsigned",
                       "void", "volatile", "while" ]

          for w in reserved:
               if w == str:
                    return 1

          return 0

     ############################################################
     #
     def isIdentifier( this, str ):

          """Checks to see if the passed in string is a valid 'C'
          identifier. """

          ident_re = re.compile( r'^\s*[a-zA-Z_][a-zA-Z0-9_]*\s*$' )

          return ident_re.match( str )
          
     ############################################################
     #
     def matchesType( this, value ):

          """Checks to see if the string sent in matches, or is valid
          given the type of node this is."""
          
          string_re = re.compile( r'^\s*\w+\s*$' )
          int_re = re.compile( r'^\s*\d+\s*$' )
          double_re = re.compile( r'^\s*[+-]?\d+(\.?\d+)?(e[+-]?\d+)?\s*$' )

          if this.class_value == "enum" \
                 or this.class_value == "flag":
               for item in this.valid_list:
                    if item == value:
                         return 1
               return 0

          if this.type == "string":
               return string_re.match( value )

          if this.type == "int":
               return int_re.match( value )

          if this.type == "double":
               return double_re.match( value )

          return 0
          
     ############################################################
     #
     def setValidValueRange( this ):

          """Will set the min and max values for the valid range in
          the case where this is a normal type parameter."""
          
          if this.valid_str == "any":
               this.min_value = "any"
               this.max_value = "any"
               return
          
          range_re = re.compile( r'^\s*(\w+)\:(\w+)\s*$' )

          range_match = range_re.match( this.valid_str )
          if not range_match:
               this.setInvalid( "Bad range specification: %s" \
                                % this.valid_str )
               return 

          this.min_value = range_match.group(1)
          this.max_value = range_match.group(2)

          if this.min_value != "any":
               if not this.matchesType( this.min_value ):
                    this.setInvalid( "Min value '%s' invalid type" \
                                     % this.min_value )
                    
          if this.max_value != "any":
               if not this.matchesType( this.max_value ):
                    this.setInvalid( "Max value '%s' invalid type" \
                                     % this.max_value )
                    
     ############################################################
     #
     def setValidValueEnumeration( this ):

          """Will set the enumeratio of the possible valid value for
          the case where the class value of this parameter is
          'enum'."""
          
          this.valid_list = []

          list_re = re.compile( r'\b\w\S*\b' )

          this.valid_list = list_re.findall( this.valid_str )
     
          if not this.valid_list:
               this.setInvalid( "Bad enumeration of valid value: %s"
                                % this.valid_str )
               return
          
          # make sure they are all valid syntactically.
          #
          item_dict = {}
          for item in this.valid_list:
               item_dict[item] = "present"
               if not this.isIdentifier( item ):
                    this.setInvalid( "Invalid enum value %s" % item )
                         
          # make sure they are all unique
          #
          if len(item_dict.keys()) != len(this.valid_list):
               this.setInvalid( "Non-unique enum values: %s"
                                % this.valid_str )
          
     ############################################################
     #
     def checkValidity( this ):

          """Checks this node to see if all its values are consistent
          and conform to the requirements."""

          # If this is non-empty, then this parameter node is
          # invalid.  We use this to accumulate as many errors as
          # possible.
          #
          this.invalid_msg = []

          # Check that parameter ID conforms to rules for C
          # identifiers.
          #
          if not this.isIdentifier( this.param_id ) \
                 or this.isReservedWord( this.param_id ):
               this.setInvalid( "Parameter name %s in not valid"
                                       % this.param_id )
          
          # Check that parameter ID conforms to rules for C
          # identifiers.
          #
          if this.varname == "__N/A__":
               this.varname = this.param_id
          else:
              if not this.isIdentifier( this.varname ) \
                     or this.isReservedWord( this.varname ):
                  this.setInvalid( "Variable name %s in not valid"
                                   % this.varname )
          
          # Make sure the class_value is one of the proper types.
          #
          if this.class_value != "flag" \
             and this.class_value != "enum" \
             and this.class_value != "normal":
               this.setInvalid( "Bad class value %s"
                                       % this.class_value )

          # Only require a 'type' for 'normal' class (ignore for all
          # others)
          #
          if this.class_value == "normal":
               if this.type != "string" \
                      and this.type != "int" \
                      and this.type != "double":
                    this.setInvalid( "Bad type %s." % this.type )

          # Valid value setting is very dependent on the class.
          #
          if this.class_value == "flag":
               
               # A flag by defintion only has valid values of true and
               # false.  The N/A string is allowed as it is generated
               # elsewhere when the valid string is omitted.
               #
               if this.valid_str != "__N/A__" \
                      and this.valid_str != "any":
                    this.setInvalid( "Flag param with bad valid_str %s."
                                            % this.valid_str )
               else:
                    this.valid_list = [ "false", "true" ]
                    
          elif this.class_value == "enum":

               # The enum type must have an enumeration of 1 or more
               # values, each of which must be a valid identifier.
               #
               this.setValidValueEnumeration( )

          elif this.class_value == "normal":
               this.setValidValueRange( )

          if this.default != "__N/A__":
               if not this.matchesType( this.default ):
                    this.setInvalid( "Default value %s wrong type"
                                            % this.default )

          zzz( "what can I check for 'depends-on' stuff?" )

          zzz( "also need to globally check for duplicate parameter names." )

     ############################################################
     #
     def isValid( this ):
          return len(this.invalid_msg) < 1

     ############################################################
     #
     def wrap(this, text, width):
          """
          A word-wrap function that preserves existing line breaks
          and most spaces in the text. Expects that existing line
          breaks are posix newlines (\n).

          Found at: http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/148061
          """
          return reduce(lambda line, word, width=width: '%s%s%s' %
                        (line,
                         ' \n'[(len(line[line.rfind('\n')+1:])
                                + len(word.split('\n',1)[0]
                                      ) >= width)],
                         word),
                        text.split(' ')
                        )

     ############################################################
     #
     def getParamName( this ):
          return this.param_id

     ############################################################
     #
     def getVarName( this ):
          return this.varname

     ############################################################
     #
     def getCfgFileString( this, prefix ):
          return "%s_CFG_%s_STR" \
                 % ( prefix, this.param_id.upper() )

     ############################################################
     #
     def getCmdArgString( this, prefix ):
          return "%s_ARG_%s_STR"  \
                 % ( prefix, this.param_id.upper() )

     ############################################################
     #
     def getVarType( this, prefix ):

          if this.class_value == "enum":
               return "%s_%s_Type" \
                      % ( prefix, this.param_id.title())
               
          if this.class_value == "flag":
               return "Boolean_Type"

          if this.type == "int":
               return "int"

          if this.type == "double":
               return "double"
          
          if this.type == "string":
               return "char"

          return "???"

     ############################################################
     #
     def getDefaultVarName( this, prefix ):
          return "%s_OPT_%s_DEFAULT" \
                 % ( prefix, this.param_id.upper() )

     ############################################################
     #
     def getDefaultValueString( this, prefix ):

          if this.class_value == "enum" \
                 or this.class_value == "flag":
               return this.getEnumValueName( this.default, prefix )

          if this.type == "string":
               return '"%s"' % this.default

          return "%s" % this.default

     ############################################################
     #
     def getEnumValueName( this, str, prefix ):

          if str == "__END__":
               return "%s_%s__END__" \
                      % ( prefix, this.param_id.title() )

          if this.class_value == "flag":
               return "Boolean_%s" %  str
          else:
               return "%s_%s_%s" \
                      % ( prefix, this.param_id.title(), str )

     ############################################################
     #
     def getEnumStringArrayDefine( this, prefix ):
          return "%s_OPT_%s_STRINGS" \
                 % ( prefix, this.param_id.upper())

     ############################################################
     #
     def getEnumStringArrayName( this, prefix ):
          return "%s_%s_Str" \
                 % ( prefix, this.param_id.title())

     ############################################################
     #
     def getTypedef( this, prefix ):
          
          if this.class_value != "enum":
               return []
          
          lines = []

          lines.append( "\ntypedef enum {\n" )

          for item in this.valid_list:
               lines.append( "  %s,\n"
                             % this.getEnumValueName( item, prefix ))

          # Use special sentinel value
          #
          lines.append( "  %s\n"
                        % this.getEnumValueName( "__END__", prefix  ) )
          
          lines.append( "} %s;\n"
                        % this.getVarType( prefix ) )

          return lines
     
     ############################################################
     #
     def getStringDefineDef( this, prefix ):
          
          if this.class_value != "enum":
               return []

          lines = []

          lines.append( "\n#define %s { \\\n"
                        % this.getEnumStringArrayDefine( prefix ) )

          for item in this.valid_list:
               lines.append( '    "%s", \\\n' % item  )

          # Use empty string as sentinel value.
          lines.append( '    "" \\\n'  )

          lines.append( "  }\n" )

          return lines

     ############################################################
     #
     def getStringArrayDefType( this, prefix ):
          
         if this.class_value != "enum":
             return ""

         return "char* %s[]" % this.getEnumStringArrayName( prefix )
       
     ############################################################
     #
     def getStringArrayDef( this, prefix ):
          
          if this.class_value != "enum":
               return []

          lines = []

          lines.append( "\n%s = %s;\n"
                        % ( this.getStringArrayDefType( prefix ),
                            this.getEnumStringArrayDefine( prefix ) ));

          return lines

     ############################################################
     #
     def getStringArrayExternDef( this, prefix ):
          
          if this.class_value != "enum":
               return []

          lines = []

          lines.append( "\nextern %s;\n"
                        % this.getStringArrayDefType( prefix ) )

          return lines

     ############################################################
     #
     def getDefaultDefine( this, prefix ):

          if this.default == "__N/A__" \
                 or this.required == "true":
               return []
          
          return [ "#define %s %s\n"
                   % ( this.getDefaultVarName( prefix ),
                       this.getDefaultValueString( prefix ) ) ]

     ############################################################
     #
     def getCfgFileStringDefine( this, prefix ):

          return [ '#define %s "%s"\n'
                   % ( this.getCfgFileString( prefix ),
                       this.getParamName() ) ]

     ############################################################
     #
     def getCmdArgStringDefine( this, prefix ):

          return [ '#define %s "-%s"\n'
                   % ( this.getCmdArgString( prefix ),
                       this.getParamName() ) ]

     ############################################################
     #
     def getStructVariableDefine( this, prefix ):
          
          lines = [ "\n  /*\n" ]

          if this.description != "" \
             and this.description != "__N/A__":
               desc_wrapped = this.wrap( this.description, 60 )
               desc_lines =  desc_wrapped.splitlines()
               for item in desc_lines:
                    lines.append( "   * %s\n" % item )
          elif this.summary != "" \
                   and this.summary != "__N/A__":
               desc_wrapped = this.wrap( this.summary, 60 )
               desc_lines =  desc_wrapped.splitlines()
               for item in desc_lines:
                    lines.append( "   * %s\n" % item )
          else:
               lines.append( "   * ???\n" % item )
               
          lines.append( "   */\n" )

          if this.type == "string":
               lines.append( "  char %s[MAX_OPT_STRING_LEN];\n"
                             % this.getVarName() )
          else:
               lines.append( "  %s %s;\n"
                             % ( this.getVarType( prefix ),
                                 this.getVarName() ))
          
          return lines
          
     ############################################################
     #
     def getVariableInitialization( this, prefix ):
          
          if this.default == "__N/A__" \
                 or this.required == "true":

               if this.class_value == "enum":
                    return [ "  options->%s = 0;\n"
                             % this.getVarName() ]
               if this.type == "string":
                    return [ "  options->%s[0] = '\\0';\n"
                             % this.getVarName() ]
               if this.type == "int":
                    return [ "  options->%s = 0;\n"
                             % this.getVarName() ]
               if this.type == "double":
                    return [ "  options->%s = 0.0;\n"
                             % this.getVarName() ]

          if this.type == "string":
               return [ "  strcpy( options->%s, %s );\n"
                        % ( this.getVarName( ),
                            this.getDefaultVarName( prefix ) ) ]
          
          return [ "  options->%s = %s;\n"
                   % ( this.getVarName( ),
                       this.getDefaultVarName( prefix )) ]
               
     ############################################################
     #
     def getOptionParseLines( this, prefix ):

          lines = []

          lines.append( "\n  ret_value = " )

          
          if this.class_value == "enum":
               lines.append( "PO_getEnumOption( opts,\n" )
               lines.append( "                         %s,\n"
                             % this.getCmdArgString( prefix ) )
               lines.append( "                         &(enum_idx),\n")
               lines.append( "                         %s );\n"
                             % this.getEnumStringArrayName( prefix ) )
               lines.append( "  if ( ret_value == PO_OPT_PRESENT_VALID )\n" )
               lines.append( "    options->%s = enum_idx;\n"
                             % this.getVarName() )

          # Note that we treat flag options the same as an enmumerated
          # type with specific values of 'true' and 'false'.
          #
          elif this.class_value == "flag":
               lines.append( "PO_getEnumOption( opts,\n" )
               lines.append( "                         %s,\n"
                             % this.getCmdArgString( prefix ) )
               lines.append( "                         &(enum_idx),\n")
               lines.append( "                         Boolean_Str );\n" )

               lines.append( "  options->%s = enum_idx;\n"
                             % this.getVarName() )

               
          elif this.type == "string":
               lines.append( "PO_getStringOption( opts,\n" )
               lines.append( "                         %s,\n"
                             % this.getCmdArgString( prefix ))
               lines.append( "                         options->%s,\n"
                             % this.getVarName() )

               if this.min_value == "any":
                    lines.append( '                         NULL,\n' )
               else:
                    lines.append( '                         "%s",\n'
                                  % this.min_value  )

               if this.max_value == "any":
                    lines.append( '                         NULL );\n' )
               else:
                    lines.append( '                         "%s" );\n'
                                  % this.max_value  )
               
          elif this.type == "int":
               lines.append( "PO_getIntegerOption( opts,\n" )
               lines.append( "                           %s,\n"
                             % this.getCmdArgString( prefix ) )
               lines.append( "                            &(options->%s),\n"
                             % this.getVarName() )

               if this.min_value == "any":
                    lines.append( '                           INT_MIN,\n' )
               else:
                    lines.append( '                           %s,\n'
                                  % this.min_value  )

               if this.max_value == "any":
                    lines.append( '                           INT_MAX );\n' )
               else:
                    lines.append( '                           %s );\n'
                                  % this.max_value  )
               
          elif this.type == "double":
               lines.append( "PO_getDoubleOption( opts,\n" )
               lines.append( "                            %s,\n"
                             % this.getCmdArgString( prefix ) )
               lines.append( "               &(options->%s),\n"
                             % this.getVarName() )

               if this.min_value == "any":
                    lines.append( '                           -1*HUGE_VAL,\n' )
               else:
                    lines.append( '                           %s,\n'
                                  % this.min_value  )

               if this.max_value == "any":
                    lines.append( '                           HUGE_VAL );\n' )
               else:
                    lines.append( '                           %s );\n'
                                  % this.max_value  )
               
          # check return value
          lines.append( "  if ( ret_value == PO_OPT_PRESENT_ERROR )\n" )
          lines.append( "    PO_handleError( opts, " )
          lines.append( '"Option \'%s\' has invalid value."'
                        % this.getVarName() );
          lines.append( " );\n" )
               
          # This is the case where the option is required.
          #
          if this.required == "true":
               lines.append( "  else if ( ret_value != PO_OPT_PRESENT_VALID )\n" );
               lines.append( "    PO_handleError( opts, " )

               lines.append( '"Option \'%s\' is required."'
                             % this.param_id );
               lines.append( " );\n" )
               
          return lines
     
     ############################################################
     #
     def getUsageLines( this, prefix ):

          lines = []

          if this.class_value == "enum":
               lines.append( '  PO_showUsageEnumType( file,\n' )
               lines.append( '                     %s,\n'
                             % this.getCmdArgString( prefix ) )
               lines.append( '                     %s );\n'
                             % this.getEnumStringArrayName( prefix ) )

          elif this.class_value == "flag":
               lines.append( '  PO_showUsageEnumType( file,\n' )
               lines.append( '                     %s,\n'
                             % this.getCmdArgString( prefix ) )
               lines.append( '                     Boolean_Str );\n')

          else:     
               lines.append( '  fprintf( file, "\\t%%s <%s>\\n", %s );\n'
                             % ( this.type,
                                 this.getCmdArgString( prefix ) ))

          return lines

     ############################################################
     #
     def getToStringCommand( this, prefix ):

          lines = []

          if this.class_value == "enum":
               lines.append( '  sprintf( str, "%%s", %s[options->%s] );\n'
                             % ( this.getEnumStringArrayName( prefix ) ,
                                 this.getVarName()  ))

          elif this.class_value == "flag":
               lines.append( '  sprintf( str, "%%s", Boolean_Str[options->%s] );\n' 
                             % this.getVarName() )

          elif this.type == "int":
               lines.append( '  sprintf( str, "%%d", options->%s );\n'
                             % this.getVarName() )

          elif this.type == "double":
               lines.append( '  sprintf( str, "%%.6f", options->%s );\n'
                             % this.getVarName() )

          elif this.type == "string":
               lines.append( '  sprintf( str, "%%s", options->%s );\n'
                             % this.getVarName() )

          return lines
     
############################################################
############################################################
############################################################
#
class OptsCodeGenerator:

     ############################################################
     #
     def __init__( this ):
          this.prefix = gFilePrefix

          tmp = "%sProgOptions" % this.prefix.title()
          identity = string.maketrans( '', '' )
          
          this.var_type = tmp.translate( identity,
                                         '!@#$%^&*()-+=[]{};:.,<>/?~|')

          tmp = "%s" % this.prefix.upper()
          trans = string.maketrans( '!@#$%^&*()-+=[]{};:.,<>/?~|',
                                    '___________________________' )
          
          this.module_prefix = "%s_OPTS" % tmp.translate( trans, '' )
          
          return

     ############################################################
     #
     def getVarType( this ):

          return this.var_type
     
     ############################################################
     #
     def getModulePrefix( this ):
          return this.module_prefix
     
     ############################################################
     #
     def writeCopyright( this, file ):
          
          file.write( """
 * pomdp-solve
 *
 * Copyright © 1998-2003, Anthony R. Cassandra
 * Copyright © 1994-1997, Brown University
 *
 * Licensed under the Creative Commons Attribution-NonCommercial 4.0 
 * International License (CC BY-NC 4.0).
 *
 * See the LICENSE.txt file for details.
""" )

     ############################################################
     #
     def writeFileHeaderStart( this, file, suffix ):

         file.write( """
/*
 *<SOURCE_HEADER>
 *
 *  <NAME>
 *    %s-options%s
 *  </NAME>
 *  <AUTHOR>
 *    Anthony R. Cassandra
 *  </AUTHOR>
 *  <CREATE_DATE>
 *    February 2005
 *  </CREATE_DATE>
 *
 *  <RCS_KEYWORD>
""" % ( this.prefix, suffix ))

         # Interesting problem with trying to write these next
         # headers raw: when this script is checked-in, CVS will
         # convert them...no good.  Thus, we have to write these in a
         # way that CVS will not do any replacement with.  We want CVS
         # to replace them in the file we generate, but need to leave
         # them intact here. 
         #
         
         file.write(" *    $" )
         file.write( "RCSfile: %s-options%s,v $\n"
                     % ( this.prefix, suffix ))
         file.write(" *    $" )
         file.write( "Source: %s-options%s,v $\n"
                     % ( this.prefix, suffix ))
         file.write(" *    $" )
         file.write( "Revision: 1.6 $\n")
         file.write(" *    $" )
         file.write( "Date: %s $\n" % gDate )
         file.write(" *    $" )
         file.write( "Author: arc $\n")
         
         file.write( """
*  </RCS_KEYWORD>
 *
 *  <COPYRIGHT>
 *
""" )

     ############################################################
     #
     def writeFileHeaderEnd( this, file, suffix ):

          file.write( """
 *
 *  </COPYRIGHT>
 *
 *</SOURCE_HEADER>
 */

/*
 * This code was automatically generated on %s by the program:
 *
 *     gen-program-opts.py
 */

""" % gDate )

          if suffix == ".h":
               file.write( "#ifndef %s_OPTIONS_H\n"
                           % this.module_prefix )
               file.write( "#define %s_OPTIONS_H\n"
                           % this.module_prefix )
               file.write( '\n#include "global.h"\n' )
               file.write( '\n#include "program-options.h"\n' )

          if suffix == ".c":
               file.write( "#include <stdio.h>\n" )
               file.write( "#include <stdlib.h>\n" )
               file.write( "#include <string.h>\n" )
               file.write( "#include <math.h>\n" )
               file.write( "#include <limits.h>\n" )
               file.write( '\n#include "%s-options.h"\n'
                           % this.prefix )

     ############################################################
     #
     def writeHeaders( this, h_file, c_file ):

          this.writeFileHeaderStart( h_file, ".h" )
          this.writeCopyright( h_file )
          this.writeFileHeaderEnd( h_file, ".h" )
          
          this.writeFileHeaderStart( c_file, ".c" )
          this.writeCopyright( c_file )
          this.writeFileHeaderEnd( c_file, ".c"  )
          
     ############################################################
     #
     def writeFooters( this, h_file, c_file ):

               h_file.write( """
#endif
/* end header file */
""")

               c_file.write( """
/* end C code */
""")

     ############################################################
     #
     def writeTypedefs( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * Enumerated types for 'enum' parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
               h_file.writelines( param.getTypedef( this.getModulePrefix()))
               
          return

     ############################################################
     #
     def writeEnumStrings( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * String arrays for 'enum' parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
              h_file.writelines( param.getStringDefineDef \
                                 ( this.getModulePrefix() ) )
               
          return

     ############################################################
     #
     def writeOptionStructure( this, param_dict, h_file ):

          title = this.getVarType()
          
          h_file.write( "\n/*\n")
          h_file.write( " * Structure to hold all parameters.\n")
          h_file.write( " */\n")

          h_file.write( "typedef struct %sStruct* %s;\n"
                        % ( title, title ) )
          h_file.write( "struct %sStruct {\n" % title )

          h_file.write( "\n  /* Executable name. */\n" )
          h_file.write( "  char __exec_name__[MAX_OPT_STRING_LEN];\n" )
          
          h_file.write( "\n  /* Executable version. */\n" )
          h_file.write( "  char __version__[MAX_OPT_STRING_LEN];\n" )
          
          h_file.write( "\n  /* Error flag for internal use only. */\n" )
          h_file.write( "  int __error__;\n" )

          for param in param_dict.values():
               h_file.writelines( param.getStructVariableDefine \
                                  ( this.getModulePrefix() ) )
               
          h_file.write( "\n}; /* end %sProgOptionStruct */\n" % title )

          return

     ############################################################
     #
     def writeDefaultDefines( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * Default values for parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
              h_file.writelines( param.getDefaultDefine \
                                 ( this.getModulePrefix() ))
               
          return

     ############################################################
     #
     def writeCfgFileDefines( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * Strings for config file parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
              h_file.writelines( param.getCfgFileStringDefine \
                                 ( this.getModulePrefix() ))
               
          return

     ############################################################
     #
     def writeCmdArgDefines( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * Strings for cmd line parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
               h_file.writelines( param.getCmdArgStringDefine \
                                  ( this.getModulePrefix() ))
               
          return

     ############################################################
     #
     def writeStringArrayExternDefs( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * String arrays for cmd line parameters.\n")
          h_file.write( " */\n")

          for param in param_dict.values():
               h_file.writelines( param.getStringArrayExternDef \
                                  ( this.getModulePrefix() ))
               
          return

     ############################################################
     #
     def writeFunctionPrototypes( this, param_dict, h_file ):

          h_file.write( "\n/*\n")
          h_file.write( " * Function prototyeps.\n")
          h_file.write( " */\n")

          h_file.write( '\nextern %s\n'
                        % this.getVarType() )
          h_file.write( '%s_new( );\n'
                        % this.getModulePrefix() )

          h_file.write( '\nextern void\n' )
          h_file.write( '%s_delete( %s );\n'
                        % ( this.getModulePrefix(),
                            this.getVarType() ))
          
          h_file.write( '\nextern ConfigFile\n' )
          h_file.write( '%s_toConfigFile( %s );\n'
                        % ( this.getModulePrefix(),
                            this.getVarType() ))
          
          h_file.write( '\nextern void \n' )
          h_file.write( '%s_showUsageBrief( FILE*, char* );\n'
                        % this.getModulePrefix() )
          
          h_file.write( '\nextern void \n' )
          h_file.write( '%s_showUsage( FILE*, char* );\n'
                        % this.getModulePrefix() )
          
          h_file.write( '\nextern %s\n'
                        % this.getVarType() )
          h_file.write( '%s_parse( ProgramOptions );\n'
                        % this.getModulePrefix() )
          
          h_file.write( '\nextern %s\n'
                        % this.getVarType() )
          h_file.write( '%s_create( int, char** );\n'
                        % this.getModulePrefix() )

          return

     ############################################################
     #
     def writeStringArrays( this, param_dict, c_file ):
          
          c_file.write( "\n/*\n")
          c_file.write( " * Strings arrays for parameters.\n")
          c_file.write( " */\n")

          for param in param_dict.values():
               c_file.writelines( param.getStringArrayDef \
                                  ( this.getModulePrefix() ))
               
          return

     ############################################################
     #
     def writeConstructor( this, param_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( "%s\n" % this.getVarType() )
          c_file.write( "%s_new( )\n{\n\n" % this.getModulePrefix() )

          c_file.write( "  %s options;\n\n"
                        % this.getVarType() )
          c_file.write( "  options = (%s) XMALLOC( sizeof( *options ));\n\n"
                        % this.getVarType() )

          c_file.write( '  strcpy( options->__exec_name__, "%s" );\n\n'
                         % gFilePrefix)

          c_file.write( '  strcpy( options->__version__, "%s" );\n\n'
                         % gVersion)

          c_file.write( "  options->__error__ = 0;\n\n" )
          
          for param in param_dict.values():
               c_file.writelines( param.getVariableInitialization \
                                  (this.getModulePrefix() ))

          c_file.write( "\n  return( options );\n\n" )
          c_file.write( "}  /* %s_new */\n"  % this.getModulePrefix() )
               
          return

     ############################################################
     #
     def writeDestructor( this, param_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( "void\n" )
          c_file.write( "%s_delete( %s options )\n{\n\n"
                        % ( this.getModulePrefix(), this.getVarType() ))

          c_file.write( "  XFREE( options );\n\n" )

          c_file.write( "}  /* %s_delete */\n"  % this.getModulePrefix() )
               
          return

     ############################################################
     #
     def writeCommandLineParse( this, param_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( "%s\n" % this.getVarType())
          c_file.write( "%s_parse( ProgramOptions opts )\n{\n"
                        % this.getModulePrefix() )

          c_file.write( "  %s options;\n" % this.getVarType())
          c_file.write( "  int enum_idx;\n" )
          c_file.write( "  int ret_value;\n\n" )

          c_file.write( "  options = %s_new();\n\n" % this.getModulePrefix() )

          c_file.write( "  PO_startValidate( opts );\n" )
          
          for param in param_dict.values():
               c_file.writelines( param.getOptionParseLines \
                                  ( this.getModulePrefix()) )
               
          c_file.write( "\n  PO_endValidate( opts );\n" )

          c_file.write( "\n  if( ! PO_isValid( opts ))\n" )
          c_file.write( "    options->__error__ = 1;\n" )

          c_file.write( "  return options;\n\n" )

          c_file.write( "}  /* %s_parse */\n"  % this.getModulePrefix() )

          return

     ############################################################
     #
     def writeUsageBrief( this, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( "void \n" )
          c_file.write( "%s_showUsageBrief( FILE* file, char* exec_name )\n"
                        % this.getModulePrefix() )
          c_file.write( "{\n" )
  
          c_file.write( '  fprintf( file, "Usage: %s [opts...] [args...]\\n", ')
          c_file.write( 'exec_name );\n')
          c_file.write( '  fprintf( file, "Use \'-h\' for help.\\n");\n')

          c_file.write( "\n}  /* %s_showUsageBrief */\n"
                        % this.getModulePrefix() )
          return
          
     ############################################################
     #
     def writeUsage( this, param_dict, category_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( "void \n" )
          c_file.write( "%s_showUsage( FILE* file, char* exec_name )\n"
                        % this.getModulePrefix() )
          c_file.write( "{\n" )

          c_file.write( '  fprintf( file, "Usage: %s [opts...] [args...]\\n", ')
          c_file.write( 'exec_name );\n')

          key_list = category_dict.keys()
          key_list.remove( "__MAIN__" )
          key_list.insert( 0, "__MAIN__" )
          for category in key_list:

               c_file.write( "\n  /*******************************/\n" )
               c_file.write( "  /* %s parameters  */\n"
                             % category )
               c_file.write( "  /*******************************/\n" )
               if category == "__MAIN__":
                    c_file.write( '  fprintf( file, "General options:\\n" );\n\n')
               else:
                    c_file.write( '  fprintf( file, "%s options:\\n" );\n\n'
                                  % category )
               
               for param in param_dict.values():
                    if param.category != category:
                         continue

                    c_file.writelines( param.getUsageLines \
                                       ( this.getModulePrefix() ) )

          c_file.write( "\n}  /* %s_showUsage */\n" \
                        % this.getModulePrefix() )
          return
          
     ############################################################
     #
     def writeConfigFileConvert( this, param_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( 'ConfigFile\n' )
          c_file.write( '%s_toConfigFile( %s options )\n'
                        % ( this.getModulePrefix(),
                            this.getVarType() ))
          c_file.write( '{\n' )
          c_file.write( '  ConfigFile cfg;\n' )
          c_file.write( '  char str[256];\n\n' )
          c_file.write( '  cfg = CF_new();\n\n' )

          for param in param_dict.values():
               c_file.writelines( param.getToStringCommand \
                                  ( this.getModulePrefix() ))
               
               c_file.write( '  CF_addParam( cfg, %s, str );\n\n'
                             % param.getCfgFileString \
                             ( this.getModulePrefix() ))
               
          c_file.write( '  return cfg;\n\n' )
          c_file.write( '} /* %s_toConfigFile */\n'
                        % this.getModulePrefix() )

     ############################################################
     #
     def writeCreationFunction( this, param_dict, c_file ):

          c_file.write( "\n%s\n" % gCCommentLine )
          c_file.write( '%s\n'
                        % this.getVarType() )
          c_file.write( '%s_create( int argc, char** argv )\n'
                        % this.getModulePrefix() )
          c_file.write( '{\n' )

          c_file.write( '  %s options;\n' % this.var_type )
          c_file.write( '  ProgramOptions opts;\n' )
          c_file.write( '  ConfigFile cfg;\n\n' )

          c_file.write( '  opts = PO_create( argc, argv );\n\n' )

          c_file.write( '  if ( opts->usage )\n' )
          c_file.write( '    {\n' )
          c_file.write( '      %s_showUsage( stdout,\n'
                        % this.getModulePrefix() )
          c_file.write( '          opts->cmd_line->exec_name );\n' )
          c_file.write( '      PO_delete( opts );\n' )
          c_file.write( '      exit( 1 );\n' )
          c_file.write( '    }\n\n' )

          c_file.write( '  if ( ! PO_isValid( opts ))\n' )
          c_file.write( '    {\n' )
          c_file.write( '      %s_showUsageBrief( stdout, \n'
                        % this.getModulePrefix()  )
          c_file.write( '          opts->cmd_line->exec_name );\n' )
          c_file.write( '      PO_delete( opts );\n' )
          c_file.write( '      exit( 1 );\n' )
          c_file.write( '    }\n\n' )

          c_file.write( '  options = %s_parse( opts );\n\n'
                        % this.getModulePrefix()  )

          c_file.write( '  if ( options->__error__ )\n' )
          c_file.write( '    {\n' )
          c_file.write( '      %s_showUsageBrief( stdout, \n'
                        % this.getModulePrefix()  )
          c_file.write( '          opts->cmd_line->exec_name );\n' )
          c_file.write( '      PO_delete( opts );\n\n' )
          c_file.write( '      %s_delete( options );\n'
                        % this.getModulePrefix()  )
          c_file.write( '      exit( 1 );\n' )
          c_file.write( '    }\n\n' )

          c_file.write( '  PO_delete( opts );\n\n' )

          c_file.write( '  return options;\n\n' )
          c_file.write( '} /* %s_create */\n'
                        % this.getModulePrefix() )

     ############################################################
     #
     def generateCode( this, param_node_dict, category_dict ):

          """Main function for starting the code generation phase."""

          # Goes into two files: a header file and a C code file, so need
          # two file handles.
          #
          h_filename = "%s-options.h" % this.prefix
          c_filename = "%s-options.c" % this.prefix
          
          h_file = open( h_filename, 'w' )
          c_file = open( c_filename, 'w' )
          
          this.writeHeaders( h_file, c_file )

          this.writeTypedefs( param_node_dict, h_file )
          this.writeEnumStrings( param_node_dict, h_file )
          this.writeOptionStructure( param_node_dict, h_file )
          this.writeDefaultDefines( param_node_dict, h_file )
          this.writeCfgFileDefines( param_node_dict, h_file )
          this.writeCmdArgDefines( param_node_dict, h_file )
          this.writeStringArrayExternDefs( param_node_dict, h_file )
          this.writeFunctionPrototypes( param_node_dict, h_file )

          this.writeStringArrays( param_node_dict, c_file )
          this.writeConstructor( param_node_dict, c_file )
          this.writeDestructor( param_node_dict, c_file )

          this.writeConfigFileConvert( param_node_dict, c_file )
          this.writeUsageBrief( c_file )
          this.writeUsage( param_node_dict, category_dict, c_file )

          this.writeCommandLineParse( param_node_dict, c_file )
          this.writeCreationFunction( param_node_dict, c_file )

          
          this.writeFooters( h_file, c_file )
          
          h_file.close()
          h_file.close()
     


############################################################
#
def getRequiredChildNodeAttrValue( parent_node, child_name, attr_name ):

     """Fetches a child node, then looks for an atribute name. The
     node must be present and it must have the attribute name
     specified.  If the node is not present, the program stops."""
     
     child_node = parent_node.getChildNodeByName( child_name )
     if not child_node:
          DIE( "Child node %s does not exist."
               % child_name )
     
     attr_value = child_node.attr[attr_name]
     if attr_value == "":
          DIE( "Node %s without % attribute."
               % ( child_name, attr_name ) )

     return attr_value

############################################################
#
def getOptionalChildNodeAttrValue( parent_node,
                                   child_name,
                                   attr_name,
                                   default_value ):

     """Fetches a child node, then looks for an atribute name. The
     node can be optional, but if present it must have the attribute name
     specified.  If the node is not present, then the default value is
     returned.  If an error occurs, the program stops."""
     
     child_node = parent_node.getChildNodeByName( child_name )
     if not child_node:
          return default_value
     
     attr_value = child_node.attr[attr_name]
     if attr_value == "":
          DIE( "Node %s without % attribute."
               % ( child_name, attr_name ) )

     return attr_value

############################################################
#
def getOptionalChildNode( parent_node,
                          child_name ):

     """Fetches a child node and returns it, or else returns 'None' if
     node with that name is not present"""
     
     child_node = parent_node.getChildNodeByName( child_name )
     if not child_node:
          return None

     return child_node

############################################################
#
def convertToParamNodes( node ):

    global gFilePrefix, gVersion
     
    category_dict = {}
    
    param_node_dict = {}
    
    # We always want to be able to read a config file which we do
    # with the '-f' option
    param_node_dict["f"] \
          = ParamNode( "f", "__MAIN__", "normal", "string", "__N/A__",
                       "any", "__N/A__", "config_filename", "true",
                       "__N/A__", "__N/A__",
                       "Config file to read.", "false",
                       "What configuration file should be read." )

    for param_node in node.content:
          
        if param_node.name == "version":
            gVersion = param_node.attr["value"]
            continue
        
        if param_node.name == "executable":
            if not param_node.attr["value"]:
                DIE( "Executable tag without 'value' attribute.")
            else:
                gFilePrefix = param_node.attr["value"]
                
            continue
            
        if param_node.name == "description":
            zzz( "handle description" )
            continue

        if param_node.name != "param":
            DIE( "Bad node name: %s" % param_node.name )

        param_id = param_node.attr["name"]
        if param_id == "":
            DIE( "Parameter without 'name' attribute." )

        class_value = getOptionalChildNodeAttrValue( param_node,
                                                     "class",
                                                     "value",
                                                     "normal" )

        category = getOptionalChildNodeAttrValue( param_node,
                                                  "category",
                                                  "value",
                                                  "__MAIN__" )

        category_dict[category] = "present"
        
        if class_value == "normal":
            type = getRequiredChildNodeAttrValue( param_node,
                                                  "type",
                                                  "value" )
        else:
            type = "__N/A__"


        units = getOptionalChildNodeAttrValue( param_node,
                                               "units",
                                               "value",
                                               "__N/A__" )
        
        if class_value == "enum":
            valid = getRequiredChildNodeAttrValue( param_node,
                                                   "valid",
                                                   "value" )
        else:
            valid = getOptionalChildNodeAttrValue( param_node,
                                                   "valid",
                                                   "value",
                                                   "any" )
            
        default = getOptionalChildNodeAttrValue( param_node,
                                                 "default",
                                                 "value",
                                                 "__N/A__" )
        
        required = getOptionalChildNodeAttrValue( param_node,
                                                 "required",
                                                 "value",
                                                 "false" )
        
        varname = getOptionalChildNodeAttrValue( param_node,
                                                 "varname",
                                                 "value",
                                                 "__N/A__" )
        
        depends_param = getOptionalChildNodeAttrValue( param_node,
                                                       "depends",
                                                       "param",
                                                       "__N/A__" )
        
        if depends_param != "__N/A__":
            depends_value = getRequiredChildNodeAttrValue( param_node,
                                                           "depends",
                                                           "value" )
        else:
            depends_value = "__N/A__"
            
        summary = getOptionalChildNodeAttrValue( param_node,
                                                 "summary",
                                                 "value",
                                                 "__N/A__" )
        
        repeatable = getOptionalChildNode( param_node,
                                           "repeatable" )
        
        if repeatable:
            repeatable = "true"
        else:
            repeatable = "false"
            
        description_node = getOptionalChildNode( param_node,
                                                 "description" )
        
        if not description_node:
            description = "__N/A__"
        else:
            description = description_node.content[0]

        param_node = ParamNode( param_id,
                                category,
                                class_value,
                                type,
                                units,
                                valid,
                                default,
                                required,
                                varname,
                                depends_param,
                                depends_value,
                                summary,
                                repeatable,
                                description )
        
        param_node_dict[param_id] = param_node
        
        DEBUG( "Found param %s with class %s."
               % ( param_id, class_value ) )
        
         
    return param_node_dict, category_dict

############################################################
#
# Handling command line
#
def usage():                                       
    print """
gen-program-opt.py : Generating C code for command line/config options.

  -h                     print this message
 
  gen-program-opt.py [-h] <param-file>

    <param-file>           the XML-ish file with parameters specifcations
"""

############################################################
#
def parseCommandLine():
    
    opts, argsproper = getopt.getopt(sys.argv[1:], 'h', '' )

    opts_hash = {}
    for key,value in opts:
        opts_hash[key] = value

    if  opts_hash.has_key('-h'):
        usage(); sys.exit(0)

    if len(argsproper) != 1:
        usage(); sys.exit("parameter file missing")

    opts_hash["--param"] = argsproper[0]

    DEBUG("Parameter file: %s" % opts_hash["--param"] )

    return opts_hash

############################################################
#
def main( opts_hash ):

     parser = SimpleXML.Parser( )
     node = parser.parseFile( opts_hash["--param"] )

     DEBUG("XML Parsing finished." )

     # DEBUG( node.toString( "" ))

     param_node_dict, category_dict \
                      = convertToParamNodes( node )

     DEBUG("ParamNode creation finished." )

     for key in param_node_dict.keys():
          if not param_node_dict[key].isValid():
               DIE( "Node %s is invalid.\n%s" \
                    % ( param_node_dict[key].param_id,
                        param_node_dict[key].invalid_msg ))
          
     DEBUG("ParamNode validation finished." )

     code_gen = OptsCodeGenerator()
     code_gen.generateCode( param_node_dict, category_dict )

     DEBUG("C Code generation finished." )
     
############################################################
#
# Starts here
#
opts_hash = parseCommandLine()

main( opts_hash )

sys.exit( 0 )



