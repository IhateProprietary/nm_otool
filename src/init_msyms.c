/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_msyms.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/01 16:41:44 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 16:43:38 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "nm.h"
#include "libft.h"

#ifdef OTOOL

void	init_msyms(msyms_t *file, mhfile_t *mach, mach_hdr_t *hdr, size_t size)
{
	ft_memset(file, 0, sizeof(*file));
	file->size = size;
	file->base = mach->base;
	file->top = mach->top;
	if (mach->arch == 0)
		file->xtob = ((hdr->cputype & 7) == 7) ? xtoa_b_intel : xtoa_b;
	else
		file->xtob = ((mach->arch & 7) == 7) ? xtoa_b_intel : xtoa_b;
}

#elif defined(NM)

void	init_msyms(msyms_t *file, mhfile_t *mach, size_t size)
{
	ft_memset(file, 0, sizeof(*file));
	file->size = size;
	file->base = mach->base;
	file->top = mach->top;
}

#endif
