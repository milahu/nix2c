set -x
./build/nix2c -f input-file.nix  >input-file.nix.c
gcc -O0 -g -shared input-file.nix.c -o input-file.nix.so
./build/loader $(readlink -f input-file.nix.so)
