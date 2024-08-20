#include <assert.h>
#include <hay/flags.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *argv[] = {"test_flags", "--dir", "../", "-p", "3000", "--verbose",
                  "--port",     "5000",  "-d",  "./", "-p",   "4000"};
  int argc = 12;
  char *port;
  int verbosity;
  flag_t flags[] = {{"-p", &port},
                    {"--port", &port},
                    {"-d"},
                    {"--dir"},
                    {"--verbose", NULL, &verbosity}};
  void *r = hay_flags_parse(flags, argc, argv);
  assert(r != NULL);
  assert(strcmp(port, "4000") == 0);
  char *d0 = hay_flags_get_val(flags, "-d");
  char *d1 = hay_flags_get_val(flags, "--dir");
  int d0i = hay_flags_get_idx(flags, "-d");
  int d1i = hay_flags_get_idx(flags, "--dir");
  assert(d0i == 9);
  assert(d1i == 2);
  assert(d0 != NULL);
  assert(d1 != NULL);
  char *dir = (d0i > d1i) ? d0 : d1;
  assert(strcmp(dir, "./") == 0);
  assert(verbosity == 1);
  return 0;
}
