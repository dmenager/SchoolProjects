#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <queue>
#include <exception>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cerrno>
#include <sys/wait.h>
#include <dirent.h>

#define BUF_SIZE 256

using namespace std;

//Arrays are not copy constructible so I need a struct for my fds
typedef struct
{
  int fd[2];
} FileDescriptors;

void tokenize(string str, queue<string>& q, char* delim);

/**Parse them args and jank. Start some processes 
 * argc = argument count
 * argv = argument vector
 * envp = initial envorionment of the command line
 **/
int main(int argc, char* argv[], char* envp[])
{
  cout << "Welcome to QUASH!\n";
  //wait for a string to be submitted
  string workingDir, cmd;
  char buf[BUF_SIZE];
  
  while(cmd != "exit" || cmd != "quit")
  {
    cmd = "";
    //list of processes
    vector<queue<string> > ps;

    //list of file descriptors
    vector<FileDescriptors> fds;
    
    //print working directory
    getcwd(buf, BUF_SIZE);

    workingDir.assign(buf, buf +1);
    printf( "%s>", buf);
    
    getline(cin, cmd);

    if(cmd == "exit" || cmd == "quit")
    {
      break;
    }

    if(cmd.substr(0, 2) == "cd")
    {
      chdir(cmd.substr(3).c_str());
      cmd = "";
      continue;
    }

    queue<string> q_cmd;

    tokenize(cmd, q_cmd, "|");

    int i = 0;
    //for each item in q_cmd, make a new procress for it
    while(!q_cmd.empty())
    {
      queue<string> q;
      ps.push_back(q);
     
      //f[0] == read
      //f[1] == write
      FileDescriptors f;
      fds.push_back(f);

      for(int j = 0; j < fds.size(); j++)
      {
        pipe(fds[j].fd);
      }

      //TODO: Fix issue where you must run argument with pipe to see output
      //create and run new process
      if(fork() == 0)
      {
        cout << "Fork Successfull on process " << i << "\n";
      
        //TODO: close the unused read/write ends
        for(int j = 0; j < fds.size(); j++)
        {
          //First executable in pipe chain
          if(q_cmd.size() > 1 && i == 0 && j == 0)
          {
            //read from screen, output to pipe
            dup2(fds[j].fd[1], STDOUT_FILENO);

            //close all unused file descriptors
            for(int k = 0; k < fds.size(); k++)
            {
                close(fds[k].fd[0]);

              if(k != j)
              {
                close(fds[k].fd[1]);
              }
            }
          }
          //some command in the middle of the pipe chain
          else if(q_cmd.size() > 1)
          {
            //redirect everything
            dup2(fds[j - 1].fd[0], STDIN_FILENO);
            dup2(fds[j].fd[1], STDOUT_FILENO);

            //close all unused file descriptors
            for(int k = 0; k < fds.size(); k++)
            {
              if(k != j - 1)
              {
                close(fds[k].fd[0]);
              }

              if(k != j)
              {
                close(fds[k].fd[1]);
              }
            }
          }
          else if(fds.size() > 1)
          {
            //read from pipe, output to screen
            dup2(fds[j].fd[0], 0);
           
            //close all unused file descriptors
            for(int k = 0; k < fds.size(); k++)
            {
              if(k != j)
              {
                close(fds[k].fd[0]);
              }

              close(fds[k].fd[1]);
            }
          }
        }

        //cout << "jank2\n";
        
        //TODO: reroute stdin/out IF PRESCRIBED BY > <
        
            //if any of the elements is ps.at(i) == < or > then redirect


        //set up argument list for new process. 
        char** cmdbuf = new char*[q_cmd.front().size() + 1];
        //cout << "cmdbuf made\n";
        strcpy(*cmdbuf, q_cmd.front().c_str());
        //cout << "copied and jank\n";
        cmdbuf[q_cmd.front().size() + 1] = (char*)0;
        //cout << "cmdbuf set\n";
        *cmdbuf = strtok(*cmdbuf, " ");

        //cout << "MADE IT 1!\n";

        //add the arguments to cmdbuf
        for(int l = 1; l < q_cmd.front().size() + 1; l++)
        {
          cmdbuf[l] = strtok(NULL, " ");

          if(cmdbuf[l] == (char*)0)
            break;
        }

        //cout << "MADE IT 2!\n";
        //run new process
        //TODO: searh in HOME as well

        execvpe(cmdbuf[0], cmdbuf, envp);
        if(errno == ENOENT)
        {
          printf("%s: command not found\n", cmdbuf[0]); 
          exit(-1);
        }
        else if(errno == ENOMEM)
        {
          printf("Not enough memory to execute %s\n", cmdbuf[0]);
          exit(-1);
        }
        else
        {
          cout << "Error: Failed to overwrite address space.\n";
          exit(-1);
        }
      
        exit(0);
      }
      
      //close everything
      for(int k = 0; k < fds.size(); k++)
      {
        close(fds[k].fd[0]);
        close(fds[k].fd[1]);
      }
      
      wait(NULL);

      q_cmd.pop();

      i++;
    }  
  }
  
  return 0;
}

void tokenize(string str, queue<string>& q, char* delim)
{

    char* cArr = new char[str.size() + 1];
    
    strcpy(cArr, str.c_str());
    
    char* t_cmd = strtok(cArr, delim);
    while(t_cmd != NULL)
    {
      string T_CMD(t_cmd);
      q.push(T_CMD);
      t_cmd = strtok(NULL, delim);
    }

    delete [] cArr;
}
