
/* ================================================================== *
    Universidade Federal de Sao Carlos - UFSCAR

    Disciplina: Estruturas de Dados 2
    Nome: Carlos A. B. de Andrade RA: 613827
    Prof. Tiago A. Almeida


    Trabalho 1 - Indexacao
    
* ================================================================== */

#include "613827_ED2_T01.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

ll contador = 0;
ll DIM = 5000000;


// aloca dinamicamente um espaco de 5000000
void inicia( ){
	d = malloc( DIM * sizeof( struct armazena1 ) ); 
	e =  malloc( DIM * sizeof( struct armazena2 ) );
}

// realoca mais memoria caso esteja quase acabando;
void realoca( ){
	DIM*=500000;
	d = realloc( d, DIM * sizeof( struct armazena1 ) ); 
	e =  realloc( e, DIM * sizeof( struct armazena2 ) );
}

// verifica se o arquivo existe
int abrearquivo( char arq[] ){
	FILE *fp;
	if( ( fp = fopen( arq, "r+" ) ) != NULL ){
			return 1;
	} 
	return 0;
}

void criaarquivo( char arq[] ){
	FILE *fp;
	fp = fopen( arq, "w+" );
}

void naotaconsistente( ){
	FILE *fp1; // ponteiro para o arquivo
	ll n, rrn, x; // tamanho do arquivo e rrn
	fp1 = fopen( "pokemons.dat", "r" );
	int i, cont; // tamanho da string e delimitador do campo
	char c, str1[40]; // caractere para ler do arquivo e string para armazenar campos
	fseek( fp1, 0, SEEK_END ); 
	n = ftell( fp1 ); rrn = 1; i = 0; x = 1;
	fseek( fp1, 0 , SEEK_SET );

	while( x < n ){
		fseek( fp1, x-1 , SEEK_SET );
		x+=192;
		cont = 0;
		while( cont < 2 ){
			fscanf( fp1, "%c", &c );
			if( c == '@' ){
				cont++;
				if( cont == 1 ){
					str1[i] = '\0'; // terminou a chave primaria
		  			strcpy( d[contador].str, str1 ); // copia a chave primaria
					d[contador].num = rrn; // copia o rrn
		  			strcpy( e[contador].str, str1 ); // copia a chave primaria
					i = 0; rrn++; // proximo rrn e zera a string
				}
				if( cont == 2 ){
					str1[i] = '\0'; // terminou o nome do pokemon
					strcpy( e[contador].nomepk, str1 ); // copia o nome do pokemon
					i = 0; // zera a string
					contador++;  // vai pro proximo indice
				}
			}
			else{
				if( c != '\n' )
					str1[i++] = c;
			}
		}
	}
	fclose( fp1 );
}

int estaconsistente( char arq[] ){
	FILE *fp;
	char str1[40], str2[40], c;
	ll val;
	fp = fopen( arq, "r+" );
	fseek( fp, 0, SEEK_END );
   	val = ftell( fp );
	fseek( fp, 0, SEEK_SET );
	int flag = 1;
	if( val ){
		if( strcmp( arq , "iprimary.idx" ) == 0 ){ 
			c = fgetc( fp );
			if( c == '1' ){// '1', o arquivo iprimary.idx esta consistente e voce pode carregar o indice em RAM com o que esta no arquivo de indice
				while( ( fscanf ( fp, "%s %lld", str1, &val ) ) != EOF ){
					  strcpy( d[contador].str, str1 );
					  d[contador].num = val;
					  contador++;
				}
			}
			else{
				flag = 0;
			}
		}

		else if(  strcmp( arq , "ipokemon.idx" ) == 0 ){
			val = 0;
			c = fgetc( fp );
			if( c == '1' ){// '1', o arquivo ipokemon.idx esta consistente e voce pode carregar o indice em RAM com o que esta no arquivo de indice
				while( ( fscanf ( fp, "%s %s", str1, str2 ) ) != EOF ){
					 strcpy( e[val].str, str1 );
					 strcpy( e[val].nomepk, str2 );
					 val++;
				}
			}
			else{
				flag = 0;
			}
		}

		else if(  strcmp( arq , "iteam.idx" ) == 0){
			c = fgetc( fp );
			if( c == '0' )
				flag = 0 ;
		}
	}
	else{
		flag = 0;
	}

	fclose( fp );
	c = '0';
	fp = fopen( arq, "w+" );
	fprintf( fp, "%c", c );
	fclose( fp );

	return flag;
	
}	


// ordena pela chave primaria
int compare( const void *a, const void *b ) {
	dados *x = ( dados * )a;
	dados *y = ( dados * )b;
	return strcmp( x->str, y->str );
}

// ordena pelo nome, em caso de nomes iguais desempata com a chave primaria
int compare2(const void *a, const void *b ) {
	dados2 *x = ( dados2 * )a;
	dados2 *y = ( dados2 * )b;
	if( ( strcmp( x->nomepk, y->nomepk ) ) != 0 ){
		return strcmp( x->nomepk, y->nomepk );
	}
	else{
		return strcmp( x->str, y->str );
	}
}

// ordena pelo rrn
int compare3( const void * a, const void * b ){
  dados *x = ( dados * )a;
  dados *y = ( dados * )b;
  return ( x->num - y->num );
}

// recebe uma string como parametro e transforma todos os caracteres em maiusculo
void aumenta( char str[] ){
	int i, n;
	n = strlen( str );
	for( i =0; i < n; i++ ){
		str[i] = toupper( str[i] );
	}	
}

// ordena
void ordena( ){
	qsort ( d, contador, sizeof( dados ), compare );
	qsort ( e, contador, sizeof( dados2 ), compare2 );
}

// busca por chave
ll busca( char chave[] ){
	/*ll i;
	for( i = 0; i < contador; i++ ){
		if ( strcmp( d[i].str , chave ) == 0 )
			return i;
	}
	return -1;*/
	ll fim, ini, best, meio;
	fim = contador-1;
	ini = 0; 
	best = -1;

	while( ini <= fim ) {
		meio = ( ini + fim ) / 2;
		if(  strcmp( chave, d[meio].str ) == 0  ){
			best = meio;
			fim = meio - 1;
		}
		if(  strcmp( chave, d[meio].str ) < 0  ){
			fim = meio - 1;
		}
		if(  strcmp( chave, d[meio].str ) > 0  ){
			ini = meio + 1;
		}
	}
	return best;
}

// busca pelo nome do pokemon
ll buscanome( char chave[] ){
	ll fim, ini, best, meio;
	fim = contador-1;
	ini = 0; 
	best = -1;
	// busca binaria
	while( ini <= fim ) {
		meio = ( ini + fim ) / 2;
		if(  strcmp( chave, e[meio].nomepk ) == 0  ){
			best = meio;
			fim = meio - 1;
		}
		if(  strcmp( chave, e[meio].nomepk ) < 0  ){
			fim = meio - 1;
		}
		if(  strcmp( chave, e[meio].nomepk ) > 0  ){
			ini = meio + 1;
		}
	}
	return best;
}

// busca por equipe
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

// busca um caractere
ll buscacaractere( char chave ){
	ll fim, ini, best, meio;
	fim = contador-1;
	ini = 0; 
	best = -1;
	// busca binaria
	while( ini <= fim ) {
		meio = ( ini + fim ) / 2;
		if(  chave == d[meio].str[0] ){
			best = meio;
			fim = meio - 1;
		}
		if(   chave < d[meio].str[0] ){
			fim = meio - 1;
		}
		if(  chave > d[meio].str[0]  ){
			ini = meio + 1;
		}
	}
	return best;
}


// pega lixo da string
void leresto( ){
	char c;
	 while ( ( c = getchar( ) ) != '\n' && c != EOF );
}

// verifica se a data eh valida
int validadata( int dia, int mes, int ano ){
	int ok = 1;
	if( !( ano >= 16 ) ){ // checa ano
		ok = 0;
	}
	if( !( mes >= 1 && mes <= 12 ) ){ // checa mes
		ok = 0;
	}
	if( !( dia >= 1 ) ){ // checa se dia nao eh negativo
		ok = 0;
	}
	if( mes == 2 ){ // se for ano bissexto muda o nro de dias em fevereiro
		if ( ( ano % 4 == 0 && ano % 100 != 0 ) || ano % 400 == 0 ){
			if( !( dia <= 29 ) ){
				ok = 0;
			}
		}
		else if( !( dia <= 28 ) ){ 
			ok = 0;
		}
	}
	else if( mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes ==8 || mes == 10 || mes == 12 ){ // meses que vao ate 31
		if(  !( dia <= 31 ) ){
			ok = 0;
		}
	}
	else if(  !( dia <= 30 ) ){
		ok = 0;
	}
	return ok;
}

// verifica os combat points
int verificacombats( char str[] ){
	int i, n, ok, x;
	ok = 1; i = 0;
	n = strlen( str );
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

// imprime a partir do arquivo de dados
void imprime( FILE *fp, char str[], ll x ){
	fseek( fp, ( ( d[x].num-1 ) * 192 ), SEEK_SET ); 
	int cont, i;
	i = cont = 0;
	char c, str1[40];
		while( cont < 9 ){	
			fscanf( fp, "%c", &c );
			if( c == '@'){	
				cont++;	
				str1[i]='\0';
				printf( "%s\n", str1 );
				i = 0;
			}
			else{
				if( c != '#' && c!= '\n' )
					str1[i++] = c;
			}			
		}
}


// 1 - INSERCAO
void cadastro( ){

	FILE *fp;
	fp = fopen( "pokemons.dat", "a+" );
	char str1[40], codigo[14];
	int ok, i, n, x, j, tamanhos[9], dia, mes, ano;
	int soma;

	ok = 0;
	// nome do Pokemon
	while( !ok ){
		scanf( "%37[^\n]",pokemon.nomepoke );
		leresto( );
		codigo[2] = pokemon.nomepoke[0];	
		codigo[3] = pokemon.nomepoke[1];
		ok = 1;  i = 0; n = strlen( pokemon.nomepoke );
		
		// verifica se ta ok
		while( i < n ){
			if( !isalpha( pokemon.nomepoke[i] ) ){
				ok = 0;
				break;
			}
			i++;
		}

		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi o nome");
		}
	} 

	ok = 0;
	// tipo do Pokemon
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]",pokemon.tipopoke );
		leresto( );
		n = strlen( pokemon.tipopoke ); 
		if( pokemon.tipopoke[0] == '/' || pokemon.tipopoke[n-1] == '/' ){
			ok = 0;
		}
		i = 0;
		while( i < n ){
			if( !isalpha( pokemon.tipopoke[i] ) && pokemon.tipopoke[i] != '/' ){
				ok = 0;
				break;
			}
			i++;
		}
		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi o tipo");
		}
	}

	ok = 0;
	// combat points
	while( !ok ){
		scanf( "%7[^\n]",pokemon.combat );
		leresto( );
		ok = verificacombats( pokemon.combat );
		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi o combat points");
		}
	}

	ok = 0;
	// data de captura
	while( !ok ){
		scanf( "%8[^\n]", pokemon.data );
		leresto( );
		n = strlen( pokemon.data );
		j = i = 0;
		// pegar o dd
		while( pokemon.data[i] != '/' ){
			str1[j++] = pokemon.data[i++];
		}
		str1[j] = '\0'; // pegou o dia
		codigo[4] = str1[0];
		codigo[5] = str1[1];

		dia = atoi( str1 ); // converteu
	
		i++; // saiu do /
		j = 0; // zerou o j

		while( pokemon.data[i] != '/' ){
			str1[j++] = pokemon.data[i++];
		}
		codigo[6] = str1[0];
		codigo[7] = str1[1];
		str1[j] = '\0'; // pegou o mes
		mes = atoi( str1 );// converteu

		i++; // saiu do /
		j = 0;// zerou o j

		// pegar o aa
		while( i <n ){
			str1[j++] = pokemon.data[i++];
		}
		str1[j] = '\0';   // pegou o ano
		ano = atoi( str1 );// converteu

		ok = validadata( dia, mes, ano );
		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi a data");
		}
	}

	ok = 0;
	// hora da captura
	while( !ok ){
		ok = 1;
		scanf( "%5[^\n]",pokemon.hora );
		leresto( );
		n = strlen( pokemon.hora );
		if( n != 5 ){
			ok = 0;
		}
		if( pokemon.hora[2] != ':' ){
			ok=0;
		}

		j = i = 0;
		while( j < 2 ){
			if( !isdigit( pokemon.hora[i] ) ){
				ok =0;
			}
			str1[j++] = pokemon.hora[i++];
		}
		
		str1[j] = '\0';
		codigo[8] = str1[0];
		codigo[9] = str1[1];

		// pega hh
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
				if( !isdigit( pokemon.hora[i] ) ){
					ok =0;
				}
				str1[j++] = pokemon.hora[i++];
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
			printf( "Campo inválido! Informe novamente: " );
			 // printf("foi a hora");
		}
	}

	ok = 0; 
	// nome do treinador
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]", pokemon.nometrei );
		leresto( );
		codigo[1] = pokemon.nometrei[0];
		n = strlen( pokemon.nometrei );
		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi o nome do treinador");
		}
	}

	ok = 0;
	// nivel do treinador
	while( !ok ){
		ok = 1;
		scanf( "%3[^\n]", pokemon.nivel );
		leresto( );
		n = strlen( pokemon.nivel );
		if( n != 3 ){
			ok = 0;
		}
		x = atoi( pokemon.nivel  );
		if( !( x >=1 && x <= 100 ) ){
			ok = 0;
		}
		if( !ok ){
			printf( "Campo inválido! Informe novamente: " );
			// printf("foi o nivel");
		}
	}

	ok = 0;
	// nome da equipe
	while( !ok ){
		ok = 1;
		scanf( "%37[^\n]", pokemon.nomeequi );
		leresto( );
		codigo[0] =pokemon.nomeequi[0];
	 	aumenta( pokemon.nomeequi );
		if( !( !( strcmp( pokemon.nomeequi, "VALOR" ) ) || !( strcmp( pokemon.nomeequi, "INSTINCT" ) )  || !( strcmp( pokemon.nomeequi, "MYSTIC" ) ) ) ){
			ok = 0;
		}
	
		if( !ok ){
			printf( "Campo inválido! Informe novamente: ");
			// printf("foi a equipe");
		}
	}
	codigo[12] = '\0';
	
	// aumenta todo mundo
	aumenta( codigo );
	aumenta( pokemon.nomepoke ); aumenta( pokemon.tipopoke );
	aumenta( pokemon.combat ); aumenta( pokemon.data ); 
	aumenta( pokemon.hora ); aumenta( pokemon.nometrei );
	aumenta( pokemon.nivel ); aumenta( pokemon.nomeequi );

	// ja existe o codigo gerado
	if( busca( codigo ) != -1 ){
		printf( "ERRO: Já existe um registro com a chave primária: %s.\n", codigo );
	}
	else{ // pega os tamanhos
		tamanhos[0] = strlen( codigo ); tamanhos[1] = strlen( pokemon.nomepoke ); tamanhos[2] = strlen( pokemon.tipopoke ); 
		tamanhos[3] = strlen( pokemon.combat ); 	tamanhos[4] = strlen( pokemon.data ); tamanhos[5] = strlen( pokemon.hora );
		tamanhos[6] = strlen( pokemon.nometrei ); tamanhos[7] = strlen( pokemon.nivel ); tamanhos[8] = strlen( pokemon.nomeequi );

		// escreve no dat
		fprintf( fp, "%s", codigo );
		fprintf( fp, "@%s", pokemon.nomepoke );
		fprintf( fp, "@%s", pokemon.tipopoke );
		fprintf( fp, "@%s", pokemon.combat );
		fprintf( fp, "@%s", pokemon.data );
		fprintf( fp, "@%s", pokemon.hora );
		fprintf( fp, "@%s", pokemon.nometrei );
		fprintf( fp, "@%s", pokemon.nivel );
		fprintf( fp, "@%s", pokemon.nomeequi );
		fprintf( fp, "@" );
		
		// descobre quantas # precisa 
		soma = 9;
		for( i = 0; i < 9; i++ ){
			soma+= tamanhos[i];
		}

		// coloca as # que faltam
		while( soma < 192 ){
			soma++;
			fprintf( fp, "#" );
		}	

		// pega a posicao no dat
		fseek( fp, 0, SEEK_END ); 
		ll pos = ftell( fp );
		pos/= 192;

		// guarda o pokemon na ram
		strcpy( d[contador].str, codigo );
		d[contador].num=pos;

		strcpy( e[contador].str, codigo );
		strcpy( e[contador].nomepk, pokemon.nomepoke );

	 	contador++;
			
		// precisa realocar
		if( contador == DIM-50 ){
			realoca( );
		}
		
		// reordena
		ordena( );

	}

	fclose( fp );
}

// 2 - ALTERACAO
void alterar( ){
	char str[40], str1[40], c;
	ll x;
	int ok , cont;
	scanf( "%12[^\n]", str1 ); // le o codigo do pokemon que vai alterar os combat points
	leresto( );
	aumenta( str1 );
	x = busca( str1 ); // verifica se esse codigo existe
	if( x != -1 && d[x].num != -1 ){
		ok = 0;
		while( !ok ){
			scanf( "%7[^\n]", str ); // le novo combat points
			leresto( );
			ok = verificacombats( str ); // verifica se eh valido
			if( !ok ){
				printf( "Campo inválido! Informe novamente: " );
			}
			else{
				FILE *fp = fopen( "pokemons.dat", "r+" );
				cont = 0;
				fseek( fp, ( ( d[x].num - 1 ) * 192 ), SEEK_SET );

				while(  cont < 3 ){
					fscanf( fp, "%c", &c );
					if( c == '@' ){	
						cont++;	
					}					
				}
				fprintf( fp, "%s@",  str );	
				fclose( fp );
			}
		}
	}
	else{
		// printf("Registro não encontrado! %s\n", str1 );
		printf( "Registro não encontrado!\n" );
	}

}

// 3 - REMOCAO
void remover( ){

	char str[40];
	ll x;
	scanf( "%12[^\n]", str ); 	
	leresto( );
	aumenta( str );
	x = busca( str ) ;

	if( x != -1 && d[x].num != -1 ){
		FILE *fp = fopen( "pokemons.dat", "r+" );
		fseek( fp, ( ( d[x].num - 1 ) * 192 ), SEEK_SET );
		d[x].num = -1;
		fprintf( fp, "*|" );	
		fclose( fp );	
	}
	else{
		// printf("Registro não encontrado! %s\n", str );
		printf( "Registro não encontrado!\n" );
	}
}


// 4 - BUSCA
void pesquisar( ){
	char str[40];
	ll x, op;
	int achou;
	scanf( "%lld", &op );
	getchar( );
	if( op == 1 ){
		// pesquisar por codigo
		scanf( "%14[^\n]", str ); 	
		leresto( );
		aumenta( str );
		x = busca( str );
		if( x != -1 && d[x].num != -1 ){
			FILE *fp = fopen( "pokemons.dat", "r+" );
			imprime( fp, str, x );
			printf( "\n" );
			fclose( fp ); 
		}
		else{
			// printf("Registro não encontrado! %s\n", str );
			printf( "Registro não encontrado!\n" );
		}
	}
	
	else if( op == 2 ){
		// pesquisar por nome
		scanf( "%37[^\n]", str ); //l e o nome do pokemon
		leresto( );
		aumenta( str );
		op = buscanome( str ); // busca pelo nome do pokemon
		achou = 0;
		if( op != -1 ){
			FILE *fp = fopen( "pokemons.dat", "r+" ); // abre  o arquivo
			while( !( strcmp( str, e[op].nomepk ) ) ){
				x = busca( e[op].str ); // busca pelo codigo do pokemon
				if( d[x].num != -1 ){ 
					achou=1;
					imprime( fp, str, x );
					printf("\n");
				}
				op++;
			}
			fclose( fp );
		}
		if( !achou ) {
			// printf("Registro não encontrado! %s\n", str );
			printf( "Registro não encontrado!\n" );
		}
	}	

	else if( op == 3 ){
		// pesquisar por equipe
		scanf( "%37[^\n]", str ); 
		leresto( );
		aumenta( str );
		x = buscaequipe( str ); // para saber se a equipe existe
		if( x != -1 ){
			x = buscacaractere( str[0] );
			FILE *fp = fopen( "pokemons.dat", "r+");
			while( x < contador && d[x].str[0] == str[0] ){
				if( d[x].num != -1 ){
					imprime( fp, str, x );
					printf( "\n" );
				}
				x++;
			}
			fclose( fp ); 
		}
		else{
			// printf("Registro não encontrado! %s\n", str );
			printf( "Registro não encontrado!\n" );
		}
	}
}

// 5 - LISTAGEM
void listagem(  ){
	ll x, j;
	int op;
	scanf( "%d", &op );
	getchar( );
	int vazio =1;
	char str[40], c;

	if( op == 1 || op == 3 ){
		FILE *fp = fopen( "pokemons.dat", "r+");
		for( x = 0; x < contador; x++){
			if( d[x].num != -1 ){
				vazio = 0;
				imprime( fp, str, x );
				printf( "\n" );
			}
		}
		fclose( fp ); 
	}

	else if( op == 2 ){
		FILE *fp = fopen( "pokemons.dat", "r+" );
		for( j = 0; j < contador; j++ ){
			x = busca( e[j].str );
			if( d[x].num != -1 ){
				fseek( fp, ( ( d[x].num - 1 ) * 192 ), SEEK_SET );
				vazio = 0;
				imprime( fp, str, x );
				printf( "\n" );
			}
		}
		fclose( fp ); 
	}

	if( vazio ){
		printf( "Arquivo vazio!" );
	}
}

// 6 - LIBERA
void libera( ){

	FILE *fp = fopen( "pokemons.dat", "r+" );
	FILE *fp1 = fopen( "pokemonscopy.dat", "w+" );

	char c, str1[40], str2[40];
	c = fgetc( fp );
    	while( c != EOF ){
		fputc( c, fp1 );
	 	c = fgetc( fp );
    	}
	fclose( fp );
	fclose( fp1 );

	ll x, pos;
	int i;

	FILE *fp3 =  fopen( "ipokemoncopy.idx", "w+" );
	for( x =0 ; x < contador; x++ ){
		pos = busca ( e[x].str );
		if( pos != -1 ){
			if( d[pos].num != -1 )
				fprintf( fp3, "%s %s\n", e[x].str, e[x].nomepk );
		}
	}

	fclose( fp3 );

	fp3 = fopen( "ipokemoncopy.idx", "r+" );
	x = 0;
	while( ( fscanf ( fp3, "%s %s", str1, str2 ) ) != EOF ){
		 strcpy( e[x].str, str1 );
		 strcpy( e[x].nomepk, str2 );
		 x++;
	}
	
	fclose( fp3 );
	remove( "ipokemoncopy.idx" );


	qsort ( d, contador, sizeof( dados ), compare3 );
	

	x = 0;
	while( d[x].num == -1 ){
		x++;
	}
	
	contador-= x;

	for( pos = 0; pos < contador; pos++ ){
		d[pos] = d[x++];
	}

	fp = fopen( "pokemons.dat", "w+" );
	fp1 = fopen( "pokemonscopy.dat", "r+" );

	for( x = 0; x < contador ; x++ ){
		fseek( fp1, ( ( d[x].num - 1 ) * 192 ), SEEK_SET );
		i=0;
		while( i < 192 ){
			fscanf( fp1, "%c", &c );
				fprintf( fp, "%c", c );
			i++;
		}
		fseek( fp, 0, SEEK_END ); 
		pos = ftell( fp );
		pos/= 192;
		d[x].num = pos;	
	}
	qsort ( d, contador, sizeof( dados ), compare );
	fclose( fp );
	fclose( fp1 );
	remove( "pokemonscopy.dat" );
}

// 7 - FINALIZA
void finaliza( ){

    FILE *fp;
    ll i, x; 
    char val = '0';
     
     val = '0';

	// atualiza o iprimary
	fp = fopen( "iprimary.idx", "w" );
	fprintf( fp, "%c\n", val );
	for( x = 0; x < contador-1; x++ ){
		fprintf( fp, "%s %lld\n", d[x].str, d[x].num );
	}
	fprintf( fp, "%s %lld", d[x].str, d[x].num );

	fseek( fp, 0, SEEK_SET );
	val = '1';
	fprintf( fp, "%c", val );
	fclose( fp );

	val = '0';

	// atualiza o ipokemon
	fp = fopen( "ipokemon.idx", "w" );
	fprintf( fp, "%c\n", val );
	for( x = 0; x < contador-1; x++ ){
		fprintf( fp, "%s %s\n", e[x].str, e[x].nomepk );
	}
	
	fprintf( fp, "%s %s", e[x].str, e[x].nomepk );	
	

	fseek( fp, 0, SEEK_SET );
	val = '1';
	fprintf( fp, "%c", val );
	fclose( fp );

	val = '0';

	// atualiza o iteam
	fp = fopen( "iteam.idx", "w" );
	fprintf( fp, "%c\n", val );
	for( x = 0; x < contador-1; x++ ){	
		if( d[x].str[0] == 'I' ){
			fprintf( fp, "%s INSTINCT\n", d[x].str );
		}
		else if( d[x].str[0] == 'M' ){
			fprintf( fp, "%s MYSTIC\n", d[x].str );
		}
		else if( d[x].str[0] == 'V' ){
			fprintf( fp, "%s VALOR\n", d[x].str );
		}	
	}

	if( d[x].str[0] == 'I' ){
		fprintf( fp, "%s INSTINCT", d[x].str );
	}
	else if( d[x].str[0] == 'M' ){
		fprintf( fp, "%s MYSTIC", d[x].str );
	}
	else if( d[x].str[0] == 'V' ){
		fprintf( fp, "%s VALOR", d[x].str );
	}

	fseek( fp, 0, SEEK_SET );
	val = '1';
	fprintf( fp, "%c", val );
	fclose( fp );
	

	free( d );
	free( e ); 
}

