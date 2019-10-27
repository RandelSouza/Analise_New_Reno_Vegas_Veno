#!/bin/bash

# Protocolo TCP utilizado 2 - Vegas
typeTCP=( 2 )

# Taxa de erro utilizada
errorRate=( 0.2 0.5 )

# Número de nós presente na topologia de rede manhattan
numberNodes=( 9 36 64 )

# Taxa de trasmissão em Mbps
dataRate=( 10 100 )

NS_LOG=manhathan=info

mkdir Througput

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
        
		cat log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep Th | cut -d" " -f4 > Througput/Througput_manhattan-$tcp-$error-$nodesQuantity-$rate.txt;
	    done
	done
    done
done
