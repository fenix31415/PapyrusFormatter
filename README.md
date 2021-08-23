# PapyrusFormatter
PapyrusFormatter is formatter for Skyrim script-language Papyrus.
Built with *win_bison_flex*.
## Features
* It just formats script files **.psc**
* Input/output may be files or just stdin/stdout
* It is possible to create backups

## Installation
* Move **PapyrusFormatter.exe** wherever you want
* Add path to the file in your **PATH**
## Usage
* `PapyrusFormatter -h` --- show help message
* `PapyrusFormatter in.psc out.psc` --- format *in.psc* file and write result to *out.psc* (may be equal)
* With `-i` flag out file sets equals to input file (`PapyrusFormatter -i scr.psc` <-> `PapyrusFormatter scr.psc scr.psc`)
* With `-b` flag *inp.psc_back* file will be created before formatting

It is usefull to add hotkey (e.g. in **notepad++**) to command `PapyrusFormatter -i "$(FULL_CURRENT_PATH)"`
