# HOW TO USE

1. You must have a windows machine with [gcc](https://sourceforge.net/projects/mingw-w64/) installed.

2. Clone the project.
```
git clone https://github.com/Silme94/Basic-Dll-Injector
cd Basic-Dll-Injector
```
3. Compile the source code using Makefile.
```
cd bin
mingw32-make
```
4. Get your own dll or use the dll example.

5. Run the command : 
```
Inject.exe <DLL> <TARGET>
```
Example command :
```
Inject.exe evil.dll nodepad.exe
```
