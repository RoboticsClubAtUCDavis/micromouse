[![Build Status](https://travis-ci.org/blhough/micromouse.svg?branch=master)](https://travis-ci.org/blhough/micromouse)
# micromouse

## Contributing

`dev` is the main development branch of the project. `dev` is a protected branch set to reject any commits that have not passed the automated checks performed by *Travis CI*. In order to push to `dev` you must first push a new branch with your changes to GitHub. The automated tests will run within a minute or two and if they pass you will be able to push your commits to `dev`.

#### The recommended workflow for making new features/changes is:
  1. Checkout `dev` and pull the latest changes from GitHub
    - `git checkout dev`
    - `git pull origin dev`
  2. Make a new branch for your changes
   - `git checkout -b <branch-name>`
  3. Make your changes and commits (There are many commands/options to accomplish this, these are just a few)
     - See what changes you made:
      - `git status`  - Shows the modified files
      - `git diff [<file-name>]` - Shows the modified lines
     - Stage the changes (Choose what changes will be part of the commit)
      - `git add <file-name>` - Add a specific file or pattern
      - `git add -p` - Interactively choose chunks of code to add (My favorite)
      - `git add .` - Add all changes (Use sparingly)
     - Make the commit
      - `git commit -m "<commit-message>"` - When the commit message is short
      - `git commit` - Will open a text editor to enter longer commit messages
  4. Get any new changes from `dev` (It is a good idea to do this throughout the development of your feature as well.)
    - `git pull origin dev`
  5. Push your branch to GitHub
    - `git push origin <branch-name>`
  6. Wait to see if the checks passed. If they pass, create a pull request on GitHub.
    - `GitHub` > `Pull requests` >  `New pull request` > `compare: <branch-name>` > `Create pull request` > Fill in the description > `Create pull request`
    
### Code Formatting
Keeping a consistent code format is important and reduces the occurrences of merge conflicts.

To keep a consistent style on all platforms we are using *Clang-format*.

#### If you are using Visual Studio, here is how to set it up:
- Go `To Tools` > `Extensions and Updates`
- Click on `Online` and in the top right search "clangformat"
- Click `Download`, `Install` and then `Restart Now`

##### To use Clang-format:
- With a file open, go to `Tools` and select either `Clang Format Selection (Ctrl-R, Ctrl-F)` or `Clang Format Document  (Ctrl-R, Ctrl-D)`

##### Notes:
- Do not use `Clang Format Document` if the file you are working in was not previously formatted. This will create a lot of unnecessary noise in the commits. However all existing code is now formatted so this should not be an issue. 
- It is best to either:
  - Format before making each commit 
  - Or wait until you are done adding code and have committed all your changes. Then format all your changes in a single commit at the end.
- We may reject your code if it is not properly formatted.
 


## Setting up the simulation

#### If you are using Visual Studio:
1. Download the **32-bit** version of [SFML 2.4.2](http://www.sfml-dev.org/download/sfml/2.4.2/).
2. Extract the files.
3. Place the extracted folder `SFML-2.4.2` at the root of the project folder.
4. Copy all DLL files in `\SFML-2.4.2\bin` to `\Simulation`
5. In Studio right-click the `Simulation` project and select `Set as StartUp Project`.
6. Build and run the project

#### On Linux:
1. Install SFML (for example, `sudo pacman -S sfml` on Arch Linux; `sudo apt-get install sfml` on Ubuntu)
2. Install `scons` and `gcc` the same way, if you do not already have them
3. Build with `scons`
4. Run with `./simulate`
