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

long int detect_ram()
{
  long int ram = 0;
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

// Function to divide memory into chunks and scan each chunk separately
void divide_and_conquer(long int bytes, unsigned int tests, unsigned char *buffer)
{
  int chunk_size = bytes / tests; // Calculate the size of each chunk
  printf("Bytes: %ld, Tests: %d,  Buffer: %p ", bytes, tests, buffer);
  for (int i = 0; i < tests; i++)
  {
    unsigned char *chunk_start = buffer + (i * chunk_size); // Get the start of the chunk
    unsigned char *chunk_end = chunk_start + chunk_size;    // Get the end of the chunk

    // Scan the chunk
    for (unsigned char *ptr = chunk_start; ptr < chunk_end; ptr++)
    {
      // Print the progress of the scan in percent with the value of the current pointer, the current chunk and the total number of chunks
      printf("\rScanning: %d%%, Chunk: %d/%d, Pointer: %p", (int)((ptr - chunk_start) * 100 / chunk_size), i + 1, tests, ptr);

      fflush(stdout);

      if (*ptr)
      {
        char *stamp = timestamp();

        printf("\r ✨ %s | Bitflip detected at %p\n\n", stamp, &buffer[i]);
        *ptr = 0;
      }
    }
  }
}

int main()
{
  short int percent = 0;
  while (percent < 1 || percent > 90)
  {
    ask_user(&percent);
  }
  clearTerminal();
  long int ram = detect_ram();

  long int bytes = (percent * ram) / 100;
  if (!bytes)
  {
    printf("Error: Can't detect RAM. Allocating 1GB.\n");
    bytes = 1073741824;
  }
  unsigned int tests = 0;
  // Calculate the number of tests to perform

  if (bytes < 1073741824)
  {
    tests = 1;
  }
  else if (bytes < 2147483648)
  {
    tests = 2;
  }
  else if (bytes < 4294967296)
  {
    tests = 4;
  }
  else if (bytes < 8589934592)
  {
    tests = 8;
  }
  else if (bytes < 17179869184)
  {
    tests = 16;
  }
  else if (bytes < 34359738368)
  {
    tests = 32;
  }
  else if (bytes < 68719476736)
  {
    tests = 64;
  }
  else if (bytes < 137438953472)
  {
    tests = 128;
  }
  else if (bytes < 274877906944)
  {
    tests = 256;
  }
  else if (bytes < 549755813888)
  {
    tests = 512;
  }
  else if (bytes < 1099511627776)
  {
    tests = 1024;
  }
  else if (bytes < 2199023255552)
  {
    tests = 2048;
  }
  else if (bytes < 4398046511104)
  {
    tests = 4096;
  }

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
  printf("----------------------------------------\n");
  while (1)
  {
    divide_and_conquer(bytes, tests, buffer);
  }
}