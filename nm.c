#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/fat.h>
#include <mach-o/ranlib.h>

#include "nm.h"
#ifndef FT_PRINTF_H
# include <stdio.h>
# define ft_dprintf dprintf
# define ft_printf printf
#endif
#ifndef LIBFT_H
# include <string.h>
# define ft_memcmp memcmp
# define ft_memcpy memcpy
# define ft_memset memset
# define ft_strcmp strcmp
# define ft_strchr strchr
# include <stdlib.h>
# define ft_atoi atoi
#endif

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
