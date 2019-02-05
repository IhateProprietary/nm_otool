/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:55:14 by jye               #+#    #+#             */
/*   Updated: 2019/02/05 16:10:48 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPE_H
# define TYPE_H

# include <stdint.h>
# include <string.h>
# include "typedef.h"

# ifdef NM

struct	s_sym
{
	char		*idx;
	uint64_t	n_value;
	uint32_t	n_type;
	uint32_t	n_sect;
};

# endif

struct	s_mach_file
{
	char	*name;
	char	*objname;
	void	*map;
	size_t	mapsize;
	size_t	truesize;
	void	*base;
	void	*top;
	int		type;
};

struct	s_mach_syms
{
	void	*stroff;
	size_t	sectsize;
	void	*sect;
	size_t	nsects;
# ifdef NM

	sym_t	**syms;
	size_t	nsyms;

# endif

	size_t	size;
};

# ifdef OTOOL

struct	s_mach_text
{
	void		*ptr;
	char		*sectname;
	char		*segname;
	size_t		size;
	uint64_t	addr;
};

# endif

#endif
