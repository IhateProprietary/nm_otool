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
#include <mach-o/ranlib.h>

#include "nm.h"
#ifndef FT_PRINTF_H
# include <stdio.h>
# define ft_dprintf dprintf
# define ft_printf printf
#endif
#ifndef LIBFT_H
# include <string.h>
# define ft_memcmp memcmp
# define ft_memcpy memcpy
# define ft_memset memset
# define ft_strcmp strcmp
# define ft_strchr strchr
# include <stdlib.h>
# define ft_atoi atoi
#endif
#define MAP_FLAGS (MAP_FILE | MAP_PRIVATE)

void		init_machfromfat(mhfile_t *file, int swap)
{
	uint32_t	nfat;
	uint32_t	value;
	farch_t		*fat;

	value = ((struct fat_header *)file->map)->nfat_arch;
	nfat = swap ? SWAP32(value) : value;
	if ((nfat * sizeof(farch_t)) > 0x1000)
		return ;
	fat = file->map + sizeof(struct fat_header);
	while (nfat--)
	{
		value = fat->cputype;
		value = swap ? SWAP32(value) : value;
		if (value == CPUARCH64 || value == CPUARCH32)
		{
			value = fat->offset;
			value = swap ? SWAP32(value) : value;
			file->base = file->map + value;
			file->truesize = fat->size;
			file->top = file->base + fat->size;
			break ;
		}
		fat++;
	}
}

void		init_machfromarch(mhfile_t *file)
{
	ar_hdr_t	*hdr;
	uint32_t	off;
	uint32_t	size;
	uint32_t	objsize;
	uint32_t	strsize;

	hdr = (ar_hdr_t *)((char *)file->base + SARMAG);
	if (ft_memcmp(hdr->ar_name, AR_EFMT1, 3) != 0 &&
		ft_memcmp(hdr->ar_fmag, ARFMAG, 2) != 0 &&
		ft_strcmp(SYMDEF, (char *)(hdr + 1)) != 0 &&
		ft_strcmp(SYMDEF, (char *)(hdr + 1)) != 0)
		return ;
	off = ft_atoi(hdr->ar_name + 3);
	size = ft_atoi(hdr->ar_size);
	if (off & 3 || off > size || size > file->truesize)
		return ;
	objsize = *((uint32_t *)((char *)(hdr + 1) + off));
	if (objsize > size)
		return ;
	strsize = *((uint32_t *)((char *)(hdr + 1) + off + objsize + sizeof(uint32_t)));
	if ((strsize + objsize) > size)
		return ;
	file->base = ((void *)(hdr + 1) + size);
	file->type = MF_ARCHIVE;
}

int			init_machheader(mhfile_t *file)
{
	uint32_t	magic;

	magic = *((uint32_t *)file->map);
	file->truesize = file->mapsize;
	if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		init_machfromfat(file, magic == FAT_CIGAM);
	magic = *((uint32_t *)file->base);
	if (magic == MH_MAGIC || magic == MH_MAGIC_64)
		file->type = MF_BINARY;
	else if (ft_memcmp(file->base, ARMAG, SARMAG) == 0)
		init_machfromarch(file);
	return (!file->type);
}

int		init_machfile(int fd, mhfile_t *file)
{
	stat_t			stat_;
	void			*map;

	fstat(fd, &stat_);
	if ((map = mmap(0, stat_.st_size, PROT_READ, MAP_FLAGS, fd, 0))
		== MAP_FAILED)
		return (1);
	ft_memset(file, 0, sizeof(*file));
	file->mapsize = stat_.st_size;
	file->top = map + stat_.st_size;
	file->map = map;
	file->base = map;
	if ((init_machheader(file)))
	{
		munmap(file->map, stat_.st_size);
		return (1);
	}
	return (0);
}

#ifdef NM
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

#ifdef OTOOL
#define HEXASCII "0123456789abcdef"
void	xtoa_b(char *dst, uint8_t *src, int n)
{
	int		i;
	uint8_t	b;

	i = 0;
	while (i < n)
	{
		b = src[i++];
		dst[2] = 0x20;
		dst[1] = HEXASCII[b & 0xf];
		b /= 0x10;
		dst[0] = HEXASCII[b & 0xf];
		dst += 3;
	}
	*dst = 0;
}

void	init_textforsection(msyms_t *file, void *base, char *sectname, mtext_t *t)
{
	size_t	nsects;
	sect_t	*sect;

	nsects = file->nsects;
	sect = file->sect;
	while (nsects--)
	{
		if (ft_strcmp(sectname, sect->sectname) == 0)
		{
			t->sectname = sect->sectname;
			t->segname = sect->segname;
			t->size = sect->size;
			t->addr = sect->addr;
			t->ptr = (char *)base + sect->offset;
			break ;
		}
		sect++;
	}
}

void	init_textforsection64(msyms_t *file, void *base, char *sectname, mtext_t *t)
{
	size_t		nsects;
	sect64_t	*sect;

	nsects = file->nsects;
	sect = file->sect;
	while (nsects--)
	{
		if (ft_strcmp(sectname, sect->sectname) == 0)
		{
			t->sectname = sect->sectname;
			t->segname = sect->segname;
			t->size = sect->size;
			t->addr = sect->addr;
			t->ptr = (char *)base + sect->offset;
			break ;
		}
		sect++;
	}
}

void	dumptext(msyms_t *file, void *base, int is32)
{
	mtext_t	text;
	size_t	i;
	char	b[50];

	ft_memset(&text, 0, sizeof(text));
	if (is32)
		init_textforsection(file, base, "__text", &text);
	else
		init_textforsection64(file, base, "__text", &text);
	ft_printf("Contents of (%s,%s) section\n", text.segname, text.sectname);
	i = 0;
	while (i < text.size)
	{
		xtoa_b(b, (uint8_t *)text.ptr + i,
			(text.size - i) < 0x10 ? text.size - i : 0x10);
		ft_printf("%-16.16llx\t%s\n", text.addr + i, b);
		i += 16;
	}
}
#endif

void	dumpbin(mhfile_t *mach, size_t size)
{
	msyms_t		file;
	mach_hdr_t	*hdr;

	ft_memset(&file, 0, sizeof(file));
	file.size = size;
	hdr = mach->base;
	if (hdr->magic == MH_MAGIC_64)
		init_machfile64(&file, (mach_hdr64_t *)hdr);
	else if (hdr->magic == MH_MAGIC)
		init_machfile32(&file, hdr);
	if (mach->name && mach->objname)
		ft_printf("\n%s(%s):\n", mach->name, mach->objname);
	else if (mach->name)
		ft_printf("%s:\n", mach->name);
#ifdef NM
	dumpsym(&file);
	free(file.syms);
#endif
#ifdef OTOOL
	dumptext(&file, mach->base, hdr->magic == MH_MAGIC);
#endif
	free(file.sect);
}

void	dumparch(mhfile_t *file)
{
	ar_hdr_t	*hdr;
	uint32_t	off;
	uint32_t	objsize;
	uint32_t	size;

	hdr = file->base;
#ifdef OTOOL
	ft_printf("Archive : %s", file->name);
#endif
	while ((void *)hdr < file->top)
	{
		if ((ft_memcmp(hdr->ar_name, AR_EFMT1, 3) != 0 &&
				ft_memcmp(hdr->ar_fmag, ARFMAG, 2) != 0))
			break ;
		size = ft_atoi(hdr->ar_size);
		off = ft_atoi(hdr->ar_name + 3);
		if (size > file->truesize || off > size || off & 3 || size & 3)
			break ;
		file->objname = (char *)(hdr + 1);
		file->base = (void *)((char *)(hdr + 1) + off);
		dumpbin(file, size);
		hdr = (ar_hdr_t *)((char *)(hdr + 1) + size);
	}
}

void	dump(mhfile_t *file)
{
	if (file->type == MF_ARCHIVE)
		dumparch(file);
	else if (file->type == MF_BINARY)
		dumpbin(file, file->truesize);
}

int		main(int ac, char **av)
{
	int			i;
	int			fd;
	mhfile_t	file;

	if (ac == 1)
		return (1);
	i = 1;
	while (i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) != -1)
		{
			if (!init_machfile(fd, &file))
			{
				file.name = av[i];
				dump(&file);
				munmap(file.map, file.mapsize);
			}
			else
				ft_dprintf(2, "%s fail\n", av[i]);
			close(fd);
		}
		i++;
	}
	return (0);
}

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

void		init_machfile64(msyms_t *file, mach_hdr64_t *hdr)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = hdr->sizeofcmds;
	offset = sizeof(mach_hdr64_t);
	file->sectsize = sizeof(sect64_t);
	ncmds = hdr->ncmds;
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)hdr + offset);
		if (ld->cmd == LC_SEGMENT_64)
			addsection64(file, ((seg_cmd64_t *)ld));
#ifdef NM
		else if (ld->cmd == LC_SYMTAB)
			addsymbole64(file, (st_cmd_t *)ld, (void *)hdr);
#endif
		offset += ld->cmdsize;
		if (offset > sizeofcmds)
			;
	}
}

void		init_machfile32(msyms_t *file, mach_hdr_t *hdr)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = hdr->sizeofcmds;
	offset = sizeof(mach_hdr_t);
	file->sectsize = sizeof(sect_t);
	ncmds = hdr->ncmds;
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)hdr + offset);
		if (ld->cmd == LC_SEGMENT)
			addsection(file, ((seg_cmd_t *)ld));
#ifdef NM
		else if (ld->cmd == LC_SYMTAB)
			addsymbole(file, (st_cmd_t *)ld, (void *)hdr);
#endif
		offset += ld->cmdsize;
		if (offset > sizeofcmds)
			;
	}
}

#ifdef NM
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
#endif

void		addsection64(msyms_t *file, seg_cmd64_t *seg)
{
	void	*mem;
	size_t	sectsize;
	size_t	offset;
	void	*sect;

	if ((seg->cmdsize - sizeof(seg_cmd64_t)) != seg->nsects * sizeof(sect64_t))
		return ;
	sectsize = seg->nsects * sizeof(sect64_t);
	offset = file->nsects * sizeof(sect64_t);
	sect = (void *)(seg + 1);
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += seg->nsects;
	file->sect = mem;
}

void		addsection(msyms_t *file, seg_cmd_t *seg)
{
	void	*mem;
	size_t	sectsize;
	size_t	offset;
	void	*sect;

	if ((seg->cmdsize - sizeof(seg_cmd_t)) != seg->nsects * sizeof(sect_t))
		return ;
	sectsize = seg->nsects * sizeof(sect_t);
	offset = file->nsects *  sizeof(sect_t);
	sect = (void *)(seg + 1);
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += seg->nsects;
	file->sect = mem;
}
#ifdef NM
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
