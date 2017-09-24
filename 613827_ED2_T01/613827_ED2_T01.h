#include<stdio.h>
#include<stdlib.h>

typedef long long int ll;

// _____________________________________

struct poke{
	char nomepoke[40];
	char tipopoke[40];
	char nometrei[40];
	char nomeequi[40];
	char combat[10];
	char data[11];
	char hora[8];
	char nivel[6];
};
typedef struct poke mon;
mon pokemon; 

// codigo e rrn_______________________
 
struct armazena1{
	char str[14]; 
	ll num;
};
typedef struct armazena1 dados;
dados *d;


// codigo e nome______________________

struct armazena2{
	char str[14]; 
	char nomepk[40];
};
typedef struct armazena2 dados2;
dados2 *e;

// _____________________________________


// FUNCOES

void inicia( );

int abrearquivo( char arq[] );

void criaarquivo( char arq[] );

int estaconsistente( char arq[] );

void leresto();

void imprime( FILE *fp, char str[], ll x );

void naotaconsistente( );

void cadastro(  );

void ordena(  );

void listagem( );

ll busca ( char chave[] );

ll buscanome( char chave[] );

int buscaequipe( char chave[] );

ll buscacaractere( char chave );

int compare( const void *a, const void *b );

int compare2(const void *a, const void *b );

int compare3( const void *a, const void *b );

void remover();

void alterar();

void realoca( );

void pesquisar();

int validadata ( int dia, int mes, int ano );

void aumenta( char str[] );

void libera( );

void finaliza( );

int verificacombats( char str[] );


