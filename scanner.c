#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>

void ask_user(int *percent)
{
  printf("How many percent of the free memory would you like to allocate?\nThe bigger the value is, the bigger the detection surface will be.\n\nJust be sure to not go over 90%, as it might freeze the system.\nYou might also need to fine tune the value, in order to keep your OS happy.\n");
  scanf("%d", percent);
}

void clearTerminal()
{
  printf("\033[2J\033[1;1H");
}

char *timestamp()
{
  time_t ltime;
  ltime = time(NULL);
  char *res = asctime(localtime(&ltime));

  res[strlen(res) - 1] = '\0';

  return res;
}

long long int detect_ram()
{
  long long int ram = 0;
  char line[256];

  FILE *fp = fopen("/proc/meminfo", "r");
  if (fp == NULL)
  {
    printf("Error opening file");
    return 1;
  }
  while (fgets(line, sizeof(line), fp))
  {
    if (strncmp(line, "MemTotal:", 9) == 0)
    {
      sscanf(line, "MemTotal: %d kB", &ram);
      break;
    }
  }
  fclose(fp);

  return ram * 1024;
}

int main()
{
  short int percent = 0;

  while (percent < 1 || percent > 90)
  {
    ask_user(&percent);
  }
  clearTerminal();
  long long int ram = detect_ram();
  long long int bytes = (percent * ram) / 100;

  unsigned int tests = 0;

  char *ascii = " ______                      __           ______                  _____        __              __              \r\n|      .-----.-----.--------|__.----.    |   __ .---.-.--.--.    |     \\.-----|  |_.-----.----|  |_.-----.----.\r\n|   ---|  _  |__ --|        |  |  __|    |      |  _  |  |  |    |  --  |  -__|   _|  -__|  __|   _|  _  |   _|\r\n|______|_____|_____|__|__|__|__|____|    |___|__|___._|___  |    |_____/|_____|____|_____|____|____|_____|__|  \r\n                                                      |_____|                                                  ";
  printf("%s \r \n", ascii);
  printf("Allocating %lld bytes of memory (%d%% of the free memory)\n", bytes, percent);
  printf("\nPress 'ctrl + c' to stop the program.\n");
  unsigned char *buffer = (unsigned char *)calloc(bytes, 1);
  if (buffer == NULL)
  {
    printf("Error allocating memory");
    return 1;
  }
  // Locking the allocated memory in RAM to prevent it from being swapped out
  mlock(buffer, bytes);
  memset(buffer, 0, bytes);
  printf("\nScan started at %s.\n", timestamp());
  while (1)
  {
    for (size_t i = 0; i < bytes; ++i)
    {
      printf("Current byte inspected: %p \033[?25l \r", &buffer[i]);
      fflush(stdout);
      if (buffer[i])
      {
        char *stamp = timestamp();
        printf("\r ✨ %s | Bitflip detected at %p\n\n", stamp, &buffer[i]);
      }
    }
  }
}