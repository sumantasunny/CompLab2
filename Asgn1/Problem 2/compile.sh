############################################################################################################
# Author:					Sumanta Dey (17CS60R79)
# Assignment:				Assignment 7
# Version:					8.0
# Created Date:				13-Oct-2017
# Last Modified Date:		27-Oct-2017
# Last Modified By:			Sumanta Dey
# Reason of Modification:	Documentation
###################################### :: Description :: ###############################################################
# Objective: To compile all the c file
############################################################################################################

#!/bin/sh
echo "______________________________________"
format=*.c
for i in $format;
do
 if [[ "$i" == "$format" ]]
 then
	echo "No Files"
 else
	#echo "file name $i"
	if [[ "$i" != "printErrorMessage.c" ]]
	then
		echo Compiling : $i
		gcc $i -o ${i%.*}
		echo "______________________________________"
	fi
 fi
done