cls

del *.exe

windres Resource.rc -o Resource.o

g++ -mwindows -m64 -static -Wall -Wextra Explorer.cpp ^
 ^
 ..\Classes\ArgumentListClass.cpp ^
 ..\Classes\DroppedFilesClass.cpp ^
 ..\Classes\FolderTreeViewWindowClass.cpp ^
 ..\Classes\FontClass.cpp ^
 ..\Classes\MenuClass.cpp ^
 ..\Classes\MessageClass.cpp ^
 ..\Classes\StatusBarWindowClass.cpp ^
 ..\Classes\TreeViewWindowClass.cpp ^
 ..\Classes\WindowClass.cpp ^
 ..\Classes\WindowClassClass.cpp ^
 ^
 Resource.o -o Explorer.exe
