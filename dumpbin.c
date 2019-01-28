#include <mach-o/loader.h>
#include <stdlib.h>
#include "nm.h"
#include "libft.h"
#include "ft_printf.h"

#if defined(NM)

void	dumparch(mhfile_t *file)
{
	ar_hdr_t	*hdr;
	uint32_t	off;
	uint32_t	objsize;
	uint32_t	size;

	hdr = file->base;
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
	dumpsym(&file);
	free(file.syms);
	free(file.sect);
}

#elif defined(OTOOL)

void	dumparch(mhfile_t *file)
{
	ar_hdr_t	*hdr;
	uint32_t	off;
	uint32_t	objsize;
	uint32_t	size;

	hdr = file->base;
	ft_printf("Archive : %s", file->name);
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
	dumptext(&file, mach->base, hdr->magic == MH_MAGIC);
	free(file.sect);
}

#endif


void	dump(mhfile_t *file)
{
	if (file->type == MF_ARCHIVE)
		dumparch(file);
	else if (file->type == MF_BINARY)
		dumpbin(file, file->truesize);
}
