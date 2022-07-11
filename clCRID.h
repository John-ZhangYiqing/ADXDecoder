#pragma once

//--------------------------------------------------
// include
//--------------------------------------------------
#include "clUTF.h"

//--------------------------------------------------
// CRID class
//--------------------------------------------------
class clCRID{
public:
	clCRID(unsigned int ciphKey1=0x207DFFFF,unsigned int ciphKey2=0x00B8F21B);

	// ロード/開放
	static bool CheckFile(void *data,unsigned int size);
	bool LoadFile(const char *filename);

	// 分離/マルチプレクサ
	bool Demux(const char *filename,const char *directory,bool adxDecode=true);
	bool Mux(const char *filename,const char *filenameMovie,const char *filenameAudio);

	// 取得
	unsigned int GetFileCount(void){return _utf.GetPageCount();}
	const char *GetFilename(unsigned int index){return _utf.GetElement(index,"filename")->GetValueString();}

private:
	struct stInfo{
		unsigned int signature;      // signature 'CRID'
		unsigned int dataSize;       // data Size
		unsigned char r08;           // unkonw(0)
		unsigned char dataOffset;    // data offset
		unsigned short paddingSize;  // padding Size
		unsigned char r0C;           // unkonw(0)
		unsigned char r0D;           // unkonw(0)
		unsigned char r0E;           // unkonw(0)
		unsigned char dataType:2;    // data type 0:Data 1:UTF(meta info) 2:Comment 3:UTF(seek info)
		unsigned char r0F_1:2;       // unkonw(0)
		unsigned char r0F_2:4;       // unkonw(0)
		unsigned int frameTime;      // frame time(0.01s unit)
		unsigned int frameRate;      // Frame rate(0.01fps unit)
		unsigned int r18;            // unkonw(0)
		unsigned int r1C;            // unkonw(0)
	};
	clUTF _utf;
	unsigned char _videoMask1[0x20];
	unsigned char _videoMask2[0x20];
	unsigned char _audioMask[0x20];
	void InitMask(unsigned int key1,unsigned int key2);
	void MaskVideo(unsigned char *data,int size);
	void MaskAudio(unsigned char *data,int size);
	static void WriteInfo(FILE *fp,const char *string);
};
