#!/bin/bash

#-------------------------------------------------------------------------------------#
echo "Criando diretório tcpNewReno tcpVegas e tcpVeno"
mkdir tcpNewReno tcpVegas tcpVeno

echo "Criando diretório tcpNewReno/Througput tcpVegas/Througput e tcpVeno/Througput"
mkdir -p tcpNewReno/Througput tcpVegas/Througput tcpVeno/Througput

echo "Criando diretório tcpNewReno/Jitter tcpVegas/Jitter e tcpVeno/Jitter"
mkdir -p tcpNewReno/Jitter tcpVegas/Jitter tcpVeno/Jitter

echo "Criando diretório tcpNewReno/Delay tcpVegas/Delay e tcpVeno/Delay"
mkdir -p tcpNewReno/Delay tcpVegas/Delay tcpVeno/Delay

echo -e "\nMovendo log_manhattan-1-* para tcpNewReno"
mv log_manhattan-1-* tcpNewReno/

echo "Movendo log_manhattan-2-* para tcpVegas"
mv log_manhattan-2-* tcpVegas/

echo -e "\nMovendo log_manhattan-3-* para tcpVeno\n"
mv log_manhattan-3-* tcpVeno/
#-------------------------------------------------------------------------------------#

echo "Gerando arquivos para o Througput o Jitter e o Delay"
# Protocolo TCP utilizado 1(New Reno)
typeTCP=(1 2 3)
# Taxa de erro utilizada
errorRate=(0.2 0.5)
# Número de nós presente na topologia de rede manhattan
#numberNodes=(9 36 64)
numberNodes=(3 6 8)
# Taxa de trasmissão in Mbps
dataRate=(10 100)

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
		if [ $tcp -eq 1 ]
		then
	                cat tcpNewReno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep Throughput | cut -d" " -f4 > tcpNewReno/Througput/Througput_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpNewReno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep jitter | cut -d":" -f2 | cut -d" " -f4 > tcpNewReno/Jitter/Jitter_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpNewReno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt  | grep delay | cut -d":" -f2 | cut -d" " -f4 > tcpNewReno/Delay/Delay_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
		elif [ $tcp -eq 2 ]
		then
	                cat tcpVegas/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep Throughput | cut -d" " -f4 > tcpVegas/Througput/Througput_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpVegas/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep jitter | cut -d":" -f2 | cut -d" " -f4 > tcpVegas/Jitter/Jitter_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpVegas/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt  | grep delay | cut -d":" -f2 | cut -d" " -f4 > tcpVegas/Delay/Delay_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
		elif [ $tcp -eq 3 ]
		then
	                cat tcpVeno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep Throughput | cut -d" " -f4 > tcpVeno/Througput/Througput_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpVeno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt | grep jitter | cut -d":" -f2 | cut -d" " -f4 > tcpVeno/Jitter/Jitter_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
			cat tcpVeno/log_manhattan-$tcp-$error-$nodesQuantity-$rate.txt  | grep delay | cut -d":" -f2 | cut -d" " -f4 > tcpVeno/Delay/Delay_manhattan-$tcp-$error-$nodesQuantity-$rate.txt
		fi
            done
        done
    done
done
#-------------------------------------------------------------------------------------#



