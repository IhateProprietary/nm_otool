#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include "nm.h"
#ifndef FT_PRINTF_H
# include <stdio.h>
# define ft_dprintf dprintf
# define ft_printf printf
#endif
#ifndef LIBFT_H
# include <string.h>
# define ft_memcpy memcpy
# define ft_memset memset
# define ft_strcmp strcmp
# define ft_strchr strchr
#endif
#define MAP_FLAGS (MAP_FILE | MAP_PRIVATE)

void		addsection(mhfile_t *file, seg_cmd64_t *cmd)
{
	void	*sect;
	void	*mem;
	size_t	nsect;
	size_t	sectsize;
	size_t	offset;

	sect = (void *)(cmd + 1);
	nsect = cmd->nsects;
	sectsize = nsect * sizeof(sect64_t);
	offset = file->nsects * sizeof(sect64_t);
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += nsect;
	file->sect = mem;
}

//static int	cmp_ascii();
/* static int	cmp_offset(); */
/* static int	cmp_rascii(); */
/* static int	cmp_roffset(); */

void		addsymbole(mhfile_t *file, st_cmd_t *cmd)
{
	nlst64_t	*psym;
	nlst64_t	**syms;
	size_t		nsyms;

	nsyms = cmd->nsyms;
	syms = malloc(nsyms * sizeof(*syms));
	if (syms == (nlst64_t **)0)
		return ;
	file->nsyms = nsyms;
	file->syms = syms;
	psym = (nlst64_t *)((char *)file->map + cmd->symoff);
	file->stroff = file->map + cmd->stroff;
	while (nsyms--)
		*syms++ = psym++;
//	__qsort(file->syms, file->nsyms, cmp_ascii);
}

void		init_machfile2(mhfile_t *file)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	void		*map;
	uint32_t	i;

	i = 0;
	map = file->map;
	file->header = file->map;
	ncmds = file->header->ncmds;
	offset = sizeof(mach_hdr64_t);
	while (i < ncmds)
	{
		ld = (ld_cmd_t *)((char *)map + offset);
		if (ld->cmd == LC_SEGMENT_64)
			addsection(file, (seg_cmd64_t *)ld);
		else if (ld->cmd == LC_SYMTAB)
			addsymbole(file, (st_cmd_t *)ld);
		offset += ld->cmdsize;
		i++;
	}
}

mhfile_t	*init_machfile(int fd)
{
	stat_t			stat_;
	mhfile_t		*file;
	void			*map;

	fstat(fd, &stat_);
	if ((file = malloc(sizeof(*file))) == (mhfile_t *)0)
		return ((void *)0);
	if ((map = mmap(0, stat_.st_size, PROT_READ, MAP_FLAGS, fd, 0))
		== MAP_FAILED)
	{
		free(file);
		return ((void *)0);
	}
	ft_memset(file, 0, sizeof(*file));
	file->mapsize = stat_.st_size;
	file->map = map;
	return (file);
}

void	destroy_machfile(mhfile_t *file)
{
	munmap(file->base, file->mapsize);
	free(file->sect);
	free(file->syms);
	free(file);
}

void	printsym(nlst64_t *lst, sect64_t *sect, char *base)
{
	uint8_t		ntype;
	void		*vmaddr;
	char		*sym;

	ntype = lst->n_type;
	vmaddr = sect ? (char *)sect->addr - sect->offset : 0;
	if ((ntype & N_TYPE) == N_UNDF)
		ft_printf("%16s %c %s\n", "", 'U', base + lst->n_un.n_strx);
	else if ((ntype & N_TYPE) == N_ABS)
		ft_printf("%16s %c %s\n", "", 'A', base + lst->n_un.n_strx);
	else if ((ntype & N_TYPE) == N_SECT)
	{
		sym = ft_strchr(LETTERSYM, sect->sectname[2]);
		ft_printf("%016llx %c %s\n", lst->n_value,
			(sym ? *sym : 's') - 0x20 * (ntype & N_EXT),
			base + lst->n_un.n_strx);
	}
	else if ((ntype & N_TYPE) == N_INDR)
		ft_printf("%16s %c %s\n", "", 'I', base + lst->n_un.n_strx);
}

void	dumpsym(mhfile_t *file)
{
	size_t		nsyms;
	uint32_t	i;
	sect64_t	*sect;
	nlst64_t	*lst;

	nsyms = file->nsyms;
	i = 0;
	sect = file->sect;
	while (i < nsyms)
	{
		lst = file->syms[i++];
		printsym(lst,
			lst->n_sect ?
			sect + (lst->n_sect - 1) : (sect64_t *)0,
			file->stroff);
	}
}

int		main(int ac, char **av)
{
	int			i;
	int			fd;
	mhfile_t	*file;

	if (ac == 1)
		return (1);
	i = 1;
	while (i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) != -1)
		{
			if ((file = init_machfile(fd)))
			{
				init_machfile2(file);
				dumpsym(file);
				destroy_machfile(file);
			}
			close(fd);
		}
		i++;
	}
	return (0);
}
