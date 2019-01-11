#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>

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

void		addsection(mhfile_t *file, size_t nsect, void *sect)
{
	void	*mem;
	size_t	sectsize;
	size_t	offset;

	sectsize = nsect * file->sectsize;
	offset = file->nsects * file->sectsize;
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += nsect;
	file->sect = mem;
}

static int	cmp_ascii(void *sym1, void *sym2)
{
	int		ret;

	ret = ft_strcmp(((sym_t *)sym1)->idx, ((sym_t *)sym2)->idx);
	if (ret == 0)
		ret = ((sym_t *)sym1)->n_value - ((sym_t *)sym2)->n_value;
	return (ret);
}

void		addsymbole64(mhfile_t *file, st_cmd_t *cmd)
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
	psym = (nlst64_t *)((char *)file->base + cmd->symoff);
	stroff = file->base + cmd->stroff;
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

void		addsymbole(mhfile_t *file, st_cmd_t *cmd)
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
	psym = (nlst64_t *)((char *)file->base + cmd->symoff);
	stroff = file->base + cmd->stroff;
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


void		init_machfile64(mhfile_t *file)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	void		*map;
	uint32_t	i;

	i = 0;
	map = file->base;
	ncmds = ((mach_hdr64_t *)file->header)->ncmds;
	offset = sizeof(mach_hdr64_t);
	file->sectsize = sizeof(sect64_t);
	while (i < ncmds)
	{
		ld = (ld_cmd_t *)((char *)map + offset);
		if (ld->cmd == LC_SEGMENT_64)
			addsection(file, ((seg_cmd64_t *)ld)->nsects,
				(void *)((seg_cmd64_t *)ld + 1));
		else if (ld->cmd == LC_SYMTAB)
			addsymbole64(file, (st_cmd_t *)ld);
		offset += ld->cmdsize;
		i++;
	}
}

void		init_machfile2(mhfile_t *file)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	void		*map;
	uint32_t	i;

	i = 0;
	map = file->base;
	ncmds = ((mach_hdr_t *)file->header)->ncmds;
	offset = sizeof(mach_hdr_t);
	file->sectsize = sizeof(sect_t);
	while (i < ncmds)
	{
		ld = (ld_cmd_t *)((char *)map + offset);
		if (ld->cmd == LC_SEGMENT)
			addsection(file, ((seg_cmd_t *)ld)->nsects,
				(void *)((seg_cmd_t *)ld + 1));
		else if (ld->cmd == LC_SYMTAB)
			addsymbole(file, (st_cmd_t *)ld);
		offset += ld->cmdsize;
		i++;
	}
}

void		set_baseptr(mhfile_t *file, int swap)
{
	uint32_t	nfat;
	uint32_t	value;
	farch64_t	*fat;

	value = ((struct fat_header *)file->map)->nfat_arch;
	nfat = swap ? SWAP32(value) : value;
	if (nfat * sizeof(farch_t) > 0x1000)
		return ;
	fat = file->map + sizeof(struct fat_header);
	while (nfat--)
	{
		value = fat->cputype;
		value = swap ? SWAP32(value) : value;
		if (value == CPUARCH64)
		{
			value = fat->offset;
			value = swap ? SWAP32(value) : value;
			file->base = file->map + value;
			break ;
		}
		if (value == CPUARCH32)
		{
			value = fat->offset;
			value = swap ? SWAP32(value) : value;
			file->base = file->map + value;
			break ;
		}
		fat++;
	}
}

int			init_machheader(mhfile_t *file)
{
	uint32_t		magic;
	mach_hdr64_t	*header;

	magic = *((uint32_t *)file->map);
	if (magic == FAT_MAGIC || magic == FAT_CIGAM)
		set_baseptr(file, magic == FAT_CIGAM);
	else
		file->base = file->map;
	if (file->base == 0)
		return (1);
	header = file->base;
	magic = header->magic;
	file->type |= (magic == MH_MAGIC || magic == MH_CIGAM) ?
		MF_ARCH32 : MF_ARCH64;
	file->header = header;
	return (magic != MH_CIGAM && magic != MH_CIGAM_64
			&& magic != MH_MAGIC && magic != MH_MAGIC_64);
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
	if ((init_machheader(file)))
	{
		munmap(map, stat_.st_size);
		free(file);
		return ((mhfile_t *)0);
	}
	if ((file->type & MF_ARCHMASK) == MF_ARCH64)
		init_machfile64(file);
	else
		init_machfile2(file);
	return (file);
}

void	destroy_machfile(mhfile_t *file)
{
	munmap(file->map, file->mapsize);
	free(file->sect);
	free(file->syms);
	free(file);
}

void	printsym(sym_t *sym, sect64_t *sect)
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
		c = ft_strchr(LETTERSYM, sect->sectname[2]);
		ft_printf("%016llx %c %s\n", sym->n_value,
			(c ? *c : 's') - 0x20 * (ntype & N_EXT),
			name);
	}
	else if ((ntype & N_TYPE) == N_INDR)
		ft_printf("%16s %c %s\n", "", 'I', name);
	else if ((ntype & N_TYPE) == N_UNDF && ntype & N_EXT && sym->n_value != 0)
		ft_printf("%16s %c %s\n", "", 'C', name);
}

void	dumpsym(mhfile_t *file)
{
	size_t		nsyms;
	uint32_t	i;
	void		*sect;
	uint8_t		n_sect;
	sym_t		*sym;

	nsyms = file->nsyms;
	i = 0;
	sect = file->sect;
	while (i < nsyms)
	{
		sym = file->syms[i++];
		n_sect = sym->n_sect;
		printsym(sym,
			 n_sect ?
			(sect + (n_sect - 1) * file->sectsize) : 0);
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
				dumpsym(file);
				destroy_machfile(file);
			}
			else
				dprintf(2, " %s fail\n", av[i]);
			close(fd);
		}
		i++;
	}
	return (0);
}
