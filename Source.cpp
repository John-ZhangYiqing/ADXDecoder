
//--------------------------------------------------
// �C���N���[�h
//--------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "clADX.h"

//--------------------------------------------------
// �������10�i���Ƃ݂Ȃ��Đ��l�ɕϊ�(�ȈՔ�)
//--------------------------------------------------
int atoi(const char *s){
	int r=0;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r=r*10+(*s-'0');
		else break;
		s++;
	}
	return sign?-r:r;
}
float atof(const char *s){
	int r1=0,r2=0,c=1;
	bool sign=false;if(*s=='+'){s++;}else if(*s=='-'){sign=true;s++;}
	while(*s){
		if(*s>='0'&&*s<='9')r1=r1*10+(*s-'0');
		else break;
		s++;
	}
	if(*s=='.'){
		s++;
		while(*s){
			if(*s>='0'&&*s<='9'){r2=r2*10+(*s-'0');c*=10;}
			else break;
			s++;
		}
	}
	float r=r1+((c>0)?r2/(float)c:0);
	return sign?-r:r;
}

//--------------------------------------------------
// �������16�i���Ƃ݂Ȃ��Đ��l�ɕϊ�
//--------------------------------------------------
long long int atoi16(const char *s){
	long long int r=0;
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
// ���C��
//--------------------------------------------------
int main(int argc,char *argv[]){

	// �R�}���h���C�����
	unsigned int count=0;
	char *filenameOut=nullptr;
	//bool decodeFlg=false;
	unsigned long long int ciphKeyEx=0;
	unsigned long long int ciphKey=0xCF222F1FE0748978;
	char *ciphKeyword=nullptr;
	int mode=16;
	int loop=0;
	bool info=false;
	bool decrypt=false;
	int analyzeKeyLevel=0;
	int toCiphKeyEx=0;
	for(int i=1;i<argc;i++){
		if(argv[i][0]=='-'||argv[i][0]=='/'){
			switch(argv[i][1]){
			case 'o':if(i+1<argc){filenameOut=argv[++i];}break;
			//case 'd':decodeFlg=true;break;
			case 'k':if(i+1<argc){ciphKeyword=argv[++i];}break;
			case 'a':if(i+1<argc){ciphKeyEx=atoi16(argv[++i]);}break;
			case 'b':if(i+1<argc){ciphKey=atoi16(argv[++i]);}break;
			case 'm':if(i+1<argc){mode=atoi(argv[++i]);}break;
			case 'l':if(i+1<argc){loop=atoi(argv[++i]);}break;
			case 'i':info=true;break;
			case 'c':decrypt=true;break;
			case 'x':if(i+1<argc){analyzeKeyLevel=atoi(argv[++i]);if(analyzeKeyLevel<=0)analyzeKeyLevel=1;}break;
			case 'y':if(i+1<argc){toCiphKeyEx=atoi(argv[++i]);if(toCiphKeyEx<=0)toCiphKeyEx=1;}break;
			}
		}else if(*argv[i]){
			argv[count++]=argv[i];
		}
	}

	//if(decodeFlg){

	// ���̓`�F�b�N
	if(!count){
		printf("Error: ���̓t�@�C�����w�肵�Ă��������B\n");
		return -1;
	}

	// �f�R�[�h
	for(unsigned int i=0;i<count;i++){

		// 2�ڈȍ~�̃t�@�C���́A�o�̓t�@�C�����I�v�V����������
		if(i)filenameOut=nullptr;

		// �f�t�H���g�o�̓t�@�C����
		char path[0x400];
		if(!(filenameOut&&filenameOut[0])){
			strcpy_s(path,sizeof(path),argv[i]);
			char *d1=strrchr(path,'\\');
			char *d2=strrchr(path,'/');
			char *e=strrchr(path,'.');
			if(e&&d1<e&&d2<e)*e='\0';
			if(analyzeKeyLevel>0)strcat_s(path,sizeof(path),".txt");
			else strcat_s(path,sizeof(path),".wav");
			filenameOut=path;
		}

		// �w�b�_���̂ݕ\��
		if(info){
			printf("%s �̃w�b�_���\n",argv[i]);
			clADX adx(0,0,nullptr);
			adx.PrintInfo(argv[i]);
			printf("\n");
		}

		// ������
		else if(decrypt){
			printf("%s �𕜍�����...\n",argv[i]);
			clADX adx(ciphKeyEx,ciphKey,ciphKeyword);
			if(!adx.Decrypt(argv[i])){
				printf("Error: �������Ɏ��s���܂����B\n");
			}
		}

		// ���ꌮ���
		else if(analyzeKeyLevel>0){
			clADX adx(0,0,nullptr);
			if(!adx.AnalyzeKey(argv[i],filenameOut,analyzeKeyLevel)){
				printf("Error: ��͂Ɏ��s���܂����B\n");
			}
		}

		// ���ꌮ�ɕϊ�
		else if(toCiphKeyEx>0){
			clADX adx(0,0,nullptr);
			switch(toCiphKeyEx){
			case 1:if(!adx.ToCiphKeyEx(ciphKeyword,&ciphKeyEx)){printf("Error: �ϊ��Ɏ��s���܂����B\n");}break;
			case 2:if(!adx.ToCiphKeyEx(ciphKey,&ciphKeyEx)){printf("Error: �ϊ��Ɏ��s���܂����B\n");}break;
			}
			printf("%012llX\n",ciphKeyEx);
		}

		// �f�R�[�h
		else{
			printf("%s ���f�R�[�h��...\n",argv[i]);
			clADX adx(ciphKeyEx,ciphKey,ciphKeyword);
			if(!adx.DecodeToWavefile(argv[i],filenameOut,mode,loop)){
				printf("Error: �f�R�[�h�Ɏ��s���܂����B\n");
			}
		}

	}

	//}

	return 0;
}
