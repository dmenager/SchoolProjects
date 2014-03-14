#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstddef>
#include <cstdio>
#include <queue>
#include <exception>
#include <stdexcept>
#include <cstdlib>

#define BUF_SIZE 256

using namespace std;

void PathSearch(string path);
void tokenize(string str, queue<string>& q, char* delim);
/**Parse them args and jank. Start some processes 
 * argc = argument count
 * argv = argument vector
 * envp = initial envorionment of the command line
 **/
char* t_cmd;
int main(int argc, char* argv[], char* envp[])
{
  cout << "Welcome to QUASH!\n";
  //wait for a string to be inputted;
  string workingDir, cmd;
  char buf[BUF_SIZE];
  
  while(cmd != "exit")
  {
    //print working directory
    getcwd(buf, BUF_SIZE);

    workingDir.assign(buf, buf +1);
    printf( "%s>", buf);
    
    cin >>cmd;
  
    
    queue<string> t_args;
    queue<string> paths;

    tokenize(cmd, t_args, " |");
    //tokenize string


    try
    {
      //if this is not a full path search through the path variable
      if(t_args.front()[0] != "/";
      {
        PathSearch(paths, t_args.front());
      
        for(int i = 0; i < paths.size(); i++)
        {
          string fullPath = paths.front();
          paths.pop();
          fullPath += path;
	  
    	  int pid = fork();
	  if(pid == 0)
	  {
	    //child
            //try to run execl
	    try
	    {
	      //execl(fullPath.c_str(), ...);
	    }
	    catch(...)
	    {
	      continue;
	    }
   	  }
        }
      }
    }
    catch(logic_error& e)
    {
       printf(e.what());
    }
  }
  
  return 0;
}

void tokenize(string str, queue<string>& q, char* delim)
{

    char cArr[str.size() + 1];
    for(unsigned int i = 0; i < str.size(); i++)
    {
      cArr[i] = str[i];
    }
    cArr[str.size() + 1] = '\0';
    
    t_cmd = strtok(cArr, delim);
    while(t_cmd != NULL)
    {
      string T_CMD(t_cmd);
      q.push(T_CMD);
      t_cmd = strtok(NULL, delim);
    }
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
