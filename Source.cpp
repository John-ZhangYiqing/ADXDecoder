
//--------------------------------------------------
// Include
//--------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <direct.h>  // _mkdir
#include "clCRID.h"
#ifndef _countof
#define _countof(_array) (sizeof(_array)/sizeof(_array[0]))
#endif

//--------------------------------------------------
// Converts string to hex number
//--------------------------------------------------
int atoi16(const char *s){
	int r=0;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r=(r<<4)|(*s-'0');
		else if(*s>='A'&&*s<='F')r=(r<<4)|(*s-'A'+10);
		else if(*s>='a'&&*s<='f')r=(r<<4)|(*s-'a'+10);
		else break;
		s++;
	}
	return sign?-r:r;
}

//--------------------------------------------------
// Get directory
//--------------------------------------------------
char *GetDirectory(char *directory,int size,const char *path){
	if(size>0)directory[0]='\0';
	for(int i=strlen(path)-1;i>=0;i--){
		if(path[i]=='\\'){
			if(i>size-1)i=size-1;
			memcpy(directory,path,i);
			directory[i]='\0';
			break;
		}
	}
	return directory;
}

//--------------------------------------------------
// directory create
//--------------------------------------------------
bool DirectoryCreate(const char *directory){

	// check
	if(!(directory&&*directory))return false;

	// Relative path(directory name only)
	if(!(strchr(directory,'\\')||strchr(directory,'/'))){
		return _mkdir(directory)==0;
	}

	// Directory name check
	if(directory[1]!=':'||directory[2]!='\\')return false;  // Check drive description
	if(!directory[3])return false;                          // Description check other than drive
	if(strpbrk(directory+3,"/,:;*<|>\""))return false;      // Checking directory prohibited characters
	if(strstr(directory,"\\\\"))return false;               // Checking directory prohibited characters
	if(strstr(directory," \\"))return false;                // Checking the '\' sign after a space

	// Directory creation
	if(_mkdir(directory)){
		char current[0x400];
		if(!GetDirectory(current,_countof(current),directory))return false;
		if(!DirectoryCreate(current))return false;
		if(_mkdir(directory))return false;
	}

	return true;
}

//--------------------------------------------------
// main
//--------------------------------------------------
int main(int argc,char *argv[]){

	// CLI analysis
	unsigned int count=0;
	char *filenameOut=NULL;
	unsigned int ciphKey1=0x207DFFFF;
	unsigned int ciphKey2=0x00B8F21B;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'||argv[i][0]=='/'){
			switch(argv[i][1]){
			case 'o':if(i+1<argc){filenameOut=argv[++i];}break;
			case 'a':if(i+1<argc){ciphKey1=atoi16(argv[++i]);}break;
			case 'b':if(i+1<argc){ciphKey2=atoi16(argv[++i]);}break;
			}
		}else if(*argv[i]){
			argv[count++]=argv[i];
		}
	}

	// force check
	if(!count){
		printf("Error: Please specify the input file。\n");
		return -1;
	}

	// seperate
	for(unsigned int i=0;i<count;i++){

		// The output file name option is invalid for the secondand subsequent files.
		if(i)filenameOut=NULL;

		// Default output file name
		char path[0x400];
		if(!(filenameOut&&filenameOut[0])){
			strcpy_s(path,sizeof(path),argv[i]);
			char *d1=strrchr(path,'\\');
			char *d2=strrchr(path,'/');
			char *e=strrchr(path,'.');
			if(e&&d1<e&&d2<e)*e='\0';
			strcat_s(path,sizeof(path),".demux");
			filenameOut=path;
		}

		printf("%s seperate...\n",argv[i]);
		DirectoryCreate(filenameOut);
		clCRID crid(ciphKey1,ciphKey2);
		if(!crid.Demux(argv[i],filenameOut,true)){
			printf("Error: seperate failed。\n");
		}

	}

	return 0;
}
