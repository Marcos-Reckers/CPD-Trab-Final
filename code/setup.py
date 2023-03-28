from distutils.core import setup, Extension
from Cython.Build import cythonize

compileMod = ["DB.pyx", 
              "database.cpp",
              "classes/strings.cpp",
              "classes/io.cpp",
              "classes/datastructures/subclasses.cpp",
              "classes/datastructures/game.cpp",
              "classes/datastructures/trees/patricia.cpp",
              "classes/datastructures/trees/b+.cpp",
              "classes/datastructures/tables/hash.cpp"]
ext_modules = cythonize([
    Extension("DB", compileMod, language='c++', extra_compile_args=["/std:c++20"])])

setup(name= "DB",ext_modules=ext_modules, zip_safe=False)
# run in cmd "python -m setup build_ext --inplace"