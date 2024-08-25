#include <assert.h>
#include <hay/flags.h>
#include <string.h>

int main() {
  char *argv[] = {"./test", "-Vr", "--port", "3000", "-d", "src", "-p", "4000"};
  int argc = 8;

  flag_t *port = hay_flags_create("port", 'p', FT_INT);
  flag_t *dir = hay_flags_create("dir", 'd', FT_STR);
  flag_t *repl = hay_flags_create("repl", 'r', FT_NULL);
  flag_t *verbose = hay_flags_create("verbose", 'V', FT_NULL);

  assert(port != nullptr);

  const char *d = hay_flags_getstr(dir, "./");

  assert(strcmp(d, "./") == 0);
}
