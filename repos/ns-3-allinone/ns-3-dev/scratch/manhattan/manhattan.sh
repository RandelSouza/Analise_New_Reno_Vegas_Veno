#!/bin/bash

# Protocolo TCP utilizado 1(New Reno), 2(Vegas) e 3(Veno)
typeTCP=(1 2 3)
# Taxa de erro utilizada
errorRate=(0.2 0.5 )
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

		./../../waf --run "manhattan --typeTCP="$tcp" --errorRate="$error" --xSize="$node" --ySize="$node" --dataRate="$rate"";

		mkdir -p manhattan-$tcp-$error-$nodesQuantity-$rate;
		mkdir -p manhattan-$tcp-$error-$nodesQuantity-$rate-xml;
		mkdir -p manhattan-$tcp-$error-$nodesQuantity-$rate-tr;

		mv ../../*.pcap manhattan-$tcp-$error-$nodesQuantity-$rate;
		mv ../../*.tr   manhattan-$tcp-$error-$nodesQuantity-$rate-tr;
		mv ../../*.xml  manhattan-$tcp-$error-$nodesQuantity-$rate-xml;							
	    done
	done
    done	   
done


