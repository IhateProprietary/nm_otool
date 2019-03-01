/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_machfile.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:46:21 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 19:41:42 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <mach-o/loader.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "nm.h"
#include "libft.h"
#define MAP_FLAGS (MAP_FILE | MAP_PRIVATE)

#ifdef NM

int		init_machfile64(msyms_t *file, int s)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = swap(((mach_hdr64_t *)file->base)->sizeofcmds, s);
	if (sizeofcmds > file->size)
		return (1);
	offset = sizeof(mach_hdr64_t);
	file->sectsize = sizeof(sect64_t);
	ncmds = swap(((mach_hdr64_t *)file->base)->ncmds, s);
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)file->base + offset);
		offset += swap(ld->cmdsize, s);
		if (offset > sizeofcmds)
			break ;
		if (swap(ld->cmd, s) == LC_SEGMENT_64)
			addsection64(file, ((seg_cmd64_t *)ld), s);
		else if (swap(ld->cmd, s) == LC_SYMTAB)
			addsymbole64(file, (st_cmd_t *)ld, s);
	}
	return (0);
}

int		init_machfile32(msyms_t *file, int s)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = swap(((mach_hdr_t *)file->base)->sizeofcmds, s);
	if (sizeofcmds > file->size)
		return (1);
	offset = sizeof(mach_hdr_t);
	file->sectsize = sizeof(sect_t);
	ncmds = swap(((mach_hdr_t *)file->base)->ncmds, s);
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)file->base + offset);
		offset += swap(ld->cmdsize, s);
		if (offset > sizeofcmds)
			break ;
		if (swap(ld->cmd, s) == LC_SEGMENT)
			addsection(file, ((seg_cmd_t *)ld), s);
		else if (swap(ld->cmd, s) == LC_SYMTAB)
			addsymbole(file, (st_cmd_t *)ld, s);
	}
	return (0);
}

#else

int		init_machfile64(msyms_t *file, int s)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = swap(((mach_hdr64_t *)file->base)->sizeofcmds, s);
	if (sizeofcmds > file->size)
		return (1);
	offset = sizeof(mach_hdr64_t);
	file->sectsize = sizeof(sect64_t);
	ncmds = swap(((mach_hdr64_t *)file->base)->ncmds, s);
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)file->base + offset);
		offset += swap(ld->cmdsize, s);
		if (offset > sizeofcmds)
			break ;
		if (swap(ld->cmd, s) == LC_SEGMENT_64)
			addsection64(file, ((seg_cmd64_t *)ld), s);
	}
	return (0);
}

int		init_machfile32(msyms_t *file, int s)
{
	ld_cmd_t	*ld;
	size_t		offset;
	uint32_t	ncmds;
	uint32_t	sizeofcmds;

	sizeofcmds = swap(((mach_hdr_t *)file->base)->sizeofcmds, s);
	if (sizeofcmds > file->size)
		return (1);
	offset = sizeof(mach_hdr_t);
	file->sectsize = sizeof(sect_t);
	ncmds = swap(((mach_hdr_t *)file->base)->ncmds, s);
	while (ncmds--)
	{
		ld = (ld_cmd_t *)((char *)file->base + offset);
		offset += swap(ld->cmdsize, s);
		if (offset > sizeofcmds)
			break ;
		if (swap(ld->cmd, s) == LC_SEGMENT)
			addsection(file, ((seg_cmd_t *)ld), s);
	}
	return (0);
}

#endif

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
	file->truetop = map + stat_.st_size;
	file->map = map;
	file->base = map;
	if ((init_machheader(file)))
	{
		munmap(file->map, stat_.st_size);
		return (1);
	}
	return (0);
}
