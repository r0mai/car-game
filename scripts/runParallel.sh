#!/bin/bash

print_usage() {
    echo "Run multiple instances of car game."
    echo "Usage: $0 -h" 
    echo "       $0 [options]"
    echo "Options:"
    echo "    -h        This help message."
    echo "    -c        Add a new config. The number of parallel runs equals to the"
    echo "              number of times this parameter is given."
    echo "    -C        Global config file applied to all runs."
}

interrupt() {
    kill "${pids[@]}"
}

# -------- Main body --------

runDir="$(dirname $0)"

globalConfigOption=()
configs=()
while getopts ":hC:c:" Option; do
    case $Option in
    h) 
	    print_usage
	    exit
	    ;;
	C)
	    globalConfigOption=("--config=$OPTARG")
	    ;;
	c)
	    configs+=("--config=$OPTARG")
	    ;;
    esac
done

trap interrupt SIGINT
trap interrupt SIGHUP
trap interrupt SIGTERM
trap interrupt SIGQUIT

pids=()

for config in "${configs[@]}"; do
    $runDir/car-game --ai "${globalConfigOption[@]}" "$config" >"$config.out" &
    pids+=($!)
done
echo "${pids[@]}"
wait "${pids[@]}"


shift $(($OPTIND - 1))

