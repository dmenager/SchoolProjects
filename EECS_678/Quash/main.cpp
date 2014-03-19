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

#define BUF_SIZE 256

using namespace std;

//Arrays are not copy constructible so I need a struct for my fds
typedef struct
{
  int fd[2];
} FileDescriptors;

void PathSearch(queue<string>& q, string path);
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
  
  while(cmd != "exit")
  {
    //list of processes
    vector<queue<string> > ps;

    //list of file descriptors
    vector<FileDescriptors> fds;
    
    //print working directory
    getcwd(buf, BUF_SIZE);

    workingDir.assign(buf, buf +1);
    printf( "%s>", buf);
    
    getline(cin, cmd);
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
      pipe(f.fd);
      tokenize(q_cmd.front(), ps.at(i), " ");
    
      q_cmd.pop();
      fds.push_back(f);
      
      //create and run new process
      if(fork() == 0)
      {
        cout << "Fork Successfull on process " << i << "\n";
      
        //TODO: close the unused read/write ends
        for(int j = 0; j < fds.size(); j++)
        {
          //don't close your own read write jank.
          if(j != i)
          {
            //if j is the file descriptor for the process that outputs to i
            //(me), leave the write end open.
            if(j == i - 1)
            {
              close(fds[j].fd[0]);
              cout << "Closed file descriptor fds[" << j << "].fd[0]\n";
              continue;
            }
            close(fds[j].fd[0]);
            close(fds[j].fd[1]);
            cout << "Closed file descriptor fds[" << j << "].fd[0]\n";
            cout << "Closed file descriptor fds[" << j << "].fd[1]\n";
          }
        }
        
        //TODO: reroute stdin/out IF PRESCRIBED BY > <
        
            //if any of the elements is ps.at(i) == < or > then redirect

        string cmdbuf = "";
        string run = ps.at(i).front();
        while(!ps.at(i).empty())
        {
          cmdbuf.append(ps.at(i).front());
          cmdbuf.append(" ");
          ps.at(i).pop();
        }

        //cout << run << "\n"; 
        //cout << cmdbuf << "\n";

        //run new process
        //TODO: make sure arguments are being passed correctly
        //TODO: searh in HOME as well
        execlp(run.c_str(), cmdbuf.c_str(), (char*)0, envp);
        if(errno == ENOENT)
        {
          printf("%s: command not found\n", run.c_str()); 
          exit(-1);
        }
        else if(errno == ENOMEM)
        {
          printf("Not enough memory to execute %s\n", run.c_str());
          exit(-1);
        }
        else
        {
          cout << "Error: Failed to overwrite address space.\n";
          exit(-1);
        }

        exit(0);
      }
      wait(NULL);
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

void PathSearch(queue<string>& q, string path)
{
  char* pPath = getenv("PATH");
  if(pPath == NULL)
  {
    string msg = "quash: " + path + ": command not found\n";
    throw logic_error(msg);
  }
  else
  {
    tokenize(pPath, q, ":");
  }
}
