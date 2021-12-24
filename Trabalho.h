#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

#pragma warning(disable : 4996)

struct Review
{
    char review_id[100];
    char review_text[2000];
    char upvotes[10];
    char app_version[10];
    char posted_date[25];
};

typedef struct
{
    int id;
    int freq;
    string versao;
} INFO;

void acessaRegistro(int i) {
    cout << i << ". " << endl;
    Review* reg = new Review;
    ifstream file;
    file.open("tiktok_app_reviews.bin", ios::binary);
    if(file) {
        file.seekg((i-1) * sizeof(Review), ios::beg);
        file.read(reinterpret_cast<char*>(reg), sizeof(Review));
        cout << "id: ";
        cout << reg->review_id << endl;
        cout << "text: ";
        cout << reg->review_text << endl;
        cout << "upvotes: ";
        cout << reg->upvotes << endl;
        cout << "app version: ";
        cout << reg->app_version << endl;
        cout << "date: ";
        cout << reg->posted_date << endl;
        file.close();
    }
}

Review* import(int N) {
    ifstream file;
    file.open("tiktok_app_reviews.bin");

    //encontra o numero de registros
    file.seekg(0, ios::end);
    int max = (file.tellg() / sizeof(Review))-1;
		// aloca um numero N de ponteiros de Review para os registros
    Review* regs = new Review[N];
    srand(time(0));
    for (int i = 0; i < N; i++) {
				// registros aleatorios
        int random = (rand() % (max - 1));
        int cursor = random * sizeof(Review);
        file.seekg(cursor, ios::beg);
        file.read(reinterpret_cast<char*>(&regs[i]), sizeof(Review));
    }
    file.close();
    return regs;
}

int hashfunc(int x, int i, int tam)
{
    return (x + i + i * i) % tam; //função hash com quadratic probing
}

int insertIntabelahash(INFO* tabelahash, int key, string v, int tam)
{
    int i = 0;
    int h;
    do
    {
        h = hashfunc(key, i, tam);
        if (tabelahash[h].id == key) //se tentar inserir uma chave ja alocada, apenas aumenta a frequencia daquela chave, nao a aloca novamente
        {
            tabelahash[h].freq++;
            return h;
        }
        else if (tabelahash[h].freq == 0 && tabelahash[h].id == 0) //verifica se o espaço ta vazio
        {
            tabelahash[h].id = key; //se o espaço estiver vazio, aloca a chave ali, e aumenta frequencia de 0 para 1
            tabelahash[h].versao = v;
            tabelahash[h].freq++;
            return h;

        }
        else //se nao estiver, hora de aumentar o i e jogar na hashfunc de novo
        {
            i++;
            cout << "Collision:" << key << "on " << h << endl;
            cout << endl;
        }
    } while (i < tam);
    return -1;
}



void inittabelahash(INFO* tabelahash, int tam)
{
    for (int i = 0; i < tam; i++)
    {
        tabelahash[i].id = 0;
        tabelahash[i].freq = 0;
        tabelahash[i].versao = "Vazio";
    }
}


void displayArray(INFO* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "Position " << i << ":" << endl << a[i].id << "-" << a[i].versao << endl;
        cout << "Freq:" << a[i].freq << endl;
    }
    cout << endl;
}

int converte(string str) //transformando uma string num int
{
    int l = str.length();
    int sum = 0;
    int aux = 0;
    int j = 1;
    for (int i = 0; i < l; i++)
    {
        aux = str[i] * j; //multiplica o valor ASCII da string por um int para evitar que
        sum += aux; //versoes com mesmos caracteres em ordens diferentes se confundam
        j++;
    }
    return sum;
}

int tamanho(int n)
{
    int aux = n;

    if (aux <= 500)
    {
        aux = aux * 1.75;
    }
    return aux;
}


struct resultados{
  int movimentos;
  int comparacoes;
  float tempo;
  int id;
  string tipo;
};

void setresult(resultados *str, int m){
  str->movimentos=0;
  str->comparacoes=0;
  str->tempo=0;
  str->id=m;
}

void salvar(resultados *str){
  ofstream saida;
  saida.open("saida.txt", fstream::app);
  saida << str->id << "." << str->tipo << endl;
  saida << "Movimentos: " << str->movimentos << endl;
  saida << "Comparações: " << str->comparacoes << endl;
  saida << "Tempo em milissegundos: " << str->tempo << endl << endl;
  saida.close();
}

void salvarhash(INFO tabela[],int m){
  ofstream saida;
  saida.open("saida.txt", fstream::app);
  saida << "Versoes mais frequentes" << endl;
  for(int i=0;i<m;i++){
  saida << tabela[i].versao << endl;
  saida << tabela[i].freq << endl << endl;
  }
  saida.close();
}

void addmov(resultados *str){
  str->movimentos=str->movimentos+1;
}

void addcomp(resultados *str){
  str->comparacoes=str->comparacoes+1;
}

void addtempo(resultados *str, float tempo){
  str->tempo=str->tempo+tempo;
}

void addtipo(resultados *str, string tipe){
  str->tipo=tipe;
}

void trocapos(Review regs[],int a, int b){
    Review *aux = new Review;
    *aux = regs[a];
    regs[a] = regs[b];
    regs[b] = *aux;
    delete aux;
}

void trocaposhash(INFO regs[],int a, int b){
    INFO *aux = new INFO;
    *aux = regs[a];
    regs[a] = regs[b];
    regs[b] = *aux;
    delete aux;
}

int Left(int i){
  return i*2;
}

int Right(int i){
  return (i*2)+1;
}


bool verificarzero(Review regs[], int p,int q){
    for(int i=p;i<=q;i++){
        if(atoi(regs[i].upvotes)!=0){
            return true;
        }
    }
    return false;
}




////////////////////
// Heapsort

void max_heapify(Review regs[], int i, int n, resultados *resultH){
  int m;
  int l = Left(i);
  int r = Right(i);
  if((l<n) && (atoi(regs[l].upvotes)>atoi(regs[i].upvotes))){
    m=l;
  }else{
    m=i;
  }
  if(r<=n && atoi(regs[r].upvotes)>atoi(regs[m].upvotes)){
    m=r;
  }
  if(m!=i && i>0){
    trocapos(regs,i,m);
    max_heapify(regs,m,n,resultH);
  }
}

void build_max_heap(Review regs[], int n, resultados *resultH){
  int i;
  for(i=n/2;i>=1;i--){
    max_heapify(regs,i,n,resultH);
  }
}

void Heapsort(Review regs[], int n, resultados *resultH){
  build_max_heap(regs,n,resultH);
  for(int i=n;i>=2;i--){
    trocapos(regs,0,i-1);
    max_heapify(regs,0,n-1,resultH);
  }
}

void StartHeapsort(Review regs[], int n,int m){
  string tipo="Heap";
  auto c = high_resolution_clock::now();
  resultados *resultH = new resultados;
  setresult(resultH,m+1);
  Heapsort(regs,n,resultH);
  auto f = high_resolution_clock::now();
  auto total = duration_cast<milliseconds>(f-c);
  float tempo = total.count();
  addtempo(resultH,tempo);
  addtipo(resultH,tipo);
  salvar(resultH);
  delete resultH;
}

/////////////////////




////////////////////
// Quicksort

int partquick(Review regs[], int p, int q, resultados *resultQ){
    int j = q;
    int i = p;
    int pivo = (j+i)/2;
    do{
        while(atoi(regs[i].upvotes)==0 && verificarzero(regs,p,q) ||(atoi(regs[i].upvotes))<(atoi(regs[pivo].upvotes))){
            i++;
            addcomp(resultQ);
        }
        addcomp(resultQ);
        while(atoi(regs[i].upvotes)==0 && verificarzero(regs,p,q) ||((atoi(regs[j].upvotes))>(atoi(regs[pivo].upvotes)))){
            j--;
            addcomp(resultQ);
        }
        addcomp(resultQ);
        if(i<j){
            if(atoi(regs[i].upvotes)!=atoi(regs[j].upvotes)){
                trocapos(regs,i,j);
            }
            addmov(resultQ);
            i++;
            j--;
        }
    }while(i<j);
    return j;
}

void Quicksort(Review regs[], int p, int r, resultados *resultQ){
  int q;
  if(r-p>0){
    q = partquick(regs,p,r,resultQ);
    Quicksort(regs,p,q,resultQ);
    Quicksort(regs,q+1,r,resultQ);
  }
}

void StartQuicksort(Review regs[],int r, int m){
  string tipo="Quick";
  resultados *resultQ = new resultados;
  setresult(resultQ,m+1);
  auto c = high_resolution_clock::now();
  Quicksort(regs,0,r-1,resultQ);
  auto f = high_resolution_clock::now();
  auto total = duration_cast<milliseconds>(f-c);
  float tempo = total.count();
  addtempo(resultQ,tempo);
  addtipo(resultQ,tipo);
  salvar(resultQ);
  delete resultQ;
}


void Ordenadecre(INFO array[], int left_position, int right_position){

    int x = left_position;
    int y = right_position;
    int temporary;                 //temporary variable for swa[[ing without losing any data
    int pivot = array[(right_position + left_position)/2].freq;       //middle element as pivot
    while(x <=y){
        while(array[x].freq > pivot)              //left side
        x++;
        while(array[y].freq < pivot)           //right side
        y--;
        if(x <=y){                       //begin the swapping
            trocaposhash(array,x,y);
            x++;
            y--;
        }
    }
}
