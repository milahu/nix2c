# https://mesonbuild.com/Meson-sample.html
# https://mesonbuild.com/Dependencies.html
# https://github.com/kstenerud/meson-examples/blob/master/executable/meson.build
# https://github.com/tiernemi/meson-sample-project/blob/master/meson.build
# https://github.com/rizinorg/rizin/blob/dev/subprojects/packagefiles/tree-sitter-c/meson.build

project(
  'nix2c',
  'c',
  version: '0.0.1',
  license: 'MIT',
  default_options: [
    'warning_level=3',
    'buildtype=debugoptimized',
  ],
)

#tree_sitter_dep = dependency('tree-sitter')

tree_sitter_nix_proj = subproject('tree-sitter-nix')
tree_sitter_nix_dep = tree_sitter_nix_proj.get_variable('tree_sitter_nix_dep')

build_args = [
  '-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
  # add debug symbols
  '-g',
  # disable optimizations
  '-O0',
]

executable(
  'nix2c',
  'src/nix2c.c',
  dependencies: [tree_sitter_nix_dep],
  install: true,
  c_args: build_args,
)

executable(
  'loader',
  'src/loader.c',
  install: true,
  c_args: build_args,
)
