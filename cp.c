/*************************** CP.C COPIE DE FICHIERS OU REPERTOIRES *************************/
/****************************
 * 
 * AUTEURS: BJÖRN GORIATCHEFF
 * 			SACHA LENARTOWICTZ
 * 
 * DATE: 04/2016
 * LANGUAGE: C, UNIX
 * TYPE: SYSTEME
 * 
 ********************************************************************************************/



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>


#define BUFFER_SIZE 1000


/* Fonction: copie_file
 * Entree: - un pointeur vers une chaine de caractères
 * 		   - un pointeur vers une chaine de caractères
 * Sortie: - un entier
 * 
 * Effectue la copie du fichier source vers le fichier cible via un buffer.
 */
 	
int copie_file(char* source, char* cible)
{
	printf("fichier\n");
    int test_in = 0,test_out = 0;
	char temp[BUFFER_SIZE];
	struct stat statSource;
    
    if(source[0]=='.')
    {
		return 0;
	}
    // ouverture des deux fichiers
    printf("open %s\n", source);
    int original = open(source, O_RDONLY);
    if (original == -1) // on vérifie que le fichier est bien ouvert
    {
		printf("Erreur : impossible d'ouvrir le fichier source\n");
		if (errno == EACCES)
		{
			printf("EACESS");
		}
		if (errno == EAGAIN)
		{
			printf("EAGAIN");
		}
		if (errno == EFAULT)
		{
			printf("EFAULT");
		}
		if (errno == EISDIR)
		{
			printf("EISDIR");
		}
		if (errno == ENFILE)
		{
			printf("ENFILE");
		}
				
		return -1;
	}
    
    int target = open(cible, O_WRONLY | O_CREAT );
    if (target == -1)
    {
		printf("Erreur : impossible d'ouvrir le fichier cible\n");
		return -1;
	}
	
	// récupération du statut du fichier source
	if (stat(source, &statSource) == -1)
	{
		printf("Erreur : récupération du statut du fichier source impossible\n");
		return -1;
	}
	
	//printf("\nstat source : %lo\n", (unsigned long)statSource.st_mode);
	
	//on remplace le statut du fichier destination par celui du fichier source
	chmod(cible, statSource.st_mode);

	test_in = read(original,&temp,BUFFER_SIZE);
	if (test_in == -1)
    {
		printf("Erreur : lecture du fichier source impossible\n");
	}
	// Tant qu'il n'y a pas d'erreur, on lit le fichier source et on le recopie 
	// dans le buffer
    while((test_in=read(original,&temp,BUFFER_SIZE))>0 )   
    {

		test_out = write(target,&temp,(ssize_t)test_in);
		
		if (test_out == -1)
		{
			printf("Erreur : écriture dans le fichier destination impossible\n");
			return -1;
		}	
    }

	
   
   // on vérifie que les protections du fichier cible sont les mêmes
	stat(cible, &statSource);
	//printf("stat cible : %lo\n", (unsigned long)statSource.st_mode); 
   
	// fermeture des fichiers
    close(original);
    close(target);

	// on arrive ici si aucune erreur n'a été retourné
	// on retourne donc 0
    return 0;
}

/* Fonction: concat_filename
 * Entree: - un tableau de caractères.
 * 		   - un tableau de caractères
 * Sortie: un tableau de caractères
 * 
 * Effectue la concatenation de la chaine de caractères nom1 et nom2 et la déverse dans une nouvelle chaine.
 */
char* concat_filename(char* nom1, char* nom2)
{
	int long1,long2;
	long1=strlen(nom1);
	long2=strlen(nom2);
	char* resultat = malloc(long1+long2+1);
	if(nom1[long1-1] != '/')
	{
		long1+=1;
		strcpy(resultat,nom1);
		strcat(resultat,"/");
		strcat(resultat,nom2);
		
		
	}
	else
	{
		
		strcpy(resultat,nom1);
		strcat(resultat,nom2);
		
	}
	return resultat;

}


/*
 * 
 * Fonction: new_directory
 * Entree: une chaîne de caractères
 * Sortie: /
 * 
 * Creation d'un nouveau repertoire au path indiqué
 * 
 */ 

char* new_directory(char* path, mode_t mode)
{
	int pos=0, sizepath=0,sizename=0,taille=65;
	char* newpath;
	char* oldpath;
	char* dirname;
	
	oldpath=malloc(sizeof(char)*taille);
	getcwd(oldpath, taille-1);
	while(errno==ERANGE)
	{
		taille+=10;
		free(oldpath);
		oldpath=malloc(sizeof(char)*taille);
		getcwd(oldpath,taille-1);
	}
	
	while(path[pos]!='/')
	{
		sizepath++;
		pos++;
	}
	sizename=strlen(path)-sizepath-1;
	newpath=malloc(sizeof(char)*sizepath);
	dirname=malloc(sizeof(char)*(sizename+1));
	for (int k=0; k<strlen(path);k++)
	{
		
		if(k<sizepath)
		{
			newpath[k]=path[k];
		}
		else if(k>sizepath)
		{
			dirname[k-sizename]=path[k];

		}
	}
	dirname[sizename]='\0';
	chdir(newpath);
	mkdir(dirname,S_IRUSR | S_IWUSR);

	/* modification des droits */
	chmod(dirname,mode);
	
	chdir(oldpath);
	free(newpath);
	free(oldpath);
	return dirname;
}


/*
 * Fonction: copiedir
 * Entree: - une chaine de caracteres
 * 		   - une chaine de caracteres
 * Sortie: un entier
 *
 * copiedir effectue une copie du repertoire source vers le repertoire cible
 * 
 */ 
int copiedir(char* source, char* cible)
{
	DIR* reper = opendir(source);
	DIR* dest=opendir(cible); // On suppose que le repertoire est deja créé

	char *fich1,*fich2;
	//struct stat statSource;
	
	if (reper != NULL)
	{
		struct  dirent* entier;
		readdir(reper);
		readdir(reper);
		while((entier = readdir(reper)) != NULL)
		{
			printf("%s\n", entier->d_name);
			fich1=concat_filename(source,entier->d_name);
			fich2=concat_filename(cible,entier->d_name);
			copie_file(fich1,fich2);			
		}
		closedir(reper);
		closedir(dest);
	}
    return 0;
}


/* Fonction: test_concat
 * Entree: /
 * Sortie: un entier
 * 
 * Verifie que la concatenation des deux chaines de caractères fonctionne normalement
 * Affiche le resultat
 */
 
int test_concat()
{
	char*bob="chainedecaract";
	char*bib="  concatenationreuissie";
	char* conc=concat_filename(bob,bib);
	printf("%s\n", conc);
	return 0;
}


/*Fonction: copie_rec
 * Entree: une chaine de caracteres
 * 			une chaine de caracteres
 * Sortie: un entier
 * 
 * Copie reccursive avec parcours dans un repertoire
 *
 */
 
int copie_rec(char* source, char* cible)
{
	printf("source: %s", source);

	if (source==NULL)
	{
		printf("copie terminée: la source est vide. Indiquer un fichier à copier");
		return 0;
	}
	else
	{
		struct stat statSource;
		stat(source, &statSource);
		if(S_ISDIR(statSource.st_mode)!=0)  // Si la source est un repertoire, S_ISDIR renvoit 0
		{
				DIR* reper = opendir(source);
				DIR* dest = opendir(cible);
				if( reper == NULL)
				{
					printf("ouverture source impossible");
				}
				else if( dest == NULL)
				{
					printf("\ncreation du repertoire");
					new_directory(cible, statSource.st_mode);
					//copie_rec(source, cible); // On relance copie_rec sur le repertoire cible créé (RECCURSIVITE)		
				}
				else 
				{
					printf("copie de l'interieur du repertoire\n");
					struct dirent* addresse;
					readdir(reper);
					readdir(reper);
					while((addresse=readdir(reper))!=NULL)
					{
						//Si l'adresse pointe vers un repertoire
						if(addresse->d_type==DT_DIR)
						{
							copie_rec(concat_filename(source,addresse->d_name),concat_filename(cible,addresse->d_name));
						}
						else
						{
							copie_file(concat_filename(source,addresse->d_name),concat_filename(cible,addresse->d_name));
						}
					}
					closedir(reper);
					closedir(dest);
				}

			
		}
		else if(S_ISREG(statSource.st_mode)!=0) // Si la source est un fichier
		{
			copie_file(source, cible);
		}
		else
		{
			printf("format de la source non usuel");
		}
		
	}
	return 0;
}

/* Fonction: main
 * Entree:  - via console
 * Sortie:  - un entier
 */
int cp(int argc, char* argv[])
{
    printf("debut!\n");
	
    if (argc != 3)
        printf("Erreur : nombre d'arguments incorrect\n");
    else
    
        if (copie_rec(argv[1],argv[2]) == -1)
        {
			printf("Erreur : la copie du fichier a échoué\n"); 
		}
		else
		{
			printf("copie reussie\n");
		}
    return 0;
}

		



/* Fonction: concat_filename
 * Entree: - un tableau de caractères.
 * 		   - un tableau de caractères
 * Sortie: un tableau de caractères
 * 
 * Effectue la concatenation de deux chaines de caractères et la déverse dans une nouvelle chaine.
 */
char* concat_filename_simple(char* nom1, char* nom2)
{
	int long1,long2;
	long1=strlen(nom1);
	long2=strlen(nom2);
	
	char* resultat = malloc(long1+long2);
	
	if(nom1[long1-1] != '-')
	{
		long1+=1;
		strcpy(resultat,nom1);
		strcat(resultat,"-");
		strcat(resultat,nom2);
	}
	else
	{	
		strcpy(resultat,nom1);
		strcat(resultat,nom2);	
	}
	return resultat;

}



