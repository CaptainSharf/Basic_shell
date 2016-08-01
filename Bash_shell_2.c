#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
char *join(char *pth[]);
void io_redir(char *args[]);
int bg(char *args[]);
void func(char *args[],char *str,const char* delims);
int spawn_proc(int in,int out,char *cmd);
int pipes(char buf[]);
int pip(char buff[]);
int main()
{
	char buff[500],*cmd[500],*comm,*args[100],*path,*user,*name;
	const char *temp;
	int j,i,itr,len,x,flag,k,bgfl;
	pid_t pid;
	//char *xa=(char)getenv("USER");
	//char sysname[20];
	//gethostname(sysname,20);
	while(1)
	{
		fgets(buff,500,stdin);
		func(cmd,buff,";\n");
		for(j=0;cmd[j]!=NULL;j++)
		{
			flag=0;
			temp=NULL;
			func(args,cmd[j]," ");
			path=getcwd(path,500);
			if(strcmp(args[0],"pwd")==0)
			{
				printf("%s\n",path);
			}
			else if(strcmp(args[0],"cd")==0)
			{
				strcat(path,"/");
				strcat(path,args[1]);
				x=chdir(path);
				if(x!=0)
					printf("Error\n");
				path=getcwd(path,500);
			}
			else
			{
				bg(args);
				pid=fork();
				if(pid==0)
				{
					io_redir(args);
					//pip(buff);
					execvp(args[0],args);
				}
				else if(pid>0)
				{

	//					printf("%s@%s:%s",xa,sysname,path);
					if(bg(args))
					{
						wait(0);
					}
				}
			}
		}
	}
	return 0;
}

void func(char *args[],char *str,const char* delims)
{
	int i=0;
	args[0]=strtok(str,delims);
	while(args[i]!=NULL)
	{
		i+=1;
		args[i]=strtok(NULL,delims);
	}
}

int bg(char *args[])
{
	int i;
	char *temp;
	for(i=0;args[i]!=NULL;i++);
	if(strcmp(args[i-1],"&")==0)
	{
		args[i-1]=0;
		return 1;
	}
	else
		return 0;
}

void io_redir(char *args[])
{
	int i,flag,fd1,fd2,j;
	char *temp1,*temp2,*temp;
	flag=0;
	for(j=0;args[j]!=NULL;j++);
	for(i=0;i<j;i++)
	{
		if(strcmp(args[i],"<")==0)
		{
			args[i]=0;
			close(0);
			fd1=open(args[i+1],O_RDONLY);
		}
		else if(strcmp(args[i],">")==0)
		{
			args[i]=0;
			close(1);
			fd2=open(args[i+1],O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
		}
	}
}

int spawn_proc(int in,int out,char *cmd)
{
	pid_t pid;
	int i;
	char *args[100];
	func(args,cmd," ");
	for(i=0;args[i]!=0;i++)
		printf("%s\n",args[i]);
	if((pid=fork())==0)
	{
		if(in!=0)
		{
			dup2(in,0);
			close(in);
		}
		if(out!=1)
		{
			dup2(out,1);
			close(out);
		}
		return execvp(args[0],args);
	}
	return pid;
}

int pipes(char buff[])
{
	char *cmd[50],*args[50];
	int i,j,in,fd[2];
	func(cmd,buff,"|\n");
	in=0;
	for(i=0;cmd[i+1]!=NULL;i++)
	{
		printf("ok\n");
		//func(args,cmd[i]," ");
		pipe(fd);
		spawn_proc(in,fd[1],cmd[i]);
		close(fd[1]);
		in=fd[0];
	}
	func(args,cmd[i]," ");
	if(in!=0)
	{
		dup2(in,0);
	}
	return execvp(args[0],args);
}

/*int pip(char buff[])
  {
  int i,flag;
  flag=0;
  const char *temp;
  for(i=0;i<strlen(buff);i++)
  if(buff[i]=='|')
  flag=1;
  temp=(char*)malloc(sizeof(strlen(buff)+1));
  strcpy(temp,buff);
  sys(temp);
  exit(0);
  }*/
