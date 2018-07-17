# Building

#### Prerequisites:
* C++ compiler (MinGW, Linux GCC tested)
* [Box2D](https://github.com/erincatto/Box2D) library.
* Either one of these libraries:

   * [Allegro 5.0/5.2](http://liballeg.org/);
   * [SDL 1.2](https://www.libsdl.org/);
   * [SDL 2.0](https://www.libsdl.org/);
   * [SFML 1.5/1.6](https://www.sfml-dev.org/)(very experimental);
   * [SFML 2.x](https://www.sfml-dev.org/)(very experimental);
   * (or a custom [fgeal](https://gitlab.com/hydren/fgeal.git) implementation)

# How to build

Assuming you are on Linux, you can build it using 'make':

* If you don't have 'make' installed, install it.
* Open the project folder.
* Copy one of the folders inside `/build` - the choice depends on the library to be used as backend.
* Paster the folder in the root of the folder where the project is located.
* Open a terminal and navigate to the copied folder.
* Execute the configure script: `./configure.sh`.
* If the previous script ended ok, build using the makefile: `make` 

> Make sure that the submodules are included in the source folder (currently [futil](https://gitlab.com/hydren/futil.git), [fgeal](https://gitlab.com/hydren/fgeal.git) and [rapidxml](https://github.com/hydren/rapidxml.git), all in the `src_libs` folder). If you cloned the repository without the `--recursive` option, they are probably absent.

### Dependencies

The `Box2D` library needs to be properly installed on your compiler. Head to https://github.com/erincatto/Box2D to find instructions about how to build it or grab a binary.

##### Backend library dependencies
* To use Allegro 5.0 as backend, besides the main library, the following Allegro 5.0 addons are required: 

   * Image I/O addon;
   * Font addon;
   * TTF addon;
   * Primitives addon;
   * Audio addon;
   * Audio codecs addon;
      
* To use SDL 1.2 as backend, besides the main library, the following extension libraries are required:

   * SDL_image
   * SDL_ttf
   * SDL_gfx
   * SDL_mixer
      
* To use SDL 2.0 as backend, besides the main library, the following extension libraries are required:

   * SDL2_image
   * SDL2_ttf
   * SDL2_gfx
   * SDL2_mixer 
      
* To use SFML as backend (experimental), all main SFML libraries are required:

   * sfml-system
   * sfml-window
   * sfml-graphics
   * sfml-audio 

### Testing the executable
Run the program by executing the produced binary in the same folder where the `resources` folder is located: `./<your-copied-folder>/terrarium`.
> The `resources/` folder must be in the same folder as the executable to work.

> Also the required libraries must be accessible, either in the system's Path variable or in the same folder. 

> A quick solution is to copy `Box2D.dll`/`libBox2D.so` and the backend libraries to the executable folder and execute with `LD_LIBRARY_PATH=./<your-copied-folder> ./<your-copied-folder>/terrarium`.

# Eclipse project
You can also use eclipse to build the project (tested to work with MinGW and Linux GCC). Here is how:

##### Prerequisites for eclipse:
- Get the Eclipse IDE for C/C++ Developers. Link: https://www.eclipse.org/downloads/packages/
- When using MinGW, make sure the g++ program is on the OS path variable. On Windows you need to add the `.../MinGW/bin` directory to your PATH variable.

### Instructions:
- Clone the code from [this](https://gitlab.com/hydren/terrarium.git) repository (make sure to use the `--recursive` options)
- If you simply downloaded, you'll probably need to download the submodules as well ([futil](https://gitlab.com/hydren/futil.git), [fgeal](https://gitlab.com/hydren/fgeal.git) and [rapidxml](https://github.com/hydren/rapidxml.git))
- Import the project using the [Import Projects wizard](http://help.eclipse.org/kepler/index.jsp?topic=%2Forg.eclipse.platform.doc.user%2Ftasks%2Ftasks-importproject.htm).
- If you want to clone from Eclipse, select workspace and on the projects pane left-click and select `Import -> Projects from git`. Choose `Clone URI`. On the URI field, paste the repository address https://gitlab.com/hydren/terrarium.git. Click next and the choose `master` and next. Wait for the download and then select `Import existing projects` and then next and finish. *Note that this method does not download the submodules automatically*.  
- Left-click project and select build configuration according to your environment (i.e. `linux-gcc-sdl1.2-release` for *Linux* with *SDL 1.2* backend, `win32-mingw-allegro5.0-release` for *Windows* with *Allegro 5.0* backend) and then build.
- Any problems indicates that something probably went wrong with previous steps (missing/wrongly installed libraries, missing/wrongly installed submodules)
- If something wents wrong because of some cross-gcc stuff, install the `C/C++ GCC Cross Compiler Support` plugin on Eclipse via `Install new software` option on Eclipse.

### Testing the executable:
- Run the program by left clicking on the executable (created inside the build folder - usually `/linux-gcc-<backend>-release/`, `/win32-mingw-<backend>-release/`, etc) and selecting `Run as application`. 
- Alternatively, open the project folder using your file explorer and run directly the executable inside the build folder. Note that this way requires that all **required DLLs** be present **on the build folder or in the system's path**.
