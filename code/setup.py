from distutils.core import setup, Extension
from Cython.Build import cythonize


compileMod = ["database.pyx", 
              "code/classes/menu.cpp",
              "code/classes/strings.cpp",
              "code/classes/io.cpp",
              "code/classes/datastructures/subclasses.cpp",
              "code/classes/datastructures/game.cpp",
              "code/classes/datastructures/trees/patricia.cpp",
              "code/classes/datastructures/trees/b+.cpp",
              "code/classes/datastructures/tables/hash.cpp"]
ext_modules = cythonize([
    Extension("database", compileMod, language='c++', extra_compile_args=["/std:c++20"])])

setup(ext_modules=ext_modules)
