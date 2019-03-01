/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printsym.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:45:41 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 20:13:47 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef NM
# include <stdint.h>
# include <mach-o/nlist.h>
# include "nm.h"
# include "ft_printf.h"
# include "libft.h"

void	printsym(sym_t *sym, sect64_t *sect, int pad)
{
	uint8_t		ntype;
	char		*name;
	char		*c;

	name = sym->idx;
	ntype = sym->n_type;
	if (name[0] == 0)
		return ;
	if ((ntype & N_TYPE) == N_UNDF)
		ft_printf("%*s %c %.*s\n", pad, "", 'U', sym->max, name);
	else if ((ntype & N_TYPE) == N_ABS)
		ft_printf("%0*llx %c %.*s\n", pad, sym->n_value, 'A', sym->max, name);
	else if ((ntype & N_TYPE) == N_SECT)
	{
		c = sect ? ft_strchr(LETTERSYM, sect->sectname[2]) : 0;
		ft_printf("%0*llx %c %.*s\n", pad, sym->n_value,
			(c ? *c : 's') - 0x20 * (ntype & N_EXT),
			sym->max, name);
	}
	else if ((ntype & N_TYPE) == N_INDR)
		ft_printf("%0*llx %c %.*s\n", pad, sym->n_value, 'I', sym->max, name);
	else if ((ntype & N_TYPE) == N_UNDF && ntype & N_EXT && sym->n_value != 0)
		ft_printf("%*s %c %.*s\n", pad, "", 'C', sym->max, name);
}

void	dumpsym(msyms_t *file, int is32)
{
	size_t	nsyms;
	size_t	i;
	size_t	nsects;
	sym_t	*sym;
	void	*psect;

	nsyms = file->nsyms;
	if (!nsyms)
		return ;
	i = 0;
	nsects = file->nsects;
	psect = file->sect;
	while (i < nsyms)
	{
		sym = file->syms[i++];
		if ((sym->n_sect <= file->nsects || sym->idx < (char *)file->top)
			&& !(sym->n_type & N_STAB))
		{
			if (sym->n_sect && file->sect)
				printsym(sym, file->sect + (sym->n_sect - 1) * file->sectsize
					, is32 ? 8 : 16);
			else
				printsym(sym, 0, is32 ? 8 : 16);
		}
	}
}

#endif
