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

/**Parse them args and jank. Start some processes 
 * argc = argument count
 * argv = argument vector
 * envp = initial envorionment of the command line
 **/
int main(int argc, char* argv[], char* envp[])
{
  cout << "Welcome to QUASH!\n";
  //wait for a string to be inputted;
  string workingDir, cmd;
  char buf[BUF_SIZE], *t_cmd;
  
  while(cmd != "exit")
  {
    //print working directory
    getcwd(buf, BUF_SIZE);

    workingDir.assign(buf, buf +1);
    printf( "%s>", buf);
    
    cin >>cmd;
  
    
    queue<string> t_args;
    //tokenize string

    char cArr[cmd.size() + 1];
    for(unsigned int i = 0; i < cmd.size(); i++)
    {
      cArr[i] = cmd[i];
    }
    cArr[cmd.size() + 1] = '\0';
    
    t_cmd = strtok(cArr, " |");
    while(t_cmd != NULL)
    {
      string T_CMD(t_cmd);
      t_args.push(T_CMD);
      t_cmd = strtok(NULL, " |");
    }

    try
    {
      PathSearch(t_args.front());
    }
    catch(logic_error& e)
    {
       printf(e.what());
    }
  }
  
  return 0;
}

void PathSearch(string path)
{
  char* pPath = getenv(path.c_str());
  if(pPath == NULL)
  {
    string msg = "quash: " + path + ": command not found\n";
    throw logic_error(msg);
  }
  else
  {
    throw logic_error("Not yet implemented");
  }
}
