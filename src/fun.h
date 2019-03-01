/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fun.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:57:32 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 20:35:19 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUN_H
# define FUN_H

# include "type.h"
# include <stdint.h>

# ifdef NM

void		ft_qsort(void **stack, ssize_t size, int (*cmp)());

void		addsymbole(msyms_t *file, st_cmd_t *cmd, int s);
void		addsymbole64(msyms_t *file, st_cmd_t *cmd, int s);
void		dumpsym(msyms_t *file, int is32);
void		init_msyms(msyms_t *file, mhfile_t *mach, size_t size);

# endif
# ifdef OTOOL

void		dumptext(msyms_t *file, int is32, int s);
void		init_msyms(msyms_t *file, mhfile_t *mach,
				mach_hdr_t *hdr, size_t size);

# endif

void		init_machfromarch(mhfile_t *file);
int			init_machheader(mhfile_t *file);
int			init_machfile(int fd, mhfile_t *file);
int			dumpbin(mhfile_t *mach, size_t size);
void		dump(mhfile_t *file);
void		dumpfat(mhfile_t *file, int s);

void		addsection(msyms_t *file, seg_cmd_t *seg, int s);
void		addsection64(msyms_t *file, seg_cmd64_t *seg, int s);
int			init_machfile32(msyms_t *file, int s);
int			init_machfile64(msyms_t *file, int s);
void		copy_imginfo(mhfile_t *file, farch_t *fat, int s);
uint32_t	swap(uint32_t val, int s);
uint64_t	swap64(uint64_t val, int s);
void		xtoa_b_intel(char *dst, uint8_t *src, int n);
void		xtoa_b(char *dst, uint8_t *src, int n);

#endif
