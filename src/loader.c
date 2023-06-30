// https://stackoverflow.com/questions/30641928/calling-function-from-shared-library-in-c

#include <stdio.h>
#include <dlfcn.h>

typedef void* (*eval_type)(void* input);

int main(int argc, char **argv) {

  if (argc != 2) return 1;

  void *lib;
  lib = dlopen(argv[1], RTLD_LAZY);
  if (!lib) {
    fprintf(stderr, "Cannot open library: %s\n", dlerror());
    return 1;
  }

  eval_type eval;

  eval = (eval_type) dlsym(lib, "eval");
  void* result = eval(NULL);

  printf("done\n");

  dlclose(lib);
  return 0;
}
