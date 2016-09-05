#ifndef __COMMANDS_FUNCTIONS_H__
# define __COMMANDS_FUNCTIONS_H__
# define _GNU_SOURCE

#include "../bomberman.h"
#include "map.h"

typedef void (*t_func)(t_map *map, int player);

typedef struct	s_command_funct t_command_funct;
struct	s_command_funct
{
  char		*key;
  t_func	function;
};

t_command_funct *init_funct_tab();


#endif