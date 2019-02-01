#include <stdint.h>
#include <ar.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/ranlib.h>
#include "nm.h"
#include "libft.h"

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
