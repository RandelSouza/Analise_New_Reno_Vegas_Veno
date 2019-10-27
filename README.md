# Análise de desempenho dos protocolos TCP New Reno, TCP Vegas e TCP Veno.
* Trabalho da disciplina de Análise de Desempenho, da Universidade Federal do Ceará (UFC) Campus Quixadá.
* Ministrada pelo Prof. Dr. Arthur de Castro Callado.
* Equipe: Abimael Silas Barros e Randel Souza Almeida.

# Configurações da Máquina Virtual
* Sistema Operacional Ubuntu 16.04 LTS
* 30 *Gigabytes* de ROM
* 1 *Gigabyte* de RAM

# Instalação do Simulador NS-3
* Todo o tutorial para conhecer, baixar, testar e várias outras informações a respeito do NS-3 podem-ser encontradas no seguinte endereço: https://www.nsnam.org/docs/release/3.30/tutorial-pt-br/html/introduction.html. A seguir são descritos os passos para instalação do NS-3 para o trabalho em questão, caso haja alguma dúvida consulte o *link* anteriormente citado para maiores informações.

* O primeiro passo consistiu na instalação das dependências necessárias para que o NS-3 funcione de maneira correta. A *wiki* https://www.nsnam.org/wiki/Installation traz informações das dependências necessárias e as opcionais dependendo da finalidade do projeto.
Através da leitura desta *wiki* foi criado um *script* para automatizar a instalação das dependências que são pré-requisito para a instalação do NS-3 e também de outras dependências complementares que foram consideradas importantes. A figura a seguir mostra o *script* criado, tal *script* foi denominado de ns3_dependencies.sh.

![code image](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/ns3dependenciescode.png)

* A figura a seguir mostra o comando utilizado para execução do *script*. </br> O comando utilizado foi `sudo ./ns3_dependencies.sh`

![execute script](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/ns3dependencies.png)


