#!/bin/bash

# Protocolo TCP utilizado 1(New Reno)
typeTCP=(2)
# Taxa de erro utilizada
errorRate=(0.2 0.5)
# Número de nós presente na topologia de rede manhattan
#numberNodes=(9 36 64)
numberNodes=(3 6 8)
# Taxa de trasmissão in Mbps
dataRate=(10 100)

NS_LOG=manhathan=info

mkdir Jitter

for tcp in "${typeTCP[@]}"
do
    for error in "${errorRate[@]}"
    do
	for node in "${numberNodes[@]}"
	do
	    for rate in "${dataRate[@]}"
	    do
	    	echo $tcp, $error, $node, $rate
		nodesQuantity=$node;			
		
		cat log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep jitter | cut -d":" -f2 | cut -d" " -f4 > Jitter/Jitter_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
	    done
	done
    done
done

