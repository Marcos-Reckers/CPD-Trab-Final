from distutils.core import setup, Extension
from Cython.Build import cythonize

compileMod = ["program.pyx", 
              "database.cpp",
              "classes/strings.cpp",
              "classes/io.cpp",
              "classes/datastructures/subclasses.cpp",
              "classes/datastructures/game.cpp",
              "classes/datastructures/trees/patricia.cpp",
              "classes/datastructures/trees/b+.cpp",
              "classes/datastructures/tables/hash.cpp"]
ext_modules = cythonize([
    Extension("program", compileMod, language='c++', extra_compile_args=["/std:c++20"])])

setup(ext_modules=ext_modules)
