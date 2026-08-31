harmony_sources = [
  'main.c',
  'harmony-application.c',
  'harmony-window.c',
  'hsla.c'
]

math_dep = meson.get_compiler('c').find_library('m', required: true)

harmony_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1', version: '>= 1.4'),
  math_dep,
]

harmony_sources += gnome.compile_resources('harmony-resources',
  'harmony.gresource.xml',
  c_name: 'harmony'
)

executable('harmony', harmony_sources,
  dependencies: harmony_deps,
       install: true,
)
