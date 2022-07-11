#pragma once

//--------------------------------------------------
// include
//--------------------------------------------------
#include <stdio.h>

//--------------------------------------------------
// ADX class
//--------------------------------------------------
class clADX{
public:
	clADX();
	~clADX();

	// check
	static bool CheckFile(void *data);

	// decode
	bool Decode(const char *filename,const char *filenameWAV);
	bool Decode(FILE *fp,void *data,int size,unsigned int address);

private:
	struct stHeader{
		unsigned short signature;    // signature 0x8000
		unsigned short dataOffset;   // Data offset(header size) - 4
		unsigned char r04;           // version? 3
		unsigned char r05;           // block size？ 18
		unsigned char r06;           // ？ 4
		unsigned char channelCount;  // number of channels
		unsigned int samplingRate;   // sampling rate
		unsigned int sampleCount;    // total number of samples
		unsigned char r10;
		unsigned char r11;
		unsigned char r12;
		unsigned char r13;
		unsigned int r14;
		unsigned short r18;
		unsigned short r1A;
		unsigned short r1C;
		unsigned short r1E;
	};
	struct stInfo{//channel Count >= 3 (channelCount-2) back to existence
		unsigned short r00;
		unsigned short r02;
	};
	struct stAINF{
		unsigned int ainf;// 'AINF'
		unsigned int r04;
		unsigned char r08[0x10];
		unsigned short r18;
		unsigned short r1A;
		unsigned short r1C;
		unsigned short r1E;
	};
	stHeader _header;
	int *_data;
	static void Decode(int *d,unsigned char *s);
};
