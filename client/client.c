#include "client.h"

int handle_user_input(int s)
{
  int ch;
  char *cmd;

  ch = getch();
  if (ch == 27)
  {
    ch = getch();
    if (ch == 91)
    {
      ch = getch();
      if (ch == 65)
        cmd = "2";
      else if (ch == 66)
        cmd = "3";
      else if (ch == 67)
        cmd = "4";
      else if (ch == 68)
        cmd = "5";
    }
    else if (ch == 27)
      return (1);
  }
  else if (ch == 32)
    cmd ="6";

  if (send(s, cmd, strlen(cmd), 0) == -1)
    return (-1);
  return (0);
}

int handle_file_desc(int s, fd_set read_fds)
{
  int i;
  int nread;
  char buf[1024];
  int ret;

  for (i = 0; i < (s + 1); i++)
  {
    if (FD_ISSET(i, &read_fds))
    {
      if (i == STDIN_FILENO)
      {
        if ((ret = handle_user_input(s)) == -1)
          printf("Input couldn't be sent\n");
        else if (ret == 1)
          return (ret);
      }
      else if (i == s)
      {
        do
        {
          if ((nread = recv(i, buf, 1024, 0)) == -1)
            printf("failed to received map\n");
          if (nread > 0)
          {
            if (strcmp(buf, "full") == 0)
            {
              printf("The server is full.\n");
              exit(0);
            }
            printf("%s\n", buf);
          }
        } while (nread == 0);
      }
    }
  }
  return (0);
}

int main_loop(int s)
{
  fd_set read_fds;
  fd_set active_fds;
  int ret;

  FD_ZERO(&active_fds);
  FD_SET(s, &active_fds);
  FD_SET(STDIN_FILENO, &active_fds);
  if (set_conio_terminal_mode() == -1)
    return (-2);
  while (1)
  {
    read_fds = active_fds;
    if((ret = select(s + 1, &read_fds, NULL, NULL, 0)) == -1)
      return (ret);
    if ((ret = handle_file_desc(s, read_fds)) == 1)
      return (ret);
  }
}

int ask_connection(int s, char *login)
{
  char *cmd;
  char *final_cmd;
  size_t size;
  int ret;

  cmd = "000";
  size = sizeof(cmd) + sizeof(login) + 1;
  if ((final_cmd = malloc(size)) == NULL)
    return (-1);
  strcpy(final_cmd, cmd);
  strcat(final_cmd, login);
  if ((ret = send(s, final_cmd, size, 0)) == -1)
    return (ret);
  return (0);
}

int main(int argc, char **argv)
{
  int s;
  struct sockaddr_in sin;
  int ret;

  s = socket(PF_INET, SOCK_STREAM, 0);
  sin.sin_family = AF_INET;
  if (argc >= 3)
    sin.sin_port = htons((uint16_t)atoi(argv[2]));
  else
    sin.sin_port = htons(PORT);
  if (argc >= 2)
    sin.sin_addr.s_addr = inet_addr(argv[1]);
  else
    sin.sin_addr.s_addr = inet_addr(IP);
  connect(s, (struct sockaddr *)&sin, sizeof(struct sockaddr_in));
  if (argc >= 4)
    ask_connection(s, argv[3]);
  else
    ask_connection(s, USERNAME);
  if((ret = main_loop(s)) == -1)
  {
    printf("select failed, program will close now.\n\n");
    return (ret);
  }
  else if (ret == -2)
  {
    printf("set_conio_terminal_mode failed, program will close now.\n\n");
    return (ret);
  }

  printf("See you later!\n\n");
  return (0);
}
