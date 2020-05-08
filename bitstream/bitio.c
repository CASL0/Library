#include<stdio.h>
#include<stdlib.h>
#include "bitio.h"

BIT_FILE *OpenOutputBitFile(const char *file_name){
	BIT_FILE *bit_file;
	FILE *fp;
	if((fp=fopen(file_name,"wb"))==NULL)
		return NULL;
	bit_file=(BIT_FILE*)malloc(sizeof(BIT_FILE));
	if(bit_file==NULL) 
		return NULL;
	bit_file->file=fp;
	bit_file->mask=0x80;
	bit_file->rack=0;
	bit_file->pacifier_count=0;
	return bit_file;
}

BIT_FILE *OpenInputBitFile(const char *file_name){
	BIT_FILE *bit_file;
	FILE *fp;
	if((fp=fopen(file_name,"rb"))==NULL)
		return NULL;
	bit_file=(BIT_FILE*)malloc(sizeof(BIT_FILE));
	if(bit_file==NULL)
		return NULL;
	bit_file->file=fp;
	bit_file->mask=0x80;
	bit_file->rack=0;
	bit_file->pacifier_count=0;
	return bit_file;
}

void CloseOutputBitFile(BIT_FILE *bit_file){
	//1バイト分書き込んでいる途中の場合
	if(bit_file->mask!=0x80)
		//バッファrackの中身をすべて書き込む
		if(putc(bit_file->rack,bit_file->file)!=bit_file->rack)	
			fprintf(stderr,"Fatal error in CloseBitFile\n");
	fclose(bit_file->file);
	free(bit_file);
}

void CloseInputBitFile(BIT_FILE *bit_file){
	fclose(bit_file->file);
	free(bit_file);
}

void OutputBit(BIT_FILE *bit_file,int bit){
	if(bit)
		//ビットが立っていたらバッファのビットを立てる
		bit_file->rack|=bit_file->mask;
	bit_file->mask>>=1;
	if(bit_file->mask==0){
		//1バイト分バッファに書き込んだらそれをファイルに書き込む
		if(putc(bit_file->rack,bit_file->file)!=bit_file->rack)
			fprintf(stderr,"Fatal error in OutputBit\n");
		//PACIFIER_COUNTバイト分読み込んだら進捗バーを表示する
		else if((bit_file->pacifier_count++ & PACIFIER_COUNT)==0)
			printf("#");
		bit_file->rack=0;
		bit_file->mask=0x80;
	}
}

void OutputBits(BIT_FILE *bit_file,unsigned long code,int bit_count){
	unsigned long mask=(unsigned)1L<<(bit_count-1);
	while(mask!=0){
		if(mask & code)
			bit_file->rack|=bit_file->mask;
		bit_file->mask>>=1;
		if(bit_file->mask==0){
			//1バイト分バッファに書き込んだらそれをファイルに書き込む
			if(putc(bit_file->rack,bit_file->file)!=bit_file->rack)
				fprintf(stderr,"Fatal error in OutputBits\n");
			//PACIFIER_COUNTバイト読み込んだら進捗バーを表示する
			else if((bit_file->pacifier_count++ & PACIFIER_COUNT)==0)
				printf("#");
			bit_file->rack=0;
			bit_file->mask=0x80;
		}
		mask>>=1;	
	}
}

int InputBit(BIT_FILE *bit_file){
	if(bit_file->mask==0x80){
		//1バイト分読み込み終わっていたら新しく読み込む
		bit_file->rack=getc(bit_file->file);
		if(bit_file->rack==EOF)
			fprintf(stderr,"Fatal error in InputBit\n");
		else if((bit_file->pacifier_count++ & PACIFIER_COUNT)==0)
			printf("#");
	}
	int is_bit=bit_file->rack & bit_file->mask;
	bit_file->mask>>=1;
	if(bit_file->mask==0) 
		bit_file->mask=0x80;
	return (is_bit?1:0);
}

unsigned long InputBits(BIT_FILE *bit_file,int bit_count){
	unsigned long mask=(unsigned long)1<<(bit_count-1);
	unsigned long input_binary=0;
	while(mask!=0){
		if(InputBit(bit_file)){
			//ビットが立っていたら
			input_binary|=mask;
		}
		mask>>=1;
	}
	return input_binary;
}

void FilePrintBinary(FILE *file,unsigned long code,int bits){
	unsigned int mask=1<<(bits-1);
	while(mask!=0){
		if(code&mask)
			fputc('1',file);
		else
			fputc('0',file);
		mask>>=1;
	}

}
