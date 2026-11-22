#ifndef OBJET_H
#define OBJET_H

#define POIDS_MAX 10000

typedef struct s_objet
{
	char nom[32];
	int poids;
	int quantite;
}   t_objet;

#endif // OBJET_H
