# Visual Studio 2017 Project Template
Allows to easily create a new VS project for each day's programming puzzle, which can then be collected in a VS solution.
The template contains some placeholder files, and three configurations:  
- The Debug and Relase configurations are standard, and are used to run the application.  
- The Test configuration is used for running the unit tests, because in the other configurations they are not discovered.

## To use the template:
1. Go into the AdventOfCode folder and add the contents to a zip file called AdventOfCode.zip
2. Move AdventOfCode.zip to the root VS ProjectTemplates directory, its default path is:
`C:\Users\<Your username>\Documents\Visual Studio 2017\Templates\ProjectTemplates`
3. Create a new project from VS, with the template "AdventOfCode" that is now available to select
4. Copy the contents of the "Starting files" folder to the root of the newly created project  
At this point, from inside VS, you should now be able to open the placeholder files in the newly created project:  
DayX-PuzzleName.h, DayX-PuzzleName.cpp and test_DayX-PuzzleName.cpp  
The Debug and Release configurations in the new project should build and run.  
Both of the placeholder unit tests should be discovered and passing when the tests are run from VS in the Test configuration.  
5. Rename the files in VS to correspond to the given day's puzzle
6. Use the TODOs in the code as guidance on how to use the skeleton files