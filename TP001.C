#include "TP001.H"
// #include "lectura.h"

FILE *archivo;			// Fuente
char caracter;
char id[255];
char msg[10];
	
int sigLex()
{
	int i=0;
	char c=0;
	memset(id, 0, sizeof(id));
	memset(msg, 0, sizeof(msg));
/*	int acepto=0;*/
/*	int estado=0;*/
/*	char msg[41];*/
	
	while((c=fgetc(archivo))!=EOF)
	{
		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco
		else if(c=='\n')
		{
			//incrementar el numero de linea
			//numLinea++;
			printf("\n");
			continue;
		}
		else if (isalpha(c))
		{
			//es un identificador (o palabra reservada)
			i=0;
			do{
				id[i]=c;
				i++;
				c=fgetc(archivo);
				//if (i>=TAMLEX)
				//	error("Longitud de Identificador excede tamaño de buffer");
			}while(isalpha(c) || isdigit(c));
/*			if (c!=EOF)*/
/*				ungetc(c,archivo);*/
/*			else*/
/*				c=0;*/
			id[i] = '\0';
/*			printf("%s ",id);*/
			ungetc(c,archivo);
			strcpy(msg,"Error: <Lexema> no encontrado");
			/*char *tmpid = ;*/
			Minuscula(id);
/*			printf("%s ",id);*/
			return Buscar(id);
			break;
		}
		else if (isdigit(c))
		{
			//es un numero
			i=0;
			int estado=0;
			int acepto=0;
			id[i]=c;
			
			while(!acepto)
			{
				switch(estado){
				case 0: //una secuencia netamente de digitos, puede ocurrir . o e
					c=fgetc(archivo);
					if (isdigit(c))
					{
						id[++i]=c;
						estado=0;
					}
					else if(c=='.'){
						id[++i]=c;
						estado=1;
					}
					else if(tolower(c)=='e'){
						id[++i]=c;
						estado=3;
					}
					else{
						estado=6;
					}
					break;
				case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto "no corresponde")
					c=fgetc(archivo);						
					if (isdigit(c))
					{
						id[++i]=c;
						estado=2;
					}
					else if(c=='.')
					{
						//i--;
						//fseek(archivo,-1,SEEK_CUR);
						sprintf(msg,"No se esperaba '%c'",c);
						estado=-1;
					}
					else{
						sprintf(msg,"No se esperaba '%c'",c);
						estado=-1;
					}
					break;
				case 2://la fraccion decimal, pueden seguir los digitos o e
					c=fgetc(archivo);
					if (isdigit(c))
					{
						id[++i]=c;
						estado=2;
					}
					else if(tolower(c)=='e')
					{
						id[++i]=c;
						estado=3;
					}
					else
						estado=6;
					break;
				case 3://una e, puede seguir +, - o una secuencia de digitos
					c=fgetc(archivo);
					if (c=='+' || c=='-')
					{
						id[++i]=c;
						estado=4;
					}
					else if(isdigit(c))
					{
						id[++i]=c;
						estado=5;
					}
					else{
						sprintf(msg,"No se esperaba '%c'",c);
						estado=-1;
					}
					break;
				case 4://necesariamente debe venir por lo menos un digito
					c=fgetc(archivo);
					if (isdigit(c))
					{
						id[++i]=c;
						estado=5;
					}
					else{
						sprintf(msg,"No se esperaba '%c'",c);
						estado=-1;
					}
					break;
				case 5://una secuencia de digitos correspondiente al exponente
					c=fgetc(archivo);
					if (isdigit(c))
					{
						id[++i]=c;
						estado=5;
					}
					else if ((c=='[')||(c==']')||(c=='{')||(c=='}')||(c==',')||(c==':')||(c=='\n')||(c==' ')||(c==EOF)||(c=='"'))
					{
						estado=6;
					}
					else
					{
						sprintf(msg,"No se esperaba '%c'",c);
/*						strcpy(msg,"No se esperaba ");*/
						estado=-1;
					}break;					
/*					else{*/
/*					}break;*/
				case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
					if (c!=EOF)
					ungetc(c,archivo);
					else
						c=0;
					id[++i]='\0';
					acepto=1;
					return Buscar("number");
					break;
				case -1:
					if (c==EOF)
						strcpy(msg, "No se esperaba el fin de archivo");
					else
					{
/*						strcpy(msg, "Error: <numerico> no valido");*/
						return Buscar("error");
					}
/*						strcpy(msg, "Error: <numerico> no valido");*/
					exit(1);
				}
			}
			break;
		} //DIGITO OFF
		else if ((c=='[')||(c==']')||(c=='{')||(c=='}')||(c==',')||(c==':'))
		{
			id[0]=c;
			c=0;
			return Buscar(id);
			break;
		}
		else if (c=='"')
		{
			i=0;
			c=fgetc(archivo);
			while((c != '"')&&(c != EOF))
			{
				id[i]=c;
				i++;
				c=fgetc(archivo);
			};
			if (c == '"')
			{
				c=0;
				return Buscar("string");
				break;
			}
			else
			{
				strcpy(msg,"Error: <String> termina inapropiadamente");
				return Buscar("error");
				break;
			}
		}
		else{
			sprintf(msg,"%c no esperado",c);
			return Buscar("error");
		}
	}
	if (c == EOF)
	{
		return Buscar("eof");
	}
}
int consLex()
{
	char c=0;	
	while((c=fgetc(archivo))!=EOF)
	{
		if (c==' ' || c=='\t')
			continue;	//eliminar espacios en blanco
		else if(c=='\n')
		{
			//incrementar el numero de linea
			//numLinea++;
			printf("\n");
			return -1;
		}
		else if (c==EOF)
		{
			return Buscar("eof");
		}
	}
}
	
int main(int argc,char* args[])
{
	CerarTabla();
	CargarLexemas();
	int ex=-1;
	if(argc > 1)
	{
		if (!(archivo=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		while (strcmp(tabla[ex].d_lex,"EOF") != 0)
		{
			ex = sigLex();
			if(tabla[ex].posi == 0){
				printf("%s ",msg);
				consLex();
			}
			else{
				printf("%s ", tabla[ex].d_lex);
			}
		}
		fclose(archivo);
	}else{
		printf("Debe pasar como parametro el path al archivo fuente.\n");
		exit(1);
	}
	return 0;
}
