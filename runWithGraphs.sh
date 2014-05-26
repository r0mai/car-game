#!/bin/bash

# Please export a directory in which feedgnuplot can be found.
if [[ -z "$FEEDGNUPLOTDIR" ]]
then
	echo "Please set \$FEEDGNUPLOTDIR to the directory of feedgnuplot!."
	exit
fi

feedGnuPlotBinary=$FEEDGNUPLOTDIR/bin/feedgnuplot

lines=$(tput lines)
columns=$(tput cols)


$@ | sed -u 's/.*ges: \(.*\),/\1/g' | $feedGnuPlotBinary --stream --terminal "dumb $columns,$lines" --lines

