/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:55:14 by jye               #+#    #+#             */
/*   Updated: 2019/03/01 20:14:50 by jye              ###   ########.fr       */
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
	size_t		max;
	uint64_t	n_value;
	uint32_t	n_type;
	uint32_t	n_sect;
};

# endif

struct	s_mach_file
{
	char		*name;
	char		*objname;
	char		*archname;
	void		*map;
	size_t		mapsize;
	size_t		truesize;
	void		*base;
	void		*top;
	void		*truetop;
	uint32_t	arch;
	int			type;
};

struct	s_mach_syms
{
	void		*stroff;
	size_t		sectsize;
	void		*sect;
	size_t		nsects;
	void		*base;
	void		*top;
# ifdef OTOOL

	void		(*xtob)();

# endif
# ifdef NM

	sym_t		**syms;
	size_t		nsyms;

# endif

	size_t		size;
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
