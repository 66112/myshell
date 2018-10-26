#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>

#define MAX 1024
#define NUM 16
int main()
{
	char* myargv[NUM];
	char buf[MAX];
	while(1){
	printf("[曹博兴@localhost mybash] ");
	fflush(stdout);
	int s=read(0,buf,sizeof(buf)-1);
	if(s>0){
		buf[s-1]=0;
	}
//	fgets(arg,sizeof(arg),stdin);     //从stdin输入，期望输入sizeof(arg),输入到arg中，返回实际输入的字符串长度
//	arg[strlen(arg)-1]='\0';
	char* ret=NULL;
	int i=0;
	myargv[i++]=strtok(buf," ");        //被切割字符串的首地址，以“ ”切割，调用成功，返回当前strtok的指针
	while(ret=strtok(NULL," "))  // 一直切割到文件末尾，返回NULL
	{
		myargv[i++]=ret;
	}
	myargv[i]=NULL;
	int st,j;
	pid_t id=fork();
	switch(id)
	{
		case -1:
			perror("fork:");
			break;
		case 0:
			for(j=0;myargv[j]!=NULL;j++){
				if(!strcmp(myargv[j],">")){
					const char* file=myargv[j+1];     //在子进程下找目标文件
					myargv[j]=NULL;                  //将 > 置成空
					close(1);                        //关闭标准输出
					int fd=open(file,O_WRONLY | O_CREAT,0644); //fd=1,以后子进程执行的结果就会输入到该文件中
					if(fd<0){
						perror("open>");
					}
					break;
				}
				if(!strcmp(myargv[j],">>")){
					const char* file=myargv[j+1];     //在子进程下找目标文件
					myargv[j]=NULL;                  //将 > 置成空
					close(1);                        //关闭标准输出
					int fd=open(file,O_WRONLY | O_CREAT | O_APPEND,0644); //fd=1,以后子进程执行的结果就会输入到该文件中
					if(fd<0){
						perror("open>");
					}
					break;
				}
				if(!strcmp(myargv[j],"<")){
					const char* file=myargv[j+1];     //在子进程下找目标文件
					myargv[j]=NULL;                  //将 > 置成空
					close(0);                        //关闭标准输出
					int fd=open(file,O_RDONLY); //fd=1,以后子进程执行的结果就会输入到该文件中
					if(fd<0){
						perror("open<");
					}
					break;
				}
			}
			execvp(myargv[0],myargv);      //一旦调用成功，后边的代码就不执行了
			//myargv[]里放的是命令行参数，ls -a -l
			break;
		default:
			while(wait(NULL)!=id);
	}
}
//	int j=0;
//	for(;j<i;j++)
//	{
//		printf("%s\n",myargv[j]);
//	}
	return 0;
}
