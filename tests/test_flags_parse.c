#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"hello", "-p", "3000", "-d", "src/"};
  int argc = 5;
  char *port = nullptr;
  char *dir = nullptr;

  flag_t flags[] = {
      {"-p", &port}, {"--port", &port}, {"--dir", &dir}, {"-d", &dir}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != nullptr);
  assert(port != nullptr);
  assert(dir != nullptr);
  assert(strcmp(port, "3000") == 0);
  assert(strcmp(dir, "src/") == 0);

  return 0;
}
