/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getopt_long.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <marvin@42.fr>                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/09/14 11:55:45 by root              #+#    #+#             */
/*   Updated: 2018/07/03 15:10:32 by jye              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define MACRO_DECLARE
#include "ft_getopt_long.h"
#include "libft.h"
#include "ft_printf.h"

#define OPT_ISOPT(opt) ((opt)[0] == '-' && (opt)[1])
#define OPT_ISLONG(opt) (!ft_strncmp((opt), "--", 2) && (opt)[2] != 0)
#define OPT_ISEND(opt) (!ft_strcmp((opt), "--"))

#define T1 ft_getopt_(&nextchar, av, ft_opt_getstruct(*nextchar, lopt_), pname)

int optind_;
int opterr_;
char *optarg_;

struct s_options	*ft_opt_getstruct_long(char *s,
		struct s_options *longopt,
		char *pname)
{
	char	*has_arg;
	size_t	ncmp;

	has_arg = ft_strchr(s, '=');
	if (has_arg)
		optarg_ = has_arg + 1;
	ncmp = (size_t)has_arg ? (size_t)(has_arg - s) : ft_strlen(s);
	while (longopt->s != NULL)
	{
		if (ncmp == ft_strlen(longopt->s) &&
				!ft_strncmp(s, longopt->s, ncmp))
			break ;
		longopt++;
	}
	if (longopt->s == NULL)
	{
		if (opterr_)
			ft_dprintf(2, "%s: unrecognized options '--%.*s'\n", pname,
					(int)ncmp, s);
		return (NULL);
	}
	return (longopt);
}

struct s_options	*ft_opt_getstruct(char s, struct s_options *longopt)
{
	while (longopt->s != NULL)
	{
		if (s == longopt->val)
			break ;
		longopt++;
	}
	return (longopt->s ? longopt : NULL);
}

static int			ft_opt_end(char **nextchar)
{
	*nextchar = NULL;
	return (-1);
}

static int			ft_getopt_(char **nextchar, char **av,
						struct s_options *lopt, char *pname)
{
	if (lopt->has_arg >= req_arg && (*nextchar)[1] == 0)
		optarg_ = av[optind_++];
	if (lopt->has_arg == req_arg && optarg_ == NULL)
		ft_dprintf(2, "%s: '-%c' doesn't allow an argument\n",
			pname, lopt->val);
	return (*(*nextchar)++);
}

int					ft_getopt_long(int ac, char **av,
		char *optstr, struct s_options *lopt_)
{
	static char			*nextchar;
	static char			*pname;

	optarg_ = 0;
	if (optind_ == 0)
		ft_getopt_init(&pname, &nextchar, av[0]);
	if (nextchar == NULL)
		return (-1);
	if ((*nextchar == 0 && optind_ >= ac) || av[optind_] == 0 ||
		OPT_ISEND(av[optind_]))
		return (ft_opt_end(&nextchar));
	else if (*nextchar == 0 && OPT_ISLONG(av[optind_]))
		return (ft_getopt_long_long(
					ft_opt_getstruct_long(av[optind_] + 2, lopt_, pname),
					av, pname));
	else if (*nextchar == 0 && OPT_ISOPT(av[optind_]))
		nextchar = av[optind_++] + 1;
	else if (*nextchar == 0)
		return (ft_opt_end(&nextchar));
	if (optstr && ft_strchr(optstr, *nextchar))
		return (T1);
	if (opterr_)
		ft_dprintf(2, "%s: unrecognized option '%c'\n", pname, *nextchar++);
	return ('?');
}
