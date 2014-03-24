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
#include <fcntl.h>


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
  bool isLastRan = false;
  
  while(cmd != "exit" || cmd != "quit")
  {
    isLastRan = false;
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
      if(cmd.length() == 2)
      {
        chdir("/home/dmenager");
        continue;
      }
      chdir(cmd.substr(3).c_str());
      cmd = "";
      continue;
    }

    queue<string> q_cmd;

    tokenize(cmd, q_cmd, "|");
   

    //make file descriptors
    for(unsigned int i = 0; i < q_cmd.size() - 1; i++)
    {
      //f[0] == read
      //f[1] == write
      FileDescriptors f;
      fds.push_back(f);

      pipe(fds[i].fd);
    }

    unsigned int i = 0;
    unsigned int z = 0;
    //for each item in q_cmd, make a new procress for it
    while(!q_cmd.empty())
    {
      //create and run new process
      if(fork() == 0)
      { 
        //TODO: close the unused read/write ends
        for(; z <= fds.size();)
        {
          if(fds.empty())
          {
            break;
          }

          //First executable in pipe chain
          if(q_cmd.size() > 1 && i == 0 && z == 0)
          {
            cout << "First argument\n";
            //read from screen, output to pipe
            dup2(fds[z].fd[1], STDOUT_FILENO);

            //close all unused file descriptors
            for(unsigned int k = 0; k < fds.size(); k++)
            {
                close(fds[k].fd[0]);

              if(k != z)
              {
                close(fds[k].fd[1]);
              }
            }
            
            break;
          }
          //some command in the middle of the pipe chain
          else if(q_cmd.size() > 1)
          {
            cout << "Middle args\n";
            //redirect everything
            dup2(fds[z - 1].fd[0], STDIN_FILENO);
            dup2(fds[z].fd[1], STDOUT_FILENO);

            //close all unused file descriptors
            for(unsigned int k = 0; k < fds.size(); k++)
            {
              if(k != z - 1)
              {
                close(fds[k].fd[0]);
              }

              if(k != z)
              {
                close(fds[k].fd[1]);
              }
            }

            break;
          }
          else if(z == fds.size())
          {

            cout << "Last arg\n";
            //read from pipe, output to screen
            dup2(fds[z - 1].fd[0], STDIN_FILENO);
           
            //close all unused file descriptors
            for(unsigned int k = 0; k < fds.size(); k++)
            {
              if(k != z - 1)
              {
                close(fds[k].fd[0]);
              }

              close(fds[k].fd[1]);
            }
  
            break;
          }
        }
        
        //TODO: reroute stdin/out IF PRESCRIBED BY > <
        
            //if any of the elements is ps.at(i) == < or > then redirect

        
        //set up argument list for new process. 
        char** cmdbuf = new char*[q_cmd.front().size() + 2];
        strcpy(*cmdbuf, q_cmd.front().c_str());
        cmdbuf[q_cmd.front().size() + 2] = (char*)0;
        *cmdbuf = strtok(*cmdbuf, " ");

        //add the arguments to cmdbuf
        for(unsigned int l = 1; l < q_cmd.front().size() + 1; l++)
        {
          cmdbuf[l] = strtok(NULL, " ");

          if(cmdbuf[l] == (char*)0)
            break;
        }

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
      
      q_cmd.pop();
     
      if(q_cmd.empty())
      {
        isLastRan = true;
      }
     
      if(isLastRan)
      {
        for(unsigned int k = 0; k < fds.size(); k++)
        {
          close(fds[k].fd[0]);
          close(fds[k].fd[1]);
        }
      }
      
      wait(NULL);
      i++;
      z++;
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
