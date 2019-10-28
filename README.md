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
* Ao termino da clonagem existirá um diretório chamado ns-3-allinone dentro do diretório ~/repos. Ao utilizar o comando `ls ns-3-allinone` para verificar o conteúdo do diretório, o resultado deve ser algo como:
```
build.py*  constants.py  dist.py*  download.py*  README  util.py
```
* Prosseguindo com o processo de instalação, foram baixados alguns scripts em Python. O próximo passo será usar estes scripts para baixar e construir a distribuição ns-3 de sua escolha. Neste trabalho utilizaremos a versão ns-3-dev que é a padrão, mas poderia ser utilizada qualquer outra release disponível no site http://code.nsnam.org/.

1. `cd ns-3-allinone/`
2. `sudo ./download.py -n ns-3-dev`

* O resultado da execução do comando anterior deve ter a seguinte saída (pode ser que o resultado seja diferente dependendo da verão escolhida):

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
