/*
 * leArqCSV.cpp
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
 * > Esse arquivo faz o controle dos menus e define os valores a serem
 * >salvos, inluindo a leitura dos pacotes.
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
#include <cstring>
#include <fstream>
#include <cstdlib>
#include "sequenceSet.hpp"


using namespace std;

// para saber o tamanho de cada campo, fiz um primeiro teste para
// pegar o tamanho máximo de cada campo do tipo texto
// adicionei mais uma posição para armazenar o término de string '\0'
struct pacote {
    
    static char chave;
    
    unsigned indice;
    float tempo;
    char origem[40];
    char destino[40];
    char protocolo[18];
    unsigned tamanho;
    char infomarcao[1650];


    friend bool operator >(const pacote &p0, const pacote &p1){ //define o operador >
        int comp;
        //operador muda de acordo com a chave usada
        switch (pacote::chave)
        {
            default:
                comp= strcmp(p0.protocolo, p1.protocolo);
                if(comp == 0)
                    return p0.indice > p1.indice;
                else
                    return comp > 0;
            case 'o':
                comp= strcmp(p0.origem, p1.origem);
                if(comp == 0)
                    return p0.indice > p1.indice;
                else
                    return comp > 0;
            case 'd':
                comp= strcmp(p0.destino, p1.destino);
                if(comp == 0)
                    return p0.indice > p1.indice;
                else
                    return comp > 0;
            case 't':
                if(p0.tempo != p1.tempo)
                    return p0.tempo > p1.tempo;
                else
                    return p0.indice > p1.indice;
            case 'T':
                if(p0.tamanho != p1.tamanho)
                    return p0.tamanho > p1.tamanho;
                else
                    return p0.indice > p1.indice;
        }
    }

    friend bool operator <(const pacote &p0, const pacote &p1){ //define o operador <
        int comp;
        //operador muda de acordo com a chave usada
        switch (pacote::chave)
        {
            default:
                comp= strcmp(p0.protocolo, p1.protocolo);
                if(comp == 0)
                    return p0.indice < p1.indice;
                else
                    return comp < 0;
            case 'o':
                comp= strcmp(p0.origem, p1.origem);
                if(comp == 0)
                    return p0.indice < p1.indice;
                else
                    return comp < 0;
            case 'd':
                comp= strcmp(p0.destino, p1.destino);
                if(comp == 0)
                    return p0.indice < p1.indice;
                else
                    return comp < 0;
            case 't':
                if(p0.tempo != p1.tempo)
                    return p0.tempo < p1.tempo;
                else
                    return p0.indice < p1.indice;
            case 'T':
                if(p0.tamanho != p1.tamanho)
                    return p0.tamanho < p1.tamanho;
                else
                    return p0.indice < p1.indice;
        }
    }

    friend bool operator ==(const pacote &p0, const pacote &p1){ //define o operador ==
        //o indicie e' semore diferente entao ele define igual
        return p0.indice == p1.indice;
    }

    friend bool operator !=(const pacote &p0, const pacote &p1){ //define o operador !=
        //o indicie e' semore diferente entao ele define diferente
        return p0.indice != p1.indice;
    }
    
    friend ostream & operator << (ostream &saida, const pacote &p){//define o operador <<
        //escreve define o que escrever dependedo da chave
        switch (pacote::chave)
        {
            default:
                saida << p.protocolo;
                break;
            case 'o':
                saida << p.origem;
                break;
            case 'd':
                saida << p.destino;
                break;
            case 't':
                saida << p.tempo;
                break;
            case 'T':
                saida << p.tamanho;
                break;
        }
        //indice e' sempre escrito
        saida << "(" << p.indice << ")";
        return saida;
    }
};
char pacote::chave;


string traduzChave(char c){ //escreve o que acada co'digo da chave significa
    
        switch(c)
        {
            case 'p':
                return "Protocolo";
            case 't':
                return "Tempo";
            case 'd':
                return "Destino";
            case 'o':
                return "Origem";
            case 'T':
                return "Tamanho";
            default:
                return "???";
        }
}

void insereChave(pacote* umPacote){ //define qual chave usar ao buscar/remover
        //mostra dependendo a chave
        switch(pacote::chave)
        {
            default:
                cout<< endl <<"Qual o protocolo?"<<endl;
                cin.ignore();
                cin.getline(umPacote->protocolo, 18);
                break;
            case 't':
                cout<< endl <<"Qual o tempo?"<<endl;
                cin.ignore();
                cin >>umPacote->tempo;
                cin.ignore();
                break;
            case 'd':
                cout<< endl <<"Qual o destino?"<<endl;
                cin.ignore();
                cin.getline(umPacote->destino, 40);
                break;
            case 'o':
                cout<< endl <<"Qual a origem?"<<endl;
                cin.ignore();
                cin.getline(umPacote->origem, 40);
                break;
            case 'T':
                cout<< endl <<"Qual o tamanho?"<<endl;
                cin >>umPacote->tamanho;
                cin.ignore();
                break;
        }    
    
}

void escolheChave(){ //cria o meno pra escolher qual chave usar no Set
    cout<<"Qual chave primaria deseja pra esse Set?"<<endl;
    cout<<"p- Protocolo"<<endl;
    cout<<"o- Origem"<<endl;
    cout<<"d- Destino"<<endl;
    cout<<"t- Tempo"<<endl;
    cout<<"T- Tamanho"<<endl;
    char chave;
    cin>> chave;
    pacote::chave = chave;
    cout<<"Chave escolhida: "<< traduzChave(pacote::chave) << endl;
}

void escreveTiltulo(){ //escreve a ascii art do titulo

cout<<"          _______  ___   _______  _______  ______      ______   _______    _______  _______  _______  _______  _______  _______  _______ " << endl;
cout<<"SUPER    |       ||   | |       ||       ||    _ |    |      | |       |  |       ||   _   ||  _____||       ||       ||       ||       |" << endl;
cout<<"|   |    |    ___||   | |_     _||   _   ||   | ||    |  _    ||    ___|  |    _  ||  |_|  || |      |   _   ||_     _||    ___||  _____|" << endl;
cout<<"|   |    |   |___ |   |   |   |  |  | |  ||   |_||_   | | |   ||   |___   |   |_| ||       || |      |  | |  |  |   |  |   |___ | |_____ " << endl;
cout<<"|   |___ |    ___||   |   |   |  |  |_|  ||    __  |  | |_|   ||    ___|  |    ___||       || |      |  |_|  |  |   |  |    ___||_____  |" << endl;
cout<<"|       ||   |___ |   |   |   |  |       ||   |  | |  |       ||   |___   |   |    |   _   || |_____ |       |  |   |  |   |___  _____| |" << endl;
cout<<"|_______||_______||___|   |___|  |_______||___|  |_|  |______| |_______|  |___|    |__| |__||_______||_______|  |___|  |_______||_______| " << endl;
}

void leArqcvs(unsigned de, unsigned ate, string arquivo, string nome){ //le um arquivo csv
    std::ifstream arquivo_csv(arquivo);
    if (not(arquivo_csv)) {
		throw runtime_error("Arquivo nao encontrado!");
    }
    
    ofstream outfile (nome);
    outfile.close();
    SequenceSet<pacote> s(70, nome, pacote::chave);
    
    string linha;
    getline(arquivo_csv,linha); // descarta primeira linha do arquivo

    string campo; // variável para obter um campo de cada linha lida
    string delimitador = "\",\""; // delimitador entre os campos
    unsigned posFimCampo; // posição final do campo

    pacote umPacote;
    
    unsigned i = 0;
    while(i < de and getline(arquivo_csv,linha)){ i++;}
	while(i < ate and getline(arquivo_csv,linha)) { //2076360
        campo = linha.erase(0,1); // remove primeiro caracter da linha (")
        // obtendo primeiro campo, um inteiro - No.
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.indice = stoul(campo);

        // obtendo segundo campo, um float - Time
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tempo = stof(campo);

        // obtendo terceiro campo, um texto - Source
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.origem, campo.c_str());

        // obtendo quarto campo, um texto - Destination
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.destino, campo.c_str());

        // obtendo quinto campo, um texto - Protocol
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        strcpy(umPacote.protocolo, campo.c_str());

        // obtendo sexto campo, um texto - Length
        posFimCampo = linha.find(delimitador);
        campo = linha.substr(0, posFimCampo);
        linha.erase(0, posFimCampo + delimitador.length());
        umPacote.tamanho = stoul(campo);

        // obtendo último campo, um texto - Info
        // nesse caso, procuro o último aspas, fazendo busca reversa
        posFimCampo = linha.rfind("\"");
        campo = linha.substr(0, posFimCampo);
        strcpy(umPacote.infomarcao, campo.c_str());

        // agora é com você, umPacote já tem o conteúdo de uma linha
        s.insere(umPacote);
        if(i % 101 == 0)
        {
            system("clear");
            cout << i - de<< "/" << ate - de << endl;
        }
        i++;
	}
    system("clear");
    cout << ate- de<< "/" << ate - de << endl;
    cout <<"Fim! os arquivos foram salvos em: " << nome << endl;
    cin.ignore();
}

void operarSet(string arquivo){ //menu para manipular um Set
    std::ifstream set(arquivo);
    if (not(set)) {
		throw runtime_error("Arquivo nao encontrado");
    }
    
    SequenceSet<pacote> s(70, arquivo);
    s.ler();
    pacote::chave = s.getChave();
    pacote umPacote;
    char entrada;
    cin.ignore();
    do{
        system("clear");
        escreveTiltulo();
        cout << endl;
        cout<<"------------------------------------------------" << endl;
        cout<<"Sequence Set " << arquivo << ":"<<endl;
        s.escreveSet();
        cout<<"Chave Prima'ria: " << traduzChave(s.getChave()) <<endl;
        cout <<"------------------------------------------------" << endl;
        cout <<"Como deseja manipula'-lo? " << endl;
        cout <<">digite 'i' para inserir" << endl;
        cout <<">digite 'b' para buscar"<< endl;
        cout <<">digite 'a' para alterar"<< endl;
        cout <<">digite 'r' para remover"<< endl;
        cout <<">digite 'm' para mostrar as informaçoes dos blocos [DEBUG]"<< endl;
        cout <<">digite 'f' para sair"<< endl;
        cin>>entrada;
        switch (entrada)
        {
            case 'a'://altera protocolo
                cout<<"Qual o indice?"<<endl;
                cin >> umPacote.indice;
                
                cout<< endl <<"Qual o protocolo?"<<endl;
                cin.ignore();
                cin.getline(umPacote.protocolo, 18);
                
                cout<< endl <<"Qual o tempo?"<<endl;
                cin >>umPacote.tempo;
                
                cout<< endl <<"Qual a origem?"<<endl;
                cin.ignore();
                cin.getline(umPacote.origem, 40);
                
                cout<< endl <<"Qual o destino?"<<endl;
                cin.getline(umPacote.destino, 40);
                
                cout<< endl <<"Qual o tamanho?"<<endl;
                cin >>umPacote.tamanho;
                
                cout<< endl <<"Qual a informaçao?"<<endl;
                cin.ignore();
                cin.getline(umPacote.infomarcao, 1650);
                try
                {
                    s.altera(umPacote);
                    cout << "Sucesso, pacote com novas informaçoes:"<<endl;
                    umPacote = s.busca(umPacote);
                    cout <<"Indice: " << umPacote.indice << endl;
                    cout <<"Tempo: " << umPacote.tempo << endl;
                    cout <<"Origem: " << umPacote.origem << endl;
                    cout <<"Destino: " << umPacote.destino << endl;
                    cout <<"Protocolo: " << umPacote.protocolo << endl;
                    cout <<"Tamanho: " << umPacote.tamanho << endl;
                    cout <<"Informacao: " << umPacote.infomarcao << endl;
                }
                catch(runtime_error &e)
                {
                    cout << "Pacote nao encontrado";
                }
                cin.ignore();
                break;
            case 'b'://busca
                cout<<"Qual o indice?"<<endl;
                cin >> umPacote.indice;
                insereChave(&umPacote);
                try
                {
                    umPacote = s.busca(umPacote);
                    cout<<"Pacote encontrado: "<<endl;
                    cout <<"Indice: " << umPacote.indice << endl;
                    cout <<"Tempo: " << umPacote.tempo << endl;
                    cout <<"Origem: " << umPacote.origem << endl;
                    cout <<"Destino: " << umPacote.destino << endl;
                    cout <<"Protocolo: " << umPacote.protocolo << endl;
                    cout <<"Tamanho: " << umPacote.tamanho << endl;
                    cout <<"Informacao: " << umPacote.infomarcao << endl;
                }
                catch(runtime_error &e)
                {
                    cout << "Pacote nao encontrado";
                }
                cin.ignore();
                break;
            case 'i'://insere
                cout<<"Qual o indice?"<<endl;
                cin >> umPacote.indice;
                
                cout<< endl <<"Qual o protocolo?"<<endl;
                cin.ignore();
                cin.getline(umPacote.protocolo, 18);
                
                cout<< endl <<"Qual o tempo?"<<endl;
                cin >>umPacote.tempo;
                
                cout<< endl <<"Qual a origem?"<<endl;
                cin.ignore();
                cin.getline(umPacote.origem, 40);
                
                cout<< endl <<"Qual o destino?"<<endl;
                cin.getline(umPacote.destino, 40);
                
                cout<< endl <<"Qual o tamanho?"<<endl;
                cin >>umPacote.tamanho;
                
                cout<< endl <<"Qual a informaçao?"<<endl;
                cin.ignore();
                cin.getline(umPacote.infomarcao, 1650);
                
                s.insere(umPacote);
                cout << "inserido!";
                cin.ignore();
                break;
            case 'r'://remove
                cout<<"Qual o indice do removido? ";
                cin >> umPacote.indice;
                insereChave(&umPacote);
                try
                {
                    cout<<"Pacote removido:" << endl ;
                    umPacote = s.remove(umPacote);
                    cout <<"Indice: " << umPacote.indice << endl;
                    cout <<"Tempo: " << umPacote.tempo << endl;
                    cout <<"Origem: " << umPacote.origem << endl;
                    cout <<"Destino: " << umPacote.destino << endl;
                    cout <<"Protocolo: " << umPacote.protocolo << endl;
                    cout <<"Tamanho: " << umPacote.tamanho << endl;
                    cout <<"Informacao: " << umPacote.infomarcao << endl;
                }
                catch(runtime_error &e)
                {
                    cout << "Pacote nao encontrado";
                }
                cin.ignore();
                break;
            case 'm'://imprime
                s.imprime();
                s.imprimeArv();
                cin.ignore();
                cin.ignore();
                break;
        }
        cout<<endl;
    }while(entrada != 'f');
}

int main(){
    char entrada = 0;
    string arquivo;
    string nome;
    //escreve o primeiro menu e as opçoes
    //escolhe as opçes tambe'm
    do
    {
        system("clear");
        escreveTiltulo();
        if(entrada == 0)
            cout<<"*Se o escrito acima estiver bagunçado, por favor aumente o tamanho horizontal do terminal." << endl;
        cout<< endl;
        cout<<"O que deseja fazer:"<<endl;
        cout<<"1 - Ler um novo CSV com pacotes."<<endl;
        cout<<"2 - Manipular um Sequence Set ja' criado."<<endl;
        cout<<"3 - Sair."<<endl;
        cin >> entrada;
        switch (entrada)
        {
            case '1':
                cout<<"Qual o nome do arquivo a ser lido?"<<endl;
                cin>> arquivo;
                cout<<"Qual o nome do sequence a ser criado?"<<endl;
                cin>> nome;
                unsigned inicio, fim;
                cout<<"Em qual pacote do arquivo deve começar a ser lido?"<<endl;
                cin>> inicio;
                cout<<"Em qual pacote do arquivo deve finalizar a leitura?"<<endl;
                cin>> fim;
                escolheChave();
                try
                {
                    leArqcvs(inicio, fim, arquivo, nome);
                    operarSet(nome);
                }
                catch(const runtime_error &e)
                {
                    cout << e.what();
                    cin.ignore();
                    cin.ignore();
                }
                break;
            case '2':
                cout<<"Qual o nome do sequence a ser manipulado?"<<endl;
                cin>> nome;
                try{
                    operarSet(nome);
                }
                catch(const runtime_error &e)
                {
                    cout << e.what();
                    cin.ignore();
                    cin.ignore();
                }
                break;
        }
    }while(entrada != '3');
   
    
    return 0;
}
