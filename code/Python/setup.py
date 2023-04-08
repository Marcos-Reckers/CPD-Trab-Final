from distutils.core import setup, Extension
from Cython.Build import cythonize

compileMod = ["DB.pyx", 
              "../CPP/database.cpp",
              "../CPP/classes/strings.cpp",
              "../CPP/classes/io.cpp",
              "../CPP/classes/datastructures/subclasses.cpp",
              "../CPP/classes/datastructures/game.cpp",
              "../CPP/classes/datastructures/trees/patricia.cpp",
              "../CPP/classes/datastructures/tables/hash.cpp"]
ext_modules = cythonize([
    Extension("DB", compileMod, language='c++', extra_compile_args=["/std:c++20"])])

setup(name= "DB",ext_modules=ext_modules, zip_safe=False)
# run in cmd "python -m setup build_ext --inplace"