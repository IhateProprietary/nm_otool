/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:45:07 by jye               #+#    #+#             */
/*   Updated: 2019/02/10 16:38:42 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "nm.h"
#include "ft_printf.h"

#include <unistd.h>
#include <fcntl.h>

int		main(int ac, char **av)
{
	int			i;
	int			fd;
	mhfile_t	file;

	if (ac == 1)
		return (1);
	i = 1;
	while (i < ac)
	{
		if ((fd = open(av[i], O_RDONLY)) != -1)
		{
			if (!init_machfile(fd, &file))
			{
				file.name = av[i];
				dump(&file);
				munmap(file.map, file.mapsize);
			}
			else
				ft_dprintf(2, "%s fail\n", av[i]);
			close(fd);
		}
		i++;
	}
	return (0);
}
