#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "Trabalho.h"

using namespace std;


void processamento()
{
    cout << "Lendo registros. Por favor, aguarde." << endl;
    std::ifstream ip("tiktok_app_reviews.csv", std::ifstream::binary);
    fstream op;
    op.open("tiktok_app_reviews.bin", ios::out | ios::binary);
    string aux;
    int cont = 0;
    ip.seekg(0, ip.end);
    int length = ip.tellg();
    ip.seekg(0, ip.beg);
    getline(ip, aux);
    while ((int)(ip.tellg()) != -1 && !ip.eof()) {
        //id
        Review* reg = new Review;
        string* id = new string;
        getline(ip, *id, ',');
        int i;
        for (i = 0; i < (int)id->length(); i++) {
            reg->review_id[i] = (*id)[i];
        }
        reg->review_id[i] = '\0';
        delete id;

        //text
        char c;
        ip >> c;
        if (c == '"') {
            char text[2000];
            for (i = 0; i < sizeof(reg->review_text); i++) {
                ip >> noskipws >> text[i];
                if (text[i] == '"') {
                    i++;
                    ip >> noskipws >> text[i];
                    if (text[i] == ',') {
                        text[i - 1] = '\0';
                        break;
                    }
                }
            }
            if (i == sizeof(reg->review_text)) {
                text[i - 1] = '\0';
                char* ch = new char;
                while (true) {
                    ip >> noskipws >> *ch;
                    if (*ch == '"') {
                        ip >> noskipws >> *ch;
                        if (*ch == ',')
                            break;
                    }
                }
                delete ch;
            }
            for (i = 0; i < sizeof(reg->review_text) && i < sizeof(text); i++) {
                reg->review_text[i] = text[i];
            }
        }
        else {
            string* text = new string;
            ip.seekg((int)ip.tellg() - 1);
            getline(ip, *text, ',');
            (*text)[(*text).length()] = '\0';
            for (i = 0; i < sizeof(reg->review_text) && i < (*text).length(); i++) {
                reg->review_text[i] = (*text)[i];
            }
            reg->review_text[i] = '\0';
            delete text;
        }

        //upvotes
        string* upvotes = new string;
        getline(ip, *upvotes, ',');
        for (i = 0; i < (int)(*upvotes).length(); i++) {
            reg->upvotes[i] = (*upvotes)[i];
        }
        reg->upvotes[i] = '\0';
        delete upvotes;

        //version
        string* version = new string;
        getline(ip, *version, ',');
        for (i = 0; i < (int)(*version).length(); i++) {
            reg->app_version[i] = (*version)[i];
        }
        reg->app_version[i] = '\0';
        delete version;

        //date
        char t[20];
        ip.read(t, 19);
        t[19] = '\0';
        for (i = 0; i < 20; i++) {
            reg->posted_date[i] = t[i];
        }
        string* s = new string;
        getline(ip, *s);
        delete s;

        //escrever no arquivo binário
        if (op.is_open())
        {
            if (((int)ip.tellg() == -1) || ip.eof())
                return;
            if (ip.tellg() <= length) {
                op.write(reinterpret_cast<char*>(reg), sizeof(Review));
                cont++;
            }
            else {
                break;
            }

        }
        else {
            cout << "Erro" << endl;
            exit(1);
        }
        delete reg;
    }
    op.close();
    cout << "Registros exportados para tiktok_app_reviews.bin" << endl;
    return;
}

void etapa1(){
    int i,j,n;
    string line;
    ifstream input;
    int m=3;
    input.open("input.dat");
    for(i=0;i<5;i++){
        getline(input,line);
        n = stoi(line);
        Review * regs = new Review[n];
        for(j=0;j<m;j++){
            regs = import(n);
            StartQuicksort(regs,n,j);
            regs = import(n);
            StartHeapsort(regs, n, j);
        }
        delete [] regs;
    }
    input.close();
}

int main() {
    ofstream saida;
    saida.open("saida.txt");
    saida.close();
    ifstream check;
    check.open("tiktok_app_reviews.bin");
    if (check.is_open()) {
        cout << "Arquivo binario encontrado." << endl;
        check.close();
    }
    else {
        processamento();
    }
    int option;
    while (true) {
        cout << endl << "Escolha:" << endl;
        cout << "1. Ordenacao" << endl;
        cout << "2. Hash;" << endl;
        cout << "3. Modulo de testes;" << endl;
        cout << "4. Encerrar programa;" << endl;
        cin >> option;
        int n; //sera o numero de registros inserido pelo usuario
        int tam,m;// sera 1.75 o valor de n
        int aux = 0;
        INFO *tabela;
        Review* regs;
        fstream op;
        switch (option){
        case 1:
            etapa1();
            break;
        case 2:
            cout << "Digite o numero de registros: ";
            cin >> n;
            cout << "Quantos registros deseja salvar no arquivo: ";
            cin >> m;
            tam = tamanho(n);
            regs = new Review[n];
            tabela = new INFO[tam];
            tam = tamanho(n);
            regs = import(n);
            inittabelahash(tabela, tam);

            for (int i = 0; i < n; i++)
            {
                aux = converte(regs[i].app_version);
                insertIntabelahash(tabela, aux, regs[i].app_version, tam);
            }
            //displayArray(tabela, tam);
            Ordenadecre(tabela,0,tam-1);
            cout << endl << "Ordena finalizado" << endl;
            displayArray(tabela, tam);
            salvarhash(tabela,m);
            break;
        case 3:{
            op.open("teste.txt", ios::out);
            Review * regs = new Review[100];
            StartQuicksort(regs, 100, 3);
            StartHeapsort(regs, 100, 3);
            tam = tamanho(100);
            regs = new Review[n];
            tabela = new INFO[tam];
            tam = tamanho(n);
            regs = import(n);
            inittabelahash(tabela, tam);
            for (int i = 0; i < n; i++)
            {
                aux = converte(regs[i].app_version);
                insertIntabelahash(tabela, aux, regs[i].app_version, tam);
            }
            displayArray(tabela, tam);
            delete regs;
            op.close();
        }
            break;
        case 4:
            break;
        default:
            cout << "Opcao invalida!" << endl;
        }
        if (option == 4)
            break;
    }
    cout << "Programa encerrado." << endl;
    return 0;
}

