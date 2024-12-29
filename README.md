# C Project Generator (CPG)

A command-line tool for generating and managing C projects.

## Features

- Create C project structure with src, include, and lib directories
- Add and remove libraries
- Add and remove functions
- Automatic Makefile management
- Build and run functionality

## Installation

```bash
git clone https://github.com/yourusername/cpg.git
cd cpg
gcc -o cpg cpg.c
sudo cp cpg /usr/local/bin/
```

## Usage

Create a new project:
```bash
cpg MyProject
```

Add a library:
```bash
cpg addlib MyProject math
```

Add a function:
```bash
cpg addfunc MyProject math add_numbers int "int a, int b"
```

Build and run:
```bash
cpg buildrun MyProject
```

## License

MIT License

## Contributing

Pull requests are welcome. For major changes, please open an issue first.