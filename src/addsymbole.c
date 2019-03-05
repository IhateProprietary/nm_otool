/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addsymbole.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:48:13 by jye               #+#    #+#             */
/*   Updated: 2019/03/03 19:08:45 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef NM
# include <stdlib.h>
# include "libft.h"
# include "nm.h"

static int	cmp_ascii(void *sym1, void *sym2)
{
	int		ret;
	size_t	max1;
	size_t	max2;
	size_t	n;

	max1 = ((sym_t *)sym1)->max;
	max2 = ((sym_t *)sym2)->max;
	n = max1 < max2 ? max1 : max2;
	ret = ft_strncmp(((sym_t *)sym1)->idx, ((sym_t *)sym2)->idx, n);
	if (ret == 0)
		ret = ((sym_t *)sym1)->n_value - ((sym_t *)sym2)->n_value;
	return (ret);
}

static void	copy_value64(sym_t *sym, nlst64_t *psym, msyms_t *file, int s)
{
	char *val;

	val = file->stroff + swap(psym->n_un.n_strx, s);
	if (val < (char *)file->top)
	{
		sym->idx = val;
		sym->max = (size_t)((char *)file->top - val);
	}
	else
		sym->idx = "";
	sym->n_value = swap64(psym->n_value, s);
	sym->n_type = psym->n_type;
	sym->n_sect = psym->n_sect;
}

static void	copy_value(sym_t *sym, nlst_t *psym, msyms_t *file, int s)
{
	char	*val;

	val = file->stroff + swap(psym->n_un.n_strx, s);
	if (val < (char *)file->top)
	{
		sym->idx = val;
		sym->max = (size_t)((char *)file->top - val) - 1;
	}
	else
		sym->idx = "";
	sym->n_value = swap(psym->n_value, s);
	sym->n_type = psym->n_type;
	sym->n_sect = psym->n_sect;
}

void		addsymbole64(msyms_t *file, st_cmd_t *cmd, int s)
{
	nlst64_t	*psym;
	sym_t		**syms;
	sym_t		*sym;
	size_t		nsyms;

	nsyms = swap(cmd->nsyms, s);
	if ((psym = (nlst64_t *)((char *)file->base + swap(cmd->symoff, s))) >=
	(nlst64_t *)((char *)file->base + file->size) || (nsyms * sizeof(nlst64_t)
	+ file->base + swap(cmd->symoff, s)) > (file->top - swap(cmd->strsize, s)))
		return ;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	file->stroff = file->base + swap(cmd->stroff, s);
	while (nsyms-- && psym <= (nlst64_t *)((char *)file->top))
	{
		copy_value64(sym, psym++, file, s);
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

void		addsymbole(msyms_t *file, st_cmd_t *cmd, int s)
{
	nlst_t	*psym;
	sym_t	**syms;
	sym_t	*sym;
	size_t	nsyms;

	nsyms = swap(cmd->nsyms, s);
	if ((psym = (nlst_t *)((char *)file->base + swap(cmd->symoff, s))) >=
	(nlst_t *)((char *)file->base + file->size) || (nsyms * sizeof(nlst_t)
	+ file->base + swap(cmd->symoff, s)) > (file->top - swap(cmd->strsize, s)))
		return ;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	file->stroff = file->base + swap(cmd->stroff, s);
	while (nsyms-- && psym <= (nlst_t *)((char *)file->top))
	{
		copy_value(sym, psym++, file, s);
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

#endif
