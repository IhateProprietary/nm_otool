/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addsymbole.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:48:13 by jye               #+#    #+#             */
/*   Updated: 2019/02/20 17:41:21 by jye              ###   ########.fr       */
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

static void	copy_value64(sym_t *sym, nlst64_t *psym, char *stroff, void *top)
{
	char *val;

	val = stroff + psym->n_un.n_strx;
	if (val < (char *)top)
		sym->idx = val;
	else
		sym->idx = stroff + 1;
	sym->n_value = psym->n_value;
	sym->n_type = psym->n_type;
	sym->n_sect = psym->n_sect;
}

static void	copy_value(sym_t *sym, nlst_t *psym, char *stroff, void *top)
{
	char *val;

	val = stroff + psym->n_un.n_strx;
	if (val < (char *)top)
		sym->idx = val;
	else
		sym->idx = stroff + 1;
	sym->idx = val;
	sym->n_value = psym->n_value;
	sym->n_type = psym->n_type;
	sym->n_sect = psym->n_sect;
}

void		addsymbole64(msyms_t *file, st_cmd_t *cmd, void *base, void *top)
{
	nlst64_t	*psym;
	sym_t		**syms;
	sym_t		*sym;
	void		*stroff;
	size_t		nsyms;

	nsyms = cmd->nsyms;
	if ((psym = (nlst64_t *)((char *)base + cmd->symoff)) >=
		(nlst64_t *)((char *)base + file->size) || ((cmd->nsyms *
		sizeof(nlst64_t) + base + cmd->symoff) > (top - cmd->strsize)))
		return ;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	stroff = base + cmd->stroff;
	file->stroff = stroff;
	while (nsyms-- && psym <= (nlst64_t *)((char *)base + file->size))
	{
		copy_value64(sym, psym++, stroff, top);
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

void		addsymbole(msyms_t *file, st_cmd_t *cmd, void *base, void *top)
{
	nlst_t	*psym;
	sym_t	**syms;
	sym_t	*sym;
	void	*stroff;
	size_t	nsyms;

	nsyms = cmd->nsyms;
	if ((psym = (nlst_t *)((char *)base + cmd->symoff)) >=
		(nlst_t *)((char *)base + file->size) || ((cmd->nsyms *
		sizeof(nlst_t) + base + cmd->symoff) > (top - cmd->strsize)))
		return ;
	syms = malloc(nsyms * sizeof(*syms) + nsyms * sizeof(**syms));
	if (syms == (sym_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	sym = (sym_t *)(syms + nsyms);
	stroff = base + cmd->stroff;
	file->stroff = stroff;
	while (nsyms-- && psym <= (nlst_t *)((char *)base + file->size))
	{
		copy_value(sym, psym++, stroff, top);
		*syms++ = sym++;
	}
	ft_qsort((void **)file->syms, file->nsyms, cmp_ascii);
}

#endif
