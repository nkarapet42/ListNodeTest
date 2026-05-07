## Usage

### Build the project
```bash
make
```
Compiles the source code and builds the executable.

### Run the program
```bash
./ListComparator
```

### Clean object files
```bash
make clean
```
Deletes compiled object files from the build.

### Full clean
```bash
make fclean
```
Deletes the executable and calls `make clean` to remove object files.

### File clean
```bash
make fileclean
```
Deletes all generated files and calls `make fclean`.

### Diff comparison
```bash
make diff
```
Builds the project if it's not builded and compares the given data with restored data.

### Rebuild
```bash
make re
```
Performs a complete recompilation (equivalent to `make fclean` followed by `make`).
