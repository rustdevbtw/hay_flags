#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"./test", "-Vr", "--port", "3000", "-d",
                  "src",    "-p",  "4000",   "-e",   "true"};
  int argc = 10;

  flag_t *port = hay_flags_create("port", 'p', FT_INT);
  flag_t *dir = hay_flags_create("dir", 'd', FT_STR);
  flag_t *repl = hay_flags_create("repl", 'r', FT_NULL);
  flag_t *verbose = hay_flags_create("verbose", 'V', FT_NULL);
  flag_t *enable = hay_flags_create("enable", 'e', FT_BOOL);

  assert(port != nullptr);

  bool e = hay_flags_getbool(enable, false);

  assert(e == false);

  flag_t *flags[] = {port, dir, repl, verbose, enable, nullptr};

  int res = hay_flags_parse(flags, argc, argv);

  assert(res == 0);

  e = hay_flags_getbool(enable, false);

  assert(e == true);
}
