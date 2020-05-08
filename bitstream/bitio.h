#include<stdio.h>
#ifndef BITIO_H
#define BITIO_H

struct BIT_FILE{
	FILE *file;
	unsigned char mask;
	int rack;
	int pacifier_count;
};

typedef struct BIT_FILE BIT_FILE;

#define PACIFIER_COUNT 2047

extern BIT_FILE *OpenOutputBitFile(const char *file_name);
extern BIT_FILE *OpenInputBitFile(const char *file_name);
extern void CloseOutputBitFile(BIT_FILE *bit_file);
extern void CloseInputBitFile(BIT_FILE *bit_file);
extern void OutputBit(BIT_FILE *bit_file,int bit);
extern void OutputBits(BIT_FILE *bit_file,unsigned long code,int bit_count);
extern int InputBit(BIT_FILE *bit_file);
extern unsigned long InputBits(BIT_FILE *bit_file,int bit_count);
extern void FilePrintBinary(FILE *file,unsigned long code,int bits);

#endif	//BITIO_H
