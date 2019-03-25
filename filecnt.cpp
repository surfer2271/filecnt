/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   filecnt.cpp
 * Author: james
 *
 * Created on February 28, 2019, 2:01 PM
 */

#include <iostream>
#include <string>
#include <dirent.h>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include  <sys/stat.h>

#include "common.h"
/*added comments for the week09 macros for the program */
#define DIRMAX 1
#define DIRNAMEMAX 25

using namespace std;
double version;
void displayVersion();


int dirCount =0;
int fileCount = 0;
int totalBytes = 0;
string path;
struct stat buf;
int size = 0;




//prototypes
void TraverseDirectory(string directory);
void TraverseFile(string file);
void TraveseSubDirectory(struct dirent* entity);


int main(int argc, char**argv) {
    
    version = 1.2;
    displayVersion();
    if(argc > 1 && argc <= 2)
    {
        
        
        string directory = argv[1];
        
        if(directory.at(0) == '~') {
        
            const char *homedir;

            if ((homedir = getenv("HOME")) == NULL) {
                homedir = getpwuid(getuid())->pw_dir;
                directory = homedir;
            }
            
            TraverseDirectory(directory);    
        
        }       
        else if(directory.at(0) == '/' || directory.at(0) == '.') {

            TraverseDirectory(directory);    
        

        }
        else {
            
              cout << "Correct usage for directory name will be with a  filecnt  /somedir/somedir2.. " << endl;
              return 1;
        }
       
       
       cout << "The total number of directories in directory " << directory << " is: " << dirCount << endl;

       cout << "The total number of files in directory " << directory << " is: " << fileCount  << endl;

       cout << "The total number of bytes occupied by all files in directory " << directory << " is: "  << totalBytes << endl;


            
    }
    else
        cout << "You didn't enter the correct number of parameters. filecnt <directeoryName> " << endl;
    
    
          
    return 0;
}



void TraverseDirectory(string directory)
{
    //string dirToOpen = directory;
    
    //convert to char* for opendir
    char * writable = new char[directory.size() + 1];
    copy(directory.begin(), directory.end(), writable);
    writable[directory.size()] = '\0'; // don't forget the terminating 0
    
    auto dir = opendir(writable);

    //set the new path for the content of the directory
    path = directory + "/";

   // cout << "Process directory: " << directory << std::endl;

    if(NULL == dir) {
       
        //cout << "could not open directory: " << directory << std::endl;
        
        return;
    }

    auto entity = readdir(dir);

    while(entity != NULL) {
        
        TraveseSubDirectory(entity);
        entity = readdir(dir);
    }

    //we finished with the directory so remove it from the path
    path.resize(path.length() - 1 - directory.length());
    closedir(dir);
}

void TraveseSubDirectory(struct dirent* entity)
{
    //find entity type
    if(entity->d_type == DT_DIR) {
        
        //define directory
        //don't process the  '..' and the '.' directories
        if(entity->d_name[0] == '.') {
            dirCount++;
            //get total number of bytes
            stat(entity->d_name, &buf);
            size=buf.st_size;
            totalBytes = totalBytes + size;
            return;
        }

        //it's an directory so process it
        TraverseDirectory(string(entity->d_name));
        return;
    }

    if(entity->d_type == DT_REG) {
        //plain file
        TraverseFile(string(entity->d_name));
        return;
    }

    //there are some other types
    //read here http://linux.die.net/man/3/readdir
    
    //cout << "Not a file or directory: " << entity->d_name << endl;
}

void TraverseFile(string file)
{
    fileCount++;
   // cout <<  "file     : " << file << endl;

    
}

