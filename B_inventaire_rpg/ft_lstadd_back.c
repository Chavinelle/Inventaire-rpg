#include "list.h"
#include <stdlib.h>

void ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list *tmp;

	if (!lst || !new)
		return ;
	new->next = NULL;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	tmp = *lst;
	while(tmp->next != NULL)
		tmp = tmp->next;
	tmp->next = new;
}
