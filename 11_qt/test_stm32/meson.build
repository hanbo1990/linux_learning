project('test_stm32', 'cpp',default_options : ['cpp_std=c++11'])

src_files = []
inc_dir = []
cross_dep = []

# Documentation: https://mesonbuild.com/Qt5-module.html
qt5 = import('qt5')
qt5dep = dependency('qt5', modules : ['Core', 'Widgets'])

translations = qt5.compile_translations(ts_files : 'test_stm32_en_150.ts', build_by_default : true)

generated_files = qt5.preprocess(moc_headers : 'mainwindow.h', ui_files : 'mainwindow.ui',
                                 dependencies: [qt5dep])

# adding source files and include dirs
src_files += files('main.cpp',
                   'mainwindow.cpp')
src_files += generated_files
inc_dir += include_directories('.')

if meson.get_cross_property('platform') == 'stm'
    sys_root_path = meson.get_cross_property('sys_root')
    inc_dir += include_directories(sys_root_path + '/usr/include/qt5/')
    inc_dir += include_directories(sys_root_path + '/usr/include/qt5/QtWidgets')

    # add external libraries
    libfreetype = meson.get_compiler('cpp').find_library('freetype')
    cross_dep += libfreetype
endif

executable( 'test_stm32',
            sources: src_files,
            include_directories: [
                inc_dir,
            ],
            dependencies : [
               qt5dep, 
               cross_dep,
            ],
            install : false )
