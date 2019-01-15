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

# if !defined(__LITTLE_ENDIAN__) && !defined(__BIG_ENDIAN__)
#  if __BYTE_ORDER__ == __LITTLE_ENDIAN_ORDER__
#   define __LITTLE_ENDIAN__ 1
#  else
#   define __BIG_ENDIAN__ 1
#  endif
# endif

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


typedef struct
{
	char		*idx;
	uint64_t	n_value;
	uint32_t	n_type;
	uint32_t	n_sect;
}	sym_t;

struct	mach_file
{
	char	*name;
	void	*map;
	size_t	mapsize;
	size_t	truesize;
	void	*base;
	void	*top;
	int		type;
};

struct	mach_syms
{
	char	*obj;
	void	*stroff;
	size_t	sectsize;
	void	*sect;
	size_t	nsects;
	sym_t	**syms;
	size_t	nsyms;
	size_t	size;
};

void	ft_qsort(void **stack, ssize_t size, int (*cmp)());


void		addsymbole(msyms_t *file, st_cmd_t *cmd, void *base);
void		addsymbole64(msyms_t *file, st_cmd_t *cmd, void *base);
void		addsection(msyms_t *file, seg_cmd_t *seg);
void		addsection64(msyms_t *file, seg_cmd64_t *seg);
void		printsym(sym_t *sym, sect64_t *sect);
void		init_machfile32(msyms_t *file, mach_hdr_t *hdr);
void		init_machfile64(msyms_t *file, mach_hdr64_t *hdr);
#endif
