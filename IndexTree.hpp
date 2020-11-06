/*
 * IndexTree.hpp
 * 
 * Trabalho de Estruturas de Dados
 * 
 * Sequence Set & 'arvore B+
 * 
 * Copyright 2020:
 *    Luiz Felipe Baptista Serapião <luiz.serapiao@estudante.ufla.br>
 *    João Paulo Paiva Lima <joao.lima1@estudante.ufla.br>
 *    Ruan Lourençoni Machado <ruan.machado@estudante.ufla.br>
 * 
 * >Esse arquivo faz o controle a Arvore B+
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
 
#include <iostream>
#include <fstream>

template<class Index>
class IndexTree;

template<class Index>
class BlocoInd{
    friend IndexTree<Index>;
    private:
        unsigned tam= 0; 
        
        //indices do bloco
        Index *indexes;
        
        //as posiçoes dos filhos e do pai
        unsigned pai = 0;
        unsigned *posicoes;
        
        void leInfo(std::fstream *arquivo){ //le as informaçoes salvas
            
            arquivo->read((char *) &this->pai, sizeof(unsigned));
            arquivo->read((char *) &this->tam, sizeof(unsigned));
            
            //le os indices
            for(unsigned i = 0; i < this->tam; i++)
                arquivo->read((char *) &this->indexes[i], sizeof(Index));
                
            //le as posiçoes dos filhos
            for(unsigned i = 0; i < this->tam + 1; i++)
                arquivo->read((char *) &this->posicoes[i], sizeof(unsigned));
        }
        
        void escrInfo(std::fstream *arquivo){//escreve as informaoes do bloco
            
            arquivo->write((char *) &this->pai, sizeof(unsigned));
            arquivo->write((char *) &this->tam, sizeof(unsigned));
            
            //escreve os indices
            for(unsigned i = 0; i < this->tam; i++)
                arquivo->write((char *) &this->indexes[i], sizeof(Index));
                
            //escreve os filhos
            for(unsigned i = 0; i < this->tam + 1; i++)
                arquivo->write((char *) &this->posicoes[i], sizeof(unsigned));
        }
        
        //METODOS DE CONTROLE
        bool insere(Index index, unsigned pos){ //insere na orden correta.
            if(this->tam == 0)
            {
                //se for o primeiro, adiciona no primeiro lugar
                this->indexes[0] = index;
                this->posicoes[1] = pos;
                this->tam++;
                return false;
            }
            else
            {
                unsigned i = this->tam;
                //se o bloco nao estiver vazio,
                //move os valores maiores que item ja colocados para manter a ordem
                while(i > 0 and !(this->indexes[i-1] < index))
                {
                    this->indexes[i] = this->indexes[i-1];
                    this->posicoes[i+1] = this->posicoes[i];
                    i--;
                }
                
                //adiciona no devido lugar
                //as posiçoes sao salvas uma unidade depois dos indices
                indexes[i] = index;
                posicoes[i+1] = pos;
                
                //aumenta o tamanho
                this->tam++;
                
                if(i == 0)
                    return true;
                return false;
            }
        }
        
        Index removeUltimo(){ //remove o maior indice do bloco.
            this->tam--;
            return this->indexes[this->tam];
        }
        
        Index removePrimeiro(){ //remove o menor indice do bloco.
            unsigned i = 0;
            //salva o index a ser retirado
            Index saida = this->indexes[0];
            this->tam--;
            
            //move os elementos para tras pra reconfigurar a seuquencia
            while(i < this->tam)
            {
                indexes[i] = indexes[i+1];
                posicoes[i] = posicoes[i+1];
                i++;
            }
            //por ser maior, os filhos preciam se mover mais uma vez
            //por o vetor ser maior 
            posicoes[i] = posicoes[i+1];
            return saida;
        }
        
        unsigned getUltPos(){ //retorna qual o u'ltimo filho
            return this->posicoes[this->tam];
        }
        
        Index getUltIndex(){ //retorna o u'ltimo indice
            return this->indexes[this->tam-1];
        }
        
        unsigned getProxPos(Index index){ //retorna o filho da direita ao um dado indice
            unsigned i = 0;
            //busca o indice
            while(i < this->tam and indexes[i] < index)
                i++;
            
            //retorna a posicao do proximo filho
            //se nao houver filho 'a direita, retorna zero(nulo)
            if(i < this->tam)
                return posicoes[i + 1]; 
            else
                return 0;
        }
        
        unsigned getAntPos(Index index){ //retorna o filho da direita ao um dado indice
            unsigned i = 0;
            //busca o indice
            while(i < this->tam and indexes[i] < index)
                i++;
            
            //retorna a posiçao do filho anterior
           return posicoes[i-1]; 
        }
        
        unsigned buscar(Index index){ //buca e retorna a posiçao de um filho
            unsigned i = 0;
            //busca o indice
            while(i < this->tam and indexes[i] < index)
                i++;
            
            //retorna o filho
            return this->posicoes[i];
        }
        
        bool alterar(Index index, Index novo){ //altera um indice especifico
            unsigned i = 0;
            //busca o indice a ser trocado
            while(i < this->tam-1 and indexes[i] < index)
                i++;
           
            //muda o indice
            this->indexes[i] = novo;
            if(i == 0)
                return true;
            return false;
        }
        
        Index remove(Index index){ //remove determinado item do bloco.
            unsigned i = this->tam -1;
            
           while(i > 0 and indexes[i] > index)
                i--;
            
            Index saida = indexes[i];
            
            //Remove e move os valores
            this->tam--;
            
            while(i < this->tam)
            {
                indexes[i] = indexes[i+1];
                posicoes[i+1] = posicoes[i+2];
                i++;
            }
            posicoes[i+1] = posicoes[i+2];
            return saida;
        }
        //FIM METODOS DE CONTROLE
        
    public:
        BlocoInd(unsigned maxTam, unsigned pai = 0){ //cria o bloco, seta os valores e cria os vetores
            this->indexes = new Index[maxTam];
            this->posicoes = new unsigned[maxTam+1];
            this->pai = pai;
        }
        
        ~BlocoInd(){//deletea os vetores de indices e filhos, que sao ponteiros
            delete this->indexes;
            delete this->posicoes;
        }
        
        void imprime(){ //escreve as informaçoes do bloco.
            //imprime o pai e o tamanho
            std::cout  <<"("<<this->pai<<")"<<this->tam<<"[";
            
            //imprime os indices e entre '{}' os filhos
            for(unsigned i = 0; i < this->tam; i ++)
                std::cout <<"{"<<this->posicoes[i] <<"}, " << this->indexes[i].indice;
            std::cout <<" {"<< posicoes[this->tam] << "}]";
        }
};

template<class Index>
class IndexTree{
    friend BlocoInd<Index>;
    private:
        //varia'veis para controle
        unsigned profundidade = 0;
        unsigned tamBloco;
        unsigned tam = 0;
        
        //varia'veis para localizaçao no arquivo
        unsigned raiz = 0;
        unsigned novo = 0;
        
        //vari'avel para manipuaçao do arquivo
        std::fstream* arquivo;
        
        BlocoInd<Index>* getProx(){ //retorna um novo bloco na posiçao disponi'vel
            if(this->novo == 0)
            {
                // se nao houver algum lugar disponivel, cria um novo bloco
                //no final do arquivo 
                this->tam++;
                //a posiçao do bloco no arquivo e' devolvida pele variavel do pai
                BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco, this->tam);
                return aux;
            }
            else
            {
                // se houver algum lugar disponivel, cria um novo bloco
                //na posiçao livre do arquivo
                BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco, this->novo);
                // le as informaçoes do lugar disponivel
                //nesse lugar esta a posiçao do proximo disponivel
                this->leBloco(aux, this->novo);
                unsigned auxPos = aux->pai;
                aux->pai = this->novo;
                //o proximo disponivel vira o novo primeiro disponivel
                this->novo = auxPos;
                return aux;
            }
        }
        
        void escreveCabeca(){ //escreve as iformaçoes do cabeçalho da a'rvore
            arquivo->seekp(0);
            arquivo->write((char *) &this->tam, sizeof(this->tam));
            arquivo->write((char *) &this->tamBloco, sizeof(this->tamBloco));
            arquivo->write((char *) &this->profundidade, sizeof(this->profundidade));
            arquivo->write((char *) &this->raiz, sizeof(this->raiz));
            arquivo->write((char *) &this->novo, sizeof(this->novo));
        }
        
        void apagaBloco(BlocoInd<Index> *bloco, unsigned pos){ //libera aposiçao do bloco
            //define a posiçao do bloco como a pro'xima disponi'vel
            bloco->pai = this->novo;
            this->escreveBloco(bloco, pos);
            this->novo = pos;
        }
        
        unsigned sizeBloco(){ //retorna o tamanho dos blocos
            return (sizeof(Index)+sizeof(unsigned)) * this->tamBloco + sizeof(unsigned)*3;
        }
        
        void leBloco(BlocoInd<Index> *aux, unsigned pos){ //le as informaçoes do bloco para o ponteiro
            if(pos == 0)
                aux = NULL;
            else
            {
                //encontra o local em que esta salvo
                arquivo->seekp(this->sizeBloco() * (pos-1) + sizeof(unsigned)*5);
                aux->leInfo(this->arquivo);
            }
        }
        
        void escreveBloco(BlocoInd<Index> *aux, unsigned pos){ //escreve as informaçoes do bloco no ponteiro
            //encontra a posiçao do bloco
            arquivo->seekp(this->sizeBloco() * (pos-1) + sizeof(unsigned)*5);
            
            //escreve o bloco
            aux->escrInfo(this->arquivo);
        }
        
        void alteraPai(unsigned pai, unsigned pos){ //altera o pai de um bloco
            //encontra a posiçao do bloco
            arquivo->seekp(this->sizeBloco() * (pos-1) + sizeof(unsigned)*5);
            
            //muda o pai do bloco
            arquivo->write((char *) &pai, sizeof(unsigned));
        }
        
        void insereBloco(BlocoInd<Index> *aux, Index index, unsigned novPos, unsigned pos){ //insere no bloco
            bool alteracao = false;
            
            //laço para subir as alteraçoes na a'rvore
            do
            {
                //se nao for  a raiz depois de alterçoes
                if(novPos != 0 and pos != 0)
                {
                    //ve se tem espaço no bloco
                    if(aux->tam < this->tamBloco)
                    {
                        //insere no bloco
                        aux->insere(index, novPos);
                        
                        //escreve o bloco
                        this->escreveBloco(aux, pos);
                        novPos = 0;
                        //essa inserçao nao altera a configuraçao da a'rvore
                        alteracao = false;
                    }
                    else
                    {
                        //se nao houver espaço
                        //separa uma nova posiçao para um novo bloco
                        BlocoInd<Index> *aux1 = this->getProx();
                        unsigned auxPos = 0;
                        
                        //divide os elementos entre os blocos, velho e novo
                        for(unsigned i = 0; i < this->tamBloco/2; i++)
                        {
                            auxPos = aux->getUltPos();
                            aux1->insere(aux->removeUltimo(), auxPos);
                            //altera o pais dos filhos, desde que nao seja folha
                            if(alteracao)
                                this->alteraPai(aux1->pai, auxPos);
                        }
                        
                        if(aux1->indexes[0] < index)
                        {
                            // se o elemento a ser adicionado for maior que a mediana do bloco
                            //adiciona no novo bloco
                            aux1->insere(index, novPos);
                            
                            //separa o primeiro indice para subir a arvore
                            index = aux1->removePrimeiro();
                            
                            //se nao for folha, altera o pai do novo
                            if(alteracao)
                                this->alteraPai(aux1->pai, novPos);
                        }
                        else
                        {
                            // se o elemento a ser adicionado for menor que a mediana do bloco
                            //adiciona no bloco antigo
                            aux->insere(index, novPos);
                            
                            //ajusta a posiçao do elemento que subira'
                            aux1->posicoes[0] = aux->getUltPos();
                            
                            //separa o u'ltimo indice para subir a arvore
                            index = aux->removeUltimo();
                            
                            //se nao for folha, altera o pai do novo
                            if(alteracao)
                            {
                                this->alteraPai(pos, novPos);
                                this->alteraPai(aux1->pai, aux1->posicoes[0]);
                            }
                        }
                        
                        //prepara as posiçoes para subir com as mudanças
                        novPos = aux1->pai;
                        auxPos = aux->pai;
                        
                        //escreve as alteraçoes do bloco antigo
                        this->escreveBloco(aux, pos);
                        
                        //prepara as posiçoes para subir com as mudanças
                        pos = auxPos;
                        
                        //escreve as o novo bloco
                        aux1->pai = pos;
                        this->escreveBloco(aux1, novPos);
                        
                        //libera a memo'ria
                        delete aux1;
                        
                        //le o pai para subir com as mudanças
                        this->leBloco(aux, pos);
                        
                        //mantem o laço
                        alteracao = true;
                    }
                }
                else if(novPos != 0)
                {
                    // se for a raiz
                    //encontra a nova posiçao disponi'vel
                    //para criar uma nova raiz
                    aux = this->getProx();
                    
                    //insere os novos filhas 'a nova raiz
                    aux->posicoes[0] = this->raiz;
                    aux->insere(index, novPos);
                    
                    //atualiza a raiz no cabeçalho
                    this->raiz = aux->pai;
                    
                    //altera os pais da raiz antiga e o novo bloco
                    this->alteraPai(this->raiz, aux->posicoes[0]);
                    this->alteraPai(this->raiz, novPos);
                    
                    //escreve a nova raiz
                    pos = aux->pai;
                    aux->pai = 0;
                    this->escreveBloco(aux, pos);
                    
                    //atualiza a profundidade
                    this->profundidade++;
                    
                    //diz que e' o fim das alteraçoes
                    alteracao = false;
                }
            }
            while(alteracao);
            
            //libera a memo'ria
            delete aux;
        }

        void removeBloco(BlocoInd<Index> *aux, Index index, unsigned pos){ //remove do bloco
            bool alteracao = false;
            
            //remove o elemento do bloco
            aux->remove(index);
            if(aux->tam < this->tamBloco/2)
            {
                //se o bloco nao for menor que o minimo
                BlocoInd<Index> *pai = new BlocoInd<Index>(this->tamBloco);
                BlocoInd<Index> *aux1 = new BlocoInd<Index>(this->tamBloco);
                
                //laço para subir com as alteraçoes
                do
                {
                    if( aux->pai != 0 and aux->tam < this->tamBloco/2)
                    {
                        //se nao for a raiz
                        //le as informaçoes do pai
                        unsigned posAux1;
                        this->leBloco(pai, aux->pai);
                        
                        //encontra o pro'ximo filho
                        posAux1 = pai->getProxPos(aux->indexes[0]);
                        unsigned auxPos = posAux1;
                        if(posAux1 == 0)
                        {
                            //se nao houver proximo filho
                            //le o filho anterior
                            posAux1 = pai->getAntPos(aux->indexes[0]);
                            this->leBloco(aux1, posAux1);
                            
                            //troca os blocos para fazer as alteraçoes
                            if(aux1->tam <= this->tamBloco/2)
                            {
                                std::swap(aux, aux1);
                                std::swap(pos, posAux1);
                            }
                        }
                        else
                            //le o bloco do proximo filho
                            this->leBloco(aux1, posAux1);
                        
                        if(aux1->tam <= this->tamBloco/2 and aux->tam <= this->tamBloco/2)
                        {
                            //se o filho ao lado esta na metade
                            //o filho retirado recebe o indice do pai que leva para o filho ao lado
                            aux->insere(pai->remove(aux1->indexes[0]), aux1->posicoes[0]);
                            this->alteraPai(pos, aux1->posicoes[0]);
                            
                            //os valores do filho ao lado sao passados para o filho retirado
                            while(aux1->tam > 0)
                            {
                                auxPos = aux1->posicoes[1];
                                aux->insere(aux1->removePrimeiro(), auxPos);
                                if(alteracao)
                                    this->alteraPai(pos, auxPos);
                            }
                            
                            //apaga o filho ao lado e escreve as mudanças do filho retirado
                            this->apagaBloco(aux1, posAux1);
                            this->escreveBloco(aux, pos);
                            pos = aux->pai;
                            this->escreveBloco(pai, pos);
                            
                            //prepara pa subir a alteraçao
                            this->leBloco(aux, pos);
                            alteracao = true;
                        }
                        else if(auxPos != 0)
                        {
                            //se o filho nao e' o u'ltimo
                            //passa um elemento do pai para o filho 'a esquerda
                            aux->insere(pai->remove(aux1->indexes[0]), aux1->posicoes[0]);
                            
                            //passa um  elemento do filho 'a direita para o pai
                            pai->insere(aux1->removePrimeiro(), posAux1);
                            
                            //slava e diz que a alteraçao nao precisa subir
                            this->escreveBloco(aux1, posAux1);
                            this->escreveBloco(aux, pos);
                            this->escreveBloco(pai, aux->pai);
                            alteracao = false;
                        }
                        else
                        {
                            //se o filho nao e' o u'ltimo
                            //passa um elemento do pai para o filho 'a direita
                            aux->insere(pai->remove(aux->indexes[0]), aux->posicoes[0]);
                            
                            //passa o primeiro filho da direita pra esqueda
                            aux->posicoes[0] = aux1->getUltPos();
                            
                            //passa um  elemento do filho 'a esquerda para o pai
                            pai->insere(aux1->removeUltimo(), pos);
                            
                            //slava e diz que a alteraçao nao precisa subir
                            this->escreveBloco(aux1, posAux1);
                            this->escreveBloco(aux, pos);
                            this->escreveBloco(pai, aux->pai);
                            alteracao = false;
                        }
                    }
                    else if(aux->pai == 0)
                    {
                        //se for a raiz
                        //testa se a raiz esta vazia
                        if(aux->tam == 0)
                        {
                            //se a raiz esta vazia
                            //atualiza a raiz
                            this->raiz = aux->posicoes[0];
                            
                            //ajusta o pai da nova raiz
                            this->alteraPai(0, this->raiz);
                            
                            //ajusta a profundidade
                            this->profundidade--;
                            
                            //libera a posiçao no arquivo
                            this->apagaBloco(aux, pos);
                        }
                        
                        //diz que nao ha' mais alteraçoes a serem feitas
                        alteracao = false;
                    }
                    else
                        //se o bloco nao estiver no final
                        //diz que nao ha' mais alterçoes
                        alteracao = false;
                }while(alteracao);
                
                //libera a memoria
                delete pai;
                delete aux1;
            }
            else
                //escreve o bloco alterado
                this->escreveBloco(aux, pos);
            //libera a memo'ria
            delete aux;
        }

    public:
    
        IndexTree(unsigned maxTam, std::string arquivo){ //cria a arvore
            this->tamBloco = maxTam;
            this->arquivo = new std::fstream(arquivo);
            //se o arquivo nao existe, cria um novo
            if(!this->arquivo->good())
            {
                std::ofstream outfile (arquivo);
                outfile.close();
                delete this->arquivo;
                this->arquivo = new std::fstream(arquivo);
            }
        }
        
        ~IndexTree(){ //apaga a arvore
            //fecha o arquivo e libera a memo'ria
            this->arquivo->close();
            delete this->arquivo;
        }
        
        void alteraArquivo(std::string arquivo){ //muda o arquivo a ser lido
            this->arquivo->close();
            delete this->arquivo;
            this->arquivo = new std::fstream(arquivo);
            //se o arquivo nao existe, cria um novo
            if(!this->arquivo->good())
            {
                std::ofstream outfile (arquivo);
                outfile.close();
                delete this->arquivo;
                this->arquivo = new std::fstream(arquivo);
            }
        }
        
        void ler(){ //le as informaçoes salvas no arquivo
            arquivo->seekp(0);
            arquivo->read((char *) &this->tam, sizeof(this->tam));
            arquivo->read((char *) &this->tamBloco, sizeof(this->tamBloco));
            arquivo->read((char *) &this->profundidade, sizeof(this->profundidade));
            arquivo->read((char *) &this->raiz, sizeof(this->raiz));
            arquivo->read((char *) &this->novo, sizeof(this->novo));
        }
        
        unsigned buscar(Index index){ //busca a posiçao de um bloco
            if(this->profundidade == 0)
                return 1;
                
            BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco);
            
            this->leBloco(aux, this->raiz);
            unsigned auxPos = this->raiz;
            
            //percorre a a'rvore ate encontrar a folha
            for(unsigned i = 0; i < this->profundidade - 1; i++)
            {
                auxPos = aux->buscar(index);
                this->leBloco(aux, auxPos);
            }
            //prepara a saida
            unsigned  saida = aux->buscar(index);
            
            //libera a memoria e retorna a posiçao
            delete aux;
            return saida;
        }
        
        void inserir(Index index, unsigned pos){ //insere um indice e posiçao
            BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco);
            if(this->raiz == 0)
            {
                //se for o primeiro, cria um bloco e define a primeira posiçao e a raiz
                this->profundidade++;
                aux = getProx();
                this->raiz = aux->pai;
                aux->pai = 0;
                aux->posicoes[0] = pos;
                this->escreveBloco(aux, this->raiz);
                delete aux;
            }
            else
            {
                this->leBloco(aux, this->raiz);
                
                //percorre a a'rvore ate encontrar a folha
                unsigned auxPos = this->raiz;
                for(unsigned i = 0; i < this->profundidade - 1; i++)
                {
                    auxPos = aux->buscar(index);
                    this->leBloco(aux, auxPos);
                }
                //insere na folha, a memoria e' liberada na funçao
                this->insereBloco(aux, index, pos, auxPos);
            }
            //atualiza o cabeçalho no arquivo
            this->escreveCabeca();
        }
        
        void remover(Index index){ //remove um indice
            BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco);
            if(this->raiz == 0)
                    throw std::runtime_error("index nao encontrado");
            else
            {
                this->leBloco(aux, this->raiz);
                
                //percorre a a'rvore ate encontrar a folha
                unsigned auxPos = this->raiz;
                for(unsigned i = 0; i < this->profundidade - 1; i++)
                {
                    auxPos = aux->buscar(index);
                    this->leBloco(aux, auxPos);
                }
                //remove o indice da folha e ajusta a arvore
                //a memo'ria e' liberada na funçao
                this->removeBloco(aux, index, auxPos);
            }
             //atualiza o cabeçalho no arquivo
            this->escreveCabeca();
        }
        
        void alterar(Index index, Index novoIndex){ //altera um indice de um bloco
            BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco);
            if(this->raiz == 0)
                    throw std::runtime_error("index nao encontrado");
            else
            {
                this->leBloco(aux, this->raiz);
                
                //percorre a a'rvore ate encontrar a folha
                unsigned auxPos = this->raiz;
                for(unsigned i = 0; i < this->profundidade - 1; i++)
                {
                    auxPos = aux->buscar(index);
                    this->leBloco(aux, auxPos);
                }
                //altera e salva o bloco
                aux->alterar(index, novoIndex);
                this->escreveBloco(aux, auxPos);
                
                //libera a memoria
                delete aux;
            }
            //atualiza o cabeçalho
            this->escreveCabeca();
        }
        
        void imprimeGalho(unsigned prof, unsigned pos){ //imprime os indices e as posiçoes da arvore
            if(prof < this->profundidade)
            {
                BlocoInd<Index> *aux = new BlocoInd<Index>(this->tamBloco);
                this->leBloco(aux, pos);
                
                for(unsigned i = 0; i < prof; i ++)
                    std::cout << " ";
                
                //imprime o bloco
                std::cout << pos;
                aux->imprime();
                std::cout << std::endl;

                //percorre toda a arvore recursivamente
                for(unsigned i = 0; i< aux->tam; i++)
                {
                    this->imprimeGalho(prof + 1, aux->posicoes[i]);
                }
                this->imprimeGalho(prof + 1, aux->getUltPos());
                delete aux;
            }
        }
        
        void imprime(){ //imprime a arvore
            //começa a recursao
            this->imprimeGalho(0, this->raiz);
        }
};
