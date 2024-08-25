#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"./test", "-Vr", "--port", "3000", "-d", "src", "-p", "4000"};
  int argc = 8;

  flag_t *port = hay_flags_create("port", FT_INT, 'p');
  flag_t *dir = hay_flags_create("dir", FT_STR, 'd');
  flag_t *repl = hay_flags_create("repl", FT_NULL, 'R');
  flag_t *verbose = hay_flags_create("verbose", FT_NULL, 'V');

  assert(port != nullptr);

  const char *d = hay_flags_getstr(dir, "./");

  assert(strcmp(d, "./") == 0);
}
