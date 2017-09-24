#include<stdio.h>
#include"613827_ED2_T01.h"


int main( ){

	char c, str1[14];
	int op;


	if( abrearquivo( "pokemons.dat" ) ){ /* 1. Verificar se existe o arquivo de dados. */
		if( abrearquivo("iprimary.idx" ) && abrearquivo( "ipokemon.idx" ) && abrearquivo( "iteam.idx" ) ){ /* 2. Verificar se existem os arquivos de ı́ndices */ 	
			/* Se existirem: verificar se estão consistentes com o arquivo de dados ( usar uma flag para isso ). */
			inicia( );
			if( !( estaconsistente( "iprimary.idx" ) && estaconsistente( "ipokemon.idx" ) &&  estaconsistente( "iteam.idx" ) ) ){
				naotaconsistente( );
			} 
		}
		else{
			/* Se não existirem: criar os ı́ndices na RAM e gravá-los no disco. */
			criaarquivo( "iprimary.idx" ); criaarquivo( "ipokemon.idx" ); criaarquivo( "iteam.idx" );
			/* Se existirem: verificar se estão consistentes com o arquivo de dados ( usar uma flag para isso ). */
			inicia( );
			if( !( estaconsistente( "iprimary.idx" ) && estaconsistente( "ipokemon.idx" ) &&  estaconsistente( "iteam.idx" ) ) ){
				naotaconsistente( );
			} 
		}
		
	}

	else{
		criaarquivo( "pokemons.dat" ); /* 2. Se não existir: criar o arquivo de dados no disco, abrindo para escrita e leitura. */
		if( abrearquivo( "iprimary.idx" ) && abrearquivo( "ipokemon.idx" ) && abrearquivo( "iteam.idx" ) ){ /* 2. Verificar se existem os arquivos de ı́ndices */ 
			/* Se existirem: verificar se estão consistentes com o arquivo de dados ( usar uma flag para isso ). */
			inicia( );
			if( !( estaconsistente( "iprimary.idx" ) && estaconsistente( "ipokemon.idx" ) &&  estaconsistente( "iteam.idx" ) ) ){
				naotaconsistente( );
			} 
		}

		else{
			/* Se não existirem: criar os ı́ndices na RAM e gravá-los no disco. */
			criaarquivo( "iprimary.idx" ); criaarquivo( "ipokemon.idx" ); criaarquivo( "iteam.idx" );
			/* Se existirem: verificar se estão consistentes com o arquivo de dados ( usar uma flag para isso ). */
			inicia( );
			if( !( estaconsistente( "iprimary.idx" ) && estaconsistente( "ipokemon.idx" ) &&  estaconsistente( "iteam.idx" ) ) ){
				naotaconsistente( );
			} 
		}
	}
	ordena( );
	while( ( scanf( "%d", &op ) ) && ( op >= 1 && op <= 7 ) ){
		getchar();
		if( op == 1 ){ // INSERCAO
			cadastro( );	
		}
		if( op == 2 ){ // ALTERACAO
			alterar();
		}
		if( op == 3 ){ // REMOCAO
			remover();
		}
		if( op ==4 ){ // BUSCA
			pesquisar( );
		}
		if( op ==5 ){ // LISTAGEM
			listagem( );
		}

		if( op == 6 ){ // LIBERA
			libera( );
		}

		if( op == 7 ){ // FINALIZA
			finaliza( ); 
			break;
		}			
	}

	return 0;
} 
