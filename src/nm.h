/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:49:58 by jye               #+#    #+#             */
/*   Updated: 2019/02/01 21:58:36 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include <sys/stat.h>
# include <mach-o/loader.h>
# include <mach-o/nlist.h>
# include <mach-o/ranlib.h>
# include <mach/machine.h>
# include <ar.h>
# include <unistd.h>

# include "swapbyte.h"
# include "type.h"
# include "typedef.h"
# include "fun.h"

# define LETTERSYM "tdb"
# define CPUARCH64 CPU_TYPE_X86_64
# define CPUARCH32 CPU_TYPE_X86

# define MF_TYPEMASK	0xf0
# define MF_NONE		0x00
# define MF_ARCHIVE 	0x10
# define MF_BINARY		0x20

#endif
