#ifdef NM
# include <stdint.h>
# include <mach-o/nlist.h>
# include "nm.h"
# include "ft_printf.h"
# include "libft.h"

void		printsym(sym_t *sym, sect64_t *sect)
{
	uint8_t		ntype;
	char		*name;
	char		*c;

	name = sym->idx;
	ntype = sym->n_type;
	if ((ntype & N_TYPE) == N_UNDF)
		ft_printf("%16s %c %s\n", "", 'U', name);
	else if ((ntype & N_TYPE) == N_ABS)
		ft_printf("%16s %c %s\n", "", 'A', name);
	else if ((ntype & N_TYPE) == N_SECT)
	{
		c = sect ? ft_strchr(LETTERSYM, sect->sectname[2]) : 0;
		ft_printf("%016llx %c %s\n", sym->n_value,
			(c ? *c : 's') - 0x20 * (ntype & N_EXT),
			name);
	}
	else if ((ntype & N_TYPE) == N_INDR)
		ft_printf("%16s %c %s\n", "", 'I', name);
	else if ((ntype & N_TYPE) == N_UNDF && ntype & N_EXT && sym->n_value != 0)
		ft_printf("%16s %c %s\n", "", 'C', name);
}

void	dumpsym(msyms_t *file)
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
		if (sym->n_sect < file->nsects)
			printsym(sym, sym->n_sect != NO_SECT ?
				file->sect + ((sym->n_sect - 1) * file->sectsize) : 0);
	}
}

#endif
