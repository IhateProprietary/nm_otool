/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jye <marvin@42.fr>                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/01 21:49:58 by jye               #+#    #+#             */
/*   Updated: 2019/02/27 16:32:25 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NM_H
# define NM_H

# include "swapbyte.h"
# include "typedef.h"
# include "type.h"
# include "fun.h"

# define LETTERSYM "tdb"
# define CPUARCH64 CPU_TYPE_X86_64
# define CPUARCH32 CPU_TYPE_X86

# define MF_TYPEMASK	0xf0
# define MF_NONE		0x00
# define MF_ARCHIVE 	0x10
# define MF_BINARY		0x20
# define MF_FAT			0x30

#endif
