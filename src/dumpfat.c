/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dumpfat.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/27 16:29:25 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 20:35:17 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach/machine.h>
#include <stdint.h>
#include "libft.h"
#include "nm.h"

uint32_t	swap(uint32_t val, int s)
{
	return (s ? SWAP32(val) : val);
}

uint64_t	swap64(uint64_t val, int s)
{
	return (s ? SWAP64(val) : val);
}

void		get_archname(mhfile_t *file)
{
	static char *archname[] = {
		"VAX", 0, 0, 0, 0,
		"MC680x0", "I386", 0, 0, "MC98000",
		"HPPA", "ARM", "MC88000", "SPARC", "I860",
		0, 0, "POWERPC"
	};
	cpu_type_t	c;
	uint32_t	cputype;

	file->archname = 0;
	cputype = file->arch;
	c = cputype & ~CPU_ARCH_ABI64;
	if (c > 0 && c < 19)
		file->archname = archname[c - 1];
	else if (cputype == CPU_TYPE_POWERPC64)
		file->archname = "POWERPC64";
	else if (cputype == CPU_TYPE_ARM64)
		file->archname = "ARM64";
}

void		dumpfat(mhfile_t *file, int s)
{
	uint32_t	nfat;
	uint32_t	magic;
	farch_t		*fat;

	nfat = swap(((struct fat_header *)file->map)->nfat_arch, s);
	if ((nfat * sizeof(farch_t)) > 0x1000)
		return ;
	fat = file->map + sizeof(struct fat_header);
	while (nfat--)
	{
		copy_imginfo(file, fat++, s);
		get_archname(file);
		magic = *((uint32_t *)file->base);
		if (magic == MH_MAGIC || magic == MH_MAGIC_64 ||
			magic == MH_CIGAM || magic == MH_CIGAM_64)
			file->type = MF_BINARY;
		else if (ft_memcmp(file->base, ARMAG, SARMAG) == 0)
			init_machfromarch(file);
		if (file->type != MF_FAT)
			dump(file);
	}
}
