#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define INFINI 10000

int **poids;    // matrice des poids (un tableau de tableaux d'entier)
int n;		// nb de sommets

typedef struct tchemin {
	int ns;		// nombre de sommets dans le chemin
	int cout;	// coût du chemin
	int *chem;	// le chemin (un tableau de sommets)
} TypeChemin;

typedef struct tmaillon {	// un élément de la file de chemin
	TypeChemin *val;
	struct tmaillon *adr;
} Maillon;

typedef struct tfile {		// la file de chemin
	Maillon *tete;
	Maillon *queue;
} TypeFile;


void init_graph( int nbsommet );	// initialise un graphe par des poids aléatoires
void charger_graph(char *nomfich);	// charger une matrice des poids à partir d'un fichier
void bfs( int depart );			// parcours en largeur à partir du sommet 'depart'
void affich();				// affiche la matrice des poids
int Existe_dans_chemin(int j, TypeChemin *ch);	// verifie si le sommet j appartient au chemin ch
void CreerFile( TypeFile *f );			// file FIFO ... à transformer en file de priorité
void Enfiler( TypeFile *f, TypeChemin *ch );	// enfiler un chemin
void Defiler( TypeFile *f, TypeChemin **ch );	// defiler un chemin
int FileVide( TypeFile f );


int main(int argc, char *argv[])
{
   if (argc != 2) {
	printf("Usage: %s [tailleGraphe | nom_fichier]\n", argv[0]);
	exit(0);
   }
   if ( argv[1][0] >= '0' && argv[1][0] <='9' )
      init_graph( atoi(argv[1]) );
   else
      charger_graph(argv[1]);
   affich();
   bfs(0);
   return 0;
}


void init_graph( int nbsommet )
{
   int i, j;
   n = nbsommet;

   poids = malloc( n*sizeof(int *) );
   for (i=0; i<n; i++)
	poids[i] = malloc( n*sizeof(int) );

   srand((unsigned int)time((time_t *)NULL));
   for (i=0; i<n; i++) {
	for (j=i+1; j<n; j++) {
	     poids[i][j] = 1+(int) (1.0*n*rand()/(RAND_MAX+1.0));
	     poids[j][i] = poids[i][j];
	}
	poids[i][i] = 0;
   }
}


void charger_graph(char *nomfich)
{
   int i, j;
   FILE *f = fopen( nomfich, "r" );
   fscanf(f, " %d", &n);

   poids = malloc( n*sizeof(int *) );
   for (i=0; i<n; i++)
	poids[i] = malloc( n*sizeof(int) );

   for (i=0; i<n; i++) 
	for (j=0; j<n; j++)
	    fscanf(f, " %d", &poids[i][j]);
   fclose(f);
}


void bfs( int depart )
{
   TypeFile f;
   int i, j, k;
   long cpt = 0;
   int cout_opt = INFINI;
   TypeChemin *nouv = NULL;
   TypeChemin *cycle_opt = malloc( sizeof(TypeChemin) );
   TypeChemin *ch = malloc( sizeof(TypeChemin) );

   cycle_opt->chem = malloc( (n+1)*sizeof(int) );
   ch->chem = malloc( sizeof(int) ); 
   ch->ns = 1; ch->chem[0] = depart; ch->cout = 0;
   CreerFile( &f );
   Enfiler(&f, ch);
   while ( !FileVide(f) ) {
	Defiler( &f, &ch );
	if ( ch->ns == n ) {
	   // chemin contenant tous les sommets
	   ch->ns = ch->ns + 1;
	   ch->chem[ ch->ns-1 ] = depart;
	   ch->cout = ch->cout + poids[ch->chem[ch->ns-2]][depart];
	   cpt++;
//printf("Solution %4d avec un cout = %d\n", cpt, ch->cout); 
	   if ( ch->cout < cout_opt ) {
		// MAJ cycle optimal 
		cout_opt = ch->cout;
   		for (i=0; i < n+1; i++)
   		    cycle_opt->chem[i] = ch->chem[i];
   		cycle_opt->ns = ch->ns;
   		cycle_opt->cout = ch->cout;
	   }
	} 
	else {  // ifnot ( ch[0] == n )
	   i = ch->chem[ ch->ns-1 ];  // le dernier sommet du chemin
	   for (j=0; j<n; j++)
		if ( j != depart && j != i && !Existe_dans_chemin(j,ch) )  {
		   nouv = malloc( sizeof(TypeChemin) );
		   nouv->chem = malloc( (ch->ns+2)*sizeof(int) );
 		   //nouv->chem = malloc( (n+1)*sizeof(int) );
		   for (k=0; k < ch->ns; k++)
   		    	nouv->chem[k] = ch->chem[k];
		   nouv->chem[ ch->ns ] = j;
		   nouv->ns = ch->ns+1;
		   nouv->cout = ch->cout + poids[i][j];
		   Enfiler( &f, nouv );
		}
     	} // endif ( ch[0] == n )
	free(ch->chem);
	free(ch);
   } // while
   // Affichage de la solution optimale
   printf("Le cycle hamiltonien de poids minimum est :\n");
   for (i=0; i <= n; i++)
	printf("%3d ", cycle_opt->chem[i]);
   printf("Son cout est = %3d\nNb total de solutions = %ld\n", cycle_opt->cout, cpt);
}


void affich()
{
   int i, j;
   FILE *f = fopen("graph.txt", "w");
   fprintf(f, "%d\n", n);
   printf("\n matrice des poids des aretes:\n");
   for (i=0; i<n; i++) {
	for (j=0; j<n; j++) {
	    printf("%3d ", poids[i][j]);
	    fprintf(f, "%d\n", poids[i][j]);
	}
	printf("\n");
   }
   printf("\n");
   fclose(f);
}


int Existe_dans_chemin( int j, TypeChemin *ch)
{
   int i;
   int trouv = 0;
   for (i=0; i < ch->ns && !trouv; i++)
	if ( j == ch->chem[i] ) trouv = 1;
   return trouv;
}


void CreerFile( TypeFile *f )
{
   f->tete = f->queue = NULL;
}


void Enfiler( TypeFile *f, TypeChemin *ch )
{
   Maillon *p = malloc( sizeof(Maillon) );
   p->val = ch;
   p->adr = NULL;
   if (f->queue != NULL)
	f->queue->adr = p;
   else
	f->tete = p;
   f->queue = p;
}


void Defiler( TypeFile *f, TypeChemin **ch )
{
   Maillon *p;
   if (f->tete != NULL) {
	*ch = f->tete->val;
	p = f->tete;
	f->tete = f->tete->adr;
	free(p);
	if (f->tete == NULL) f->queue = NULL;
   }
}


int FileVide( TypeFile f )
{
   return (f.tete == NULL);
}

