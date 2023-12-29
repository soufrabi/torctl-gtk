project('torctl-gtk', 'c')

gtkdep = dependency('gtk4')
vtedep = dependency('vte-2.91-gtk4')

sourcefiles=files('main.c')

executable('torctl-gtk', sourcefiles, dependencies: [gtkdep, vtedep], install: false)
