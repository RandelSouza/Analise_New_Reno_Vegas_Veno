#!/bin/bash

# Protocolo TCP utilizado 1 - New Reno, 2 - Vegas e 3 - Veno
typeTCP=( 1 2 3 )

# Taxa de erro utilizada
errorRate=( 0.2 0.5 )

# Número de nós presente na topologia de rede manhattan
numberNodes=( 9 36 64 )

# Taxa de trasmissão em Mbps
dataRate=( 10 100 )

NS_LOG=halteres=info

for tcp in "${typeTCP[@]}"
do
    for error in "${errorRate[@]}"
    do
	for node in "${numberNodes[@]}"
	do
	    for rate in "${dataRate[@]}"
	    do
	    	echo $tcp, $error, $node, $rate;
		nodesQuantity=$node;
		mkdir -p halteres-$tcp-$error-$nodesQuantity-$rate;

		for k in `seq 35`
	    	do
		    ./../../waf --run "ht --typeTCP="$tcp" --errorRate="$error" --xSize="$node" --ySize="$node" --dataRate="$rate" --n="$((($RANDOM %10) + 1))" --m="$((($RANDOM %10) + 1))"" >> log_halteres-$tcp-$error-$nodesQuantity-$rate.txt;
         	    mv ../../*.dat halteres-$tcp-$error-$nodesQuantity-$rate;
		done
	    done
	done
    done
done
