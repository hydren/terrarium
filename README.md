# terrarium
An attempt to do a Terraria clone game, with some new features.

It is still in the first stages, so its pretty dumb.

It's coded in C++, using Allegro 5 and Box2D. Currently we're using Eclipse IDE with its CDT plugin. Previously we have been using CodeLite and QtCreator shortly after, so there are branches using their project layout (but the code is mostly oudated).

# How to build

Unfortunately, I am pretty lay on using makefiles. Here is how to build using our way:

Prerequisites:
- Get the Eclipse IDE for C/C++ Developers. Link: https://www.eclipse.org/downloads/packages/
- Get a C++ compiler. This tutorial assumes GCC/MinGW. Link for MinGW: http://sourceforge.net/projects/mingw/files/
- Get Allegro 5 on your compiler. Either build or download packages. Link: https://www.allegro.cc/files/ or http://alleg.sourceforge.net/download.html. On linux, download the packages from your distro or compile from sources.
- Get Box2D on your compiler. This one you have to build. Link: https://code.google.com/p/box2d/downloads/list. I have built a MinGW binary here http://www.dcc.ufrj.br/~carlosfaruolo/arquivos/Box2D-devel-2.3.0-mingw32.zip, but this link sometimes gets broken.
- When using gcc/MinGW, make sure the g++ program is on the OS path variable. On linux this is almost always true, but on Windows you need to add the /bin directory to your PATH variable.

Building:
- Open Eclipse, select workspace and on the projects pane left-click and select Import -> Projects from git. Choose URI. On the URI field, paste the github adress https://github.com/hydren/terrarium.git , then your account and password. Click next and the choose 'master' (recommended) and next. Wait for the download and then select "import existing projects" and then next and finish.
- Left-click project and select build configuration according to your environment (Linux, Windows).
- Any problems indicates that something probably went wrong with previous steps (missing/wrongly installed Allegro 5, missing/wrongly installed Box2D, etc)
- (Optional) If something wents wrong because of some cross-gcc stuff, install the "C/C++ GCC Cross Compiler Support" plugin on Eclipse via "Install new software" functionality on Eclipse. But it shouldn't be needed.

Running:
- Run the program left clicking on the executable (created inside the build folder - usually "/Debug Linux/", "/Debug Windows/", etc) and selecting run as application. 
- Alternatively, open the project folder using your file explorer and run directly the executable inside the build folder. Keep in mind that the executable must run in the same folder as the "resources" folder. You can also copy the executable to the project root to run it, though it pollutes your project folder.
