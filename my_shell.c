/*************************************************************************************************
 * 
 * AUTEUR: BJÖRN GORIATCHEFF
 * ECOLE: POLYTECH_PARIS_SUD
 * DATE: 04/2016-05/2016
 * LANGUAGE: C, UNIX
 * TYPE: SYSTEME

//////////////////////////////////////////////////////////////////////////////////////////////////////


//\//\//\//\//\//\//\//\  MY_SHELL.C  PROJET DE REALISATION D'UN SHELL COMPLET /\\/\\/\\/\\/\\/\\/\\/\\
 

//////////////////////////////////////////////////////////////////////////////////////////////////////


******************************************************************************************************/

//Librairies utilisées
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#include "my_shell.h"
#include "cp.c"
#include "func_intern.h"
#include "func_intern.c"

// VALEURS GLOBALES

#define MAX_BUFFER_SIZE 50
#define MAX_ARG 10

// Variables globales

// Nombre d'arguments de la commande
int argcount;
// Tableau des arguments de la commande
char* buffer[MAX_ARG];
// Ligne de commande non traitée
char ligne[MAX_BUFFER_SIZE];
//Environnement
const char* PATHENV;
char **TABENV;
int countrep;
// Chemin de travail
char* dirpath;



// On recupère le chemin d'acces courant


/*
 * Fonction: invite_commande
 * Entrée: /
 * Sortie: /
 * 
 * Affiche la ligne de l'invite de commande du shell
 */
void invite_commande()
{
	/* nom du repertoire courant */
	char* directpath;
	/* obtention du nom du repertoire courant */
	directpath=fin_path();
	/* Affichage du nom */
	printf("%s",directpath);
	/* Affichage du nom du shell */
	printf("> MyShell : ");

	fflush(stdout);
	free(directpath);
}


/*
 * Fonction: lire_commande
 * Entrée: /
 * Sortie: /
 * 
 * Lecture de la ligne de commande entrée grâce à l'appel system 'fgets'
 * Transformation en string et stockage dans un buffer temporaire (ligne) avant traitement
 */ 
void lire_commande()
{
	if (!fgets(ligne, MAX_BUFFER_SIZE, stdin))
	{	
	printf("Erreur: la commande entrée est vide");
	}
	//save_story(ligne);
	
}


void get_pathenv()
{
	
	PATHENV=getenv("PATH");
	//printf("%s",PATHENV);
	
	 //Traitement de PATHENV
	
	parse_pathenv(TABENV);
	
	
}

void parse_pathenv()
{
	/* Position dans le tableau des noms de repertoires TABENV (counterep<20)*/
	countrep=0;
	/* position dans PATHENV */
	int pos=0;
	/* Taille du nom de repertoire */
	int argsize=0;
	/* test si la case precedente est utile =1 /inutile = 0 */
	int predspace=0;
	/* Initialisation du tableau contenant les noms des repertoires de recherche */
	TABENV=malloc(sizeof(char*)*20);
	while(PATHENV[pos]!='\0')
	{
		/* On tombe sur un caractère ':' il s'agit de la fin d'un nom de repertoire*/
		if(PATHENV[pos]==':')
		{
			if(predspace==1)
			{
				TABENV[countrep][argsize]='\0';
				countrep++;
				argsize=0;
				predspace=0;
			}
			/* Cas où le nom du repertoire commence par ':'*/
			else
			{
				pos++;
			}
		}
		/* On copie le caractère lu */
		else
		{
			if(predspace==0)
			{
				TABENV[countrep]=malloc(sizeof(char)*100);
			}
			TABENV[countrep][argsize]=PATHENV[pos];
			argsize++;
			pos++;
			predspace=1;
		}
		
	}
}

/* Fonction: get_work_path
 * 
 * Obtention du chemin d'acces au repertoire courant 
 * 
 */	


	

void get_work_path()
{
	int taille=65;
	dirpath=malloc(sizeof(char)*taille);
	getcwd(dirpath, sizeof(char)*taille);
	while(errno==ERANGE)
	{
		taille+=10;
		free(dirpath);
		dirpath=malloc(sizeof(char)*taille);
		getcwd(dirpath, sizeof(char)*taille);
	}
}

/*
 * Fonction: fin_path
 * 
 * Recupere le nom du repertoire courant
 * 
 */ 

char* fin_path()
{
	int taille=65;
	int inc=0;
	char* directpath;
	int pos=strlen(dirpath);
	while(dirpath[pos]!='/')
	{
		pos--;
	}
	directpath=malloc(sizeof(char)*taille-pos);
	for(inc=0;inc<taille-pos;inc++)
	{
		directpath[inc]=dirpath[inc+pos];
	}
	return directpath;

}

/*
 * Fonction: debut_path
 * 
 * Recupere le debut du path à la longueur souhaitée ( nombre de sous-repertoire à partir de la racine = maxslash-1 ) 
 * 
 * 
 * 
 */ 

char* debut_path(int maxslash)
{
	char* directpath;
	// On copie la racine et le premier sous repertoire de celle-ci!
	int taille=0;
	int numslash=0;
	while(dirpath[taille]!='/' | numslash < maxslash-1)
	{
		if(dirpath[taille]=='/')
		{
			numslash++;
		}
		taille++;
	}
	directpath=malloc(sizeof(char)*taille-1);
	for(int pos=0;pos<taille;pos++)
	{
		directpath[pos]=dirpath[pos];
	}
	return directpath;
}


		
/*
 * Fonction: transforme_ln
 * Entrée: une chaine de caracteres
 * Sortie: /
 * 
 * 
 */
void transforme_ln(char *ligne)
{
	int max = strlen(ligne);
	ligne[max-1]='\0';
}


/*
 * Fonction: parse_ln
 * Entree: une chaine de caractères
 * Sortie: vide
 * 
 * parse_ln découpe la ligne de commande entrée en un tableau de mots sans le \0 final
 * Chaque case du tableau correspond à un arguments executable de la commande entrée par l'utilisateur
 */
 
 
void parse_ln(char* ligne_pars)
{
	//Position dans la ligne traitée en entrée
	int position=0; 
	//Taille du mot traité
	int argsize=0;  
	
	// Le nombre d'arguments de la commande est initialisé
	int arg=0; 
	//
	//Booleen reprentant si la position precedente dans la ligne entrée est un caractere utile ( 'a' ou '-o') ou un espace (' ') inutile.
	int predspace=0;

	//Sauvegarde de la commande
	//save_story(ligne);	
	
	// Tant que le caractere traité n'est pas le dernier de la ligne
	while(ligne_pars[position]!='\0') 
	{
		if(ligne_pars[position]==' ') // Si le caractère traité est un espace (donc inutile)
		{
			if (predspace==1)         // Si la case precedente est une case utile -> le mot traité est terminé -> 
			{
				buffer[arg][argsize]='\0'; // La chaine de caracteres representant le mot precedent est terminée par \0 
				position++;           
				arg++;           	// Le nombre d'argument de la commande est incrementé car on termine un mot
				argsize=0;			// Reset de la taille de l'argument
				predspace=0;          // l'indicateur d'utilité de case est à l'etat inutile
			}
			else                      // Si La case precedente est inutile
			{
				position++;
			}
			
		}
		else                          // Si le caractère traité est utile 
		{
			if (predspace==0)         // Si le precedent caractère traité etait inutile
			{
				buffer[arg] = malloc(sizeof(char)*20); // Un nouveau buffer: une chaine de caractere de taille 20 pour un argument
			}
			buffer[arg][argsize]=ligne_pars[position];
			argsize++;
			position++;
			predspace=1;
		}
			
	}
	argcount=arg+1;
}


/*
 * Fonction: verif_commande
 * Entrée: /
 * Sortie: int
 * 
 * Verifie si la commande contenue dans le buffer est interne ou externe
 */
 
int verif_commande()
{
	// Buffer interne temporaire de comparaison de la commande (premier element du buffer)
	char* commande=buffer[0];
	// Premier argument (commande exclue)
	char* arg=buffer[1];
	
	// La commande entrée est cp "copier"
	if(strcasecmp(commande,"cp")==0)
	{	
		cp(3,buffer);
	}
	// La commande entrée est cd
	else if(strcasecmp(commande,"cd")==0)
	{
		if (argcount==1)
		{
			cd_fast();
		}
		else
		{
			cd(arg);
		}
	}
	// La commande entrée est quit
	else if(strcasecmp(commande,"quit")==0)
	{
		quit();
	}
	else if(strcasecmp(commande,"touch")==0)
	{
		if(argcount==2)
		{
			touch(arg);
		}
		else
		{
			touch_s(buffer, argcount);
		}
		
	}
	else if(strcasecmp(commande,"cat")==0)
	{
		if(argcount==2)
		{
			cat_one(arg);
		}
		else
		{
			cat(arg, buffer[2]);
		}
		
	}
	else if(strcasecmp(commande,"history")==0)
	{
	
	
	}
	else 
	{
		execute_commande();	
	}
	buffer[0]="0";
	return 0;
	
}
		


/*
 * Fonction: execute_commande
 * Entrée: /
 * Sortie: /
 * 
 * Execute la commande contenue dans le buffer: tableau des arguments de la commande
 * 
 */

void execute_commande()
{
	int statut;
	pid_t pid;
	

	// Le processus est scindé afin d'executer le code dans le processus fils
	pid = fork(); 
	
	// Le fork a echoué
	if (pid<0)
	{
		printf("fork failed");
	}
	
	// Le processus traité est le fils
	else if (pid==0) 
	{	
		//execvp(buffer[0], buffer);     // Execute la commande interne
		//get_pathenv();
		search_commande();
		exit(0);                      // Le processus se termine
	}
	// Le processus traité est le père
	else 
	{
		waitpid(pid,&statut,0); // Mise en attente tant que le fils n'est pas terminé
	}
}


int search_commande()
{
	char* commande;
	int k;
	int test=0;
	/* On parcourt le tableau TABENV des repertoires de recherche */
	for(k=0;k<countrep+1;k++)
	{
		/* Concatenation du chemin d'acces et du nom de la commande */
		commande=concat_filename(TABENV[k],buffer[0]);
		if((test=open(commande, O_RDWR | O_RDONLY | O_WRONLY))!=-1)
		{
			printf("erreur gt_pathen");
		}
		/* Execution de la commande */
		else
		{
			execv(commande, &buffer[0]);
		}
		free(commande);
		
	}
	return 0;
}


/* Fonction: affiche_buffer
 * Entrée: /
 * Sortie: /
 * 
 * Affiche le buffer contenant un mot de la commande
 * Cette fonction permet de verifier que le parse est correctement effectué
 * 
 */
 
void affiche_buffer()
{
	printf("\n");
	printf("buffer:\n");
	for(int k=0;k<MAX_ARG;k++) 
	{
		// Affiche les elements du buffer 1 par 1
		printf("%s", buffer[k]); 
		printf("  ");	
	}
	printf("\n");
}

/* Fonction: liberte_buffer
 * Entrée: /
 * Sortie: int
 * 
 * Libere l'espace memoire utilisé par le buffer lors de la lecture et l'execution d'une commande (ou pipe)
 * 
 */
int liberte_buffer()
{
	// Si la commande entrée comporte plusieurs arguments
	if (argcount>1)
	{
		// Le buffer est initialisé à null
		while(argcount!=0)
		{
			argcount--;
			buffer[argcount]=NULL;
		}
	}
	//Reset de la variable globale argcount
	argcount=0;
	free(dirpath);
	
	return 0;
}

/* Fonction: main
 * 
 * Execution du main
 * Boucle infini
 */
 
 
int main(void)
{
	get_pathenv();
	while(1)
	{
		// Initialisation du shell
		
		//get_pathenv();
		get_work_path();
		//printf("path env: %s\n", PATHENV);
		invite_commande(); 
		// Lecture de l'entrée            
		lire_commande();        
		// Transformation de la chaine de caractere en supprimant le dernier element        
		transforme_ln(ligne); 
		// Découpage de la chaine en un tableau de mots (buffer)         
		parse_ln(ligne);  
		//affiche_buffer();
		// Execution de la commande           
		verif_commande();
		//printf("\n\nargcount: %d\n\n", argcount);
		liberte_buffer();
		//affiche_buffer();
		
	}
	return 0;	
}


