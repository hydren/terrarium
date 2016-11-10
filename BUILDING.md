# Building

#### Prerequisites:
* C++ compiler (MinGW, Linux GCC tested)
* [Box2D](https://github.com/erincatto/Box2D) library.
* [fgeal](https://github.com/hydren/fgeal) library.

# How to build

Assuming you are on Linux, you can build it using 'make':

* If you don't have 'make' installed, install it.
* Open a shell, go to `terrarium/build`.
* Execute the configure script: `./configure.sh`.
* The script will ask about a `fgeal` include folder. Type-in were its headers are located in your machine (maybe in a `.../fgeal/src` directory or `/usr/include`).
* It will also ask about where `fgeal` library are located. So type-in as well were the library are located in your machine (again, maybe in `.../fgeal` or `/usr/lib/x86_64-linux-gnu`, but it could be anywere, really)
* If the previous script ended ok, build using the makefile: `make` 

> The `Box2D` library needs to be properly installed on your compiler. Head to https://github.com/erincatto/Box2D to find instructions about how to build it or grab a binary.
> The `fgeal` library also needs to be present. It is a wrapper library around other game library, like Allegro. Head to https://github.com/hydren/fgeal to find instructions about how to build it or grab a binary.

##### Testing the executable
Run the program it by executing: `./build/terrarium`
> The `resources/` folder must be in the same folder as the executable to work.
> Also the required libraries must be accessible, either in the system's Path variable or in the same folder. A quick solution is to copy both `Box2D.dll`/`libBox2D.so` and `libfgeal.dll`/`libfgeal.so` to the executable folder and execute with `LD_LIBRARY_PATH=. ./build/terrarium`.

#### Eclipse project
You can also use eclipse to build the project (tested to work with MinGW and Linux GCC). Here is how:

##### Prerequisites for eclipse:
- Get the Eclipse IDE for C/C++ Developers. Link: https://www.eclipse.org/downloads/packages/
- When using MinGW, make sure the g++ program is on the OS path variable. On Windows you need to add the `.../MinGW/bin` directory to your PATH variable.

##### Instructions:
- Clone the code from [this](https://github.com/hydren/terrarium.git) repository (make sure to use the `--recursive` options)
- If you simply downloaded, you'll need to download the submodules as well (https://github.com/hydren/futil.git)
- Import the project using the [Import Projects wizard](http://help.eclipse.org/kepler/index.jsp?topic=%2Forg.eclipse.platform.doc.user%2Ftasks%2Ftasks-importproject.htm).
- If you want to clone from Eclipse, select workspace and on the projects pane left-click and select `Import -> Projects from git`. Choose `Clone URI`. On the URI field, paste the github adress https://github.com/hydren/terrarium.git. Click next and the choose `master` and next. Wait for the download and then select `Import existing projects` and then next and finish.
- Left-click project and select build configuration according to your environment (i.e. `linux-gcc-release` for *Linux*, `win32-mingw-release` for *Windows*) and then build.
- Any problems indicates that something probably went wrong with previous steps (missing/wrongly installed libraries, missing/wrongly installed submodules)
- If something wents wrong because of some cross-gcc stuff, install the `C/C++ GCC Cross Compiler Support` plugin on Eclipse via `Install new software` option on Eclipse.

##### Testing the executable:
- Run the program by left clicking on the executable (created inside the build folder - usually `/linux-gcc-release/`, `/win32-mingw-release/`, etc) and selecting `Run as application`. 
- Alternatively, open the project folder using your file explorer and run directly the executable inside the build folder. Note that this way requires that all **required DLLs** be present **on the build folder or in the system's path**.
