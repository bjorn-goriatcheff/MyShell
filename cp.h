/******************************   CP.H HEADER FILE FOR CP  ****************************/

int copie_file(char* source, char* cible);

char* concat_filename(char* nom1, char* nom2);

int copiedir(char* source, char* cible);

int test_concat();

int copie_rec(char* source, char* cible);

int cp(int argc, char* argv[]);


char* new_directory(char* path, mode_t mode);
