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

# define TYPEDEF(x, y) typedef struct x y##_t

# define STAT_T TYPEDEF(stat, stat)
# define MACH_HDR_T TYPEDEF(mach_header, mach_hdr)
# define MACH_HDR64_T TYPEDEF(mach_header64, mach_hdr64)
# define SEG_CMD_T TYPEDEF(segment_command, seg_cmd)
# define SEG_CMD64_T TYPEDEF(segment_command64, seg_cmd64)
# define SECT_T TYPEDEF(section, sect)
# define SECT64_T TYPEDEF(section64, sect64)
# define LD_CMD_T TYPEDEF(load_command, ld_cmd)
# define ST_CMD_T TYPEDEF(symtab_command, st_cmd)
# define NLST64_T TYPEDEF(nlist_64, nlst64)
# define NLST_T TYPEDEF(nlist, nlst)
# define FARCH_T TYPEDEF(fat_arch, farch)
# define RANLIB_T TYPEDEF(ranlib, ranlib)
# define AR_HDR_T TYPEDEF(ar_hdr, ar_hdr)
# define MHFILE_T TYPEDEF(s_mach_file, mhfile)
# define MSUB_T TYPEDEF(s_mach_sub, msub)
# define MSYMS_T TYPEDEF(s_mach_syms, msyms)
# define MTEXT_T TYPEDEF(s_mach_text, mtext)

STAT_T;
MACH_HDR_T;
MACH_HDR64_T;
SEG_CMD_T;
SEG_CMD64_T;
SECT_T;
SECT64_T;
LD_CMD_T;
ST_CMD_T;
NLST64_T;
NLST_T;
FARCH_T;
RANLIB_T;
AR_HDR_T;

MHFILE_T;
MSUB_T;
MSYMS_T;
# ifdef OTOOL

MTEXT_T;

# endif

#endif
