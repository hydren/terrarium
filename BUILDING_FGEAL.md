# Building using a custom fgeal implementation

Its possible to build the project using a custom [fgeal](https://gitlab.com/hydren/fgeal) implementation. To so, follow the process detailed in [BUILDING.md](https://gitlab.com/hydren/terrarium/blob/master/BUILDING.md), but with the following differences:

- Use the `build/others/linux-gcc-libfgeal-release` folder instead of the other builds.
- Besides `Box2D`, the `fgeal` library also needs to be present. It is a wrapper library around other game library (i.e. SDL). Head to https://gitlab.com/hydren/fgeal to find instructions about how to build it or grab a binary.
- When running the `configure.sh` script, the script will ask about a `fgeal` include folder. Type in were its headers are located in your machine (maybe in a `.../fgeal` directory or `/usr/include`).
* It will also ask where the `fgeal` library are located. So type in as well were the library are located in your machine (again, maybe in `.../fgeal` or `/usr/lib/x86_64-linux-gnu`, but it could be anywere, really)
