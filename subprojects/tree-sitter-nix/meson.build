# https://mesonbuild.com/Meson-sample.html
# https://mesonbuild.com/Dependencies.html
# https://github.com/kstenerud/meson-examples/blob/master/executable/meson.build
# https://github.com/tiernemi/meson-sample-project/blob/master/meson.build
# https://github.com/rizinorg/rizin/blob/dev/subprojects/packagefiles/tree-sitter-c/meson.build
# https://github.com/nix-community/tree-sitter-nix

project(
  'tree-sitter-nix',
  'c',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'buildtype=debugoptimized',
  ],
)

tree_sitter_dep = dependency('tree-sitter')

tree_sitter_nix_lib = static_library(
  'tree-sitter-nix',
  [
    'src/tree-sitter-nix/src/parser.c',
    # fix: undefined reference to tree_sitter_nix_external_scanner_create
    'src/tree-sitter-nix/src/scanner.c',
  ],
  include_directories: ['src/tree-sitter-nix/src'],
  implicit_include_directories: false,
  dependencies: tree_sitter_dep.partial_dependency(includes: true),
  c_args: [
    # the "payload" parameter is not used in src/tree-sitter-nix/src/scanner.c
    '-Wno-unused-parameter',
    # the "eof" variable is not used in src/tree-sitter-nix/srctree_sitter/parser.h
    '-Wno-unused-but-set-variable',
  ],
)

tree_sitter_nix_dep = declare_dependency(
  link_with: tree_sitter_nix_lib,
  include_directories: ['src/tree-sitter-nix/src/tree_sitter'],
  dependencies: tree_sitter_dep
)
