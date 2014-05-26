#!/bin/bash

# Please export a directory in which feedgnuplot can be found.
if [[ -z "$FEEDGNUPLOTDIR"  ]] && ! which feedgnuplot 2>&1 1>/dev/null
then
	echo "Please set \$FEEDGNUPLOTDIR to the directory of feedgnuplot!."
	exit
fi

if which feedgnuplot
then
	feedGnuPlotBinary=`which feedgnuplot`
else
	feedGnuPlotBinary=$FEEDGNUPLOTDIR/bin/feedgnuplot
fi

lines=$(tput lines)
columns=$(tput cols)

# output format is:
# Generation: 641, Current best fitness: 23749.9, Population averages: 5833.79,


cleanup() {
	tput rmcup
}

trap cleanup SIGINT

tput smcup
$@ | sed -u 's/Generation: \(.*\), Current best fitness: \(.*\), Population averages: \(.*\),/\1 \2 \3/g' | $feedGnuPlotBinary --stream --terminal "dumb $columns,$lines" --lines --domain --title 'Car-game' --xlen 100 --legend 0 'Best Fitness' --legend 1 'Population average'

cleanup
