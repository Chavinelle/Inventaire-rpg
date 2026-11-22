#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "objet.h"

static int read_line(char *buffer, int size)
{
	int i;
	int c;
	if(fgets(buffer, size, stdin) == NULL)
		return (0);
	i = 0;
	while(buffer[i] != '\0')
	{
		if(buffer[i] == '\n')
		{
			buffer[i] = '\0';
			return (1);
		}
		i++;
	}
	c = getchar();
	if(c == '\n' || c == EOF)
	   return (1);
	while(c != '\n' && c != EOF)
		c = getchar();
	return (-1);
}

static int parse_positive_int(char *str, int *result)
{
	int i;
	long long value;

	if(str[0] == '\0')
		return (0);
	i = 0;
	value = 0;
	while(str[i] != '\0')
	{
		if(str[i] < '0' || str[i] > '9')
			return (0);
		value = value * 10 + (str[i] - '0');
		if(value > 2147483647LL)
			return (0);
		i++;
	}
	if(value <= 0)
		return (0);
	*result = (int)value;
	return (1);
}

static void del_objet(void *content)
{
	free(content);
}

static long long poids_actuel(t_list *inventaire)
{
	t_objet *objet;
	long long total;
	total = 0;
	while(inventaire != NULL)
	{
		objet = (t_objet *)inventaire->content;
		total += (long long)objet->poids * objet->quantite;
		inventaire = inventaire->next;
	}
	return (total);
}

static void ramasser_objet(t_list **inventaire)
{
	char nom[32];
	char buffer[64];
	int poids;
	int quantite;
	int status;
	long long actuel;
	long long demande;
	t_objet *objet;
	t_list *nouveau;
	int i;

	if (!inventaire)
		return ;

	printf("\n--- RAMASSER UN OBJET ---\n");

	printf("Nom de l'objet : ");
	status = read_line(nom, 32);
	if(status != 1 || nom[0] == '\0')
	{
		printf("Nom invalide ou trop long.\n");
		return ;
	}

	printf("Poids unitaire (en grammes) : ");
	status = read_line(buffer, 64);
	if(status != 1 || !parse_positive_int(buffer, &poids))
	{
		printf("Poids invalide.\n");
		return ;
	}

		printf("Quantite : ");
		status = read_line(buffer, 64);
		if(status != 1 || !parse_positive_int(buffer, &quantite))
		{
			printf("Quantite invalide.\n");
			return ;
		}

		actuel = poids_actuel(*inventaire);
		demande = (long long)poids * quantite;

		if(actuel + demande > POIDS_MAX)
		{
			printf("\nRamassage refuse !\n");
			printf("Poids actuel : %lld g\n", actuel);
			printf("Poids demande : %lld g\n", demande);
			printf("Depassement : %lld g\n",
					actuel + demande - POIDS_MAX);
			return ;
		}

		objet = malloc(sizeof(t_objet));
		if(!objet)
		{
			printf("Erreur : allocation de l'objet impossible.\n");
			return ;
		}

			i = 0;
			while(nom[i] != '\0')
			{
				objet->nom[i] = nom[i];
				i++;
			}
			objet->nom[i] = '\0';

			objet->poids = poids;
			objet->quantite = quantite;

			nouveau = ft_lstnew(objet);
		if(!nouveau)
		{
			free(objet);
			printf("Erreur : creation du maillon impossible.\n");
			return ;
		}

		ft_lstadd_back(inventaire, nouveau);
		printf("\nObjet ajoute avec succes !\n");
	}


static int noms_identiques(char *nom1, char *nom2)
{
	int i;
	i = 0;
	while (nom1[i] != '\0' && nom2[i] != '\0')
	{
		if(nom1[i] != nom2[i])
			return (0);
		i++;
	}
	if(nom1[i] == '\0' && nom2[i] == '\0')
		return (1);
	return (0);
}

static void jeter_objet(t_list **inventaire)
{
	char nom[32];
	int status;
	t_list *current;
	t_list *previous;
	t_objet *objet;

	if (!inventaire)
		return ;

	printf("\n--- JETER UN OBJET ---\n");
	printf("Nom de l'objet a jeter : ");

	status = read_line(nom, 32);
	if(status != 1 || nom[0] == '\0')
	{
		printf("Nom invalide ou trop long.\n");
		return ;
	}

	current = *inventaire;
	previous = NULL;

	while(current != NULL)
	{
		objet = (t_objet *)current->content;

		if(noms_identiques(objet->nom, nom))
		{
			if(previous == NULL)
				*inventaire = current->next;
			else
				previous->next = current->next;

			ft_lstdelone(current, del_objet);
			printf("Objet supprime avec succes !\n");
			return ;
		}

		previous = current;
		current = current->next;
	}

	printf("Objet introuvable dans l'inventaire.\n");
}

static void afficher_objet(void *content)
{
	t_objet *objet;
	objet = (t_objet *)content;

	printf("%-20s %10d g %10d %12lld g\n",
			objet->nom,
			objet->poids,
			objet->quantite,
			(long long)objet->poids * objet->quantite);
}
static void lister_inventaire(t_list *inventaire)
{
	long long total;
	printf("\n======= MON INVENTAIRE =======\n");

	if(inventaire == NULL)
	{
		printf("L'inventaire est vide.\n");
		return ;
	}
	printf("\n%-20s %10s %10s %14s\n",
			"Objet", "Poids", "Quantite", "Poids total");
	printf("----------------------------------\n");

	ft_lstiter(inventaire, afficher_objet);

	total = poids_actuel(inventaire);

	printf("----------------------------------\n");
	printf("Poids total transporte : %lld g\n", total);
	printf("Capacite maximal : %d g\n", POIDS_MAX);
	printf("Poids restant : %lld g\n", POIDS_MAX - total);
	printf("Nombre de maillons : %d\n", ft_lstsize(inventaire));
}

static void afficher_menu(void)
{
	printf("\n");
	printf("=====================================\n");
	printf("||         INVENTAIRE RPG          ||\n");
	printf("=====================================\n");
	printf("1. Ramasser un objet\n");
	printf("2. Jeter un objet\n");
	printf("3. Lister l'inventaire\n");
	printf("4. Quitter\n");
	printf("=====================================\n");
	printf("Votre choix : ");
}

int main(void)
{
	t_list *inventaire;
	char buffer[64];
	int choix;
	int status;
	inventaire = NULL;

	while(1)
	{
		afficher_menu();
		status = read_line(buffer, 64);
		if(status == 0)
			break ;
		if(status != 1 || !parse_positive_int(buffer, &choix))
		{
			printf("Choix invalide. Veuillez recommencer.\n");
			continue ;
		}
		if(choix == 1)
			ramasser_objet(&inventaire);
		else if(choix == 2)
			jeter_objet(&inventaire);
		else if(choix == 3)
			lister_inventaire(inventaire);
		else if(choix == 4)
			break ;
		else
			printf("Choix invalide. Choisissez entre 1 et 4.\n");
	}
	ft_lstclear(&inventaire, del_objet);
	printf("\nInventaire, libere. Au revoir !\n");
	return (0);
}
