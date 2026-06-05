# libftpp

[doxygen-doc]:
    https://www.doxygen.nl/
[graphviz-doc]:
    https://graphviz.org/

An advanced C++17 toolbox library, built as a static archive (`libftpp.a`). It
bundles reusable building blocks across data structures, design patterns,
mathematics, threading and networking.

[TOC]

## Build

```bash
make            # builds libftpp.a
make re         # clean rebuild
make clean      # remove object files
make fclean     # remove objects and the archive
```

Requires a C++17 compiler. The library is compiled with
`-Wall -Wextra -Werror -std=c++17`.

## Usage

Include the umbrella header and link against the archive:

```cpp
#include "libftpp.hpp"
```

```bash
c++ -std=c++17 your_app.cpp lib/libftpp.a -o your_app -pthread
```

Individual modules can also be included directly, e.g.
`#include "includes/network/server.hpp"`.

## Modules

| Module | Header | Contents |
| --- | --- | --- |
| Data structures | `dataStructures/data_structures.hpp` | `DataBuffer` (binary serialization), `Pool<T>` (fixed-capacity object pool) |
| Design patterns | `designPatterns/design_patterns.hpp` | `Memento`, `Observer`, `Singleton`, `StateMachine`, `ObservableValue<T>` |
| IOStream | `iostream/thread_safe_iostream.hpp` | `ThreadSafeIOStream` + the thread-local `threadSafeCout` for prefixed, thread-safe output |
| Mathematics | `mathematics/mathematics.hpp` | `IVector2`, `IVector3`, `PerlinNoise2D`, `Random2DCoordinateGenerator` |
| Threading | `thread/threading.hpp` | `Thread`, `PersistentWorker`, `WorkerPool`, `ThreadSafeQueue<T>` |
| Networking | `network/network.hpp` | `Message`, `Client`, `Server` (TCP, typed message passing) |

## Documentation

API documentation is generated with [Doxygen][doxygen-doc]
(class and include graphs require [Graphviz][graphviz-doc]).

`make` builds the docs automatically when the `docs/` directory is missing,
so a fresh build produces them once. After that they are left untouched:

```bash
make docs        # (re)generate the documentation explicitly
make docs-clean  # remove the generated docs/ directory
```

Open `docs/html/index.html` in a browser. The generated docs are not
committed. If `doxygen` is not installed, the build is not blocked - docs are
simply skipped.
