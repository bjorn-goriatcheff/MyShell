/*
 * Fonction: parent_path
 * Entrée: une chaine de caractères
 * Sortie: un entier
 * 
 * Remplace le chemin d'acces en entrée par celui menant au repertoire parent
 */ 



int parent_path(char *path_entry)
{
	// Taille initiale du chemin d'accès
	int position = strlen(path_entry)-1;
	
	//Modification du chemin d'acces (reduction)
	while (path_entry[position]!='/')
	{	
		//remplacement des caracteres par une terminaison de chaine de caracteres
		path_entry[position]='\0';
		position--;
	}
	path_entry[position]='\0';
	
		
	return 0;
}

/*
 * Fonction: cd
 * Entree: une chaîne de caractères
 * Sortie: un entier
 * 
 * Modifie le dossier courant par le chemin d'acces rentré en paramètre
 * 
 */ 

int cd(char *pathdir)
{
	char *tempname;
	char *currentdir;
	int taille=64;
	if (pathdir==NULL)
	{
		printf("le chemin entré n'existe pas");
		return -1;
	}
	else if(pathdir[0]!='/')
	{
		currentdir=malloc(sizeof(char)*taille);
		getcwd(currentdir, taille);
		
		while(errno==ERANGE | taille>500)
		{
			taille+=10;
			free(currentdir);
			currentdir=malloc(sizeof(char)*taille);
			getcwd(currentdir, taille);
		}
		tempname=concat_filename(currentdir, pathdir);
		pathdir=tempname;
		
		//Le repertoire courant est modifié par celui visé
		chdir(pathdir);
		//Le repertoire courant est affiché
		printf("Le repertoire courant est: \n%s\n", pathdir);
		
		//Les pointeurs sont liberés
		free(currentdir);
		free(tempname);
	}
	return 0;
}

/*
 * Fonction: cd_fast
 * Entree: //
 * Sortie: un entier
 * 
 * Modifie le repertoire courant par le dossier parent.
 * Reste à la racine sinon
 * 
 */ 
int cd_fast()
{
	// La varible path est declarée, elle contient le chemin d'acces au repertoire
	char *path;
	// Taille du buffer
	int taille = 64;
	
	// Le buffer path est alloué à une taille arbitraire
	path=malloc(sizeof(char)*taille);
	// Le chemin d'acces au repertoire courant est copier dans le buffer "path"
	getcwd(path, taille-1);
	
	// La taille du buffer est insuffisante
	while(errno==ERANGE | taille>500)
	{
		
		taille+=10;
		free(path);
		//Nouveau buffer "path" de taille augmentée
		path=malloc(sizeof(char)*taille);
		//La copie du chemin d'accès est executée une nouvelle fois
		getcwd(path, taille-1);
	}
	
	//Modification du chemin d'acces 
	parent_path(path);
	//Accès au nouveau repertoire courant
	chdir(path);	
	
	//Le repertoire courant est affiché
	printf("Le repertoire courant est: \n%s\n", path);
	// On libere le buffer
	free(path);
	
	return 0;
}

void quit()
{
	exit(0);
}

/*
 * 
 * Fonction: Touch
 * Entree: un tableau de chaînes de caractère
 * 			un entier
 * Sortie: un entier
 * 
 * Creer autant de fichier que de filenum avec les noms sités dans filename
 */
  
int touch_s(char** filename, int filenum)
{
	int target;
	int num;
	for(num=0;num<filenum;num++)
	{
		target = open(filename[num], O_WRONLY);
	
	
		if (target==-1)
		{
			target = open(filename[num], O_CREAT );
			if (target==-1)
			{
				printf("Erreur : impossible de creer le fichier %s\n", filename[num]);
				return -1;
			}
		}
		close(target);
	}
	return 0;
}
/*
 * 
 * Fonction: touch
 * Entree: une chaîne de caractère
 * Sortie: un entier
 * 
 * Creer un fichier avec le nom filename
 * 
 */
int touch(char* filename)
{
	int target;
	
	target = open(filename, O_WRONLY);
	
	
	if (target==-1)
	{
		target = open(filename, O_CREAT );
		if (target==-1)
		{
			printf("Erreur : impossible de creer le fichier %s\n", filename);
			return -1;
		}
	}
	close(target);
	
	return 0;
}
/*
 * Fonction: cat_one
 * Entree une chaîne de caractères
 * sortie: une chaîne de caractères
 * 
 * cat permet d'afficher le contenu du fichier entré sur la sortie stdout
 * 
 * 
 */ 
char* cat_one(char* file)
{
	int test_in;
	int tete;
	char temp[BUFFER_SIZE];
	
	if(file==NULL)
	{
		printf("le fichier demandé est vide");
		return NULL;
	}
	else
	{
		tete=open(file, O_RDONLY);                                                  /* Ouverture du fichier à afficher */
																		
		if(tete==-1)																/* Erreur lors de l'ouverture */
		{
			printf("ouverture impossible du fichier %s\n", file);
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
				
		
			return NULL;															/* Le vide est renvoyé afin de ne pas propager un resultat faux */
		}
		else
		{
			while((test_in=read(tete,&temp,BUFFER_SIZE*sizeof(char)))>0)
			{
				if (test_in==-1)
				{
					printf("lecture impossible");
				}
				else
				{
					printf("%s",temp);
				}
			}
			printf("\n");						
		}
		close(tete);
		return file;
	} 
}

/*
 * Fonction: cat
 * Entrée: une chaîne de caractères
 * 			une chaîne de caractères
 * Sortie: une chaîne de caractères
 * 
 * Effectue la concatenation des deux chaînes de caractères entrées en paramètres.
 * Affiche le resultat sur la sortie stdout
 * Renvoit le resultat sous la forme d'un fichier nommé par "nom1-nom2" (la concatenation des deux noms)
 */ 
char* cat(char* debut, char*fin)
{
	int test_in, test_out;
	int tete,queue,catt=0;
	char temp[BUFFER_SIZE];
	char* resultat;
	resultat=concat_filename_simple(debut,fin);
	
	queue=open(fin, O_RDONLY);
	tete=open(debut, O_RDONLY);
	
	/* Le fichier queue ne s'ouvre pas */
	if(queue==-1)
	{
		printf("ouverture queue impossible");
	}
	/* Le fichier tete ne s'ouvre pas) */
	if(tete==-1)
	{
		printf("ouverture de la tete impossible");
		
	}
	/* Les deux fichiers sont ouvert correctement */
	else
	{																			/* Ouverture du resultat */
		touch(resultat);
		catt = open(resultat,O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR );
		if(catt == -1)
		{
			printf("ecriture du fichier final impossible");
		}
																				/* copie de la tete */
		else
		{

			while((test_in =read(tete,temp,BUFFER_SIZE))>0)
			{
				test_out = write(catt,temp,(ssize_t)test_in);
				if(test_out==-1)
				{
					printf("ecriture tete imposible");
				}
				test_out = write(1,&temp,(ssize_t)test_in);
				if(test_out==-1)
				{
					printf("ecriture tete stdout imposible");
				}
				
			}
			close(tete);
			
		/* copie de la fin */
			while((test_in=read(queue,temp,BUFFER_SIZE))>0)
			{			
				test_out = write(catt,temp,(ssize_t)test_in);
				if(test_out==-1)
				{	
					printf("ecriture queue imposible");
				}
				test_out = write(1,temp,(ssize_t)test_in);
				if(test_out==-1)
				{	
					printf("ecriture queue stdout imposible");
				}
			}
		}	
	}
	
	close(queue);
	close(catt);
	
	//cat_one(resultat);
	
	return resultat;
}
		
		
/*
 * Fonction: save_story
 * entrée: une chaîne de caractères
 * sortie: un entier
 * 
 * Effectue la sauvegarde de l'historique des commandes
 * 
 */ 		
	

int save_story(char* commande)
{
	
	char* history = "history.bs.txt";
	char* racine = debut_path(3);
	/* création de l'emplacement du fichier history.shell (souvent /User/NAME/history.shell.txt */
	char* emplacement=concat_filename(racine,history);
	//printf("%s\n",emplacement);
	
	int target;
	int test;
	
	target = open(emplacement, O_APPEND);
	
	
	if (target==-1)
	{
		target = open(emplacement, O_CREAT );
		if (target==-1)
		{
			if(errno==EAGAIN)
			{
				printf("impossible d'ecrire les données: accès refusé");
			}	
		}
		else
		{	
			test=write(target,commande,sizeof(char)*strlen(commande));
		}
			
	}
	else
	{

		test=write(target,commande,sizeof(char)*strlen(commande));
		
	}
	close(target);
	
	
	free(emplacement);
	return 0;
	
	
	
	
}


