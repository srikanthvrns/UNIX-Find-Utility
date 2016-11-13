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

int visitDir(char *a);
unsigned int ftype;
int main(int argc,char *argv[])
{
if(argc==1)
	{perror("Arguments Needed"); exit(2);}
	struct stat buf;
	lstat(argv[1], &buf);
	ftype = buf.st_mode & 0xF000;
	printf("ftype=%x\n", ftype);
	if (ftype == S_IFDIR) {printf ("Dir \t %s \n",argv[1]);
		visitDir(argv[1]);}
	else if (ftype == S_IFREG) {printf ("reg \t %s \n",argv[1]);}
	else
	printf("Oth- \t %s \n",argv[1]);
	return 0;
}

int visitDir(char *arg)
{
	char *fpath;
	DIR *dirp;
	struct dirent *dp;
	//time_t mt; //mode_t s_m;
	struct stat buff;
	if((dirp = opendir(arg))==NULL)
	{perror("\n");}
	else{              
		while ((dp = readdir(dirp)) != NULL) 
		{
			if(strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..")==0)
			continue; 
			//printf( "\nfilename \t%s \t", dp->d_name);
			fpath=malloc(strlen(arg)+strlen(dp->d_name)+2);
			strcpy(fpath,arg);
			strcat(fpath,"/");
			strcat(fpath,dp->d_name);
			if(lstat(fpath, &buff)==0)//fd = open (dp->d_name, O_RDWR);
			{ 
				ftype = buff.st_mode & 0xF000;
				if (ftype == S_IFDIR)
				{printf("Dir \t %s \n",dp->d_name);
					if(buff.st_mode & S_IEXEC)
						visitDir(fpath);
					else
					perror("");	//printf("%s",fpath); 
					
					//}
				}
				else if (ftype == S_IFREG && buff.st_mode & S_IRUSR) {printf("Reg \t %s \n",dp->d_name);}
				else printf("Oth \t %s \n",dp->d_name);	
				//s_m = ;
				//if (s_m){
				//mt = buff.st_mtime;
				//printf("Last modification time= %s \n",ctime(&mt));               
				//}else {printf("%s\n",fpath);perror("");}
			}else {perror("yo");continue;}
			free(fpath);
		}
		closedir(dirp);}
	return 0;
}
