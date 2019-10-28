# Análise de desempenho dos protocolos TCP New Reno, TCP Vegas e TCP Veno.

![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf1.jpg-1.png)

* Trabalho da disciplina de **Análise de Desempenho**, da **Universidade Federal do Ceará (UFC) Campus Quixadá**.
* Ministrada pelo **Prof. Dr. Arthur de Castro Callado**.
* Equipe: **Abimael Silas Barros** e **Randel Souza Almeida**.

![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf2.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf3.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf4.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf5.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf6.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf7.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf8.jpg-1.png)

# Configurações da Máquina Virtual

* Sistema Operacional Ubuntu 16.04 LTS
* 30 *Gigabytes* de ROM
* 1 *Gigabyte* de RAM

# Instalação do Simulador NS-3

* Todo o tutorial para conhecer, baixar, testar e várias outras informações a respeito do NS-3 podem-ser encontradas no seguinte endereço: https://www.nsnam.org/docs/release/3.30/tutorial-pt-br/html/introduction.html. A seguir são descritos os passos para instalação do NS-3 para o trabalho em questão, caso haja alguma dúvida consulte o *link* anteriormente citado para maiores informações.

* O primeiro passo consistiu na instalação das dependências necessárias para que o NS-3 funcione de maneira correta. A *wiki* https://www.nsnam.org/wiki/Installation traz informações das dependências necessárias e as opcionais dependendo da finalidade do projeto. Através da leitura desta *wiki* foi criado um *script* para automatizar a instalação das dependências que são pré-requisito para a instalação do NS-3 e também de outras dependências complementares que foram consideradas importantes. A figura a seguir mostra o *script* criado, tal *script* foi denominado de ns3_dependencies.sh. 

![code image](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/ns3dependenciescode.png)

* A figura a seguir mostra o comando utilizado para execução do *script*. </br> O comando utilizado foi `sudo ./ns3_dependencies.sh`

![execute script](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/ns3dependencies.png)

* Após a instalação de todas as dependências, seguiu-se para a instalação do NS-3. O tutorial presente no site do *nsnam* informa que é uma boa prática criar-se um diretório denominado **repos** no diretório *home*.</br> Para isso empregou-se os seguintes comandos:</br> 1. `cd /home/`</br> 2. `sudo mkdir repos`</br> 3. `cd repos`

* Estando dentro do diretório *repos* será obtido uma cópia do ns-3-allinone através da execução do seguinte comando:</br> `sudo hg clone http://code.nsnam.org/ns-3-allinone`. A saída da execução do comando hg será algo do tipo: 
```
destination directory: ns-3-allinone
requesting all changes
adding changesets
adding manifests
adding file changes
added 31 changesets with 45 changes to 7 files
7 files updated, 0 files merged, 0 files removed, 0 files unresolved
```
* Ao termino da clonagem existirá um diretório chamado ns-3-allinone dentro do diretório ~/repos. Ao utilizar o comando `ls ns-3-allinone` para verificar o conteúdo do diretório, o resultado deve ser algo como:
```
build.py*  constants.py  dist.py*  download.py*  README  util.py
```
* Prosseguindo com o processo de instalação, foram baixados alguns scripts em Python. O próximo passo será usar estes scripts para baixar e construir a distribuição ns-3 de sua escolha. Neste trabalho utilizaremos a versão ns-3-dev que é a padrão, mas poderia ser utilizada qualquer outra release disponível no site http://code.nsnam.org/.

1. `cd ns-3-allinone/`
2. `sudo ./download.py -n ns-3-dev`

* O resultado da execução do comando anterior deve ter a seguinte saída (pode ser que o resultado seja diferente dependendo da versão escolhida):

```
    #
    # Get NS-3
    #

Cloning ns-3 branch
 =>  hg clone http://code.nsnam.org/ns-3-dev ns-3-dev
requesting all changes
adding changesets
adding manifests
adding file changes
added 4634 changesets with 16500 changes to 1762 files
870 files updated, 0 files merged, 0 files removed, 0 files unresolved
```

* Depois que o script download.py tiver completado sua tarefa, veremos vários diretórios novos dentro de ~/repos/ns-3-allinone para entrar executa-se `cd ns-3-allinone` e para listar os diretóirios/arquivos utiliza-se o `ls`, os diretórios/arquivos são vistos a seguir:
```
build.py*     constants.pyc  download.py*  nsc/        README      util.pyc
constants.py  dist.py*       ns-3-dev/     pybindgen/  util.py
```
* Por fim, no diretório ns-3-dev (para entrar executa-se `cd  ns-3-dev`) que está dentro do diretório ~/repos/ns-3-allinone deve existir, depois dos passos anteriores, o seguinte conteúdo após o comando `ls`:
```
AUTHORS       doc       ns3            scratch   testpy.supp  VERSION   waf-tools
bindings      examples  README         src       utils        waf*      wscript
CHANGES.html  LICENSE   RELEASE_NOTES  test.py*  utils.py     waf.bat*  wutils.py
```

* Construindo o Waf.  O waf é utilizado para configurar e construir o projeto do ns-3. Não é estritamente necessário neste ponto, mas será valioso quando se forem necessárias alterações nas configurações do projeto. Provavelmente a mudança mais útil que será feita futuramente é a construção de uma versão do código otimizado. Por padrão, o projeto é construído com a opção de depuração (debug), para verificação de erros. Então, para construir um projeto otimizado, deve-se executar o seguinte comando (ainda com suporte a testes e exemplos):

```
./waf -d optimized --enable-examples --enable-tests configure
```
* Isto executa o Waf fora do diretório local (o que é bem conveniente). Como o sistema em construção verifica várias dependências, deverá aparecer uma saída similar com a que segue:
```
Checking for program g++                 : ok /usr/bin/g++
Checking for program cpp                 : ok /usr/bin/cpp
Checking for program ar                  : ok /usr/bin/ar
Checking for program ranlib              : ok /usr/bin/ranlib
Checking for g++                         : ok
Checking for program pkg-config          : ok /usr/bin/pkg-config
Checking for -Wno-error=deprecated-declarations support : yes
Checking for -Wl,--soname=foo support                   : yes
Checking for header stdlib.h                            : ok
Checking for header signal.h                            : ok
Checking for header pthread.h                           : ok
Checking for high precision time implementation         : 128-bit integer
Checking for header stdint.h                            : ok
Checking for header inttypes.h                          : ok
Checking for header sys/inttypes.h                      : not found
Checking for library rt                                 : ok
Checking for header netpacket/packet.h                  : ok
Checking for pkg-config flags for GSL                   : ok
Checking for header linux/if_tun.h                      : ok
Checking for pkg-config flags for GTK_CONFIG_STORE      : ok
Checking for pkg-config flags for LIBXML2               : ok
Checking for library sqlite3                            : ok
Checking for NSC location                               : ok ../nsc (guessed)
Checking for library dl                                 : ok
Checking for NSC supported architecture x86_64          : ok
Checking for program python                             : ok /usr/bin/python
Checking for Python version >= 2.3                      : ok 2.5.2
Checking for library python2.5                          : ok
Checking for program python2.5-config                   : ok /usr/bin/python2.5-config
Checking for header Python.h                            : ok
Checking for -fvisibility=hidden support                : yes
Checking for pybindgen location                         : ok ../pybindgen (guessed)
Checking for Python module pybindgen                    : ok
Checking for pybindgen version                          : ok 0.10.0.640
Checking for Python module pygccxml                     : ok
Checking for pygccxml version                           : ok 0.9.5
Checking for program gccxml                             : ok /usr/local/bin/gccxml
Checking for gccxml version                             : ok 0.9.0
Checking for program sudo                               : ok /usr/bin/sudo
Checking for program hg                                 : ok /usr/bin/hg
Checking for program valgrind                           : ok /usr/bin/valgrind
---- Summary of optional NS-3 features:
Threading Primitives          : enabled
Real Time Simulator           : enabled
Emulated Net Device           : enabled
GNU Scientific Library (GSL)  : enabled
Tap Bridge                    : enabled
GtkConfigStore                : enabled
XmlIo                         : enabled
SQlite stats data output      : enabled
Network Simulation Cradle     : enabled
Python Bindings               : enabled
Python API Scanning Support   : enabled
Use sudo to set suid bit      : not enabled (option --enable-sudo not selected)
Build tests                   : enabled
Build examples                : enabled
Static build                  : not enabled (option --enable-static not selected)
'configure' finished successfully (2.870s)
```
* Vamos configurar uma construção do ns-3 com suporte a depuração, bem como, vamos incluir exemplos e testes. Para isto devemos executar:
```
./waf -d debug --enable-examples --enable-tests configure
```
* Com o sistema configurado, agora podemos construir nossa versão digitando:
```
./waf
```
* Para executar os testes de unidade do ns-3, basta chamar o arquivo ./test.py da seguinte forma:
```
./test.py -c core
```
* Estes testes são executados em paralelo pelo Waf. No final, uma mensagem como a que segue deve aparecer.
```
47 of 47 tests passed (47 passed, 0 failed, 0 crashed, 0 valgrind errors)
Esta é uma mensagem importante.
```
* Também haverá saídas da execução do teste e estas geralmente são algo como:
```
Waf: Entering directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
Waf: Leaving directory `/home/craigdo/repos/ns-3-allinone/ns-3-dev/build'
'build' finished successfully (1.799s)

Modules built:
aodv                      applications              bridge
click                     config-store              core
csma                      csma-layout               dsdv
emu                       energy                    flow-monitor
internet                  lte                       mesh
mobility                  mpi                       netanim
network                   nix-vector-routing        ns3tcp
ns3wifi                   olsr                      openflow
point-to-point            point-to-point-layout     propagation
spectrum                  stats                     tap-bridge
template                  test                      tools
topology-read             uan                       virtual-net-device
visualizer                wifi                      wimax

PASS: TestSuite ns3-wifi-interference
PASS: TestSuite histogram
PASS: TestSuite sample
PASS: TestSuite ipv4-address-helper
PASS: TestSuite devices-wifi
PASS: TestSuite propagation-loss-model

...

PASS: TestSuite attributes
PASS: TestSuite config
PASS: TestSuite global-value
PASS: TestSuite command-line
PASS: TestSuite basic-random-number
PASS: TestSuite object
PASS: TestSuite random-number-generators
95 of 95 tests passed (95 passed, 0 failed, 0 crashed, 0 valgrind errors)
```
* Este comando é normalmente executado pelos usuários para verificar se o ns-3 foi construído corretamente.

* Os códigos normalmente são executados sob o controle do Waf. Isto assegura que os caminhos das bibliotecas compartilhadas estejam corretas e que estarão disponíveis em tempo de execução. Para executar um programa, basta usar a opção --run no Waf. Para executar um equivalente ao “Olá mundo” (Hello world) no ns-3, utilizamos o comando:
```
./waf --run hello-simulator
```
* O Waf primeiro verifica se o programa foi construído corretamente e se necessário, o constrói. Então executa o programa, que fornece a seguinte saída:
```
Hello Simulator
```

# Organizando os scripts para a Coleta dos dados
* Considerando que o clone deste projeto já tenha sido feito deste o momento da instalação das dependências, o passo subsequente consiste em fazer uma cópia dos diretórios ht/ e mt/ deste projeto, para a pasta scratch do NS-3 **~/repos/ns-3-allinone/ns-3-dev/scratch/**. Considerando que o diretório corrente seja o deste trabalho.
```
sudo cp -R ht/ ~/repos/ns-3-allinone/ns-3-dev/scratch/
sudo cp -R mt/ ~/repos/ns-3-allinone/ns-3-dev/scratch/
```
* Com os scrips devidamente copiados para a pasta scratch, construiremos novamente a versão do NS-3.
```
cd ~/repos/ns-3-allinone/ns-3-dev/
sudo ./waf
```
# Executando os scripts
* Dentro de cada pasta que foi anteriormente cópiada existem dois arquivos um .sh e outro .cc. Vale ressaltar que ht refere-se aos cenários com a topologia halteres e mt correspondem aos cenários com a topologia manhattan. Como são muitos cenários a finalização de cada script poderá demorar. Os comandos para execução de cada script são vistos a seguir. 
---
```
cd scratch/ht/
sudo ./ht.sh
```
---
```
cd scratch/mt/
sudo ./mt.sh
```
---

* Após a finalização de todos os scripts de cada cenário, o passo seguinte será filtrar os dados relevantes para as métricas de interrese que são variação do atraso e utilização do canal.

---
```
cd scratch/ht/

sudo ./tcpNewReno/gerarDadosJitter.sh
sudo ./tcpNewReno/gerarDadosThrougput.sh

sudo ./tcpVegas/gerarDadosJitter.sh
sudo ./tcpVegas/gerarDadosThrougput.sh

sudo ./tcpVeno/gerarDadosJitter.sh
sudo ./tcpVeno/gerarDadosThrougput.sh

```
---
```
cd scratch/mt/

sudo ./tcpNewReno/gerarDadosJitter.sh
sudo ./tcpNewReno/gerarDadosThrougput.sh

sudo ./tcpVegas/gerarDadosJitter.sh
sudo ./tcpVegas/gerarDadosThrougput.sh

sudo ./tcpVeno/gerarDadosJitter.sh
sudo ./tcpVeno/gerarDadosThrougput.sh

```
---

# Gerando os Gráficos
* Os scripts para geração dos gráficos seguiram o seguinte modelo da figura abaixo, modificando a pasta gerada para cada topologia e métrica. Od códigos dos demais gráficos não estão presentes pois foram criados sem muita dinamicidade, e portanto quando estiver mais bem escrito eles serão colocados neste repositorio. No entando pode-se utilizar o modelo abaixo modificando conforme a necessidade.

```python

from glob import glob
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np

def calc(df):
    mean = df[0].mean()
    std = df[0].std()
    margemdeerro = 1.96 * (std / np.sqrt(len(df[0]))) 
    return mean, margemdeerro


lista = []

Th = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpNewReno/Jitter/Jitter_manhattan-1-0.2-3-10.txt")
Th1 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVegas/Jitter/Jitter_manhattan-2-0.2-3-10.txt")
Th6 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVeno/Jitter/Jitter_manhattan-3-0.2-3-10.txt")

Th2 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpNewReno/Jitter/Jitter_manhattan-1-0.2-6-10.txt")
Th3 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVegas/Jitter/Jitter_manhattan-2-0.2-6-10.txt")
Th7 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVeno/Jitter/Jitter_manhattan-3-0.2-6-10.txt")

Th4 = glob("~/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpNewReno/Jitter/Jitter_manhattan-1-0.2-8-10.txt")
Th5 = glob("~/randel/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVegas/Jitter/Jitter_manhattan-2-0.2-8-10.txt")
Th8 = glob("~/randel/repos/ns-3-allinone/ns-3-dev/scratch/ht/tcpVeno/Jitter/Jitter_manhattan-3-0.2-8-10.txt")

lista.append(pd.read_csv(Th[0], header=None))
lista.append(pd.read_csv(Th1[0], header=None))
lista.append(pd.read_csv(Th6[0], header=None))

lista.append(pd.read_csv(Th2[0], header=None))
lista.append(pd.read_csv(Th3[0], header=None))
lista.append(pd.read_csv(Th7[0], header=None))

lista.append(pd.read_csv(Th4[0], header=None))
lista.append(pd.read_csv(Th5[0], header=None))
lista.append(pd.read_csv(Th8[0], header=None))

lista2 = []
for i in lista:
    lista2.append(calc(i))

print lista2

x = [2,12,21]
labels=['9','36', '64']

plt.errorbar(1,lista2[0][0], yerr=lista2[0][1], linestyle='', capsize=6,elinewidth="1", marker='o',fmt='o', label="TCP New Reno", color='k' )
plt.errorbar(2,lista2[1][0], yerr=lista2[1][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o', label="TCP Vegas", color='r')
plt.errorbar(3,lista2[2][0], yerr=lista2[2][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',label="TCP Veno", color='b')

plt.errorbar(11,lista2[3][0], yerr=lista2[3][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='k')
plt.errorbar(12,lista2[4][0], yerr=lista2[4][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='r')
plt.errorbar(13,lista2[5][0], yerr=lista2[5][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='b')

plt.errorbar(20,lista2[6][0], yerr=lista2[6][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='k')
plt.errorbar(21,lista2[7][0], yerr=lista2[7][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='r')
plt.errorbar(22,lista2[8][0], yerr=lista2[8][1], linestyle='',capsize=6,elinewidth="1",marker='o', fmt='o',color='b')

plt.xticks(x,labels)
plt.xlabel('Numero de maquinas')
plt.ylabel('Jitter (Variacao do atraso)\n')
plt.legend(loc='best')

plt.title("Topologia Halteres com taxa de perda de 0.2,\n tendo 10Mbps de capacidade do canal\n e utilizando trafego de aplicacao FTP(File Transfer Protocol) e CBR(Constant bit rate)")
plt.savefig("/home/randel/manhattan01.png")
plt.show()
```

# Gráficos gerados

![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf9.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf10.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf11.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf12.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf13.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf14.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf15.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf16.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf17.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf18.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf19.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf20.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf21.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf22.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf23.jpg-1.png)
![text](https://github.com/RandelSouza/Analise_New_Reno_Vegas_Veno/blob/master/img/analisetcppdf24.jpg-1.png)

