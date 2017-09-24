/* ==========================================================================
 * Universidade Federal de São Carlos - Campus Sorocaba
 * Disciplina: Estruturas de Dados 2
 * Prof. Tiago A. de Almeida
 *
 * Trabalho 03B - Hashing com encadeamento
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
#define MAX_REGISTROS   5000
#define TAM_ARQUIVO     (MAX_REGISTROS * TAM_REGISTRO + 1)

/* Saídas do usuário */
#define OPCAO_INVALIDA 			"Opcao invalida!\n\n"
#define REGISTRO_N_ENCONTRADO 	"Registro nao encontrado!\n\n"
#define CAMPO_INVALIDO 			"Campo invalido! Informe novamente.\n\n"
#define ERRO_PK_REPETIDA 		"ERRO: Ja existe um registro com a chave primaria: %s.\n\n"
#define REGISTRO_INSERIDO 		"Registro %s inserido com sucesso.\n\n"

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

/* Registro da Tabela Hash
 * Contém a chave primária, o RRN do registro atual e o ponteiro para o próximo
 * registro. */
typedef struct chave Chave;
struct chave {
	char pk[TAM_PRIMARY_KEY];
	int rrn;
	Chave *prox;
};

/* Estrutura da Tabela Hash */
typedef struct {
  int tam;
  Chave **v;
} Hashtable;

/* Variáveis globais */
char ARQUIVO[TAM_ARQUIVO];


/* ==========================================================================
 * ========================= PROTÓTIPOS DAS FUNÇÕES =========================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore();

/* Recebe do usuário uma string simulando o arquivo completo. */
void carregar_arquivo();

/* Exibe o jogador */
void exibir_registro(int rrn);

/* <<< DECLARE AQUI OS PROTOTIPOS >>> */

/* Calcula o primeiro primo maior ou igual ao valor informado pelo usuário. */
int prox_primo( int tam );

/* Cria a tabela hash */
void criar_tabela( 	Hashtable *tabela, int tam );

/* Le os dados do arquivo dados e carrega para a tabela hash */
void carregar_tabela( Hashtable *tabela );

/* Inicializa a lista */
void inicializa( Chave **p );

/* Cadastrar um novo pokemon */
void cadastrar( Hashtable *tabela );

/* Recebe do usuario uma string com uma chave primaria e caso a chave ja esteja cadastrada altera os combat points */
void alterar( Hashtable tabela );

/* Busca um pokemon na tabela hash */
void buscar( Hashtable tabela );

/* Remove um pokemon caso ele exista */
void remover( Hashtable *tabela );

/* Imprime a tabela hash */ 
void imprimir_tabela( Hashtable tabela );

/* Recupera o registro do arquivo de dados a partir do RRN */
Pokemon recuperar_registro( int rrn );

/* Libera a memoria alocada para a criacao da tabela hash */
void liberar_tabela( Hashtable *tabela );

/* Insere na tabela hash */
int insere( Chave k, Hashtable *tabela );

/* Calcula a funcao de hash h(k) */
int h( char k[] , int T );

/* Recebe uma string como parametro e transforma todos os caracteres em maiusculo */
void aumenta( char str[] );

/* Verifica se os combat points sao validos */
int verificacombats( char str[] );

/* Verifica se a data eh valida */
int verificadata( int dia, int mes, int ano );

/* Pesquisa por um pokemon atraves do seu codigo */
int pesquisar( char str[], Hashtable tabela );

/* ==========================================================================
 * ============================ FUNÇÃO PRINCIPAL ============================
 * =============================== NÃO ALTERAR ============================== */
int main() {

	/* Arquivo */
	int carregarArquivo = 0;
	scanf("%d\n", &carregarArquivo); // 1 (sim) | 0 (nao)
	if (carregarArquivo) carregar_arquivo();



	/* Tabela Hash */
	int tam;
	scanf("%d", &tam);
	tam = prox_primo(tam); // retorna o proximo primo

	Hashtable tabela;
	criar_tabela(&tabela, tam); 
	if (carregarArquivo) carregar_tabela(&tabela);



	/* Execução do programa */
	int opcao = 0;
	while(opcao != 6) {
		scanf("%d", &opcao);
		switch(opcao) {

		case 1:
			getchar();
			cadastrar(&tabela);
			break;
		case 2:
			getchar();
			alterar(tabela);
			break;
		case 3:
			getchar();
			buscar(tabela);
			break;
		case 4:
		    getchar();
			remover(&tabela);
			break;
		case 5:
			imprimir_tabela(tabela);
			break;
		case 6:
			liberar_tabela(&tabela);
			break;

		case 10:
			printf("%s\n", ARQUIVO);
			break;

		default:
			ignore();
			printf(OPCAO_INVALIDA);
			break;
		}
	}
	return 0;
}



/* ==========================================================================
 * ================================= FUNÇÕES ================================
 * ========================================================================== */

/* Descarta o que estiver no buffer de entrada */
void ignore() {
	char c;
	while ((c = getchar()) != '\n' && c != EOF);
}

/* Recebe do usuário uma string simulando o arquivo completo. */
void carregar_arquivo() {
	scanf("%[^\n]\n", ARQUIVO);
}

/* Exibe o Pokémon */
void exibir_registro(int rrn) {

	Pokemon p = recuperar_registro(rrn);

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


/* <<< IMPLEMENTE AQUI AS FUNCOES >>> */

/* Cria a tabela hash */
void criar_tabela( 	Hashtable *tabela, int tam ){
	int i;
	tabela->tam = tam; // pega o tamanho da tabela
	tabela->v = malloc( tam * sizeof( Chave ) ); // aloca dinamicamente uma tabela de tamanho tam
	for( i = 0; i< tabela->tam; i++ ){
		inicializa( &tabela->v[i] ); // inicializa a lista encadeada
	}
}

/* Le os dados do arquivo dados e carrega para a tabela hash */
void carregar_tabela( Hashtable *tabela ){
	int i, nregistros;
	Pokemon p;
	Chave k;
	nregistros = strlen(ARQUIVO) / TAM_REGISTRO; // pega a quantidadde de registros do arquivo de dados
	for( i = 0; i< nregistros; i++ ){
		p = recuperar_registro( i ); // Le do arquivo de dados
		// Copia para a chave
		strcpy( k.pk,  p.primary_key );
		k.rrn = i;
		insere( k, tabela ); // Insere 
	}
}

/* Insere na tabela hash */
int insere( Chave k , Hashtable *tabela ){
	int hdek;
	Chave *no, *i;
	// Cria novo no
	no = ( Chave* )malloc( sizeof( Chave ) );
	strcpy( no->pk, k.pk );
	no->rrn = k.rrn;
	hdek = h( k.pk, tabela->tam );  // calcula a hash function
	i = tabela->v[hdek];
	// procura a posicao correta ( insere ordenado )
	while( i->prox != NULL && strcmp( i->prox->pk, k.pk ) < 0 ){
		i = i->prox;
	}
	// inseriu
	no->prox = i->prox;
	i->prox = no;
	return 1; 
}

/* Inicializa a lista */
void inicializa( Chave **i ){
	// Inicializa lista encadeada com no cabeca
	Chave *no;
	no = ( Chave* )malloc( sizeof( Chave ) );
	no->prox = NULL; 
	strcpy(no->pk, "");
	*i = no;
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

/* Calcula a funcao de hash h(k) */
int h( char k[] , int T ){
	// Calcula a h(k) 
	int i, n, s;
	n = strlen( k );
	s = i = 0;
	while ( i<n ){
		s+= ( k[i] *( i+1 ) )%T; 
		i++;
	}
	return s%T;
}

/* Calcula o primeiro primo maior ou igual ao valor informado pelo usuário. */
int prox_primo( int tam ){
	int d;
	if( tam <= 1 )  return 2; // retorna o menor primo sempre que tam for menor que o menor primo
	else if( !( tam%2 ) ){ 	
		if( tam != 2 ){	
			return prox_primo( tam+1 ); // se eh par e nao eh o 2 entao nao eh primo, testa o proximo valor impar
		}
	}
	else if( !( tam% 3 ) ){ 
		if( tam != 3 ){
			return prox_primo( tam+2 ); // se tam eh da forma tam = 3k e k != 1 entao nao eh primo, testa o proximo valor impar
		}
	}
	d = 5; // todos os primos sao da forma 6k+-1, Percorre-se ate a raiz de n, verifica se existe decomposicao em fatores primos que nao seja ele mesmo
	while( d*d <= tam ){
		if ( !( tam%d ) || !( tam%( d+2 ) ) ){
			return prox_primo( tam+2 ); // se existe algum divisor primo d tal que d != tam , testa o proximo valor impar 
		}
		d+= 6;
	}
	return tam; // retorna o valor primo.
}

/* Cadastrar um novo pokemon */
void cadastrar( Hashtable *tabela ){
	char str1[40], codigo[14];
	int ok, i, n, x, j, tamanhos[9], dia, mes, ano, soma, nregistros;
	Pokemon poke;
	Chave aux;	
	ok = 0;

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
			//printf( "foi o tipo %s\n", poke.tipo_pokemon );
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
			//printf( "foi o combat points %s\n", poke.combat_points );
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
			//printf( "foi a data %s\n", poke.data_captura );
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
			//printf( "foi a hora %s\n", poke.hora_captura );
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
			//printf( "foi o nome do treinador %s\n", poke.nome_treinador );
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
			//printf( "foi o nivel %s\n", poke.nivel_treinador  );
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
			//printf( "foi a equipe %s\n", poke.nome_equipe );
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

	/* Imprime todo mundo 
	APAGAR
	puts( codigo );
	puts( poke.nome_pokemon ); puts( poke.tipo_pokemon );
	puts( poke.combat_points ); puts( poke.data_captura ); 
	puts( poke.hora_captura ); puts( poke.nome_treinador );
	puts( poke.nivel_treinador ); puts( poke.nome_equipe );
	APAGAR 	*/

	if( pesquisar( codigo, *tabela ) != -1 ){
		printf( ERRO_PK_REPETIDA, codigo );
	}
	
	else{
		// Pega os tamanhos
		tamanhos[0] = strlen( codigo ); tamanhos[1] = strlen( poke.nome_pokemon ); tamanhos[2] = strlen( poke.tipo_pokemon ); 
		tamanhos[3] = strlen( poke.combat_points ); 	tamanhos[4] = strlen( poke.data_captura ); tamanhos[5] = strlen( poke.hora_captura );
		tamanhos[6] = strlen(  poke.nome_treinador ); tamanhos[7] = strlen( poke.nivel_treinador ); tamanhos[8] = strlen( poke.nome_equipe );

		nregistros = strlen(ARQUIVO) / TAM_REGISTRO;

		// Cria uma chave
		strcpy( aux.pk, codigo );
		aux.rrn = nregistros++ ;

		// Insere na Tabela Hash
		ok = insere( aux, tabela );

		printf("%s\n", codigo);
		printf( REGISTRO_INSERIDO, codigo );

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
	}
}

/* Recebe do usuario uma string com uma chave primaria e caso a chave ja esteja cadastrada altera os combat points */
void alterar( Hashtable tabela ){
	char str[40], str1[40], c;
	int ok, x, cont, j, k;
	scanf( "%12[^\n]", str1 ); // le o codigo do pokemon que vai alterar os combat points
	ignore( );
	aumenta( str1 );
	x = pesquisar( str1, tabela ); // busca pelo codigo na tabela

	if(	x != -1 ){
		ok = 0;
		while( !ok ){
			scanf( "%7[^\n]", str ); // le novo combat points
			ignore( );
			ok = verificacombats( str ); // verifica se eh valido
			if( !ok ){
				printf( CAMPO_INVALIDO );
				// APAGAR
				// printf( "foi o combat points do alterar %s\n", poke.combat_points );
				// APAGAR
			}
			else{
				cont = 0;
				j = ( x )*192; // Pega a posicao no arquivo de dados
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
		printf( REGISTRO_N_ENCONTRADO );
	}
}

/* Busca um pokemon na tabela hash */
void buscar( Hashtable tabela ){
	char str[40];
	int x;
	// Pesquisar por codigo
	scanf( "%12[^\n]", str );  // Le o codigo
	ignore( ); 
	aumenta( str ); // Aumenta ele
	x= pesquisar( str, tabela ); // busca pelo codigo na tabela
	if( x != -1 ){
		exibir_registro( x ); // Exibe os dados
	}
	else{
		printf( REGISTRO_N_ENCONTRADO ); // Nao achou
	}
}

/* Remove um pokemon caso ele exista */
void remover( Hashtable *tabela ){
	char str[40];
	int hdek, x, cont, k, l;
	Chave *i, *j;
	scanf( "%12[^\n]", str ); 	
	ignore( );
	aumenta( str );
	hdek = h( str, tabela->tam ); // calcula a hash function
	j = tabela->v[hdek]->prox;
	if( j == NULL ){
		printf( REGISTRO_N_ENCONTRADO );
		return;
	}
	i = j;
	while( j->prox != NULL && strcmp( j->pk, str ) ){ 
		i = j;	 	
		j = j->prox;
	}
	if( strcmp( j->pk, str ) ==0 ){
		i->prox = j->prox;
		x = j->rrn;
		k = ( x )*192; // Pega a posicao no arquivo de dados
		strcpy( str, "*|" ); 
		cont = strlen( str ); 
		l = 0;
		while( l < cont ){ // Sobrescreve o arquivo de dados
			ARQUIVO[k++] = str[l++];
		}
		free( j );
		return;
	} 
	printf( REGISTRO_N_ENCONTRADO );
}

void imprimir_tabela( Hashtable tabela ){
	int j;
	Chave *i;
	j=0; 
	while( j< tabela.tam ){
		printf( "[%d]", j );
		for( i = tabela.v[j]->prox; i != NULL; i = i->prox ){
			if( isalpha( i->pk[0] ) ){
				printf( " %s", i->pk );
			}
		}
		printf( "\n" );
		j++;
	}
	printf( "\n" );
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

/* Pesquisa por um pokemon atraves do seu codigo */
int pesquisar( char str[], Hashtable tabela ){
	int hdek;
	Chave *i;
	hdek = h( str, tabela.tam ); // calcula a hash function
	for( i = tabela.v[hdek]->prox; i != NULL; i = i->prox ){
		if( strcmp( i->pk, str ) ==0 ){ // achou
			return i->rrn;
		} 
	}
	return -1;
}

/* Libera a memoria alocada para a criacao da tabela hash */
void liberar_tabela( Hashtable *tabela ){
	int i;
	Chave *j, *k;
	for( i = 0; i< tabela->tam; i++ ){
		j = tabela->v[i]->prox;
		// Desaloca no por no
		while( j != NULL ){
			k = j->prox;
			free( j );
			j = k;
		}
		tabela->v[i]->prox = NULL;
	}
	free( tabela->v ); // Desaloca o vetor de ponteiros
}

