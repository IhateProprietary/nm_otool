/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_machheader.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:47:00 by jye               #+#    #+#             */
/*   Updated: 2019/03/02 17:40:56 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <ar.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <mach-o/ranlib.h>
#include "nm.h"
#include "libft.h"

void	copy_imginfo(mhfile_t *file, farch_t *fat, int s)
{
	uint32_t	value;
	char		*val;

	val = (char *)(uint64_t)swap(fat->offset, s);
	file->arch = swap(fat->cputype, s);
	val += (uint64_t)file->map;
	if (val < (char *)file->truetop - 0x400)
		file->base = val;
	value = swap(fat->size, s);
	if (value < file->mapsize)
		file->truesize = value;
	val = file->base + swap(fat->size, s);
	file->top = val > (char *)file->truetop ? file->truetop : val;
}

void	init_machfromfat(mhfile_t *file, int s)
{
	uint32_t	nfat;
	uint32_t	value;
	farch_t		*fat;

	nfat = swap(((struct fat_header *)file->map)->nfat_arch, s);
	if ((nfat * sizeof(farch_t)) > 0x1000)
		return ;
	fat = file->map + sizeof(struct fat_header);
	while (nfat--)
	{
		value = swap(fat->cputype, s);
		if (value == CPUARCH64)
		{
			copy_imginfo(file, fat, s);
			break ;
		}
		fat++;
	}
}

void	init_machfromarch(mhfile_t *file)
{
	ar_hdr_t	*hdr;
	uint32_t	off;
	uint32_t	size;
	uint32_t	objsize;
	uint32_t	strsize;

	hdr = (ar_hdr_t *)((char *)file->base + SARMAG);
	if (ft_memcmp(hdr->ar_name, AR_EFMT1, 3) != 0 &&
		ft_memcmp(hdr->ar_fmag, ARFMAG, 2) != 0 &&
		ft_strcmp(SYMDEF, (char *)(hdr + 1)) != 0 &&
		ft_strcmp(SYMDEF, (char *)(hdr + 1)) != 0)
		return ;
	off = ft_atoi(hdr->ar_name + 3);
	size = ft_atoi(hdr->ar_size);
	if (off & 3 || off > size || size > file->truesize)
		return ;
	objsize = *((uint32_t *)((char *)(hdr + 1) + off));
	if (objsize > size)
		return ;
	strsize = *((uint32_t *)((char *)(hdr + 1)
						+ off + objsize + sizeof(uint32_t)));
	if ((strsize + objsize) > size)
		return ;
	file->base = ((void *)(hdr + 1) + size);
	file->type = MF_ARCHIVE;
}

int		init_machheader(mhfile_t *file)
{
	uint32_t	magic;

	magic = *((uint32_t *)file->map);
	file->truesize = file->mapsize;
	if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		init_machfromfat(file, magic == FAT_CIGAM);
	if (file->base > (file->mapsize + file->map))
		return (1);
	magic = *((uint32_t *)file->base);
	if (magic == MH_MAGIC || magic == MH_MAGIC_64)
		file->type = MF_BINARY;
	else if (magic == FAT_CIGAM || magic == FAT_MAGIC)
		file->type = MF_FAT;
	else if (ft_memcmp(file->base, ARMAG, SARMAG) == 0)
		init_machfromarch(file);
	return (!file->type);
}
