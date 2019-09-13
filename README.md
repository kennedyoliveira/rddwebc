# (RDDWEBCrawling) - [Alpha Version]

// TODO add some description on what it does, what its purpose, what problem it solves etc

Tested on Debian, Ubuntu, and Mint.

## Requirements
- libboost-all-dev
- libcurl4-openssl-dev

## How to build
```
g++ -std=c++17 -m64 -fstack-protector-all rddwebc.cpp -o rddwebc -lcurl -lpthread -lboost_system -lboost_filesystem
```

## License
Copyright (C) 2019, Rafael Godoy, <0x67rafael@protonmail.com> .

RDDWEBC is truly Open Source/Free Software and meets all definitions as such. 
It means that you are free to modify and redistribute all contents of the curl distributed archives.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>