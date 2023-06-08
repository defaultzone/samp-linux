# SA:MP Client for Linux based on GTK4

A client for San Andreas: Multiplayer modification for Linux built on GTK4.

### Build / Installation / Execution
> **To build** application, execute following in your terminal. You must
> install `cmake` to properly build or install the application.
```shell
# Note: in all cases, you must be in the repository directory.
cmake -B cmake-build .
cmake --build cmake-build/
# Compiled file will be in cmake-build/samp-linux directory.
```
> **Installation**
```shell
cmake -B cmake-build .
cmake --build cmake-build/
sudo cmake --install cmake-build/
```
> **Execution:** enter below command in terminal or launch the `SA:MP Linux` application from your application selection menu.
```shell
samp-linux
```
