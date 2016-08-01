#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
char *pat; //pat is a global variable which contains the path in a process
void pwd();
void cd(char*);
int main()
{
    char buff[100];
    char *x=getenv("USER");//x gets the username
    char sysname[100];
    gethostname(sysname,100);//gets sysname
    int temp,i,j;
    pat=getcwd(pat,100);
    printf("<%s@%s:%s>",x,sysname,pat);
    fgets(buff,100,stdin);
    char *q,*cmd[100];
    q=strtok(buff,";\n");
    i=0;
    while(q!=NULL)//for the semicolon
    {
        cmd[i]=q;
        q=strtok(NULL,";\n");
        i++;
    }
    cmd[i]=NULL;//contains list of all independent commands
    j=0;
    char *path;
    while(cmd[j]!=NULL)
    {
        int rc=fork();
        if (rc==0)
        {
            char *p,*myargs[1000];
            p=strtok(cmd[j]," \n");
            for(i=0;p!=NULL;i++)
            {
                myargs[i]=p;
                p=strtok(NULL," \n");
            }
            if (strcmp(myargs[0],"pwd")==0)//pwd
            {
                pwd();
                exit(0);
            }
            else if(strcmp(myargs[0],"echo")==0)//echo
            {
                printf("%s\n",myargs[1]);
                exit(0);
            }
            else if(strcmp(myargs[0],"cd")==0)//cd
            {
                cd(myargs[1]);
                printf("%s\n",pat);
                exit(0);
            }
            else
                execvp(myargs[0],myargs);
            for(i=0;myargs[i]!=NULL;i++)
            {
                myargs[i]=NULL;
            }
        }
        else if(rc<0)
        {
            fprintf(stderr,"Fork failed");
        }
        else if(rc >0)
        {
            int wc=wait(NULL);
        }
        j++;
    }
    return 0;
}
//implementing pwd
void pwd()
{
    char dir[100];
    if (getcwd(dir,100)==NULL)
    {
        fprintf(stderr,"Error");
        exit(1);
    }
    else
    {
        printf("%s\n",dir);
    }
}
//implementing cd
void cd(char *path)
{
    strcat(pat,"/");
    strcat(pat,path);
    printf("%s\n",pat);
    chdir(pat);
}
