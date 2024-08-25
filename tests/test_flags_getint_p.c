#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"./test", "-Vr", "-d", "src", "-p", "4000"};
  int argc = 6;

  flag_t *port = hay_flags_create("port", 'p', FT_INT);
  flag_t *dir = hay_flags_create("dir", 'd', FT_STR);
  flag_t *repl = hay_flags_create("repl", 'r', FT_NULL);
  flag_t *verbose = hay_flags_create("verbose", 'V', FT_NULL);

  assert(port != nullptr);

  int p = hay_flags_getint(port, 3000);

  assert(p == 3000);

  flag_t *flags[] = {port, dir, repl, verbose, nullptr};

  int res = hay_flags_parse(flags, argc, argv);

  assert(res == 0);

  p = hay_flags_getint(port, 3000);

  assert(p == 4000);
}
