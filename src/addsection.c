/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addsection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 18:57:10 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 20:31:39 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "nm.h"
#include "libft.h"

void		addsection64(msyms_t *file, seg_cmd64_t *seg, int s)
{
	void	*mem;
	size_t	sectsize;
	size_t	offset;
	void	*sect;

	sectsize = swap(seg->nsects, s) * sizeof(sect64_t);
	if ((swap(seg->cmdsize, s) - sizeof(seg_cmd64_t)) != sectsize)
		return ;
	offset = file->nsects * sizeof(sect64_t);
	sect = (void *)(seg + 1);
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += swap(seg->nsects, s);
	file->sect = mem;
}

void		addsection(msyms_t *file, seg_cmd_t *seg, int s)
{
	void	*mem;
	size_t	sectsize;
	size_t	offset;
	void	*sect;

	sectsize = swap(seg->nsects, s) * sizeof(sect_t);
	if ((swap(seg->cmdsize, s) - sizeof(seg_cmd_t)) != sectsize)
		return ;
	offset = file->nsects * sizeof(sect_t);
	sect = (void *)(seg + 1);
	if ((mem = malloc(sectsize + offset)) == (void *)0)
		return ;
	if (file->sect)
		ft_memcpy(mem, file->sect, offset);
	free(file->sect);
	ft_memcpy(mem + offset, sect, sectsize);
	file->nsects += swap(seg->nsects, s);
	file->sect = mem;
}
