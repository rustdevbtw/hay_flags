#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"hello", "-p", "3000"};
  int argc = 3;

  bool vc = false;
  flag_t flags[] = {
      {"-p"}, {"--port"}, {"--dir"}, {"-d"}, {"--verbose", nullptr, &vc}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != nullptr);
  bool c = hay_flags_get_check(flags, "--verbose");
  assert(c == false);
  assert(vc == false);

  return 0;
}
