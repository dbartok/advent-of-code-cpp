# Visual Studio Project Template
The project template allows to easily create a new VS project for each day's programming puzzle. These projects can then be collected in a VS solution.

The template was (most recently) made with Visual Studio 2019.

The template contains a set of placeholder files, and three configurations:
- The Debug and Relase configurations are standard, and are used to run the application.
- The Test configuration is used for running the unit tests.

All three of these configurations are supported for both the x86 and x64 platforms.

Note: In the Visual Studio Menu, "Test -> Test Settings -> Default Processor Architecture" has to always be set to the same platform as selected for the Test configuration.

## To use the template:
1. Go into the AdventOfCode folder and add the contents to a zip file called `AdventOfCode.zip`.
2. Move `AdventOfCode.zip` to the root VS ProjectTemplates directory, its default path is:
`C:\Users\<Your username>\Documents\Visual Studio <Your VS version>\Templates\ProjectTemplates`.
3. Create a new project from VS, with the new "AdventOfCode" template that is now available to select.
4. Copy the contents of the "Starting files" folder to the root of the newly created project.
At this point, from inside VS, you should now be able to open the placeholder files in the newly created project:  
DayX-PuzzleName.h, DayX-PuzzleName.cpp, DayX-Main.cpp and test_DayX-PuzzleName.cpp  
The Debug and Release configurations in the new project should build and run.  
Both of the placeholder unit tests should be discovered and passing when the tests are run from VS in the Test configuration.  
5. Rename the files in VS to correspond to the given day's puzzle.
6. Use the TODOs in the code as guidance on how to use the skeleton files.

## Visual Studio settings
In order to help with keeping the coding style consistent, the Visual Studio C++ Text Editor settings used throughout the repository can be imported to Visual Studio from `Settings\TextEditorCPP.vssettings`.  
[Trailing Whitespace Visualizer](https://marketplace.visualstudio.com/items?itemName=MadsKristensen.TrailingWhitespaceVisualizer) is recommended with the "Remove on save" option enabled to make sure no trailing whitespaces are present in the files.

## Warnings & Code Analysis
All warnings are enabled (/Wall) along with extensive Code Analysis (Microsoft All Rules and also Core C++ Check).

### Ignoring warnings in libraries

Both the standard library and boost produce several externally unfixable warnings when compiled with /Wall.
We are also not interested in the Code Analysis warning messages for libraries.
Unfortunately, there is currently no easy way in VS to disable warning messages for all included libraries.
Therefore, whenever a header from a library is included, the include has to be surrounded with disable warning directives.

There are some convenience macros available for this in `Common/DisableLibraryWarningsMacros.h`.  
For the includes surrounded by these macros, the following warnings will be disabled:  
- Externally unfixable warnings.
- All Code Analysis warnings originating from the Microsoft All Rules analysis.
- All Core C++ Check warnings.

Example usage:  

```cpp
#include <AdventOfCodeCommon/DisableLibraryWarningsMacros.h>

BEGIN_LIBRARIES_DISABLE_WARNINGS
#include <boost/functional/hash/hash.hpp>
#include <boost/algorithm/string.hpp>

#include <vector>
#include <string>
END_LIBRARIES_DISABLE_WARNINGS
```

See the following links from Microsoft for more information:  
[How to: Enable and Disable Code Analysis for Specific C/C++ Warnings](https://msdn.microsoft.com/en-us/library/zyhb0b82(v=vs.100).aspx)  
[Managing warnings in the C++ Core Guidelines Checker](https://blogs.msdn.microsoft.com/vcblog/2017/08/14/managing-warnings-in-the-c-core-guidelines-checker/)
