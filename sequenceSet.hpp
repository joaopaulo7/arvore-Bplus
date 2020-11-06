/*
 * sequenceSet.hpp
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
 * >Esse arquivo faz o controle do Sequence Set
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
#include "IndexTree.hpp"

template <class Tipo>
class SequenceSet;

template <class Tipo>
class Bloco{
    friend SequenceSet<Tipo>;
    private:
        //Elementos para a manipulaçao 
        //de memo'ria prima'ria
        Bloco *ant;
        Bloco *pos;
        
        //Elementos para a manipulaçao
        //de memoria secunda'ria
        unsigned anti = 0;
        unsigned posi = 0;
        
        //Controles para o Bloco
        unsigned tam = 0;
        unsigned maxTam;
        
        //Payload  do bloco
        //E' um ponteiro para receber um vetor
        //de dados de tamanho qualquer
        Tipo *itens;
        
        //DEBUG
        void imprime(){
            printf("%u", this->tam);
            printf("[");
            for (unsigned i = 0; i < this->tam; i++)
                std::cout << itens[i] << ",";
            printf("]\n");
        }
        //FIM DEBUG
        
        //METODOS AUXILIARES
        Tipo getUltimo(){  //retorna o u'ltimo valor guardado no  Bloco.
            if(tam > 0)
                return itens[tam-1];
            else
                throw std::out_of_range("O Bloco esta vazio.");
        }
        
        Tipo getPrimeiro(){ //retorna o primeiro valor guardado no  Bloco.
            if(tam > 0)
                return itens[0];
            else
                throw std::out_of_range("O Bloco esta vazio.");
        }
        
        bool temEspaco(){ //checa se o vetor nao esta' cheio.
            return maxTam > tam;
        }
        //FIM METODOS AUXILIARES
        
        //METODOS DE CONTROLE
        void insere(Tipo item){ //insere na orden correta.
            if(tam == 0)
                this->itens[0] = item;
            else
            {
                unsigned i = this->tam;
                //move os valores maiores que item ja colocados para manter a ordem
                while(i > 0 and this->itens[i-1] > item)
                {
                    this->itens[i] = this->itens[i-1];
                    i--;
                }
                itens[i] = item;
            }
            this->tam++;
        }
        
        Tipo removeUltimo(){ //remove o maior elemento do bloco.
            this->tam--;
            return this->itens[this->tam];
        }
        
        Tipo remove(Tipo item){ //remove determinado item do bloco.
            unsigned i = 0;
            
           while(i < this->tam-1 and !(itens[i] > item) and itens[i] != item)
                i++;
            
            Tipo saida = itens[i];
            
            //Se for o item requerido, remove e move os valores
            if(saida == item) 
            {
                this->tam--;
                while(i < this->tam)
                {
                    itens[i] = itens[i+1];
                    i++;
                }
            }
            else
                throw std::runtime_error("Objeto nao encontrado");

            return saida;
        }
        
        const Tipo busca(Tipo item){ //busca o item requerido no bloco.
            //Caso o item nao estiver no bloco, retorna o u'ltimo.
            unsigned i = 0;
            
            while(i < this->tam-1 and (itens[i] < item or itens[i] != item))
                i++;
            
            if(itens[i] != item)
                throw std::runtime_error("Objeto nao encontrado");
            
            return itens[i];
        }
        
        void altera(Tipo item){ //altera um  determinado item no bloco.
            unsigned i = 0;
            
            while(i < this->tam-1 and (itens[i] < item or itens[i] != item))
                i++;
                
            if(itens[i] == item)
                itens[i] = item;
            else
                throw std::runtime_error("Objeto nao encontrado");
        }
        //FIM METODOS DE CONTROLE
        
        //METODOS DE LEITURA E ESCRITA
        void salvar(std::fstream* arquivo){ //escreve os valores do bloco
            //os ponteiros sao dispensa'veis no arquivo bina'rio
            arquivo->write((char *) &this->maxTam, sizeof(this->maxTam));
            arquivo->write((char *) &this->tam, sizeof(this->tam));
            arquivo->write((char *) &this->anti, sizeof(this->anti));
            arquivo->write((char *) &this->posi, sizeof(this->posi));
            for(unsigned i = 0; i < this->tam; i++)
                arquivo->write((char *) &this->itens[i], sizeof(Tipo));
        }
    
        void ler(std::fstream* arquivo){//le os valores do bloco no arquivo
            //os ponteiros sao dispensa'veis no arquivo bina'rio
            arquivo->read((char *) &this->maxTam, sizeof(unsigned));
            arquivo->read((char *) &this->tam, sizeof(unsigned));
            arquivo->read((char *) &this->anti, sizeof(this->anti));
            arquivo->read((char *) &this->posi, sizeof(this->posi));
            for(unsigned i = 0; i < this->tam; i++)
                arquivo->read((char *) &this->itens[i], sizeof(Tipo));
        }
        //FIM METODOS DE LEITURA E ESCRITA
        
    public:
        Bloco(unsigned maxTam, Bloco<Tipo>* ant = NULL, Bloco<Tipo>* pos = NULL){ //inicia os valores
            this->ant = ant;
            this->pos = pos;
            this->maxTam = maxTam;
            //O vetor e' iniciado dinamicamente com o tamanho maximo informado.
            this->itens = new Tipo[maxTam]; 
        }
        
        ~Bloco(){//destroi o bloco
            //libera a memo'ria do ponteiro que carrega vetor.
            delete this->itens;
        }
};

template <class Tipo>
class SequenceSet{
    private:
        IndexTree<Tipo>* arvore;
    
        //elementos de estado
        std::fstream* arquivo;
        
        //controle de memoria secunda'ria
        unsigned raizi = 0;
        unsigned novi = 0;
        
        //quantidade de blocos tamanho dos blocos
        unsigned tam;
        unsigned tamBloco;
        
        char chave;
        
        
        //METODOS AUXILIARES
        unsigned sizeBloco(){ //retorna o tamanho de um bloco.
            return this->tamBloco*sizeof(Tipo) + 4*sizeof(unsigned);
        }
        
        Bloco<Tipo>* getProx(){ //retorna o porximo bloco a ser preenchido.
            //se estiver usando memoria primaria
            //ou nao houver espaço livre no arquivo
            //cria um bloco novo
            if(this->novi == 0)
            {
                this->tam++;
                Bloco<Tipo>* b = new Bloco<Tipo>(tamBloco);
                b->anti = tam; //a posiçao dele no arquivo 'e passada como a posiçao do anterior
                b->posi = 0;
                return b;
            }
            else
            {
                //se houver um espaço no arquivo, ele e' dado
                //pra ser preenchido. O proximo espaço livre
                //asume o lugar do primeiro.
                Bloco<Tipo> *aux = this->leBloco(this->novi);
                aux->anti = this->novi; //a posiçao dele no arquivo 'e passada como a posiçao do anterior
                this->novi = aux->posi;
                return aux;
            }
        }
        
        Bloco<Tipo>* divideElementos(Bloco<Tipo> *b){ //divide os elementos de um bloco com um novo.
            Bloco<Tipo>* novo = this->getProx();
            for(unsigned i = 0; i < this->tamBloco/2; i++)
                novo->insere(b->removeUltimo());
            return novo;
        }
        
        bool insereBloco(Bloco<Tipo>* aux, Tipo item, unsigned pos){//adiciona um elemento a um bloco e o divide se necessario.
            //Se houver espaço no bloco, o elemento e' adicionado
            if(aux->temEspaco())
                aux->insere(item);
            else
            {
                //se nao houver espaço, usa o "divide elementos"
                //para gerar o pro'ximo bloco.
                Bloco<Tipo>* aux1 = aux->pos;
                aux->pos = this->divideElementos(aux);
                
                //liga os elementos de controle da memoria prima'ria
                aux->pos->ant = aux;
                aux->pos->pos = aux1;
                if(aux1 != NULL)
                    aux1->ant = aux->pos; 
                
                //liga os elementos de controle da memoria secunda'ria
                unsigned auxi = aux->posi;
                aux->posi = aux->pos->anti;
                aux->pos->anti = pos;
                aux->pos->posi = auxi;
                if(aux1 != NULL)
                    aux1->anti = aux->posi;
                
                //adiciona o item no bloco
                if(aux->pos->getPrimeiro() < item)
                    aux->pos->insere(item);
                else
                    aux->insere(item);
                return true;
            }
            return false;
        }
        
        void espalhaBloco(Bloco<Tipo>* b){// divide um elemento entre o seu anterior e seu posterior
            if(b->pos != NULL and b->ant != NULL)
            {
                unsigned fim = b->tam/2;
                
                //remove o indice do bloco da a'rvore
                this->arvore->remover(b->itens[0]);
                
                Tipo index = b->pos->itens[0];
                
                for(unsigned i = 0; i < fim; i++)
                {
                    //se o bloco estourar ao inserir e estiver trabalhando na memo'ria secundaria
                    //salva o bloco que nao sera mais indexado por aux
                    if(this->insereBloco(b->pos, b->removeUltimo(), b->posi))
                        escreveBloco(b->pos->pos, b->pos->posi);
                }
                for(unsigned i = 0; i < fim; i++)
                {
                    //se o bloco estourar ao inserir e estiver trabalhando na memo'ria secundaria
                    //salva o bloco que nao sera mais indexado por aux
                    if(this->insereBloco(b->ant, b->removeUltimo(), b->anti))
                        escreveBloco(b->ant->ant, b->ant->anti);
                }
            
                //liga os elementos aos lado do retirado.
                unsigned pos = b->ant->posi;
            
                b->ant->pos = b->pos;
                b->pos->ant = b->ant;
                
                b->ant->posi = b->posi;
                b->pos->anti = b->anti;
                
                //altera o indice da a'rvore do proximo bloco
                //o que teve o menor alterado
                this->arvore->alterar(index, b->pos->itens[0]);
                
                //salva os blocos
                escreveBloco(b->pos, b->posi);
                escreveBloco(b->ant, b->anti);
                
                //Adiciona o retirado
                //nos espaços disponi'veis
                b->posi = this->novi;
                this->novi = pos;
                this->atualizaSeq();
            }
        }
        //FIM METODOS AUXILIARES
        
        //METODOS DE MEMORIA SECUNDARIA
        void atualizaSeq(){ //atualiza os valores do cabeçalho do Sequence Set
            
            arquivo->seekp(0);
            arquivo->write((char *) &this->tam, sizeof(this->tam));
            arquivo->write((char *) &this->tamBloco, sizeof(this->tamBloco));
            arquivo->write((char *) &this->raizi, sizeof(this->tamBloco));
            arquivo->write((char *) &this->novi, sizeof(this->tamBloco));
            arquivo->write((char *) &this->chave, sizeof(char));
        }
        
        Bloco<Tipo>* leBloco(unsigned posi){ //le retorna um objeto bloco com os valores lidos
            if(posi == 0)
                return NULL;
            
            //ao usar 0 como valor nulo da posiçao, devemos ajusta-las para
            //nao inutilizar um pedaço do arquivo.
            posi--; 
            
            //encontra o bloco requerido.
            arquivo->seekp(this->sizeBloco() * posi + sizeof(unsigned)*4 + sizeof(char));
            
            //cria um objeto e deposita os valore lidos nele.
            Bloco<Tipo>* aux = new Bloco<Tipo>(this->tamBloco);
            aux->ler(arquivo);
            return aux;
        }
        
        void escreveBloco(Bloco<Tipo>* b, unsigned posi){ //escreve no arquivo e apaga um bloco da memoria prima'ria
            
            //encontra a posiçao do bloco a ser salvo e salva
            arquivo->seekp(this->sizeBloco() * (posi-1) + sizeof(unsigned)*4+ sizeof(char));
            b->salvar(arquivo);
            delete b;
        }
        //FIM METODOS DE MEMORIA SECUNDARIA
        
        
    public:
        
        SequenceSet(unsigned tamBloco, std::string arquivo, char chave = 'p'){ //cria e define o Sequence Set
            //cria a arvore para o sequence set num arquivo novo
            this->arvore = new IndexTree<Tipo>(4, arquivo+"-arvoreIndex.bin");
            this->arquivo = new std::fstream(arquivo);
            this->tamBloco = tamBloco;
            this->tam = 0;
            this->chave = chave;
        }
        
        ~SequenceSet(){ //apaga os blocos da memoria e destroi o Sequence Set
            this->arquivo->close();
            delete this->arquivo;
            delete arvore;
        }
        
        void ler(){ //le o Sequence Set do Arquivo
            this->arquivo->seekp(0);
            this->arquivo->read((char *) &this->tam, sizeof(unsigned));
            this->arquivo->read((char *) &this->tamBloco, sizeof(unsigned));
            this->arquivo->read((char *) &this->raizi, sizeof(unsigned));
            this->arquivo->read((char *) &this->novi, sizeof(unsigned));
            this->arquivo->read((char *) &this->chave, sizeof(char));
            //le o cabeçalho da arvore de indices
            this->arvore->ler();
        }
        
        //METODOS DE CONTROLE
        void alterarArquivo(std::string arquivo){ //altera os arquivos a serem lidos
            //fecha e libera a memo'ria, apos le define os novos arquivos
            this->arquivo->close();
            delete this->arquivo;
            this->arquivo = new std::fstream(arquivo);
            delete this->arvore;
            this->arvore->alteraArquivo(arquivo+"-arvoreIndex.bin");
        }
        
        void insere(Tipo item){ //inserir quando usando a memo'ria secund'aria
            
            unsigned pos = this->raizi;
            Bloco<Tipo> *aux;
            
            //nao for o primeiro do arquivo
            if(this->raizi != 0){
                //busca o local do bloco a ser adicionado na a'rvore
                pos = this->arvore->buscar(item);
                aux = this->leBloco(pos);
            }
            else
            {
                //cria um novo bloco se for o primeiro
                aux = getProx();
                this->raizi = aux->anti;
                aux->anti = 0;
                pos= raizi;
                this->arvore->inserir(item, pos);
            }
            
            aux->pos = leBloco(aux->posi);
            if(aux->pos !=NULL)
                aux->pos->ant = aux;
                
            //se um novo bloco for criado ao inserir
            //salva e apaga esse bloco
            if(this->insereBloco(aux, item, pos))
            {
                if(aux->pos->pos != NULL)
                    this->escreveBloco(aux->pos->pos, aux->pos->posi);
                
                //adiciona o novo bloco 'a a'rvore
                this->arvore->inserir(aux->pos->itens[0], aux->posi);
                this->escreveBloco(aux->pos, aux->posi);
            }
            else
                delete aux->pos;
            
            //escreve o bloco e atualiza o cabeçalho
            this->escreveBloco(aux, pos);
            this->atualizaSeq();
        }
        
        const Tipo busca(Tipo item){ //buscar quando usando a memo'ria secund'aria
            
            if(this->tam < 1)
                throw std::runtime_error("Sequence Set Vazio");
                
            Bloco<Tipo> *aux = leBloco(this->raizi);
            Tipo auxt;
            
            //busca o bloco com o valor na a'rvore
            aux = this->leBloco(this->arvore->buscar(item));
                    
            auxt = aux->busca(item);
            
            //libera a memoria primaria e retorna
            delete aux;
            return auxt;
        }
        
        void altera(Tipo item){ //alterar quando usando a memo'ria secund'aria
            if(this->tam < 1)
                throw std::runtime_error("Sequence Set Vazio");
                
            Bloco<Tipo> *aux = leBloco(this->raizi);
            unsigned pos = this->raizi;
            
            //busca o bloco com o valor na a'rvore
            pos = this->arvore->buscar(item);
            aux = this->leBloco(pos);
                    
            aux->altera(item);
            
            //escreve o bloco e apaga da memoria primaria
            //atualiza o cabeçalho
            this->escreveBloco(aux, pos);
            this->atualizaSeq();
        }
        
        Tipo remove(Tipo item){ //remover quando usando a memo'ria secund'aria
            if(this->tam < 1)
                throw std::runtime_error("Sequence Set Vazio");
                
            Bloco<Tipo> *aux = leBloco(this->raizi);
            Tipo auxt;
            unsigned pos = this->raizi;
            
            //busca o bloco com o valor na a'rvore
            pos = this->arvore->buscar(item);
            aux = this->leBloco(pos);
                    
            auxt = aux->remove(item);
                
            //se o bloco for menor que o limite
            //ele e' espalhado prar proximo e anterior
            if(aux->tam < this->tamBloco/2)
            {
                //le o anterior e posteior usados na 
                //memoria prima'ria no espalha bloco
                aux->ant = leBloco(aux->anti);
                if(aux->ant != NULL)
                    aux->ant->pos = aux;
                aux->pos = leBloco(aux->posi);
                this->espalhaBloco(aux);
            }
            
            //escreve o bloco e atualiza o cabeçalho
            this->escreveBloco(aux, pos);
            this->atualizaSeq();
            return auxt;
        }
        
        void setArquivo(std::string arquivo){ //define o arquivo a ser utilizado.
            this->arquivo->close();
            delete this->arquivo;
            this->arquivo = new std::fstream(arquivo);
        }
       
        char getChave(){ //retorna qual a chave prim'aria do Set 
            return this->chave;
        }
        
        void escreveSet(){ //escreve as informaçoes do set
            std::cout <<"Quantidade de Blocos: " << this->tam << std::endl;  
            std::cout <<"Tamanho doo Set em MB: " << ((float)this->tam*this->sizeBloco() + 5*sizeof(unsigned))/1000000 << std::endl;
            std::cout <<"Tamanho dos Blocos: " << this->tamBloco << std::endl;
            std::cout <<"Tamanho dos Blocos em MB: " << (float)this->sizeBloco()/1000000 << std::endl;
        }
        //FIM METODOS DE CONTROLE
        
        //DEBUG
        void imprime(){ //imprime blocos, tamanho, anterior, proximo e informaçoes
            
            unsigned posAux;
            Bloco<Tipo>* aux = this->leBloco(this->raizi);
            //percorre o arquivo
            while(aux !=NULL)
            {
                //imprime informaçoes
                printf("[%u / %u]", aux->anti, aux->posi);
                aux->imprime();
                printf("->");
                posAux = aux->posi;
                //passa para o proximo bloco
                delete aux;
                aux =  this->leBloco(posAux);
            }
        }
        
        void imprimeArv(){//imprime a a'rvore
            this->arvore->imprime();
        }
        //FIM DEBUG
};


