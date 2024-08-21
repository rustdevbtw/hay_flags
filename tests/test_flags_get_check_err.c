#include <assert.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *argv[] = {"hello", "-p", "3000"};
  int argc = 3;

  int vc = 0;
  flag_t flags[] = {
      {"-p"}, {"--port"}, {"--dir"}, {"-d"}, {"--verbose", NULL, &vc}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != NULL);
  int c = hay_flags_get_check(flags, "--verbose");
  assert(c == 0);
  assert(vc == 0);

  return 0;
}
