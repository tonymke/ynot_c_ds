# ynot\_c\_ds

A data structures library written in C written as a refresher on the language.

Implemented in pure C90 ("ANSI C").

This project uses the GNU build system ("Autotools"), and has been validated to
work in the latest official releases of the following OSs as of this writing:
Arch Linux, Debian, FreeBSD 13.1, macOS 12, OpenBSD 7.1, and Windows 11 (via
[msys2](https://www.msys2.org/)'s [mingw-w64](https://www.mingw-w64.org/)
environment).

## Building and Testing

If building from source control, you will need a POSIX environment and the
autotools to generate a configure script via `autoreconf -fi`
[[1]](https://www.gnu.org/software/autoconf/manual/autoconf-2.68/html_node/autoreconf-Invocation.html).
Consult your operating system vendor for instructions on getting the autotools
installed for your system.

1. Configure your build environment with `./configure`
	* see `./configure --help` for additional configuration options
	* The provided `./maintainer_configure` script configures the project with
	  options suitable for hacking on the project (strict C90 conformance,
	  enable all warnings, warnings as errors, etc).
2. Compile the project with `make`
3. Run the tests with `make check`
4. Install the library with `make install`
	* You will need permission to write to your configured prefix directory.
      You probably want to `sudo make install`.

## Data structures offered

- list (doubly linked list)
- array (dynamic array)
- map (hashmap)
- queue (length-bound doubly linked list)
- pqueue (priority queue - max heap)
- ptrie (prefix trie - radix tree)

See the public [header file](src/ynot_c_ds.h) for function prototypes.

## License

[MIT](LICENSE.txt)
