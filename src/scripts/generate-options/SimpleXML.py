#!/usr/bin/python

import sys
import os
import re

############################################################
#
class Node:

	"""Class that captures information about a particular pipece of
	information in a simplistic XML format."""

	def __init__( this, name ):
		this.name = name    # either 'tag' or 'content'
		this.attr = { }
		this.content = []  # either a list of Node objects or a string

	def addAttribute( this, name, value ):
		this.attr[name] = value.strip()

	def addContent( this, content ):
		this.content.append( content )

	def getChildNodeByName( this, name ):
		for item in this.content:
			if not isinstance( item, Node ):
				continue
			if item.name == name:
				return item

		return None
		
	def toString( this, indent ):
		print "%s<%s" % ( indent, this.name ),
		for attr_name in this.attr.keys():
			print ' %s="%s"' % ( attr_name, this.attr[attr_name] ),
		if len( this.content ) < 1:
			print "/>"
			return

		print ">"
		for c in this.content:
			if isinstance( c, Node ):
				c.toString( "%s    " % indent )
			else:
				print c

		print "%s</%s>" % ( indent, this.name )

############################################################
#
class Parser:
	
	############################################################
	# Constructor
	#
	def __init__( this ):

		this.debug_mode = 0

		this.error_count = 0
		this.line_num = 0
		
		this.blank_line_re = re.compile( r'^\s*$' )
		this.whitespace_re = re.compile( r'\s+' )
		this.comment_re = re.compile( r'^\s*\<\!\-\-.*-\-\>\s*$' )

		this.singletag_re = re.compile( r'^\s*\<(\w+)(.*)\/\>\s*$' )
		this.opentag_re = re.compile( r'^\s*\<(\w+)(.*)\>\s*$' )
		this.closetag_re = re.compile( r'^\s*\<\/(\w+)\>\s*$' )

		this.attribute_list_re = re.compile( r'\w+\=\"[^\"]+\"' )
		this.attribute_re = re.compile( r'^(\w+)\=\"(.+)\"$' )

     ############################################################
	#
	def debug( this, msg ):
		if this.debug_mode:
			print "DEBUG: %s" % msg

     ############################################################
	#
	def error( this, msg ):
		this.error_count += 1
		sys.stderr.write( "ERROR line %d: %s\n" \
					   % ( this.line_num, msg ) )
        

	############################################################
	#
	def addCDATA( this, node, str ):

		"""Add CDATA string to a node"""
		
		if this.blank_line_re.match( str ):
			return

		cdata = this.whitespace_re.sub( " ", str )

		this.debug( "Adding CDATA:\n%s" % cdata )

		node.addContent( cdata )

	############################################################
	#
	def parseAttributes( this, node, attr_str ):

		"""Adding attributes to a node from a string"""
		

		if this.blank_line_re.match( attr_str ):
			this.debug( "No attributes for tag: %s" % node.name )
			return

		attr_list = this.attribute_list_re.findall( attr_str )
		if not attr_list:
			this.error( "Bad attribute syntax %s" % attr_str )
			return

		this.debug( "Found %d attributes for tag: %s" \
				   % ( len(attr_list), node.name ))

		for attr in attr_list:
			re_match = this.attribute_re.match( attr )
			if not re_match:
				this.error( "Bad attribute %s" % attr )
				continue

			attr_name = re_match.group( 1 )
			attr_value = re_match.group( 2 )

			this.debug( '   Attribute found %s="%s"' \
					   % ( attr_name, attr_value ))

			node.addAttribute( attr_name, attr_value )

     ############################################################
	#
	def parse( this, paramfile, xml_node ):

		"""Parses a parameter file which is a simplified subset of XML"""

		this.debug( "-- Entering parse()" )

		cdata = ""
		while 1:

			line = paramfile.readline()
			if line == "":
				break

			line = line.rstrip()   # strip newline
			this.line_num += 1

			if this.blank_line_re.match( line ):
				continue

			# Comments can be anywhere and we just throw them away.
			#
			re_match = this.comment_re.match( line )
			if re_match:
				this.debug( "Ignoring comment: %s" % line )
				continue

			# Handling single tag.
			#
			re_match = this.singletag_re.match( line )
			if re_match:
				this.addCDATA( xml_node, cdata )
				cdata = ""
				tag_name = re_match.group(1)
				this.debug( "Match single tag: %s" % tag_name )
				attr_str = re_match.group(2)
				new_node = Node( tag_name )
				this.parseAttributes( new_node, attr_str )
				xml_node.addContent( new_node )
				continue

			# Handling open tag.
			#
			re_match = this.opentag_re.match( line )
			if re_match:
				this.addCDATA( xml_node, cdata )
				cdata = ""
				tag_name = re_match.group(1)
				this.debug( "Match open tag: %s" % tag_name )
				attr_str = re_match.group(2)
				new_node = Node( tag_name )
				this.parseAttributes( new_node, attr_str )
				xml_node.addContent( new_node )
				this.parse( paramfile, new_node )
				continue

			# Check for closed tag now, though this will only be
			# valid if we have seen the matching open tag.
			#
			re_match = this.closetag_re.match( line )
			if re_match:
				this.addCDATA( xml_node, cdata )
				cdata = ""
				tag_name = re_match.group(1)
				if tag_name != xml_node.name:
					this.error( "Orphan close tag '%s'" % tag_name )
					continue

				this.debug( "Closing tag '%s' found." % tag_name )
				return

			cdata = "%s%s" % ( cdata, line )
			continue

		this.addCDATA( xml_node, cdata )
		cdata = ""

		if xml_node.name != "ROOT":
			this.error( "Failed to close tag %s" %  xml_node.name )

     ############################################################
	#
	def parseFile( this, filename ):

		"""Opens a parameter specification file and handles all input. """

		paramfile = open( filename, "r")

		this.error_count = 0
		this.line_num = 0

		try:
			node = Node( "ROOT" )
			this.parse( paramfile, node )

			if this.error_count > 0:
				print "Parsing failed with %d errors. Aborted." \
					 % this.error_count
				sys.exit( 1 )

			this.debug( "Parsing succeeded." )
			return node

		finally:
			paramfile.close()

