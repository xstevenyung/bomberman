/*
** keyboard.c for bomberman in /Users/stevenyung/workspace/current/bomberman
** 
** Made by YUNG Steven
** Login   <yung_s@etna-alternance.net>
** 
** Started on  Tue Sep  6 22:12:31 2016 YUNG Steven
** Last update Tue Sep  6 22:12:33 2016 YUNG Steven
*/

#include "keyboard.h"

struct termios orig_termios;

void reset_terminal_mode()
{
  tcsetattr(0, TCSANOW, &orig_termios);
}

int set_conio_terminal_mode()
{
  struct termios new_termios;
  int ret;

  tcgetattr(0, &orig_termios);
  memcpy(&new_termios, &orig_termios, sizeof(new_termios));
  if ((ret = atexit(reset_terminal_mode)) == -1)
    return (ret);
  cfmakeraw(&new_termios);
  tcsetattr(0, TCSANOW, &new_termios);
  return (0);
}

int getch()
{
  int r;
  unsigned char c;

  if ((r = read(0, &c, sizeof(c))) < 0)
  {
    return (r);
  }
  else
  {
    return (c);
  }
}
