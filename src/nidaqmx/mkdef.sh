#!C:\Tools\UnxUtils\bin\sh

# This script : mkdef.sh
#
# Purpose :
#	Prints a definition file to be used as argument to dlltool to the standard output.
#
# Author :
#	David Picard - 2006
#
# Context :
#	Libraries compiled with Microsoft Visual C++ (.lib) need to be
#	adapted before they can be linked using MinGW compiler, gcc implementation
#	for Win32.
#	1- Before executing this script, use the free LibDump or MS dumpbin to
#	   produce a dump file.
#	2- Launch the script. It will produce a definition file that you can pass
#	   on to dlltool to produce a .a library, that can be linked with MinGW.


PATH=$PATH:C:/Tools/UnxUtils/usr/local/wbin

# check number of arguments :
if [ -z "$1" ] || [ -z "$2" ] || [ -n "$3" ]
then
	echo
	echo "mkdef.sh - David Picard - 2006"
	echo "Usage : ./mkdef.sh <export_file> <dump_file>"
	echo "\tCreate dump file with free LibDump or MS dumpbin.exe."
	echo "\tCreate export file with pexports."
	exit -1
fi

# export file (from pexports)
EXP=$1

# dump file (from dumpbin /exports)
DUMP=$2


# copy first 2 lines
sed -n /LIBRARY/p $EXP
sed -n /EXPORTS/p $EXP


# copy lines starting with a question mark without modification (?)
sed -n '/^\?.*/p' $EXP


# loop over source file lines :
# sed : select lines in DUMP containing the current line in EXP, with at least 1
#	and at most 26 leading characters beeing either a space or an underscore
#	and strip off leading spaces and/or underscores
# sort | head : limit the output to 1 line (the 1st, in alphanumerical order)

for i in `cat $EXP | tr -d '\r'`
do
	# reject lines that start with the '?' character
	if [ `echo $i | cut -c 1` != '?' ]
	then
		sed -n "s/\(^[ _]\{1,26\}\)\($i\)/\2/p" $DUMP | sort | head -1
	fi
done
