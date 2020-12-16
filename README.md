# SIUtils
Provide Api to use many C++ function throught qml

# INSTALLATION

In your project CMakeFiles.txt
```
...

add_subdirectory(SIUtils)

...

target_link_libraries(... SIUtils)

```

In your main c++ file
```
...

#include <utils.h>

...

Utils::init();

```

In your main qml file
```
import SIUtils 1.0

...

Utils.qtVersion()

```
# FEATURES



# COPYRIGHT
Many Options here was extracted from: https://github.com/Aseman-Land/QtAseman Core.
