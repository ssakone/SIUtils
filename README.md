# SIUtils
Provide Api to use many Qt/C++ function in qml

#### INSTALLATION

- In your project CMakeFiles.txt
```
...
add_subdirectory(SIUtils)

...
target_link_libraries(... SIUtils)```

- In your main c++ file
```
#include <utils.h>
...

Utils::init();
```

- In your main qml file
```
import SIUtils 1.0

Utils.qtVersion()
```
#### FEATURES

| Qt/C++ function           | Qml Call                  |
| ------------------------- | ------------------------- |
| QFile read text from file | SIUtils.readText(path)    |
| QPdfWriter Image to PDF | SIUtils.imageToPdf([im1,im2], outputPdf, callback)    |


#### COPYRIGHT
Many Options here was extracted from: https://github.com/Aseman-Land/QtAseman Core.
