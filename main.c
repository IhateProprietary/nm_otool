#ifdef __APPLE__
# include <mach-o/loader.h>
# include <mach-o/stab.h>
# include <mach-o/nlist.h>
#else
# include "loader.h"
# include "stab.h"
# include "nlist.h"
#endif
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#define __NSECT 16

/*
**
*/

typedef struct stat stat_t;
typedef struct mach_header_64 mach_header64_t;
typedef struct segment_command_64 seg_command_t;
typedef struct section_64	sect64_t;
typedef struct load_command load_cmd_t;
typedef struct symtab_command st_cmd_t;
typedef struct nlist_64 nlst64_t;

typedef struct {
	char *name;
	nlst64_t *info;
} sym_t;

int		main(int ac, char **av, char **ev)
{
	mach_header64_t	*header;
	load_cmd_t		*lcmd;
	seg_command_t	*segment;
	stat_t 	_stat;
	int		fd;
	ssize_t	r;
	void	*map;
	void	*data1;
	void	*data2;

	if (ac == 1)
		return (1);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		return (1);
	fstat(fd, &_stat);
	map = mmap(0, _stat.st_size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
	header = map;
	printf("magic: %lx\ncputype: %lx\nfiletype: %lx\nncmds: %lx\nsizeofcmds: %lx\nflags: %lx\n",
		   header->magic, header->cputype, header->filetype, header->ncmds, header->sizeofcmds, header->flags);
	lcmd = map + sizeof(*header);
	for (uint32_t offset = sizeof(*header), ncmd = header->ncmds + 1, cmdsize = lcmd->cmdsize, i = 0; i < ncmd; i++)
	{
		segment = offset + map;
		cmdsize = segment->cmdsize;
		printf("%lx\ncmdsize == %lx type == %lx flag %lx\n",
			   offset, segment->cmdsize, segment->cmd, segment->flags);
		if (segment->cmd == LC_SEGMENT_64)
		{
			printf("cmd->segname == %s\n", segment->segname);
			sect64_t *sect = (sect64_t *)(segment + 1);
			size_t	nsect = segment->nsects;
			for (uint32_t i = 0; i < nsect; i++, sect++)
			{
				printf("sectname == %s, %lx, %lx\n", sect->sectname, sect->addr, sect->flags);
			}
		}
		else if (segment->cmd == LC_SYMTAB)
		{
			st_cmd_t *_cmd = (st_cmd_t *)segment;
			void	*stroff = _cmd->stroff + map;
			printf("symbol offset == %lx, size == %lx\n", _cmd->stroff, _cmd->strsize);
			printf("symoff == %lx nsyms == %lu\n", _cmd->symoff, _cmd->nsyms);
			uint32_t nsyms = _cmd->nsyms;
			uint32_t offset = _cmd->symoff;
			nlst64_t *sym = map + offset;
			for (uint32_t i = 0; i < nsyms; i++, sym++)
			{
				printf("strx %-30s n_type %x n_sect %x n_desc %x n_value %x EXT == %d\n",
					   stroff + sym->n_un.n_strx,
					   sym->n_type,
					   sym->n_sect,
					   sym->n_desc,
					   sym->n_value,
					   sym->n_type & N_EXT);
			}
		}
		offset += cmdsize;
		printf("__________\n");
	}
	printf("section %lu seg_cmd %lu\n", sizeof(*segment), sizeof(struct section_64));
}
