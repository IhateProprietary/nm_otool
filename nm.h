#ifndef NM_H
# define NM_H

# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/ranlib.h>
# include <mach/machine.h>
# include <ar.h>
# include <unistd.h>

# include "swapbyte.h"

# define LETTERSYM "tdb"
# define CPUARCH64 CPU_TYPE_X86_64
# define CPUARCH32 CPU_TYPE_X86

# define MF_TYPEMASK	0xf0
# define MF_NONE		0x00
# define MF_ARCHIVE 	0x10
# define MF_BINARY		0x20

typedef struct stat					stat_t;
typedef struct mach_header			mach_hdr_t;
typedef struct mach_header_64		mach_hdr64_t;
typedef struct segment_command		seg_cmd_t;
typedef struct segment_command_64	seg_cmd64_t;
typedef struct section				sect_t;
typedef struct section_64			sect64_t;
typedef struct load_command 		ld_cmd_t;
typedef struct symtab_command 		st_cmd_t;
typedef struct nlist_64 			nlst64_t;
typedef struct nlist				nlst_t;
typedef struct fat_arch				farch_t;
typedef struct ranlib				ranlib_t;
typedef struct ar_hdr				ar_hdr_t;

typedef struct mach_file			mhfile_t;
typedef struct mach_sub				msub_t;
typedef struct mach_syms			msyms_t;
#ifdef OTOOL
typedef struct mach_text			mtext_t;
#endif
#ifdef NM
typedef struct
{
	char		*idx;
	uint64_t	n_value;
	uint32_t	n_type;
	uint32_t	n_sect;
}	sym_t;
#endif

struct	mach_file
{
	char	*name;
	char	*objname;
	void	*map;
	size_t	mapsize;
	size_t	truesize;
	void	*base;
	void	*top;
	int		type;
};

struct	mach_syms
{
	void	*stroff;
	size_t	sectsize;
	void	*sect;
	size_t	nsects;
#ifdef NM
	sym_t	**syms;
	size_t	nsyms;
#endif
	size_t	size;
};
#ifdef OTOOL
struct	mach_text
{
	void		*ptr;
	char		*sectname;
	char		*segname;
	size_t		size;
	uint64_t	addr;
};
#endif
#ifdef NM
void		ft_qsort(void **stack, ssize_t size, int (*cmp)());

void		addsymbole(msyms_t *file, st_cmd_t *cmd, void *base);
void		addsymbole64(msyms_t *file, st_cmd_t *cmd, void *base);
void		printsym(sym_t *sym, sect64_t *sect);
#endif
void		addsection(msyms_t *file, seg_cmd_t *seg);
void		addsection64(msyms_t *file, seg_cmd64_t *seg);
void		init_machfile32(msyms_t *file, mach_hdr_t *hdr);
void		init_machfile64(msyms_t *file, mach_hdr64_t *hdr);
#endif
