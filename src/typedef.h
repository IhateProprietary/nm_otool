/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:58:03 by jye               #+#    #+#             */
/*   Updated: 2019/02/01 22:49:06 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPEDEF_H
# define TYPEDEF_H

# define T_ASD(x) x##_t

# define LOKEK(x) typedef struct x x##_t

# define ASD LOKEK(stat)

ASD;

typedef struct stat					T_ASD(stat);
typedef struct mach_header			mach_hdr_t;
typedef struct mach_header_64		mach_hdr64_t;
typedef struct segment_command		seg_cmd_t;
typedef struct segment_command_64	seg_cmd64_t;
typedef struct section				sect_t;
typedef struct section_64			sect64_t;
typedef struct load_command			ld_cmd_t;
typedef struct symtab_command		st_cmd_t;
typedef struct nlist_64				nlst64_t;
typedef struct nlist				nlst_t;
typedef struct fat_arch				farch_t;
typedef struct ranlib				ranlib_t;
typedef struct ar_hdr				ar_hdr_t;

typedef struct s_mach_file			mhfile_t;
typedef struct s_mach_sub			msub_t;
typedef struct s_mach_syms			msyms_t;
# ifdef OTOOL

typedef struct s_mach_text			mtext_t;

# endif

#endif
