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
  int i0 = hay_flags_get_idx(flags, "-p");
  assert(i0 == 2);
  int i1 = hay_flags_get_idx(flags, "-d");
  assert(i1 == -1);

  return 0;
}
