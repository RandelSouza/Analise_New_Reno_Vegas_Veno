# Análise de desempenho dos protocolos TCP New Reno, TCP Vegas e TCP Veno.

* Trabalho da disciplina de **Análise de Desempenho**, da **Universidade Federal do Ceará (UFC) Campus Quixadá**.
* Ministrada pelo **Prof. Dr. Arthur de Castro Callado**.
* Equipe: **Abimael Silas Barros** e **Randel Souza Almeida**.

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

* Após a instalação de todas as dependências, seguiu-se para a instalação do NS-3. O tutorial presente no site do *nsnam* informa que é uma boa prática criar-se um diretório denominado **repos** no diretório *home*.</br> Para isso empregou-se os seguintes comandos:</br> 1. `cd /home/`</br> 2. `sudo mkdir repos`</br> 3. `cd repos`

* Estando dentro do diretório *repos* será obtido uma cópia do ns-3-allinone através da execução do seguinte comando:</br>
`sudo hg clone http://code.nsnam.org/ns-3-allinone`. A saída da execução do comando hg será algo do tipo: 
```
destination directory: ns-3-allinone
requesting all changes
adding changesets
adding manifests
adding file changes
added 31 changesets with 45 changes to 7 files
7 files updated, 0 files merged, 0 files removed, 0 files unresolved
```
* Ao termino da clonagem existirá um diretório chamado ns-3-allinone dentro do diretório ~/repos. Ao utilizar o comando `ls` para verificar o conteúdo do diretório, o resultado deve ser algo como:
```
build.py*  constants.py  dist.py*  download.py*  README  util.py
```







