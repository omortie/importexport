**Preface**

This is my very first project using C++/Qt that includes 2 separated modules for importing and exporting data 
from/to database/file. it was a part of a bigger ERP system and supports very basic databases and file types.

***Supported File Types***

- [x] CSV

- [x] TXT

- [ ] PDF

- [ ] XLSX

**Notes**

There is a support for Python scripting (with `BUILD_WITH_SCRIPT_SUPPORT` 
and [`PythonQt`](https://mevislab.github.io/pythonqt/)) which examples are not included in this repository

***TODOs***

- [x] Ported to Qt6
- [ ] Restructure CMake to avoid compiler warnings (use AUTOMOC and AUTOUIC etc.)
- [x] Use a better logger library (Qt logging framework)
- [ ] Use Shared library
- [ ] Remove QMake files
- [ ] Write CI build scripts
- [ ] Write Unit tests

