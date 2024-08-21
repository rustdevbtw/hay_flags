#include <assert.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *argv[] = {"hello", "-p", "3000", "-d", "src/"};
  int argc = 5;
  char *port = NULL;
  char *dir = NULL;

  flag_t flags[] = {
      {"-p", &port}, {"--port", &port}, {"--dir", &dir}, {"-d", &dir}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != NULL);
  assert(port != NULL);
  assert(dir != NULL);
  assert(strcmp(port, "3000") == 0);
  assert(strcmp(dir, "src/") == 0);

  return 0;
}
