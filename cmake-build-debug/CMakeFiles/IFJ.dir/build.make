# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\JetBrains\Toolbox\apps\CLion\ch-0\202.7660.37\bin\cmake\win\bin\cmake.exe

# The command to remove a file.
RM = C:\JetBrains\Toolbox\apps\CLion\ch-0\202.7660.37\bin\cmake\win\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\CLionProjects\School\IFJ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\CLionProjects\School\IFJ\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles\IFJ.dir\depend.make

# Include the progress variables for this target.
include CMakeFiles\IFJ.dir\progress.make

# Include the compile flags for this target's objects.
include CMakeFiles\IFJ.dir\flags.make

CMakeFiles\IFJ.dir\main.c.obj: CMakeFiles\IFJ.dir\flags.make
CMakeFiles\IFJ.dir\main.c.obj: ..\main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/IFJ.dir/main.c.obj"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\IFJ.dir\main.c.obj /FdCMakeFiles\IFJ.dir\ /FS -c C:\CLionProjects\School\IFJ\main.c
<<

CMakeFiles\IFJ.dir\main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ.dir/main.c.i"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\IFJ.dir\main.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\CLionProjects\School\IFJ\main.c
<<

CMakeFiles\IFJ.dir\main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ.dir/main.c.s"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\IFJ.dir\main.c.s /c C:\CLionProjects\School\IFJ\main.c
<<

CMakeFiles\IFJ.dir\scanner.c.obj: CMakeFiles\IFJ.dir\flags.make
CMakeFiles\IFJ.dir\scanner.c.obj: ..\scanner.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/IFJ.dir/scanner.c.obj"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\IFJ.dir\scanner.c.obj /FdCMakeFiles\IFJ.dir\ /FS -c C:\CLionProjects\School\IFJ\scanner.c
<<

CMakeFiles\IFJ.dir\scanner.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ.dir/scanner.c.i"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\IFJ.dir\scanner.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\CLionProjects\School\IFJ\scanner.c
<<

CMakeFiles\IFJ.dir\scanner.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ.dir/scanner.c.s"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\IFJ.dir\scanner.c.s /c C:\CLionProjects\School\IFJ\scanner.c
<<

CMakeFiles\IFJ.dir\parser.c.obj: CMakeFiles\IFJ.dir\flags.make
CMakeFiles\IFJ.dir\parser.c.obj: ..\parser.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/IFJ.dir/parser.c.obj"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\IFJ.dir\parser.c.obj /FdCMakeFiles\IFJ.dir\ /FS -c C:\CLionProjects\School\IFJ\parser.c
<<

CMakeFiles\IFJ.dir\parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ.dir/parser.c.i"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\IFJ.dir\parser.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\CLionProjects\School\IFJ\parser.c
<<

CMakeFiles\IFJ.dir\parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ.dir/parser.c.s"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\IFJ.dir\parser.c.s /c C:\CLionProjects\School\IFJ\parser.c
<<

CMakeFiles\IFJ.dir\symtable.c.obj: CMakeFiles\IFJ.dir\flags.make
CMakeFiles\IFJ.dir\symtable.c.obj: ..\symtable.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/IFJ.dir/symtable.c.obj"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoCMakeFiles\IFJ.dir\symtable.c.obj /FdCMakeFiles\IFJ.dir\ /FS -c C:\CLionProjects\School\IFJ\symtable.c
<<

CMakeFiles\IFJ.dir\symtable.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/IFJ.dir/symtable.c.i"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe > CMakeFiles\IFJ.dir\symtable.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\CLionProjects\School\IFJ\symtable.c
<<

CMakeFiles\IFJ.dir\symtable.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/IFJ.dir/symtable.c.s"
	C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\IFJ.dir\symtable.c.s /c C:\CLionProjects\School\IFJ\symtable.c
<<

# Object files for target IFJ
IFJ_OBJECTS = \
"CMakeFiles\IFJ.dir\main.c.obj" \
"CMakeFiles\IFJ.dir\scanner.c.obj" \
"CMakeFiles\IFJ.dir\parser.c.obj" \
"CMakeFiles\IFJ.dir\symtable.c.obj"

# External object files for target IFJ
IFJ_EXTERNAL_OBJECTS =

IFJ.exe: CMakeFiles\IFJ.dir\main.c.obj
IFJ.exe: CMakeFiles\IFJ.dir\scanner.c.obj
IFJ.exe: CMakeFiles\IFJ.dir\parser.c.obj
IFJ.exe: CMakeFiles\IFJ.dir\symtable.c.obj
IFJ.exe: CMakeFiles\IFJ.dir\build.make
IFJ.exe: CMakeFiles\IFJ.dir\objects1.rsp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable IFJ.exe"
	C:\JetBrains\Toolbox\apps\CLion\ch-0\202.7660.37\bin\cmake\win\bin\cmake.exe -E vs_link_exe --intdir=CMakeFiles\IFJ.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100183~1.0\x86\mt.exe --manifests  -- C:\PROGRA~2\MICROS~3\2019\COMMUN~1\VC\Tools\MSVC\1427~1.291\bin\Hostx86\x86\link.exe /nologo @CMakeFiles\IFJ.dir\objects1.rsp @<<
 /out:IFJ.exe /implib:IFJ.lib /pdb:C:\CLionProjects\School\IFJ\cmake-build-debug\IFJ.pdb /version:0.0  /machine:X86 /debug /INCREMENTAL /subsystem:console  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib 
<<

# Rule to build all files generated by this target.
CMakeFiles\IFJ.dir\build: IFJ.exe

.PHONY : CMakeFiles\IFJ.dir\build

CMakeFiles\IFJ.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\IFJ.dir\cmake_clean.cmake
.PHONY : CMakeFiles\IFJ.dir\clean

CMakeFiles\IFJ.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" C:\CLionProjects\School\IFJ C:\CLionProjects\School\IFJ C:\CLionProjects\School\IFJ\cmake-build-debug C:\CLionProjects\School\IFJ\cmake-build-debug C:\CLionProjects\School\IFJ\cmake-build-debug\CMakeFiles\IFJ.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\IFJ.dir\depend

