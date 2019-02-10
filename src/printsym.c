/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printsym.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:45:41 by jye               #+#    #+#             */
/*   Updated: 2019/02/10 17:17:27 by jye              ###   ########.fr       */
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
		ft_printf("%*s %c %s\n", pad, "", 'U', name);
	else if ((ntype & N_TYPE) == N_ABS)
		ft_printf("%0*llx %c %s\n", pad, sym->n_value, 'A', name);
	else if ((ntype & N_TYPE) == N_SECT)
	{
		c = sect ? ft_strchr(LETTERSYM, sect->sectname[2]) : 0;
		ft_printf("%0*llx %c %s\n", pad, sym->n_value,
			(c ? *c : 's') - 0x20 * (ntype & N_EXT),
			name);
	}
	else if ((ntype & N_TYPE) == N_INDR)
		ft_printf("%0*llx %c %s\n", pad, sym->n_value, 'I', name);
	else if ((ntype & N_TYPE) == N_UNDF && ntype & N_EXT && sym->n_value != 0)
		ft_printf("%*s %c %s\n", pad, "", 'C', name);
}

void	dumpsym(msyms_t *file, uint32_t magic, void *top)
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
		if ((sym->n_sect <= file->nsects || sym->idx < (char *)top)
			&& !(sym->n_type & N_STAB))
			printsym(sym, sym->n_sect != NO_SECT ?
				file->sect + ((sym->n_sect - 1) * file->sectsize) : 0,
				magic == MH_MAGIC_64 ? 16 : 8);
	}
}

#endif
