/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dumptext.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:48:43 by jye               #+#    #+#             */
/*   Updated: 2019/02/10 16:20:52 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef OTOOL
# include <stdint.h>
# include "nm.h"
# include "libft.h"
# include "ft_printf.h"
# define HEXASCII "0123456789abcdef"

void	xtoa_b(char *dst, uint8_t *src, int n)
{
	int		i;
	uint8_t	b;

	i = 0;
	while (i < n)
	{
		b = src[i++];
		dst[2] = 0x20;
		dst[1] = HEXASCII[b & 0xf];
		b /= 0x10;
		dst[0] = HEXASCII[b & 0xf];
		dst += 3;
	}
	*dst = 0;
}

void	init_textforsection(msyms_t *file, void *base,
					char *sectname, mtext_t *t)
{
	size_t	nsects;
	sect_t	*sect;

	nsects = file->nsects;
	sect = file->sect;
	while (nsects--)
	{
		if (ft_strcmp(sectname, sect->sectname) == 0)
		{
			t->sectname = sect->sectname;
			t->segname = sect->segname;
			t->size = sect->size;
			t->addr = sect->addr;
			t->ptr = (char *)base + sect->offset;
			break ;
		}
		sect++;
	}
}

void	init_textforsection64(msyms_t *file, void *base,
						char *sectname, mtext_t *t)
{
	size_t		nsects;
	sect64_t	*sect;

	nsects = file->nsects;
	sect = file->sect;
	while (nsects--)
	{
		if (ft_strcmp(sectname, sect->sectname) == 0)
		{
			t->sectname = sect->sectname;
			t->segname = sect->segname;
			t->size = sect->size;
			t->addr = sect->addr;
			t->ptr = (char *)base + sect->offset;
			break ;
		}
		sect++;
	}
}

void	dumptext(msyms_t *file, void *base, int is32)
{
	mtext_t	text;
	size_t	i;
	int		pad;
	char	b[50];

	ft_memset(&text, 0, sizeof(text));
	if (is32)
		init_textforsection(file, base, "__text", &text);
	else
		init_textforsection64(file, base, "__text", &text);
	ft_printf("Contents of (%s,%s) section\n", text.segname, text.sectname);
	i = 0;
	pad = 8 + 8 * !is32;
	while (i < text.size)
	{
		xtoa_b(b, (uint8_t *)text.ptr + i,
			(text.size - i) < 0x10 ? text.size - i : 0x10);
		ft_printf("%-*.*llx\t%s\n", pad, pad, text.addr + i, b);
		i += 16;
	}
}
#endif
