# randJob [![FOSSA Status](https://app.fossa.io/api/projects/git%2Bgithub.com%2FRajdeepKonwar%2FrandJob.svg?type=shield)](https://app.fossa.io/projects/git%2Bgithub.com%2FRajdeepKonwar%2FrandJob?ref=badge_shield)
Launches a random video file from the current working directory (provided the directory contains video files) using the Open Source video framework [libVLC](https://wiki.videolan.org/LibVLC/) with book-keeping of played videos and excluding previously-played videos from the randomizer pool.

### Requirements
1. gcc version 6.3.0 or higher (thank me later for upgrading your gcc)
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-6
sudo apt install g++-6
sudo update-alternatives --instal /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ /usr/bin/g++-6
```
2. Boost 1.64.0 or higher
```
sudo apt-get install libboost-all-dev
```
3. libVLC
```
sudo apt-get install libvlc-dev
```

### Compile Instruction
```
g++ randJob.cpp -o randJob -lboost_system -lboost_filesystem -lvlc
```

### Run Instruction
```
./randJob
```
