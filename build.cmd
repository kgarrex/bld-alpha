@echo off

cl /c .\src\IClass.c .\src\ICompiler.c .\src\ILinker.c .\src\VisualCCompiler.c .\src\VisualCLinker.c .\src\File.c .\src\IFileSystem.c .\src\layerdb.c .\src\layerdb_sqlite.c .\data\MasterDb.c .\src\process.c

cl  bld.c /link ..\lib-dilly\dilly.obj .\VisualCCompiler.obj .\VisualCLinker.obj .\ICompiler.obj .\IClass.obj .\ILinker.obj .\File.obj .\IFileSystem.obj .\layerdb.obj .\layerdb_sqlite.obj .\MasterDb.obj .\process.obj

move .\dilly.obj           .\obj
move .\VisualCCompiler.obj .\obj
move .\VisualCLinker.obj   .\obj
move .\ICompiler.obj       .\obj
move .\IClass.obj          .\obj
move .\ILinker.obj         .\obj
move .\File.obj            .\obj
move .\IFileSystem.obj     .\obj
move .\layerdb.obj         .\obj
move .\layerdb_sqlite.obj  .\obj
move .\MasterDb.obj        .\obj
move .\bld.obj             .\obj
move .\process.obj         .\obj
echo.Build Succeeded
echo.
::call bld
