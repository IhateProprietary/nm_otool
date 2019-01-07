#ifndef NM_H
# define NM_H

# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# define LETTERSYM "tdb"

typedef struct stat					stat_t;
typedef struct mach_header_64		mach_hdr64_t;
typedef struct segment_command_64	seg_cmd64_t;
typedef struct section_64			sect64_t;
typedef struct load_command 		ld_cmd_t;
typedef struct symtab_command 		st_cmd_t;
typedef struct nlist_64 			nlst64_t;
typedef struct mach_file			mhfile_t;

struct	mach_file
{
	void			*map;
	void			*stroff;
	void			*base;
	size_t			mapsize;
	mach_hdr64_t	*header;
	sect64_t		*sect;
	size_t			nsects;
	nlst64_t		**syms;
	size_t			nsyms;
};

#endif
