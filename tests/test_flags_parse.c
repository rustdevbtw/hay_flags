#include <assert.h>
#include <errno.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"./test", "-Vr", "--port", "3000", "-d", "src", "-p", "4000"};
  int argc = 8;

  flag_t *port = hay_flags_create("port", FT_INT, 'p');
  flag_t *dir = hay_flags_create("dir", FT_STR, 'd');
  flag_t *repl = hay_flags_create("repl", FT_NULL, 'R');
  flag_t *verbose = hay_flags_create("verbose", FT_NULL, 'V');

  flag_t *flags[] = {port, dir, repl, verbose};
  int res = hay_flags_parse(flags, argc, argv);

  assert(errno == 0);
  assert(res == 0);
}
