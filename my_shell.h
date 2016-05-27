/******************* HEADER MY_SHELL.C ***************/

void lire_commande();

void invite_commande();

void execute_commande();

void affiche_buffer();

void transforme_ln(char *ligne);

void vide_buffer(int nb_args);

void parse_ln(char* ligne_pars);

int liberte_buffer();

char* fin_path();

char* debut_path(int maxslash);

void parse_pathenv();

int search_commande();
