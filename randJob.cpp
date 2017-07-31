/**
 @author Rajdeep Konwar (rkonwar AT ucsd.edu)

 @section DESCRIPTION
 Launches a random mp4/flv video file with book-keeping and
 excluding previously played videos from the randomizer pool

 @section REQUIREMENTS
 1. gcc version 6.3.0 or higher
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt update
    sudo apt install gcc-6
    sudo apt install g++-6
    sudo update-alternatives --instal /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ /usr/bin/g++-6
 2. Boost 1.64.0 or higher
    sudo apt-get install libboost-all-dev
 3. libVLC
    sudo apt-get install libvlc-dev

 @section COMPILE INSTRUCTION
 g++ randJob.cpp -o randJob -lboost_system -lboost_filesystem -lvlc

 @section LICENSE
 Copyright (c) 2017, Rajdeep Konwar
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <algorithm>
#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <vlc/vlc.h>

using namespace boost::filesystem;

int main() {
  //! Sets path to the current directory
  path  p ( boost::filesystem::current_path() );

  try {
    if( exists( p ) && is_directory( p ) ) {
      //! Variables
      bool        newVid  = false;
      int         random, i = 1;
      std::size_t found1, found2;
      std::string wanted1( ".mp4" );
      std::string wanted2( ".flv" );
      std::string line;
      FILE        *fp;

      //! libVLC objects
      libvlc_instance_t     *inst;
      libvlc_media_player_t *mp;
      libvlc_media_t        *m;

      //! STLs
      std::vector< path >           allFiles, files;
      std::vector< path >::iterator iter;
      std::vector< std::string >    played;

      //! Gets all files and stores into a vector from the currect directory
      copy( directory_iterator( p ), directory_iterator(), back_inserter( allFiles ) );
      sort( allFiles.begin(), allFiles.end() );

      //! Push all the video files (containing .mp4 or .flv) into another vector
      for( iter = allFiles.begin(); iter != allFiles.end(); ++iter ) {
        std::string temp( (*iter).string() );
        found1  = temp.find( wanted1 );
        found2  = temp.find( wanted2 );

        if( found1 != std::string::npos || found2 != std::string::npos ) {
          files.push_back( *iter );
        }
      }

      //! Reads mano.job to get previously played videos
      std::ifstream inFile( "mano.job" );
      if( inFile ) {
        while( std::getline( inFile, line ) ) {
          played.push_back( line );
        }

        inFile.close();
      }

      //! Exits program when all videos watched!
      if( played.size() == files.size() ) {
        std::cout << "No new video found! Delete mano.job\n";
        exit( EXIT_FAILURE );
      }

      do {
        //! Picks a random video
        srand( (unsigned)time( NULL ) );
        random = rand() % files.size();
        path f = files.at( random );

        //! Repicks if file does not contain .mp4 and .flv
        std::string ext{f.extension().string()};
        if( ext != ".mp4" && ext != ".flv" )
          continue;

        std::string randFile( f.string() );
        //! Breaks loop if randomly picked video hasn't been played previously
        if( std::find(played.begin(), played.end(), randFile) == played.end() )
          newVid  = true;
      } while( newVid == false );

      fp  = std::fopen( "mano.job", "a+" );
      if( !fp ) {
        std::cout << "Cannot open mano.job in append mode!\n";
        exit( EXIT_FAILURE );
      }

      //! Adds the new video to list of played videos
      fprintf( fp, "%s\n", files.at( random ).c_str() );
      fclose( fp );

      //! Video info
      std::cout << "Now playing: " << files.at( random ).c_str() << std::endl;

      //! Loads the VLC engine
      inst  = libvlc_new( 0, NULL );

      //! Creates a new item
      m   = libvlc_media_new_path( inst, files.at( random ).c_str() );

      //! Creates a media player playing environement
      mp  = libvlc_media_player_new_from_media( m );

      //! No need to keep the media now
      libvlc_media_release( m );

      //! plays the media player
      libvlc_media_player_play( mp );

      //! Infinite loop to keep media player running
      do {
        ;
      } while( i > 0 );

      //! Stops playing
      libvlc_media_player_stop( mp );

      //! Frees the media_player
      libvlc_media_player_release( mp );

      libvlc_release( inst );
    }
    else
      std::cout << p << " does not exist\n";
  }

  catch( const filesystem_error& ex ) {
    std::cout << ex.what() << std::endl;;
  }

  return 0;
}
