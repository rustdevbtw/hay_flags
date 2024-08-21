#include <assert.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *argv[] = {"hello", "-p", "3000", "--dir", "src/"};
  int argc = 5;

  flag_t flags[] = {{"-p"}, {"--port"}, {"--dir"}, {"-d"}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != NULL);
  char *port = hay_flags_get_val(flags, "-p");
  assert(port != NULL);
  assert(strcmp(port, "3000") == 0);
  char *dir = hay_flags_get_val(flags, "--dir");
  assert(dir != NULL);
  assert(strcmp(dir, "src/") == 0);

  return 0;
}
