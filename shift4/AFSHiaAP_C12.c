#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <pthread.h>

int key = 17;
char libs[]= "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
static const char *dirpath = "/home/claus/shift4";
void encode(char input[]);
void decode(char input[]);

static int xmp_getattr(const char *path, struct stat *stbuf)
{
  int res;
	char fpath[1000], trupath[1000]={0};
    
    if(strcmp(path, ".")!= 0 && strcmp(path,"..")!=0)
    {	
		strcpy(trupath, path);
        encode(trupath);
        sprintf(fpath,"%s%s",dirpath,trupath);
    }
    else
    {
        sprintf(fpath,"%s%s",dirpath,path);
    }
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else
	{	
		char temp[100];
		strcpy(temp,path);
		encode(temp);

		sprintf(fpath, "%s%s",dirpath,temp);
		
	}
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		char fname[1000]={0};
		
		struct stat st;
		memset(&st, 0, sizeof(st));
		
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		
		strcpy(fname, de->d_name);
		
		char fullpath[1000];
		sprintf(fullpath,"%s%s", fpath, de->d_name);
		if(strstr(fpath,youtuber) && de->d_type == DT_REG)
		{
			if(strcmp(fname, ".") && strcmp(fname,".."))
        		{	
           			decode(fname);
    				char npath[1000];
				sprintf(npath,"%s.iz1",fname);
				res = (filler(buf, npath, &st, 0));
				check(fullpath,npath);
       	 		}
		}
        		else
			{
				if(strcmp(fname, ".") != 0 && strcmp(fname, "..") != 0)
        			{
            				decode(fname);
					res = (filler(buf, fname, &st, 0));
					check(fullpath,fname);
					if(res!=0) break; 
        			}
			}
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  char fpath[1000];
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
	int res = 0;
  int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
        int res;
        char fpath[1000];
	char cpath[1000];
	char youtuber[1000]="/YOUTUBER";
	sprintf(cpath, "%s", path);
        sprintf(fpath, "%s%s", dirpath,cpath);

	if(strstr(fpath,youtuber))
	{
		res = mkdir(fpath,0750);
	}
	else
	{
		res = mkdir(fpath,mode);
	}

        if(res==-1)
        {
                return -errno;
        }
        return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	(void) fi;
        int res;
        char fpath[1000];
	char cpath[1000];
	char youtuber[1000]="/YOUTUBER";
	sprintf(cpath, "%s", path);
        sprintf(fpath, "%s%s", dirpath, cpath);

	if(strstr(fpath,youtuber))
	{
		res = creat(fpath,0640);
	}
	else
	{
		res = creat(fpath,mode);
	}

        if(res==-1)
        {
                return -errno;
        }
	close(res);
        return 0;
}

static int xmp_chmod (const char *path, mode_t mode)
{
        int res;
        char fpath[1000];
	char cpath[1000];
	char youtuber[1000]="/YOUTUBER";
	sprintf(cpath, "%s", path);
        sprintf(fpath, "%s%s", dirpath, cpath);

	if(strstr(fpath,youtuber))
	{
		int status;
		pid_t child1;
		child1= fork();
		if (child1==0)
		{
			char *argv[5] = {"zenity", "--error", "--title=Error", "--text=File ekstensi iz1 tidak boleh diubah permisionnya", NULL};
                	execv("/usr/bin/zenity", argv);
		}
		else
		{
			while(wait(&status)>0);
		}
	}
	else
	{
		res = chmod(fpath,mode);
	}

        if(res==-1)
        {
                return -errno;
        }
        return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fpath[1000];
	char cpath[1000];
	sprintf(cpath, "%s", path);
	sprintf(fpath,"%s%s", dirpath,path);
	res = lchown(fpath,uid,gid);
	if(res==-1)
	{
		return -errno;
	}
	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.mkdir      	= xmp_mkdir,
	.create     	= xmp_create,
	.chmod      	= xmp_chmod,
	.chown		= xmp_chown
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

void encode(char input[]){
	int now;
	int i; 
   	int j;

	for(i=0;i<strlen(input);i++){
		if(input[i]=='/')
			input[i]=input[i];
		
		else
            {
		    for(j=0;j<strlen(libs);j++){
			    if(input[i] == libs[j]){
				now = j + key ;
				break;
			}
		}
		if(now>strlen(libs))
		now = now - 94;
		
		input[i] = libs[now];}
	}
}

void decode(char input[]){
	int now;
	int i; 
    	int j;

	for(i=0;i<strlen(input);i++){
		if(input[i]=='/')
			input[i]=input[i];
		
		else
            {
		    for(j=0;j<strlen(libs);j++){
			    if(input[i] == libs[j]){
				now = j - key ;
				break;
			}
		}
		if(now<0)
		now = now + 94;
		
		input[i] = libs[now];}
	}
}

void check(const char *path, const char *cpath)
{
	char copy[1000];
	sprintf(copy,"%s%s",dirpath,path);
	struct stat file;
   	lstat(path,&file);
    	struct group *grp;
    	struct passwd *pwd;

   	grp = getgrgid(file.st_uid);
    	pwd = getpwuid(file.st_gid);

    	if ((strcmp(grp->gr_name, "rusak")!=0 && (strcmp(pwd->pw_name, "chipset")!=0 || !strcmp(pwd->pw_name, "ic_controller")!=0) && access(path,R_OK) !=0))
	{
		char filemiris[]="/filemiris.txt";
		char newfile[1000];
		sprintf(newfile,"%s%s",dirpath,filemiris);
        	FILE *out_file = fopen(newfile,"a");
        	fprintf(out_file,"%s %d %d %ld %s %s\n", cpath, file.st_uid, file.st_gid, file.st_atime, grp->gr_name, pwd->pw_name);
		fclose(out_file);
        	remove(path);
    	}
}
