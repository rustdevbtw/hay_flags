#include <assert.h>
#include <hay/flags.h>

int main() {
  char *argv[] = {"hello", "-p", "3000", "--dir", "src/"};
  int argc = 5;

  flag_t flags[] = {{"-p"}, {"--port"}, {"--dir"}, {"-d"}};

  void *r = hay_flags_parse(flags, argc, argv);

  assert(r != nullptr);
  int i0 = hay_flags_get_idx(flags, "-p");
  assert(i0 == 2);
  int i1 = hay_flags_get_idx(flags, "--dir");
  assert(i1 == 4);

  return 0;
}
