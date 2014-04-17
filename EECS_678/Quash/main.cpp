//QiiQ

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
#include <fstream>


#define BUF_SIZE 256

using namespace std;

vector<int> pid_list;
vector<string> ps_status;

//Arrays are not copy constructible so I need a struct for my fds
typedef struct
{
  int fd[2];
} FileDescriptors;

typedef struct
{
  int job_id;
  pid_t job_pid;
  const char *status;

} joblist;

void tokenize(string str, queue<string>& q, char* delim);

/**Parse them args and jank. Start some processes 
 * argc = argument count
 * argv = argument vector
 * envp = initial envorionment of the command line
 **/
int main(int argc, char* argv[], char* envp[])
{
  cout << *envp << "\n";
  //wait for a string to be submitted
  string workingDir, cmd;
  char buf[BUF_SIZE];
  bool isLastRan = false;
  int bg_mark = 0;
 

  //This is the program loop
  while(cmd != "exit" || cmd != "quit")
  {
    sleep(1);
    isLastRan = false;
    cmd = "";

    //list of file descriptors
    vector<FileDescriptors> fds;
    
    //print working directory
    getcwd(buf, BUF_SIZE);

    workingDir.assign(buf, buf +1);
    printf( "%s>", buf);
    
    getline(cin, cmd);
    
    if(cmd == "")
    {
      continue;
    }
    //Convert string to cstring to check to last character
    //if the last character is '&', then enable the background run
    //which will result in skipping the wait() function
    //last but not least, get rid of the '&' from the command
    char* cstr = new char [cmd.length()+1];
    strcpy(cstr, cmd.c_str());
    if ( cstr[cmd.length()-1] == '&')
    {
      bg_mark=1;
      cstr[cmd.length()-1] = ' ';

      cmd = cmd.substr(0, cmd.size() - 1);

    }

    if(cmd == "exit" || cmd == "quit")
    {
      break;
    }

    //check for a few specific commands
    if(cmd == "jobs")
    {
      cout << "list size is: " << int(pid_list.size()) << "\n";
      for(int i = 0; i < pid_list.size(); i++)
      {
	      pid_t p = waitpid(pid_list[i], NULL, WNOHANG);
        if (p == -1)
          ps_status[i]=" finished command";
        cout<<"JOBID: "<< i << " PID " << pid_list[i]<< ps_status[i]<<"\n";
      }
      continue;
    }
    else if(cmd.substr(0, 2) == "cd")
    {
      if(cmd.length() == 2)
      {
        chdir(getenv("HOME"));
        continue;
      }
      chdir(cmd.substr(3).c_str());
      cmd = "";
      continue;
    }
    //set the home/path variable
    else if(cmd.substr(0, 4) == "HOME" || cmd.substr(0, 4) == "PATH")
    {
      char* var = new char(cmd.size() + 1);
      strcpy(var, cmd.c_str());
      char* path = strtok(var, "=");
      if(cmd.substr(0, 4) == "HOME")
      {
        setenv("HOME", cmd.substr(5).c_str(), 1);
      }
      else
      {
        setenv("PATH", cmd.substr(5).c_str(), 1);
      }

      delete [] var;
      continue;
    }

    //command queue
    queue<string> q_cmd;

    //fill q_cmd with a list of commands (delimitted by |). So q_cmd[i] is the
    //ith - 1 command input by user
    tokenize(cmd, q_cmd, "|");
   

    //make file descriptors for pipes
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
      //create a new process
      int fork_id = fork();
      if(bg_mark == 1)
      {
        pid_list.push_back(fork_id);
        ps_status.push_back(" running in background");
      }
      //run new process
      if(fork_id == 0)
      {
        //reroute stdin/out IF PRESCRIBED BY > <
        //reroutes output of command to file
        size_t found = q_cmd.front().find(">");
        if(found != string::npos)
        {

          FILE* stream;
          
          //Retreives the file descriptor of file
          stream = fopen(q_cmd.front().substr(found + 2).c_str(), "w");

          //do the redirection
          dup2(fileno(stream), STDOUT_FILENO);

          //remove the "> <file>" part of the argument now
          char* cAr = new char[q_cmd.front().size() + 1];
          strcpy(cAr, q_cmd.front().c_str());
          q_cmd.front() = strtok(cAr, ">");
        }

        //close the unused read/write ends
        for(; z <= fds.size();)
        {
          if(fds.empty())
          {
            break;
          }
          
          //First executable in pipe chain
          if(q_cmd.size() > 1 && i == 0 && z == 0)
          {
            //cout << "First argument\n";
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
            //cout << "Middle args\n";
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

            //cout << "Last arg\n";
            //read from pipe, output to screen
            dup2(fds[z - 1].fd[0], STDIN_FILENO);
           
            //close all unused file descriptors
            for(unsigned int k = 0; k < fds.size(); k++)
            {
              if(k != (z - 1))
              {
                close(fds[k].fd[0]);
              }

              close(fds[k].fd[1]);
            }
  
            break;
          }
        }
        
        //set up argument list for new process.
        
        //cmdbuf holds list of arguments for process at front of q_cmd
        char** cmdbuf = new char*[q_cmd.front().size() + 1];

        //Arr DOES cause a memory leak here, but I don't care enough right now
        char* Arr = new char[q_cmd.front().size() + 1];
        strcpy(Arr, q_cmd.front().c_str());
        char* val = strtok(Arr, " ");
        cmdbuf[0] = val;
        
        //fill cmdbuf with the arguments
        for(int s = 1; s < q_cmd.front().size(); s++)
        {
          char* myval = strtok(NULL, " " );
          cmdbuf[s] = myval;
        }
        cmdbuf[q_cmd.front().size() + 1] = (char*)0;

        //run new process
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
      if(bg_mark == 0)
      {
        wait(NULL);
      }
      else
      {
	bg_mark = 0;
      }
      i++;
      z++;
    }  
  }
  
  return 0;
}

/**
 * str = string to tokenize
 * q = queue to hold tokens
 * delim = marker by which we use to separate tokens
 * */
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
