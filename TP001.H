/*********** Librerias utilizadas **************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<malloc.h>
int numLinea = 1;
/***********************************************/
struct token{
	int  posi;
	char c_lex[20];
	char d_lex[20];
};

struct lectura{
	int  posi;
	char lex[20];
	int  linea;
	char d_lex[255];
};
struct espera{
	char esp[20];
};
typedef struct token token_t;
typedef struct lectura lectura_t;
typedef struct espera espera_t;

token   tabla[50];
lectura lecturas[255];
/***********************************************/

void CerarTabla()
{
	int i;
	tabla[0].posi = 0;
	for(i=1;i<50;i++)
	{
		tabla[i].posi = -1;
	}
	for(i=0;i<255;i++)
	{
		lecturas[i].posi = -1;
	}
	
}
int VerUltimo()
{
	int i;
	for(i=1;i<50;i++)
	{
		if(tabla[i].posi == -1)
		{
			break;
		}
	}	
	return i;
}
int VerUltimoLectura()
{
	int i;
	for(i=1;i<255;i++)
	{
		if(lecturas[i].posi == -1)
		{
			break;
		}
	}	
	return i;
}
int Buscar(char *lexema, char *valor)
{
	int i;
	i=0;
//	char c_lex_lec[50];
	while (tabla[i].posi != -1)
	{
		/*strcmp*/
		if(strcmp(tabla[i].c_lex,lexema) == 0)
		{
			int ultimo = (int) VerUltimoLectura();
//			printf("%d \n",ultimo);
//			strcpy(c_lex_lec,tabla[i].c_lex);
//			printf("%s \n",c_lex_lec);
//			printf("%s \n",valor);
//			lectura[ultimo].c_lex[] = tabla[i].c_lex;
//			lectura[ultimo].d_lex = valor;
			lecturas[ultimo].posi = ultimo;
			lecturas[ultimo].linea = numLinea;
			strcpy(lecturas[ultimo].lex, tabla[i].d_lex);
//			strcpy(lecturas[ultimo].c_lex, c_lex_lec);
			strcpy(lecturas[ultimo].d_lex, valor);
			return tabla[i].posi;
			break;
		}
		i++;
	}	
}

void insertarLexemas(char *c_lex_ac,char *d_lex_ac)
{
	int ultimo = (int) VerUltimo();
	strcpy(tabla[ultimo].c_lex, c_lex_ac);
	strcpy(tabla[ultimo].d_lex, d_lex_ac);
	tabla[ultimo].posi = (int) VerUltimo();
}

int Minuscula(char *texto)
{
	int idx;
	for(idx=0;idx<strlen(texto);idx++)
		texto[idx]=tolower(texto[idx]);
}

int Mayuscula(char *texto)
{
	int idx;
	for(idx=0;idx<strlen(texto);idx++)
		texto[idx]=toupper(texto[idx]);
}

void CargarLexemas()
{
	insertarLexemas("[","L_CORCHETE");
	insertarLexemas("]","R_CORCHETE");
	insertarLexemas("{","L_LLAVE");
	insertarLexemas("}","R_LLAVE");
	insertarLexemas(",","COMA");
	insertarLexemas(":","DOS_PUNTOS");
	insertarLexemas("string","LITERAL_CADENA");
	insertarLexemas("number","LITERAL_NUM");
	insertarLexemas("true","PR_TRUE");
	insertarLexemas("false","PR_FALSE");
	insertarLexemas("null","PR_NULL");
	insertarLexemas("eof","EOF");
}
void RevisarLexemas()
{
	printf("\n");
	int z;
	int k;
	int y;
	k = 0;
	int contaErr = 0;
	espera esperando[255];
	for (y=0;y<255;y++)
	{
		strcpy(esperando[y].esp,"null");
	}
	for(z=0;z<255;z++)
	{
		if (lecturas[z].posi != -1)
		{
			if (k == 0)
			{
				if((strcmp(lecturas[z].lex,"L_CORCHETE")== 0) || (strcmp(lecturas[z].lex,"LITERAL_CADENA")== 0))
				{
					if(strcmp(lecturas[z].lex,"L_CORCHETE")== 0)
					{
						strcpy(esperando[k].esp, "EOF");
						k++;
						strcpy(esperando[k].esp, "R_CORCHETE");
						k++;
						strcpy(esperando[k].esp, "LITERAL_CADENA");
					}
					else if (strcmp(lecturas[z].lex,"LITERAL_CADENA")== 0)
					{
						strcpy(esperando[k].esp, "COMA");
						k++;
					}
				}
				if(strcmp(lecturas[z].lex,"EOF")== 0)
				{
					printf("\n");
					if ( contaErr == 0 )
					{
						printf("Terminado...");
					}
					else
					{
						printf("Terminado... con errores... cant: %d",contaErr);
					}					
					exit(1);
				}
			}
			else
			{
				if( strcmp(esperando[k].esp,lecturas[z].lex) == 0 )
				{
					if ( strcmp(lecturas[z].lex,"COMA")==0 )
					{
						if ( strcmp(lecturas[(z+1)].lex,"L_CORCHETE")==0 )
						{
							strcpy(esperando[k].esp, "R_CORCHETE");
							k++;
							strcpy(esperando[k].esp, "LITERAL_CADENA");
							z++;
						}
						else if ( strcmp(lecturas[(z+1)].lex,"L_LLAVE")==0 )
						{
							if ( strcmp(lecturas[(z+1)].lex,"R_LLAVE")!=0 )
							{
								strcpy(esperando[k].esp, "R_LLAVE");
								k++;
								if (strcmp(lecturas[(z+4)].lex,"LITERAL_CADENA")==0 )
								{
									strcpy(esperando[k].esp, "LITERAL_CADENA");	
								}
								else if (strcmp(lecturas[(z+4)].lex,"LITERAL_NUM")==0 )
								{
									strcpy(esperando[k].esp, "LITERAL_NUM");
								}
								else if (strcmp(lecturas[(z+4)].lex,"PR_TRUE")==0 )
								{
									strcpy(esperando[k].esp, "PR_TRUE");
								}
								else if (strcmp(lecturas[(z+4)].lex,"PR_FALSE")==0 )
								{
									strcpy(esperando[k].esp, "PR_FALSE");
								}
								else if (strcmp(lecturas[(z+4)].lex,"PR_NULL")==0 )
								{
									strcpy(esperando[k].esp, "PR_NULL");
								}
								else
								{
									printf("ERROR 1 \n");
									printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
									contaErr++;
								}
								k++;
								strcpy(esperando[k].esp, "DOS_PUNTOS");
								k++;
								strcpy(esperando[k].esp, "LITERAL_CADENA");
							}
							else
							{
								strcpy(esperando[k].esp, "R_LLAVE");
							}
							z++;
						}
						else if ( strcmp(lecturas[(z+1)].lex,"LITERAL_CADENA")==0 )
						{
							if (strcmp(lecturas[(z+2)].lex,"DOS_PUNTOS")==0)
							{
								if (strcmp(lecturas[(z+3)].lex,"LITERAL_CADENA")==0 )
								{
									strcpy(esperando[k].esp, "LITERAL_CADENA");	
								}
								else if (strcmp(lecturas[(z+3)].lex,"LITERAL_NUM")==0 )
								{
									strcpy(esperando[k].esp, "LITERAL_NUM");
								}
								else if (strcmp(lecturas[(z+3)].lex,"PR_TRUE")==0 )
								{
									strcpy(esperando[k].esp, "PR_TRUE");
								}
								else if (strcmp(lecturas[(z+3)].lex,"PR_FALSE")==0 )
								{
									strcpy(esperando[k].esp, "PR_FALSE");
								}
								else if (strcmp(lecturas[(z+3)].lex,"PR_NULL")==0 )
								{
									strcpy(esperando[k].esp, "PR_NULL");
								}
								else
								{
									printf("ERROR  2 \n");
									printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
									contaErr++;
								}
								k++;
								strcpy(esperando[k].esp, "DOS_PUNTOS");
								k++;
								strcpy(esperando[k].esp, "LITERAL_CADENA");
							}
							else if (strcmp(lecturas[(z+2)].lex,"R_CORCHETE")==0)
							{
								strcpy(esperando[k].esp, "LITERAL_CADENA");
							}
							else if (strcmp(lecturas[(z+2)].lex,"COMA")==0)
							{
								//strcpy(esperando[k].esp, "COMA");
								strcpy(esperando[k].esp, "LITERAL_CADENA");
							}
							else
							{
								printf("ERROR  3 \n");
								printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
								contaErr++;
							}
						}
					}
					else if ( strcmp(lecturas[z].lex,"LITERAL_CADENA")==0 )
					{
//							printf("Literal cadena: %s",lecturas[z].lex );
						if ( strcmp(lecturas[(z+1)].lex,"DOS_PUNTOS")==0 || strcmp(lecturas[(z+1)].lex,"R_LLAVE")==0 || strcmp(lecturas[(z+1)].lex,"R_CORCHETE")==0 )
						{
							k--;
						}
						else if ( strcmp(lecturas[(z+1)].lex,"COMA")==0 )
						{
							strcpy(esperando[k].esp, "COMA");
						}
						else
						{
							printf("ERROR  4 \n");
							printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
							contaErr++;
						}
					}
					else if ( strcmp(lecturas[z].lex,"LITERAL_NUM")==0 || strcmp(lecturas[z].lex,"PR_TRUE")==0 || strcmp(lecturas[z].lex,"PR_FALSE")==0 || strcmp(lecturas[z].lex,"PR_NULL")==0 )
					{
						if ( strcmp(lecturas[(z+1)].lex,"COMA")==0 )
						{
							strcpy(esperando[k].esp, "COMA");
						}
						else if ( strcmp(lecturas[(z+1)].lex,"R_LLAVE")==0 )
						{
							k--;
						}
						else
						{
							printf("ERROR  5 \n");
							printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
							contaErr++;
						}
					}
					else if ( strcmp(lecturas[z].lex,"DOS_PUNTOS")==0 )
					{
						k--;
					}
					else if ( strcmp(lecturas[z].lex,"R_LLAVE")==0 )
					{
						if (strcmp(lecturas[(z+1)].lex,"COMA")==0)
						{
							strcpy(esperando[k].esp, "COMA");
						}
						else if (strcmp(lecturas[(z+1)].lex,"R_CORCHETE")==0)
						{
							k--;
						}
						else
						{
							printf("ERROR  6 \n");
							printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
							contaErr++;
						}
					}
					else if ( strcmp(lecturas[z].lex,"R_CORCHETE")==0 )
					{
						if (strcmp(lecturas[(z+1)].lex,"COMA")==0)
						{
							strcpy(esperando[k].esp, "COMA");
						}
						else if (strcmp(lecturas[(z+1)].lex,"R_CORCHETE")==0)
						{
							k--;
						}
						else if (strcmp(lecturas[(z+1)].lex,"EOF")==0)
						{
							//printf("SALI");
							//exit(1);
							k--;
						}
						else
						{
							printf("ERROR  7 \n");
							printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
							contaErr++;
						}
					}
				}
				else
				{
					printf("ERROR  8 \n");
					printf("esperado: %s recibido: %s linea aprox: %d \n", esperando[k].esp,lecturas[z].lex,lecturas[z].linea );
					contaErr++;
				}
			}
		}
	}
	printf("\n");
	if ( (int) contaErr == (int) 0 )
	{
		printf("Terminado...");
	}
	else
	{
		printf("Terminado... con errores... cant: %d",contaErr);
	}
}

/*
LEXEMA 		COMPONENTE 		LÉXICO Expresión Regular
	[ 		L_CORCHETE 		[
	]		R_CORCHETE		]
	{ 		L_LLAVE 		{
	} 		R_LLAVE 		}
    , 		COMA 			,
	: 		DOS_PUNTOS 		:
	string 	LITERAL_CADENA 	" .* "
	number 	LITERAL_NUM 	[0-9]+(\.[0-9]+)?((e|E)(+|-)?[0-9]+)?
	true 	PR_TRUE 		true | TRUE
	false 	PR_FALSE 		false | FALSE
	null 	PR_NULL 		null | NULL
	eof 	EOF 			match con el fin de archivo
*/

/************* Definiciones ********************/

/***********************************************/
