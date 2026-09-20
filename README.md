# How to use codebase:
Each task should be contained in at least one file

All code files must be placed in the `main` folder

All files except `main` should be .h (header) files. This is because .ino files are compiled together in alphabetical order, so dependant files are not necesarrily visible to their depending files. Instead, #include "dependant_file.h" should be used at the top of a file.

Git can be managed from the command line or by opening the project in vscode

**NEVER COMMIT BROKEN CODE! ALWAYS CHECK YOUR CODE WORKS BEFORE YOU COMMIT**

If you know how to use git branches it is preferred for you to work in a dedicated branch for each task and merge into main when finished, to avoid file conflicts