#!/bin/bash

# Protocolo TCP utilizado 1(New Reno), 2(Vegas) e 3(Veno)
typeTCP=(1 2 3)
# Taxa de erro utilizada
errorRate=(0.2 0.5)
# Número de nós presente na topologia de rede manhattan
#numberNodes=(9 36 64)
numberNodes=(3 6 8)
# Taxa de trasmissão in Mbps
dataRate=(10 100)

NS_LOG=manhathan=info 

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
			mkdir -p manhattan-$tcp-$error-$nodesQuantity-$rate;		

			for k in `seq 30`
	    	do
				./../../waf --run "mt --typeTCP="$tcp" --errorRate="$error" --xSize="$node" --ySize="$node" --dataRate="$rate" --n="$((($RANDOM %10) + 1))" --m="$((($RANDOM %10) + 1))"" >> log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt						
			done

			mv ../../*.dat manhattan-$tcp-$error-$nodesQuantity-$rate;	
	    done
	done
    done	   
done


