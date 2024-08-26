#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"./test", "-Vr", "--port", "3000", "-d",
                  "src",    "-p",  "4000",   "-e",   "true"};
  int argc = 10;

  flag_t *port = hay_flags_create("port", 'p', FT_INT);
  flag_t *dir = hay_flags_create("dir", 'd', FT_STR);
  flag_t *repl = hay_flags_create("repl", 'r', FT_BOOL);
  flag_t *verbose = hay_flags_create("verbose", 'V', FT_BOOL);
  flag_t *enable = hay_flags_create("enable", 'e', FT_BOOL);

  assert(port != nullptr);

  bool v = hay_flags_getbool(verbose, false);

  assert(v == false);

  flag_t *flags[] = {port, dir, repl, verbose, enable, nullptr};

  int res = hay_flags_parse(flags, argc, argv);

  assert(res == 0);

  v = hay_flags_getbool(verbose, false);

  assert(v == true);
}
