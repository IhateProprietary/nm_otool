/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addsymbole.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:48:13 by jye               #+#    #+#             */
/*   Updated: 2019/02/01 21:48:15 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef NM
# include <stdlib.h>
# include "libft.h"
# include "nm.h"

static int	cmp_ascii(void *sym1, void *sym2)
{
	int		ret;

	ret = ft_strcmp(((sym_t *)sym1)->idx, ((sym_t *)sym2)->idx);
	if (ret == 0)
		ret = ((sym_t *)sym1)->n_value - ((sym_t *)sym2)->n_value;
	return (ret);
}

void		addsymbole64(msyms_t *file, st_cmd_t *cmd, void *base)
{
	nlst64_t	*psym;
	sym_t		**syms;
	sym_t		*sym;
	void		*stroff;
	size_t		nsyms;

	nsyms = cmd->nsyms;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	psym = (nlst64_t *)((char *)base + cmd->symoff);
	stroff = base + cmd->stroff;
	file->stroff = stroff;
	while (nsyms--)
	{
		sym->idx = stroff + psym->n_un.n_strx;
		sym->n_value = psym->n_value;
		sym->n_type = psym->n_type;
		sym->n_sect = psym++->n_sect;
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

void		addsymbole(msyms_t *file, st_cmd_t *cmd, void *base)
{
	nlst_t	*psym;
	sym_t	**syms;
	sym_t	*sym;
	void	*stroff;
	size_t	nsyms;

	nsyms = cmd->nsyms;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	psym = (nlst_t *)((char *)base + cmd->symoff);
	stroff = base + cmd->stroff;
	file->stroff = stroff;
	while (nsyms--)
	{
		sym->idx = stroff + psym->n_un.n_strx;
		sym->n_value = psym->n_value;
		sym->n_type = psym->n_type;
		sym->n_sect = psym++->n_sect;
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

#endif
