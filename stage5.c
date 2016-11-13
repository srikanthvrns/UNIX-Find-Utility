#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include<sys/stat.h>
#include<errno.h>
#include <fcntl.h>
#include <sys/param.h>
#include <sys/vnode.h>
#include <sys/fs/ufs_fs.h>
#include <sys/fs/ufs_inode.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <sys/wait.h>
#include <libgen.h>
#include <unistd.h>

int visitDir(char *a);
unsigned int ftype;
char* selTable[] = {"-name","-mtime","-user",NULL};
char *selection;
char *v;
char **cmd_list;
int k=0;
char *PF;

void cmdExec()
{
	pid_t pid;
	if((pid=fork())==0)
	{execv(PF,cmd_list);}
	else
	wait(NULL);
}

int ProcArg(int N,char *args[])
{	int i,j=5;
	cmd_list = (char**)malloc(sizeof(char*)*(N-2));
	printf("source=%s\t",args[1]);
	for(i=0;i<3;i++)
	{
		if(strcmp(args[2],selTable[i]) == 0 )
		{
			selection = args[2];
			v = args[3];
			if(N>4)
			{cmd_list[0]=(args[4]+1);
				while( j<N && strncmp(args[j],"-",1)==0)
				{cmd_list[j-4]=args[j];
			j++;}
			k=j-4;
			cmd_list[k]="";
			printf("value of k%d",k);
			if(args[N-1]!=NULL)
			 {
			cmd_list[j-3]=args[N-1];
			j++;
			}
			
			cmd_list[j-3]=NULL;
			printf("value of j%d",j-3);}
			printf("selection=%s\t",args[2]);
			printf("value=%s\n",v); 
		}}
	return 0;
}

int main(int argc,char *argv[])
{
if(argc<2)
	{
	perror("Arguments Needed"); exit(2);
	}
	struct stat buf;
	//printf("No_Of_Arguments %d \n",argc);
	ProcArg(argc,argv);
	printf("OUT");
	lstat(argv[1], &buf);
	ftype = buf.st_mode & 0xF000;
	//printf("ftype=%x\n", ftype);
	PF = pathfind(getenv("PATH"),cmd_list[0],"rx");
	
	if (ftype == S_IFDIR ) {
		visitDir(argv[1]);}
	else if (ftype == S_IFREG) {printf ("reg \t %s \n",argv[1]);}
	else
	printf("Oth- \t %s \n",argv[1]);
	return 0;
}

time_t curr_t;

int visitDir(char *arg)
{
	unsigned long int w = atoi(v);
	//printf("current time%ld\n",(time(NULL)-w*86400));
	char *fpath;
	DIR *dirp;
	struct dirent *dp;
	struct passwd *getpwnam(const char *);
	unsigned long int mt; //mode_t s_m;
	struct stat buff;
	if((dirp = opendir(arg))==NULL)
	{
		perror("NO");
	}
	else{              
		while ((dp = readdir(dirp)) != NULL) 
		{
			if(strcasecmp(dp->d_name, ".") == 0 || strcasecmp(dp->d_name, "..")==0)
			continue; 
			fpath=(char*)malloc(strlen(arg)+strlen(dp->d_name)+2);
			strcpy(fpath,arg);
			strcat(fpath,"/");
			strcat(fpath,dp->d_name);
			if(lstat(fpath, &buff)==0)
			{ 
				mt = buff.st_mtime;  
				ftype = buff.st_mode & 0xF000;
				if (ftype == S_IFDIR)
				{if(strcasecmp(selection,"-name") == 0)
					{if(strcasecmp(dp->d_name,v) == 0)
						{printf("Dir \t %s \n",fpath);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}}
					}
					else if(strcasecmp(selection,"-mtime") == 0)
					{if(buff.st_mtime > (time(NULL) - w*86500))
						{printf("Dir \t %s \n",dp->d_name);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}}
					}
					else if(getpwnam(v)!=NULL)
					{if(buff.st_uid == (getpwnam(v)->pw_uid))
						{printf("Dir \t %s \n",fpath);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}}
					}
					
					visitDir(fpath);
                     free(fpath);			
				}
				else if (ftype == S_IFREG) 
				{
					if(strcasecmp(selection,"-name") == 0)
					{if(strcasecmp(dp->d_name,v) == 0)
						{		printf("Reg \t %s \n",fpath);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}
						}
					}
					else if(strcasecmp(selection,"-mtime") == 0)
					{if(buff.st_mtime > (time(NULL) - w*86500))
						{		printf("Reg \t %s \n",dp->d_name);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}
						}
					}
					else if(getpwnam(v)!=NULL)
					{
						if(buff.st_uid == (getpwnam(v)->pw_uid))
						{		printf("Reg \t %s \n",fpath);
							if(k!=0)
							{cmd_list[k]=fpath;
							cmdExec();}
						}
					}
				}//printf("Last modification time= %ld \n",mt);}
				
				else
				{if(strcasecmp(dp->d_name,v) == 0)
					{printf("Oth \t %s \n",fpath);
						cmd_list[k]=fpath;
						cmdExec();}
					else if(mt > (time(NULL)- w*86500))
					{printf("Oth \t %s \n",dp->d_name);
						cmd_list[k]=fpath;
						cmdExec();}
					else if(getpwnam(v)!=NULL){
						if(buff.st_uid == (getpwnam(v)->pw_uid))
						printf("Oth \t %s \n",fpath);
						cmd_list[k]=fpath;
						cmdExec();}
				}
				//}else {printf("%s\n",fpath);perror("");}
			}else {perror("lstat not entered");continue;}
			free(fpath);
		}
		closedir(dirp);
	}
	return 0;
}
