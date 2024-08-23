#include <assert.h>
#include <hay/flags.h>
#include <string.h>

int main() {
  char *argv[] = {"hello", "-p", "3000", "--dir", "src/"};
  int argc = 5;

  flag_t flags[] = {{"-p"}, {"--port"}, {"--dir"}, {"-d"}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != nullptr);
  char *port = hay_flags_get_val(flags, "-p");
  assert(port != nullptr);
  assert(strcmp(port, "3000") == 0);
  char *dir = hay_flags_get_val(flags, "-d");
  assert(dir == nullptr);

  return 0;
}
