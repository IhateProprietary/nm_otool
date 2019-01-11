#ifndef NM_H
# define NM_H

# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <unistd.h>
# include <mach/machine.h>
# include <ar.h>

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

# define MF_ARCHMASK	0xff00
# define MF_ARCH32		0x3200
# define MF_ARCH64		0x6400

# define MF_NORMAL		0x00
# define MF_ARCHIVE 	0x10

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
typedef struct mach_file			mhfile_t;
typedef struct fat_arch				farch_t;
typedef struct fat_arch_64			farch64_t;

typedef struct
{
	char		*idx;
	uint64_t	n_value;
	uint32_t	n_type;
	uint32_t	n_sect;
}	sym_t;

struct	mach_file
{
	void		*map;
	size_t		mapsize;
	int			type;
	uint32_t	nfile;
	void		*header;
	void		*base;
	void		*stroff;
	size_t		sectsize;
	void		*sect;
	size_t		nsects;
	sym_t		**syms;
	size_t		nsyms;
};

void	ft_qsort(void **stack, ssize_t size, int (*cmp)());

#endif
