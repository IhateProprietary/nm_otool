#include <stdint.h>
#include <mach-o/loader.h>
#include <sys/stat.h>
#include <sys/mann.h>
#include "nm.h"
#define MAP_FLAGS (MAP_FILE | MAP_PRIVATE)

#ifdef NM

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
		else if (ld->cmd == LC_SYMTAB)
			addsymbole64(file, (st_cmd_t *)ld, (void *)hdr);
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
		else if (ld->cmd == LC_SYMTAB)
			addsymbole(file, (st_cmd_t *)ld, (void *)hdr);
		offset += ld->cmdsize;
		if (offset > sizeofcmds)
			;
	}
}

#else

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
		offset += ld->cmdsize;
		if (offset > sizeofcmds)
			;
	}
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
	file->map = map;
	file->base = map;
	if ((init_machheader(file)))
	{
		munmap(file->map, stat_.st_size);
		return (1);
	}
	return (0);
}
