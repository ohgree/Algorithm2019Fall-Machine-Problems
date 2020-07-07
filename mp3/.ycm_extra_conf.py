import os
import re
import subprocess
import ycm_core

def LoadSystemIncludes():
    regex = re.compile(ur'(?:\#include \<...\> search starts here\:)(?P<list>.*?)(?:End of search list)', re.DOTALL);
    process = subprocess.Popen(['clang', '-v', '-E', '-x', 'c++', '-'], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE);
    process_out, process_err = process.communicate('');
    output = process_out + process_err;
    includes = [];
    for p in re.search(regex, output).group('list').split('\n'):
        p = p.strip();
        if len(p) > 0 and p.find('(framework directory)') < 0:
            includes.append('-isystem');
            includes.append(p);
    return includes;

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]
scriptPath = os.path.dirname(os.path.abspath(__file__));
compilation_database_folder = os.path.join(scriptPath, 'build')
database = None if not os.path.exists(compilation_database_folder) else ycm_core.CompilationDatabase(compilation_database_folder)

# Your custom flags
flags = [
    '-Wall',
    '-Wextra',
    '-std=c++14',
    '-x',
    'c++',
    '-I',
    '.',
]

# Add system includes
systemIncludes = LoadSystemIncludes();
flags = flags + systemIncludes;

# Set this to the absolute path to the folder (NOT the file!) containing the
# compile_commands.json file to use that instead of 'flags'. See here for
# more details: http://clang.llvm.org/docs/JSONCompilationDatabase.html
#
# You can get CMake to generate this file for you by adding:
#   set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
# to your CMakeLists.txt file.
#
# Most projects will NOT need to set this to anything; you can just change the
# 'flags' list of compilation flags. Notice that YCM itself uses that approach.
compilation_database_folder = ''

if os.path.exists( compilation_database_folder ):
  database = ycm_core.CompilationDatabase( compilation_database_folder )
else:
  database = None

SOURCE_EXTENSIONS = [ '.cpp', '.cxx', '.cc', '.c', '.m', '.mm' ]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return list( flags )
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def IsHeaderFile( filename ):
  extension = os.path.splitext( filename )[ 1 ]
  return extension in [ '.h', '.hxx', '.hpp', '.hh' ]


def GetCompilationInfoForFile( filename ):
  # The compilation_commands.json file generated by CMake does not have entries
  # for header files. So we do our best by asking the db for flags for a
  # corresponding source file, if any. If one exists, the flags for that file
  # should be good enough.
  if IsHeaderFile( filename ):
    basename = os.path.splitext( filename )[ 0 ]
    for extension in SOURCE_EXTENSIONS:
      replacement_file = basename + extension
      if os.path.exists( replacement_file ):
        compilation_info = database.GetCompilationInfoForFile(
          replacement_file )
        if compilation_info.compiler_flags_:
          return compilation_info
    return None
  return database.GetCompilationInfoForFile( filename )


def Settings( filename, **kwargs ):
  if database:
    # Bear in mind that compilation_info.compiler_flags_ does NOT return a
    # python list, but a "list-like" StringVec object
    compilation_info = GetCompilationInfoForFile( filename )
    if not compilation_info:
      return None

    final_flags = MakeRelativePathsInFlagsAbsolute(
      compilation_info.compiler_flags_,
      compilation_info.compiler_working_dir_ )

  else:
    relative_to = DirectoryOfThisScript()
    final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }

