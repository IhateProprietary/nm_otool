/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fun.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:57:32 by jye               #+#    #+#             */
/*   Updated: 2019/02/10 16:18:43 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FUN_H
# define FUN_H

# include "type.h"
# include <stdint.h>

# ifdef NM

void		ft_qsort(void **stack, ssize_t size, int (*cmp)());

void		addsymbole(msyms_t *file, st_cmd_t *cmd, void *base);
void		addsymbole64(msyms_t *file, st_cmd_t *cmd, void *base);
void		dumpsym(msyms_t *file, uint32_t magic);

# endif
# ifdef OTOOL

void		dumptext(msyms_t *file, void *base, int is32);

# endif

int			init_machheader(mhfile_t *file);
int			init_machfile(int fd, mhfile_t *file);
int			dumpbin(mhfile_t *mach, size_t size);
void		dump(mhfile_t *file);

void		addsection(msyms_t *file, seg_cmd_t *seg);
void		addsection64(msyms_t *file, seg_cmd64_t *seg);
int			init_machfile32(msyms_t *file, mach_hdr_t *hdr);
int			init_machfile64(msyms_t *file, mach_hdr64_t *hdr);

#endif
