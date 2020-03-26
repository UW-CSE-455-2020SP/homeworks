# Welcome to CSE 455 (Computer Vision) Spring 2020!

In this repository you will find instructions on how to build your own image processing/computer vision library from (mostly) scratch. The work is divided out into different homework assignments, found in the `src/` directory.

To get started, make sure you have `git`, a C compiler, and `make` installed. Then run:

```
git clone https://github.com/UW-CSE-455-2020SP/homeworks
cd homeworks
make
```

and check to see that everything compiles correctly. We recommend using Linux or MacOS for the homeworks since installing `make` is easier. Linux uses GNU C++ compiler, while MacOS uses XCode C++ compiler. 

To get started, open up the README for homework 0 in src/hw0/README.md, or view it [here](src/hw0/README.md). Good luck and have fun!


## Instructions for Mac Users
In MacOS, make sure you have the latest version of Xcode and perform `xcode-select --install`. 
If `make` still results in an error, try [this](https://github.com/frida/frida/issues/338#issuecomment-426777849) solution.

## Instructions for Windows Users
We do **NOT** recommend using Windows OS for this assignment because C++ compilation is more complex under the Windows environment. However, if you only have Windows computers available, you can still manage your Python packages, C++ compiler, and Makefile with Anaconda.

Installation Steps:
1. Download [Anaconda](https://www.anaconda.com/distribution/) with Python 3.6+
2. Install Anaconda with "admin" rights: PLEASE select "All Users (requires admin privileges)", "Add Anaconda to the system PATH environment variable", and "Register Anaconda as the system Python 3.x".
3. Download and install Git from [here](https://git-scm.com/download/win). This installation includes MinGW so that you can use Makefile easier in Windows.
4. Restart your machine.
5. Now, you can follow the same steps as Mac/Linux users. Remember to use the MinGW terminal if you have trouble running any of the above commands.

