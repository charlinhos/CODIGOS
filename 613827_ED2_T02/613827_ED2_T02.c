/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 02 - Árvore B
 *
 * RA: 613827
 * Aluno: Carlos Alberico Bezerra de Andrade
 * ========================================================================== */


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


/* Tamanho dos campos dos registros */
#define	TAM_PRIMARY_KEY	13
#define	TAM_NOME		38
#define	TAM_TIPO		38
#define TAM_CP			8
#define	TAM_DATA		9
#define	TAM_HORA		6
#define	TAM_NIVEL		4

#define TAM_REGISTRO    192
#define MAX_REGISTROS   10000
#define TAM_ARQUIVO     (MAX_REGISTROS * TAM_REGISTRO + 1)


/* Saídas do usuário */
#define OPCAO_INVALIDA          "Opcao invalida!\n"
#define MEMORIA_INSUFICIENTE    "Memoria insuficiente!"
#define REGISTRO_N_ENCONTRADO   "Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO          "Campo invalido! Informe novamente.\n"
#define ERRO_PK_REPETIDA        "ERRO: Ja existe um registro com a chave primaria: %s.\n"
#define ARQUIVO_VAZIO           "Arquivo vazio!"
#define NOS_PERCORRIDOS         "Busca por %s. Nos percorridos:\n"


/* Registro do Pokémon */
typedef struct {
	char primary_key[TAM_PRIMARY_KEY];
	char nome_pokemon[TAM_NOME];
	char tipo_pokemon[TAM_TIPO];
	char combat_points[TAM_CP];
	char data_captura[TAM_DATA]; 	// DD/MM/AA
	char hora_captura[TAM_HORA];	// HH:MM
	char nome_treinador[TAM_NOME];
	char nivel_treinador[TAM_NIVEL];
	char nome_equipe[TAM_NOME];
} Pokemon;


/* Registro da Árvore-B
 * Contém a chave primária e o RRN do respectivo registro */
typedef struct {
	char pk[TAM_PRIMARY_KEY];	// chave primária
	int rrn;					// rrn do registro
} Chave;


/* Estrutura da Árvore-B */
typedef struct node node_Btree;
struct node {
	int num_chaves;		// numero de chaves armazenadas
	Chave *chave;		// vetor das chaves e rrns [m-1]
	node_Btree **desc;	// poarvore bnteiros para os descendentes, *desc[m]
	int folha;			// flag folha da arvore
};

typedef struct {
	node_Btree *raiz;
} Iprimary;


/* Registro do índice secundário - ipokemon.idx
 * Contém o nome do Pokémon e a chave primária do registro */
typedef struct {
	char nome_pokemon[TAM_NOME];
	char primary_key[TAM_PRIMARY_KEY];
} Ipokemon;


/* Registro do índice secundário - iteam.idx
 * Contém o nome da equipe e a chave primária do registro */
typedef struct {
	char nome_equipe[TAM_NOME];
	char primary_key[TAM_PRIMARY_KEY];
} Iteam;


/* Registro auxiliar para retorno da funcao */
struct auxiliar{
    Chave *chave_promovida;
    node_Btree *filho_direito;
};


/* Variáveis globais */
char ARQUIVO[TAM_ARQUIVO];
int M;


/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo();

/* Exibe o Pokémon */
void exibir_registro(int rrn);

/* Cadastrar um novo pokemon */
void cadastrar( Iprimary *iprimary, Ipokemon ipokemon[], Iteam iteam[], int *nregistros );

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */

/* Cria o indice secundario Ipokemon */
void criar_ipokemon( Ipokemon ipokemon[], int nregistros );

/* Cria o indice secundario Iteam */
void criar_iteam( Iteam iteam[], int nregistros );

/* Cria o indice primario */
void criar_iprimary( Iprimary *iprimary, int nregistros, int ordem );

/* Recebe do usuario uma string com uma chave primaria e caso a chave ja esteja cadastrada altera os combat points */
void alterar( Iprimary iprimary );

/* Buscar ( Pode ser por codigo, pokemon, equipe ) */
void buscar( Iprimary iprimary, Ipokemon ipokemon[], Iteam iteam[], int nregistros );

/* Listar (Pode ser do iprimary, ipokemon, iteam ) */
void listar( Iprimary iprimary, Ipokemon ipokemon[], Iteam iteam[], int nregistros );

/* Apaga todos os nos ( Libera a B-Tree da memoria )*/
void apagar_no( node_Btree **no );

/* Descarta o que estiver no buffer de entrada */
void ignore( );

/* Ordena o indice secundario Ipokemon usando o algoritmo de ordenacao MergeSort */
void msortpokeip( Ipokemon vet[], int ini, int fim );
void mpokeip( Ipokemon vet[], int ini, int meio, int fim );

/* Ordena o indice secundario Iteam usando o algoritmo de ordenacao MergeSort */
void msortteamip( Iteam vet[], int ini, int fim );
void mteamip( Iteam vet[], int ini, int meio, int fim );

/* Recebe uma string como parametro e transforma todos os caracteres em maiusculo */
void aumenta( char str[] );

/* Verifica se a data eh valida */
int verificadata( int dia, int mes, int ano );

/* Verifica se os combat points sao validos */
int verificacombats( char str[] );

/* Busca um pokemon pelo nome ( Retorna a primeira ocorrencia do nome ) */
int buscanome( Ipokemon ipokemon[], char chave[], int contador );

/* Busca um caractere( Retorna a primeira ocorrencia do caractere ) */
int buscacaractere( Iteam iteam[], char chave , int contador );

/* Recupera o registro do arquivo de dados a partir do RRN */
Pokemon recuperar_registro( int rrn );

/* Inicializa a arvore */
void inic( Iprimary *iprimary );

/* Listar em pre ordem */
void preorder( node_Btree *x, int level );

/* A Funcao CRIA_NO do pdf */
node_Btree *CRIA_NO( );

/* A Funcao INSERE do pdf */
void insere( Iprimary *t, Chave k );

/* A Funcao BUSCA do pdf */
node_Btree* busca(  node_Btree *x, char k[], int listagem );

/* A Funcao INSERE_AUX do pdf */
struct auxiliar insere_aux( node_Btree *x, Chave k );

/* A Funcao DIVIDE_NO do pdf */
struct auxiliar divide_no( node_Btree *x, Chave k, node_Btree *filho_direito );

/* Metodo de comparacao para o qsort */
// int compare2(const void *a, const void *b );
// int compare3(const void *a, const void *b );

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main() {

	/* Arquivo */
	int carregarArquivo = 0, nregistros = 0;
	scanf("%d\n", &carregarArquivo); // 1 (sim) | 0 (nao)
	if (carregarArquivo) {
		nregistros = carregar_arquivo();
	}

	/* Índice primário */
	int ordem;
	scanf("%d", &ordem);
	Iprimary iprimary;
	criar_iprimary(&iprimary, nregistros, ordem);

	/* Índice secundário de nomes de Pokémon */
	Ipokemon *ipokemon = (Ipokemon *) malloc (MAX_REGISTROS * sizeof(Ipokemon));
	if (!ipokemon) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_ipokemon( ipokemon, nregistros);

	/* Índice secundário de equipes */
	Iteam *iteam = (Iteam *) malloc (MAX_REGISTROS * sizeof(Iteam));
	if (!iteam) {
		perror(MEMORIA_INSUFICIENTE);
		exit(1);
	}
	criar_iteam(iteam, nregistros);

	/* Execução do programa */
	int opcao = 0;
	while(opcao != 5) {
		scanf("%d", &opcao);
		switch(opcao) {
		/* Cadastrar um novo Pokémon */
		case 1:
			getchar();
			cadastrar(&iprimary, ipokemon, iteam, &nregistros);
			break;
		// Alterar os combat points de um Pokémon
		case 2:
			getchar();
			alterar(iprimary);
			break;
		// Buscar um Pokémon
		case 3:
			buscar(iprimary, ipokemon, iteam, nregistros);
			break;
		// Listar todos os Pokémons
		case 4:
			listar(iprimary, ipokemon, iteam, nregistros);
			break;
		// Liberar a memória alocada
		case 5:
			apagar_no(&iprimary.raiz);
			free(ipokemon);
			free(iteam);
			break;
        // Imprimir o arquivo de dados
		case 10:
			printf("%s\n", ARQUIVO);
			break;
        // Ignora a entrada e exibe mensagem de erro

		default:
			ignore();
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}

/* ==========================================================================
 * =============================== AUXILIARES ===============================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* <<< IMPLEMENTE AQUI AS FUNCOES AUXILIARES >>> */

/*
// Metodo de comparacao para o qsort ( Ipokemon )
int compare2(const void *a, const void *b ) {
	Ipokemon *x = ( Ipokemon * )a;
	Ipokemon *y = ( Ipokemon * )b;
	if( ( strcmp( x->nome_pokemon, y->nome_pokemon ) ) != 0 ){
		return strcmp( x->nome_pokemon, y->nome_pokemon );
	}
	else{
		return strcmp( x->primary_key, y->primary_key );
	}
} */

// Metodo de comparacao para o qsort ( Iteam )
/*int compare3(const void *a, const void *b ) {
	Iteam *x = ( Iteam * )a;
	Iteam *y = ( Iteam * )b;
	if( ( strcmp( x->nome_equipe, y->nome_equipe ) ) != 0 ){
		return strcmp( x->nome_equipe, y->nome_equipe );
	}
	else{
		return strcmp( x->primary_key, y->primary_key );
	}
}*/

/* Ordena pelo nome, em caso de nomes iguais desempata com a chave primaria ( MergeSort ) */
void msortpokeip( Ipokemon vet[], int ini, int fim ){
	/* MergeSort */
	int meio;
   	if( ini < fim ){
         meio = ( ini+fim )/2; // Divide no meio
         msortpokeip( vet, ini, meio ); // Do comeco ate metade
         msortpokeip( vet, meio+1, fim ); // Da metade+1 ate o fim
         mpokeip( vet, ini, meio, fim ); // Mescla as partes
    }
}
void mpokeip( Ipokemon vet[], int ini, int meio, int fim ){
	/* Merge */
	int i, m, k, l;
	Ipokemon *temp = (Ipokemon *) malloc ( ( fim+2 ) * sizeof(Ipokemon)); // Aloca o temporario
	if (!temp) {
		perror(MEMORIA_INSUFICIENTE); // Deu ruim
		exit(1);
	}
	i = l = ini;
	m = meio+1;
	while( ( l <= meio ) && ( m <= fim ) ){
		if( ( strcmp( vet[l].nome_pokemon, vet[m].nome_pokemon ) ) == 0 ){ // Nomes iguais
			if( ( strcmp( vet[l].primary_key, vet[m].primary_key ) ) < 0 ){ // Desempata com a chave
					temp[i] = vet[l]; // Guarda o menor no temporario
					l++;
			}
			else{
				temp[i] = vet[m]; // Guarda o menor no temporario
				m++;
			}
			i++;
		}
		else{
			if( ( strcmp( vet[l].nome_pokemon, vet[m].nome_pokemon ) ) < 0 ){ // Compara pelo nome
						temp[i] = vet[l]; // Guarda o menor no temporario
						l++;
			}
			else{
				temp[i] = vet[m]; // Guarda o menor no temporario
				m++;
			}
			i++;
		}
	}
	if( l > meio ){ // Pega o que sobrou
		for( k = m; k <= fim; k++ ){
			temp[i] = vet[k];
			i++;
		}
	}
	else{ // Pega o que sobrou
		for( k = l; k <= meio; k++ ){
			temp[i] = vet[k];
			i++;
		}
	}
	for( k = ini; k <= fim; k++ ){ // Copia pro vetor original
		vet[k] = temp[k];
	}
}

/* Ordena pelo time, em caso de nomes iguais desempata com a chave primaria ( MergeSort ) */
void msortteamip( Iteam vet[], int ini, int fim ){
    /* MergeSort */
    int meio;
    if( ini < fim ){
         meio = ( ini+fim )/2; // Divide no meio
         msortteamip( vet, ini, meio ); // Do comeco ate metade
         msortteamip( vet, meio+1, fim ); // Da metade+1 ate o fim
         mteamip( vet, ini, meio, fim ); // Mescla as partes
    }
}

void mteamip( Iteam vet[], int ini, int meio, int fim ){
	/* Merge */
	int i, m, k, l;
	Iteam *temp = (Iteam *) malloc ( ( ( fim+2 ) ) * sizeof(Iteam)); // Aloca o temporario
	if (!temp) {
		perror(MEMORIA_INSUFICIENTE); // Deu ruim
		exit(1);
	}
	i = l = ini;
	m = meio+1;
	while( ( l <= meio ) && ( m <= fim ) ){
		if( ( strcmp( vet[l].nome_equipe, vet[m].nome_equipe ) ) == 0 ){  // Nomes iguais
			if( ( strcmp( vet[l].primary_key, vet[m].primary_key ) ) < 0 ){ // Desempata com a chave
					temp[i] = vet[l]; // Guarda o menor no temporario
					l++;
			}
			else{
				temp[i] = vet[m]; // Guarda o menor no temporario
				m++;
			}
			i++;
		}
		else{
			if( ( strcmp( vet[l].nome_equipe, vet[m].nome_equipe ) ) < 0 ){ // Compara pelo nome
					temp[i] = vet[l]; // Guarda o menor no temporario
					l++;
			}
			else{
				temp[i] = vet[m]; // Guarda o menor no temporario
				m++;
			}
			i++;
		}
	}
	if( l > meio ){ // Pega o que sobrou 
		for( k = m; k <= fim; k++ ){
			temp[i] = vet[k];
			i++;
		}
	}
	else{ // Pega o que sobrou 
		for( k = l; k <= meio; k++ ){
			temp[i] = vet[k];
			i++;
		}
	}
	for( k = ini; k <= fim; k++ ){ // Copia pro vetor original
		vet[k] = temp[k];
	}
}


/* Busca pelo nome do pokemon ( Retorna na primeira ocorrencia do nome do pokemon ) */
int buscanome( Ipokemon ipokemon[], char chave[], int contador ){
	int fim, ini, best, meio;
	fim = contador-1;
	ini = 0; 
	best = -1;
	// Busca binaria
	while( ini <= fim ) {
		meio = ( ini + fim ) / 2;
		if(  strcmp( chave, ipokemon[meio].nome_pokemon ) == 0  ){ // Achou
			best = meio; // Esse eh o melhor ate agora
			fim = meio - 1; // Tenta pegar a primeira ocorrencia
		}
		if(  strcmp( chave, ipokemon[meio].nome_pokemon ) < 0  ){  // Ta na parte esquerda
			fim = meio - 1;
		}
		if(  strcmp( chave, ipokemon[meio].nome_pokemon ) > 0  ){ // Ta na parte direita
			ini = meio + 1;
		}
	}
	return best;
}

/* Busca por equipe */
int buscaequipe( char chave[] ){
	if( !( strcmp( chave, "VALOR" ) ) ){
		return 0;
	}
	if( !( strcmp( chave, "INSTINCT" ) ) ){
		return 1;
	} 
	if( !( strcmp( chave, "MYSTIC" ) ) ){
		return 2;
	}
	return -1;
}

/* Busca um caractere ( Retorna a primeira ocorrencia do caractere ) */
int buscacaractere( Iteam iteam[], char chave , int contador ){
	int fim, ini, best, meio;
	fim = contador-1;
	ini = 0; 
	best = -1;
	// Busca binaria
	while( ini <= fim ) {
		meio = ( ini + fim ) / 2;
		if(  chave == iteam[meio].nome_equipe[0] ){ // Achou
			best = meio; // Esse eh o melhor ate agora
			fim = meio - 1; // Tenta pegar a primeira ocorrencia
		}
		if(   chave < iteam[meio].nome_equipe[0] ){ // Ta na parte esquerda
			fim = meio - 1;
		}
		if(  chave > iteam[meio].nome_equipe[0]  ){ // Ta na parte direita
			ini = meio + 1;
		}
	}
	return best;
}


/* Inicializa a arvore */
void inic( Iprimary *iprimary ){
	iprimary->raiz = NULL;
}


/* Recebe uma string como parametro e transforma todos os caracteres em maiusculo */
void aumenta( char str[] ){
	int i, n;
	n = strlen( str );
	for( i = 0; i < n; i++ ){
		str[i] = toupper( str[i] );
	}	
}

/* Verifica se a data eh valida */
int verificadata( int dia, int mes, int ano ){
	int ok = 1;
	if( !( ano >= 16 ) ){ // Checa ano
		ok = 0;
	}
	if( !( mes >= 1 && mes <= 12 ) ){ // Checa mes
		ok = 0;
	}
	if( !( dia >= 1 ) ){ // Checa se dia nao eh negativo
		ok = 0;
	}
	if( mes == 2 ){ // Se for ano bissexto muda o nro de dias em fevereiro
		if ( ( ano % 4 == 0 && ano % 100 != 0 ) || ano % 400 == 0 ){
			if( !( dia <= 29 ) ){
				ok = 0;
			}
		}
		else if( !( dia <= 28 ) ){ 
			ok = 0;
		}
	}
	else if( mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes ==8 || mes == 10 || mes == 12 ){ // Meses que vao ate 31
		if(  !( dia <= 31 ) ){
			ok = 0;
		}
	}
	else if(  !( dia <= 30 ) ){
		ok = 0;
	}
	return ok;
}

/* Verifica se os combat points sao validos */
int verificacombats( char str[] ){
	int i, n, ok, x;
	ok = 1; i = 0;
	n = strlen( str );
	if( n != 7 ) ok = 0;
	while( i < n ){
		if( i != 4 ){
			if( !isdigit( str[i] ) ){
					ok = 0;
			}
			else {
				x = str[i] -'0';
				if( !( x >= 0 && x <= 9 ) ){
					ok = 0;
				}
			}
		}
		else if( str[i] != '.' ){
			ok = 0;
		}
		i++;
	}
	return ok;
}


/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Recebe do usuário uma string simulando o arquivo completo e retorna o número
 * de registros. */
int carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
	return strlen(ARQUIVO) / TAM_REGISTRO;
}

/* Exibe o Pokémon */
void exibir_registro(int rrn) {
	Pokemon p = recuperar_registro(rrn); // Recupera o registro
	// Imprime as paradas
	printf("%s\n", p.primary_key); 
	printf("%s\n", p.nome_pokemon); 	
	printf("%s\n", p.tipo_pokemon); 
	printf("%s\n", p.combat_points); 
	printf("%s\n", p.data_captura); 
	printf("%s\n", p.hora_captura); 
	printf("%s\n", p.nome_treinador); 
	printf("%s\n", p.nivel_treinador); 
	printf("%s\n", p.nome_equipe); 
	printf("\n");	
}

/* <<< IMPLEMENTE AQUI AS FUNCOES PRINCIPAIS >>> */

/* Lista em pre ordem */
void preorder( node_Btree *x, int level ){
	int i;
	if( x != NULL ){
		printf( "%d - ", level ); // Imprime que nivel que ta
		i = 0; 
		while( i < x->num_chaves ){ 
			if( i ){ // Se nao eh o primeiro imprime a virgula
				printf( ", " );
			}
			printf( "%s", x->chave[i].pk ); // Imprime as chaves
			i++;
		}
		printf( "\n" ); // Pula a linha
		
		i = 0; 
		while( i <= x->num_chaves ){
			preorder( x->desc[i], level+1 ); // Vai pro descendente, proximo nivel ( Recursao )
			i++;
		}
	}
}

/* A Funcao CRIA_NO do pdf */
node_Btree *CRIA_NO( ){
	int i;
	// Aloca as paradas
	node_Btree *novo;
	novo = ( node_Btree * )malloc( sizeof( node_Btree ) ); // Aloca o no
	novo->chave = ( Chave * )malloc( ( M-1 ) * ( sizeof( Chave ) ) ); // Aloca as chaves
	novo->desc = ( node_Btree ** )malloc( ( M ) * ( sizeof( node_Btree ) ) ); // Aloca o vetor de descendentes
	for ( i = 0; i < M ; i++ ){
		novo->desc[i] = NULL; // Seta todos os descendentes para NULL
	}
	// Retorna o novo no
	return novo;
}

/* A Funcao INSERE_AUX do pdf */
struct auxiliar insere_aux( node_Btree *x, Chave k ){
	int i;
	struct auxiliar ret;
	if( x->folha ){
		if( x->num_chaves < M-1 ){
			i = x->num_chaves-1;
			while( i >= 0 && ( strcmp( k.pk, x->chave[i].pk ) ) < 0 ){
				x->chave[i+1] = x->chave[i];
				i--;
			}
			x->chave[i+1] = k;		
			x->num_chaves++;

			ret.chave_promovida = NULL;
			ret.filho_direito = NULL;
 			return ret;	
		}
		else {
			ret = divide_no( x, k, NULL );
			return ret;
		}
	}
	else{
		i = x->num_chaves-1;
		while( i >= 0  && ( strcmp( k.pk, x->chave[i].pk ) ) < 0 ){
			i--;
		}
		i++;
		ret = insere_aux( x->desc[i], k );

		if( ret.chave_promovida != NULL ){ 
			 k = *( ret.chave_promovida );
			if( x->num_chaves < M-1 ){
				i = x->num_chaves-1;
				while( i >= 0 && ( strcmp( k.pk, x->chave[i].pk ) ) < 0 ){
					x->chave[i+1] = x->chave[i];
					x->desc[i+2] = x->desc[i+1];
					i--;
				}
				x->chave[i+1] = k;
				x->desc[i+2] = ret.filho_direito;
				x->num_chaves++; // no pdf ta x mas acho que eh n, provavelmente erro de digitacao (n[X] ← x[X] + 1)
				
				ret.chave_promovida = NULL;
				ret.filho_direito = NULL;
 				return ret;		
			}
			else{
				ret =  divide_no( x, k, ret.filho_direito );
				return ret;
			}
		}
		else{	
			ret.chave_promovida = NULL;
			ret.filho_direito = NULL;
 			return ret;		
		}
	}	
}

/* A Funcao INSERE do pdf */
void insere( Iprimary *t, Chave k ){
	node_Btree *x;
	struct auxiliar r;
	if( t->raiz == NULL ){ 
		// Ta vazia
		x = CRIA_NO( );
		x->folha = 1;
		x->num_chaves = 1;
		x->chave[0] = k;
		t->raiz = x;		
	}
	else{
		// Nao ta vazia
		r = insere_aux( t->raiz, k );
		if( r.chave_promovida != NULL ){
			x = CRIA_NO( );
			x->folha = 0;
			x->num_chaves = 1;
			x->chave[0] = *( r.chave_promovida );
			x->desc[0] = t->raiz;	
			x->desc[1] = r.filho_direito;
			t->raiz = x;
		}
	}
}

/* A Funcao BUSCA do pdf */
node_Btree* busca( node_Btree *x, char k[], int listagem){
	int i = 0;

	if( listagem ){ // Se for listagem do percurso
		while( i < x->num_chaves ){
			if( i ) // Se nao eh o primeiro imprime uma virgula antes
				printf( ", " );
			printf( "%s", x->chave[i].pk );
			i++;
		}
		printf( "\n" ); // Pula a linha pra ficar igual o caso de saida
	}

	i = 0;
	while( i < x->num_chaves && ( strcmp( k, x->chave[i].pk ) ) > 0 ){
		i++;
	}

	if( i < x->num_chaves && ( strcmp( k, x->chave[i].pk ) ) == 0 ){ // Achou o no
		return x;	
	}

	if( x->folha ){ // Nao achou 
		return NULL;
	}
	else {
		return busca( x->desc[i], k , listagem ); // Busca no descendente ( Recursao )
	}
}

/* A Funcao DIVIDE_NO do pdf */
 struct auxiliar divide_no( node_Btree *x, Chave k, node_Btree *filho_direito ){
	int i, j, chave_alocada;
	node_Btree *y;
	struct auxiliar r;

	i = x->num_chaves-1;
	chave_alocada = 0;

	y = CRIA_NO( );
	y->folha = x->folha;
	y->num_chaves = ( M-1 )/2;
	
	for( j = y->num_chaves-1; j >= 0; j-- ){
		if( !chave_alocada && ( strcmp( k.pk, x->chave[i].pk ) ) > 0 ){
			y->chave[j] = k;
			y->desc[j+1] = filho_direito;
			chave_alocada = 1;
		}
		else{
			y->chave[j] = x->chave[i];
			y->desc[j+1] = x->desc[i+1];
			i--; 
		}
	}

	if( !chave_alocada ){
		while( i>=0 && ( strcmp( k.pk, x->chave[i].pk ) ) < 0 ){
			x->chave[i+1] = x->chave[i];
			x->desc[i+2] = x->desc[i+1];
			i--;
		}
		x->chave[i+1] = k;
		x->desc[i+2] = filho_direito;
	}
	r.chave_promovida = &( x->chave[( M/2 )] );
	y->desc[0] = x->desc[( M/2 )+1];
	x->num_chaves = ( M/2 );

	r.filho_direito = y;
	return r;

}

/* Recupera o registro do arquivo de dados a partir do RRN */
Pokemon recuperar_registro( int rrn ){
	Pokemon p;
	int i , j,  cont;
	char c, str1[40];
	i = rrn * 192;
	j = cont = 0;
	while( cont < 9 ){ // Enquanto nao pegou todos os campos
		c = ARQUIVO[i++]; 
		if( c == '@' ){ // Se eh delimitador ( @ )
			cont++;
			if( cont == 1 ){ // Eh a chave
				str1[j] = '\0';
				strcpy( p.primary_key, str1 );
				j = 0;
			}
			if( cont == 2 ){ // Eh o nome
				str1[j] = '\0';
				strcpy( p.nome_pokemon, str1 );
				j = 0;
			}
			if( cont == 3 ){ // Eh o tipo pokemon
				str1[j] = '\0';
				strcpy( p.tipo_pokemon, str1 );
				j = 0;
			}
			if( cont == 4 ){ // Eh o combat points
				str1[j] = '\0'; 
				strcpy( p.combat_points, str1 ); 		
				j = 0;
			} 
			if( cont == 5 ){ // Eh a data captura
				str1[j] = '\0';
				strcpy( p.data_captura, str1 );
				j = 0;
			} 
			if( cont == 6 ){
				str1[j] = '\0';
				strcpy( p.hora_captura, str1 );
				j = 0;
			}

			if( cont == 7 ){ // Eh o nome treinador
				str1[j] = '\0';
				strcpy( p.nome_treinador, str1 );
				j = 0;
			}
			
			if( cont == 8 ){ // Eh o nivel treinador
				str1[j] = '\0';
				strcpy( p.nivel_treinador, str1 );
				j = 0;
			}

			if( cont == 9 ){
				str1[j] = '\0';
				strcpy( p.nome_equipe, str1 );
				j = 0;
			}
		}
		else{
			str1[j++] = c; // Pega o proximo caractere
		}
	}
	
	return p;
	
}

/* Recebe do usuario uma string com uma chave primaria e caso a chave ja esteja cadastrada altera os combat points */
void alterar( Iprimary iprimary ){
	int i, ok, cont, j, k;
	char str[40], str1[40], c;
	scanf( "%12[^\n]", str1 ); // Le o codigo do pokemon que vai alterar os combat points
	ignore( );
	node_Btree *x;
	aumenta( str1 ); // Aumenta ele
	if( iprimary.raiz != NULL ){
		x = busca( iprimary.raiz, str1, 0 ); // Busca
		if( x != NULL ){ // Achou o no
	 		i = 0; 
			while( strcmp ( x->chave[i].pk, str1 ) ){ // Achou a chave
				i++;
			}
			ok = 0;
			while( !ok ){
				scanf( "%7[^\n]", str ); // Le novo combat points
				ignore( );
				ok = verificacombats( str ); // Verifica se eh valido
				if( !ok ){
					printf( CAMPO_INVALIDO );
				}
				else{
					cont = 0;
					j = ( x->chave[i].rrn )*192; // Pega a posicao no arquivo de dados

					while( cont < 3 ){ // Vai ate chegar no campo certo
						c = ARQUIVO[j++];
						if( c == '@' ){	
							cont++;	// Chegou em um delimitador
						}					
					}
					cont = strlen( str ); 
					k = 0;
					while( k < cont ){ // Sobrescreve o arquivo de dados
						ARQUIVO[j++] = str[k++];
					}
				}
			}
		}
		else{
			printf( REGISTRO_N_ENCONTRADO ); // Nao existe
		}

	}
	else{
		printf( REGISTRO_N_ENCONTRADO ); // Nao existe
	}
}

/* Buscar ( Pode ser por codigo, pokemon, equipe ) */
void buscar( Iprimary iprimary, Ipokemon ipokemon[], Iteam iteam[], int nregistros ){
	char str[40];
	int op, pos;
	pos = 0;
	node_Btree *x;
	scanf( "%d", &op ); // Le a op, ( nome, codigo, equipe )
	getchar( );
	if( op == 1 ){
	// Pesquisar por codigo
		scanf( "%12[^\n]", str );  // Le o codigo
		ignore( ); 
		aumenta( str ); // Aumenta ele
		printf( NOS_PERCORRIDOS, str );
		if( iprimary.raiz != NULL ){
			x = busca( iprimary.raiz, str, 1 );
			printf( "\n" );
			if( x != NULL ){ // Achou o no
				pos = 0;
				while( pos < x->num_chaves  && strcmp( x->chave[pos].pk, str ) ){ // Procura a chave
					pos++;
				}
				exibir_registro( x->chave[pos].rrn ); // Exibe os dados
			}
			else{
				printf( REGISTRO_N_ENCONTRADO ); // Nao achou
			}
		}
	}

	if( op == 2 ){
		scanf( "%37[^\n]", str ); // Le o nome do pokemon
		ignore( );
		aumenta( str );
		op = buscanome( ipokemon, str, nregistros ); // Pega o codigo
		if( op != -1 ){ 
			while( !( strcmp( str, ipokemon[op].nome_pokemon ) ) ){ // Procura todos com esse nome
				x = busca( iprimary.raiz, ipokemon[op].primary_key, 0 );
				if( x != NULL  ){ // Achou o no
					pos = 0; 
					while( strcmp ( x->chave[pos].pk, ipokemon[op].primary_key ) ){ // Procura a chave
						pos++; 
					}
					exibir_registro( x->chave[pos].rrn ); // Exibe os dados
				}
				op++; // Vai pro proximo pokemon da ordem lexicografica de nome
			}
		}
		else{
			printf( REGISTRO_N_ENCONTRADO ); // Nao achou
		}
	}

	if( op == 3 ){
		// Pesquisar por equipe
		scanf( "%37[^\n]", str ); 
		ignore( );
		aumenta( str );
		op = buscaequipe( str ); // Para saber se a equipe existe
		if( op != -1 ){ 
			op = buscacaractere( iteam, str[0], nregistros ); // Acha primeira ocorrencia dessa equipe
			while( iteam[op].primary_key[0] == str[0] ){ // Enquanto for dessa equipe
				x = busca( iprimary.raiz, iteam[op].primary_key, 0 );
				if( x != NULL ){ // Achou o no
					pos = 0;
					while( strcmp ( x->chave[pos].pk, iteam[op].primary_key ) ){ // Procura a chave
						pos++;
					}
					exibir_registro( x->chave[pos].rrn ); // Exibe os dados
				}
				op++; // Vai pro proximo pokemon dessa equipe
			}
		}
		else{
			printf( REGISTRO_N_ENCONTRADO );
		}
	} 
}

/* Listar (Pode ser do iprimary, ipokemon, iteam ) */
void listar( Iprimary iprimary, Ipokemon ipokemon[], Iteam iteam[], int nregistros ){
	int op, j, pos;
	node_Btree *x;
	scanf( "%d", &op ); // Listar por codigo
	if( op == 1 ){
		preorder( iprimary.raiz, 1 ); // Preordem
		printf("\n");
	}
	if( op == 2 ){ // Listar por nome
		for( j = 0; j < nregistros; j++ ){
			x = busca( iprimary.raiz, ipokemon[j].primary_key, 0 ); // Busca ele
			if( x != NULL  ){ // Achou o no
				pos = 0;
				while( strcmp ( x->chave[pos].pk, ipokemon[j].primary_key ) ){ // Acha a chave
					pos++;
				}			
				exibir_registro( x->chave[pos].rrn ); // Exibe os dados
			}
		}
	}
	if( op == 3 ){ // Listar por nome da equipe
		for( j = 0; j < nregistros; j++ ){
			x = busca( iprimary.raiz, iteam[j].primary_key, 0 ); // Busca
			if( x != NULL  ){ // Achou o no
				pos = 0;
				while( strcmp ( x->chave[pos].pk, iteam[j].primary_key ) ){ // Procura a chave
					pos++;
				}
				exibir_registro( x->chave[pos].rrn ); // Exibe os dados
			}	
		}
	}

}

/* Apaga todos os nos ( Libera a B-Tree da memoria ) */
void apagar_no( node_Btree **no ){
	int i;
	if( *no != NULL ){
		free( ( *no )->chave ); // Libera o vetor de chaves
		for( i=0; i <= ( *no )->num_chaves; i++ ){
			apagar_no( &( ( *no )->desc[i] ) );  // Apaga o descendente ( recursao )
		}
		free( ( *no )->desc ); // Libera o vetor de descendente
		free( *no ); // Libera o no
	}
}

/* Cria o indice secundario Iteam */
void criar_iteam( Iteam iteam[], int nregistros ){
	int i;
	Pokemon p;
	for( i = 0; i< nregistros; i++ ){ 
		p = recuperar_registro( i ); // Le do arquivo de dados
		strcpy( iteam[i].primary_key, p.primary_key );
		strcpy( iteam[i].nome_equipe, p.nome_equipe );
	}
	// qsort ( iteam, nregistros, sizeof( Iteam  ), compare3 );
	 msortteamip( iteam, 0, nregistros-1 ); // Ordena os dados
}

/* Cria o indice secundario Ipokemon */ 
void criar_ipokemon( Ipokemon ipokemon[], int nregistros ){
	int i;
	Pokemon p;
	for( i = 0; i< nregistros; i++ ){
		p = recuperar_registro( i ); // Le do arquivo de dados
		strcpy( ipokemon[i].primary_key, p.primary_key );
		strcpy( ipokemon[i].nome_pokemon, p.nome_pokemon );
	}
	// qsort ( ipokemon, nregistros, sizeof( Ipokemon  ), compare2 );
	msortpokeip( ipokemon ,0, (nregistros-1) ); // Ordena os dados

}

/* Cria o indice primario Iprimary */ 
void criar_iprimary( Iprimary *iprimary, int nregistros, int ordem ){
	M = ordem;
	int i;
	Pokemon p;
	Chave k;
	inic( iprimary ); // Inicializa a arvore
	for( i = 0; i< nregistros; i++ ){
		p = recuperar_registro( i ); // Le do arquivo de dados
		strcpy( k.pk,  p.primary_key );
		k.rrn = i;
		insere( iprimary, k ); // Insere na arvore
	}
}

/* Cadastrar um novo pokemon */
void cadastrar( Iprimary *iprimary, Ipokemon ipokemon[], Iteam iteam[], int *nregistros ){
	node_Btree *acha;
	Chave aux;	

	char str1[40], codigo[14];
	int ok, i, n, x, j, tamanhos[9], dia, mes, ano, soma, insercao;
	Pokemon poke;

	insercao = ok = 0;
	// Nome do Pokemon
	while( !ok ){
		scanf( "%37[^\n]",poke.nome_pokemon );
		ignore( );
		codigo[2] = poke.nome_pokemon[0];	
		codigo[3] = poke.nome_pokemon[1];
		ok = 1;  i = 0; n = strlen( poke.nome_pokemon );
		
		// Verifica se ta ok
		while( i < n ){
			if( !isalpha( poke.nome_pokemon[i] ) ){
				ok = 0;
				break;
			}
			i++;
		}

		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
		    // printf( "foi o nome %s\n", poke.nome_pokemon );
		    // APAGAR
		}
	} 

	ok = 0;
	// Tipo do Pokemon
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]", poke.tipo_pokemon );
		ignore( );
		n = strlen( poke.tipo_pokemon ); 
		if( poke.tipo_pokemon[0] == '/' || poke.tipo_pokemon[n-1] == '/' ){
			ok = 0;
		}
		i = 0;
		while( i < n ){
			if( !isalpha( poke.tipo_pokemon[i] ) && poke.tipo_pokemon[i] != '/' ){
				ok = 0;
				break;
			}
			i++;
		}
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi o tipo %s\n", poke.tipo_pokemon );
			// APAGAR
		}
	}

	ok = 0;
	// Combat points
	while( !ok ){
		scanf( "%7[^\n]", poke.combat_points );
		ignore( );
		ok = verificacombats( poke.combat_points );
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi o combat points %s\n", poke.combat_points );
			// APAGAR
		}
	}

	ok = 0;
	// Data de captura
	while( !ok ){
		scanf( "%8[^\n]", poke.data_captura );
		ignore( );
		n = strlen( poke.data_captura );
		j = i = 0;
		// Pegar o dd
		while( i < n && poke.data_captura[i] != '/' ){
			str1[j++] = poke.data_captura[i++];
		}
		str1[j] = '\0'; // Pegou o dia
		codigo[4] = str1[0];
		codigo[5] = str1[1];
		
		ok = ( isdigit( str1[0] ) )?1:0; 
	
		if( ok )
			dia = atoi( str1 ); // Converteu
		
		i++; // Saiu do /
		j = 0; // Zerou o j

		while( i< n && poke.data_captura[i] != '/' ){
			str1[j++] = poke.data_captura[i++];
		}
	
		codigo[6] = str1[0];
		codigo[7] = str1[1];
		str1[j] = '\0'; // Pegou o mes
		
		if( ok ){
			ok = ( isdigit( str1[0] ) )?1:0; 
			if( ok )
				mes = atoi( str1 ); // Converteu
		}
		i++; // Saiu do /
		j = 0;// Zerou o j

		// Pegar o aa
		while( i < n ){
			str1[j++] = poke.data_captura[i++];
		}
		str1[j] = '\0';   

		if( ok ){
			ok = ( isdigit( str1[0] ) )?1:0;
			if( ok ){
				ano = atoi( str1 ); // Converteu	
				ok = verificadata( dia, mes, ano );
			}
		}
		if( n != 8 ) 
			ok = 0;
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi a data %s\n", poke.data_captura );
			// APAGAR
		}
	}

	ok = 0;
	// Hora da captura
	while( !ok ){
		ok = 1;
		scanf( "%5[^\n]",poke.hora_captura );
		ignore( );
		n = strlen( poke.hora_captura );
		if( n != 5 ){
			ok = 0;
		}
		if( poke.hora_captura[2] != ':' ){
			ok=0;
		}

		j = i = 0;
		while( j < 2 ){
			if( !isdigit( poke.hora_captura[i] ) ){
				ok =0;
			}
			str1[j++] = poke.hora_captura[i++];
		}
		
		str1[j] = '\0';
		codigo[8] = str1[0];
		codigo[9] = str1[1];

		// Pega hh
		if( ok ){
			x = atoi( str1 );
			if( !( x >= 0 && x <= 23 ) ){
				ok = 0;
			}
		}
		if( ok ){
			j = 0;
			i++;
			while( i < n ){
				if( !isdigit( poke.hora_captura[i] ) ){
					ok =0;
				}
				str1[j++] = poke.hora_captura[i++];
			}
		}
		str1[j] = '\0';
		codigo[10] = str1[0];
		codigo[11] = str1[1];
		if( ok ){
			x = atoi( str1 );
			if( !( x >= 0 && x <= 59 ) ){
				ok = 0;
			}
		}
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi a hora %s\n", poke.hora_captura );
			// APAGAR
		}
	}

	ok = 0; 
	// Nome do treinador
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]", poke.nome_treinador );
		ignore( );
		codigo[1] = poke.nome_treinador[0];
		n = strlen( poke.nome_treinador );
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi o nome do treinador %s\n", poke.nome_treinador );
			// APAGAR
		}
	}

	ok = 0;
	// Nivel do treinador
	while( !ok ){
		ok = 1;
		scanf( "%3[^\n]", poke.nivel_treinador );
		ignore( );
		n = strlen( poke.nivel_treinador );
		if( n != 3 ){
			ok = 0;
		}
		x = atoi( poke.nivel_treinador  );
		if( !( x >=1 && x <= 100 ) ){
			ok = 0;
		}
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi o nivel %s\n", poke.nivel_treinador  );
			// APAGAR
		}
	}

	ok = 0;
	// Nome da equipe
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]", poke.nome_equipe );
		ignore( );
		codigo[0] = poke.nome_equipe[0];
	 	aumenta( poke.nome_equipe );
		if( !( !( strcmp( poke.nome_equipe, "VALOR" ) ) || !( strcmp( poke.nome_equipe, "INSTINCT" ) ) || !( strcmp( poke.nome_equipe, "MYSTIC" ) ) ) ){
			ok = 0;
		}
	
		if( !ok ){
			printf( CAMPO_INVALIDO );
			// APAGAR
			// printf( "foi a equipe %s\n", poke.nome_equipe );
			// APAGAR
		}
	}
	codigo[12] = '\0';

	// Aumenta todo mundo
	aumenta( codigo );
	aumenta( poke.nome_pokemon ); aumenta( poke.tipo_pokemon );
	aumenta( poke.combat_points ); aumenta( poke.data_captura ); 
	aumenta( poke.hora_captura ); aumenta( poke.nome_treinador );
	aumenta( poke.nivel_treinador ); aumenta( poke.nome_equipe );

	// Imprime todo mundo 
	// APAGAR
	/*puts( codigo );
	puts( poke.nome_pokemon ); puts( poke.tipo_pokemon );
	puts( poke.combat_points ); puts( poke.data_captura ); 
	puts( poke.hora_captura ); puts( poke.nome_treinador );
	puts( poke.nivel_treinador ); puts( poke.nome_equipe );*/
	// APAGAR

	if( iprimary->raiz != NULL ){
		acha = busca( iprimary->raiz, codigo, 0 ); // Verifica se ja inseriu
		if( acha == NULL ){
			insercao = 1;
		}

		else{
			printf( ERRO_PK_REPETIDA, codigo );
		}
	}
	else{	
		insercao = 1;
	}
	if( insercao ){ // Nao inseriu ainda
		// Pega os tamanhos
		tamanhos[0] = strlen( codigo ); tamanhos[1] = strlen( poke.nome_pokemon ); tamanhos[2] = strlen( poke.tipo_pokemon ); 
		tamanhos[3] = strlen( poke.combat_points ); 	tamanhos[4] = strlen(  poke.data_captura ); tamanhos[5] = strlen( poke.hora_captura );
		tamanhos[6] = strlen(  poke.nome_treinador ); tamanhos[7] = strlen( poke.nivel_treinador ); tamanhos[8] = strlen( poke.nome_equipe );

		// Insere
		strcpy( aux.pk, codigo );
		aux.rrn = *nregistros;
		insere( iprimary, aux );

		// Escreve no dat
		strcat( ARQUIVO, codigo);
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.nome_pokemon );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.tipo_pokemon );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.combat_points );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.data_captura );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.hora_captura );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.nome_treinador );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.nivel_treinador );
		strcat( ARQUIVO, "@");strcat( ARQUIVO, poke.nome_equipe );
		strcat( ARQUIVO, "@");
		
		// Descobre quantas # precisa 
		soma = 9;
		for( i = 0; i < 9; i++ ){
			soma+= tamanhos[i];
		}

		// Coloca as # que faltam
		while( soma < 192 ){
			soma++;
			strcat( ARQUIVO, "#" );
		}	
		// Escreve no Ipokemon
		strcpy( ipokemon[( *nregistros )].primary_key, codigo );
		strcpy( ipokemon[( *nregistros )].nome_pokemon, poke.nome_pokemon );

		// Escreve no Iteam
		strcpy( iteam[( *nregistros )].primary_key, codigo );
		strcpy( iteam[( *nregistros )].nome_equipe, poke.nome_equipe );
		
		// Atualiza a quantidade de registros
		( *nregistros )++;

		// Ordena
		// qsort ( ipokemon, ( *nregistros ), sizeof( Ipokemon  ), compare2 );
		// qsort ( iteam, ( *nregistros ), sizeof( Iteam  ), compare3 );
   		 msortpokeip( ipokemon , 0, ( ( *nregistros ) - 1 ) );
		 msortteamip( iteam , 0, ( ( *nregistros ) - 1 ) );
	}
}

