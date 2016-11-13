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

int visitDir(char *a);
unsigned int ftype;
char* selTable[] = {"-name","-mtime","-user",NULL};
char *selection;
char *v;

int ProcArg(int count,char *args[])
{
int i;
	//printf("source=%s\t",args[1]);
	for(i=0;i<3;i++)
	{
		if(strcmp(args[2],selTable[i]) == 0 )
		{selection = args[2];
			v = args[3];
			//printf("selection=%s\t",args[2]);
			printf("value=%s\n",v); 
		}}
	return 0;
}

int main(int argc,char *argv[])
{	
if(argc<2)
	{perror("Arguments Needed"); exit(2);}
struct stat buf;
	printf("No_Of_Arguments %d \n",argc);
	if(argc>2)
	{ProcArg(argc,argv);}
	lstat(argv[1], &buf);
	ftype = buf.st_mode & 0xF000;
	//printf("ftype=%x\n", ftype);
	if (ftype == S_IFDIR) {
		visitDir(argv[1]);}
	else if (ftype == S_IFREG) {printf ("reg \t %s \n",argv[1]);}
	else
	printf("Oth- \t %s \n",argv[1]);
	return 0;
}
time_t curr_t;

int visitDir(char *arg)
{unsigned long int w = atoi(v);
	//printf("current time%ld\n",(time(NULL)-w*86400));
	char *fpath;
	DIR *dirp;
	struct dirent *dp;struct passwd *getpwnam(const char *);
	unsigned long int mt; //mode_t s_m;
	struct stat buff;
	if((dirp = opendir(arg))==NULL)
	{perror("\n");}
	else{              
		while ((dp = readdir(dirp)) != NULL) 
		{
			if(strcasecmp(dp->d_name, ".") == 0 || strcasecmp(dp->d_name, "..")==0)
			continue; 
			fpath=malloc(strlen(arg)+strlen(dp->d_name)+2);
			strcpy(fpath,arg);
			strcat(fpath,"/");
			strcat(fpath,dp->d_name);
			if(lstat(fpath, &buff)==0)
			{ 
				mt = buff.st_mtime;  
				ftype = buff.st_mode & 0xF000;
				if (ftype == S_IFDIR)
				{
				if(strcasecmp(selection,"-name") == 0)
				{if(strcasecmp(dp->d_name,v) == 0)
					printf("Dir \t %s \n",fpath);}
					else if(strcasecmp(selection,"-mtime") == 0)
					{if(buff.st_mtime > (time(NULL) - w*86500))
					printf("Dir \t %s \n",dp->d_name);}
					else if(getpwnam(v)!=NULL)
					{if(buff.st_uid == (getpwnam(v)->pw_uid))
					printf("Dir \t %s \n",fpath);}
					visitDir(fpath);
				}
				else if (ftype == S_IFREG) 
				{
				if(strcasecmp(selection,"-name") == 0)
				{if(strcasecmp(dp->d_name,v) == 0)
					printf("Reg \t %s \n",fpath);
					}
					else if(strcasecmp(selection,"-mtime") == 0)
					{if(buff.st_mtime > (time(NULL) - w*86500))
					printf("Reg \t %s \n",dp->d_name);
					}
					else if(getpwnam(v)!=NULL){
					if(buff.st_uid == (getpwnam(v)->pw_uid))
					printf("Reg \t %s \n",fpath);}
					}
				else
				{
				if(strcasecmp(dp->d_name,v) == 0)
					printf("Oth \t %s \n",fpath);
					else if(mt > (time(NULL)- w*86500))
					printf("Oth \t %s \n",dp->d_name);
					else if(getpwnam(v)!=NULL){
					if(buff.st_uid == (getpwnam(v)->pw_uid))
					printf("Oth \t %s \n",fpath);}
					}
				
			}else {perror("No such File");continue;}
			free(fpath);
		}
		closedir(dirp);
	}
	return 0;
}
