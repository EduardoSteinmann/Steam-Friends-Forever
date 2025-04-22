# Steam-Friends-Forever

A CLI tool for visualizing your social network on STEAM and never running out of friends to play games with.

# How it Works
Steam Friends Forever (SFF) first asks a user what graph data structure they wish to use, and Adjaceny List (1) or an Adjacency Matrix (2).
It then allows a user to enter in the STEAM ID of a profile.
SFF will then pull in all the friends of that profile, calculate which genres/categories of games
that profile plays the most along with what friends played that category the most.

SFF mimics the style of command line folder navigation, utilizing commands such as CD to change to navigate to a certain user and ls to print out all of that user's friends.
To view all the commands available, a user may type help in the command line which will pull up the following menu:

![image](https://github.com/EduardoSteinmann/Steam-Friends-Forever/blob/main/assets/CLI_Help.png)

# Building and Running

SFF is built with C++ and currently utilizes libcurl for HTTP requests and responses and nlohmann/json for JSON response parsing. 

For ease of use, the library headers and content are already included in the project. To build the project, you must have CMAKE and an up to date C++ compiler on your system.

To then run the project, if you are on x64 Windows, insure that the DLL and .def library files in the build directory are located in the same directory as the executable.

If you are on another platform, such as MacOS or Linux, please download libcurl and place the static library files in the lib directory and the dynamic library files in the same directory
where the executable will run.

Alternatively, if you are on Windows x64, you can also download the prebuilt release and directly run the program.
