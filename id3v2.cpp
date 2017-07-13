#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "id3v2.h"
using namespace std;
int ID3v2::GetHeader(fstream *file,ID3v2_Header* header) {
  file->read(header->Header,3);
  if(strcmp(header->Header,"ID3")!=0) return -1;
  file->read((char*)&header->Ver,1);
  file->read((char*)&header->Revision,1);
  file->read((char*)&header->Flag,1);
  char sizecode[4];
  file->read(sizecode,sizeof(sizecode));
  header->Size = HeaderSize(sizecode);
  return 0;
}

int ID3v2::GetFrames(fstream *file,Frames* f) {
  while(!file->eof()) {
    char s[4],ID[5];
    memset(ID,0,sizeof(ID));
    memset(s,0,sizeof(s));
    file->read(ID,4);
    if(CheckFrameID(ID)==false) return 0;
    file->read(s,4);
    file->seekg(2,ios::cur); //skip flag bits
    int Size = FrameSize(s);
    if(!strcmp(ID,"TIT2")||(!strcmp(ID,"TPE1"))||(!strcmp(ID,"TALB"))) {
      Size-=3;
      char code;
      file->read(&code,1);
      file->seekg(2,ios::cur);
      switch(code) {
        case ISO: {
          char data[Size+3];
          memset(data,0,sizeof(data));
          file->seekg(-2,ios::cur);
          file->read(data,Size+2);
          wchar_t *buf = new wchar_t[Size+3];
          memset(buf,0,sizeof(buf));
          for(int i=0;i<Size+3;++i)
            buf[i] = data[i]&0xff;
          if(!strcmp(ID,"TIT2"))
            f->TIT2 = buf;
          else if(!strcmp(ID,"TPE1"))
            f->TPE1 = buf;
          else if(!strcmp(ID,"TALB"))
            f->TALB = buf;
					break;
				}
				case UTF16:case UTF16BE: {
          char data[Size+1];
          wchar_t *buf = new wchar_t[Size/2+1];
          memset(buf,0,sizeof(buf));
          memset(data,0,sizeof(data));
          file->read(data,Size);
          for(int i=0;i<Size/2;++i) {
              buf[i] = (data[2*i+1]<<8 &0xff00) | (data[2*i]&0xff);
              if(buf[i]==0xa0) buf[i] = 0x20;
          }
          buf[Size/2] = 0x0;
          if(!strcmp(ID,"TIT2"))
            f->TIT2 = &buf[0];
          else if(!strcmp(ID,"TPE1"))
            f->TPE1 = &buf[0];
          else if(!strcmp(ID,"TALB"))
            f->TALB = &buf[0];
					break;
				}
				case UTF8: { //todo
					break;
        }
      }
    }
    else
    if(!strcmp(ID,"TYER")||!strcmp(ID,"COMM")||!strcmp(ID,"TCON")) {
      char *data = new char[Size];
      file->seekg(1,ios::cur);
      file->read(data,Size-1);
      data[Size-1]=0;
      if(!strcmp(ID,"TYER"))
        f->TYER = data;
      else if(!strcmp(ID,"COMM"))
        f->COMM = data;
      else if(!strcmp(ID,"TCON"))
        f->TCON = data;
    }
    else
    if(!strcmp(ID,"APIC")) {
      Size+=256;
      f->APIC.type=4;
      char b[Size];
      file->read(b,sizeof(b));
      int pos=0;
      for(pos=0;pos<=30;++pos) {
        if((b[pos]&0xff)==0xff&&((b[pos+1]&0xff)==0xd8)) { //JPEG
          f->APIC.type=JPEG;
          break;
        }
        if((b[pos]&0xff)==0x89&&((b[pos+1]&0xff)==0x50)
          &&((b[pos+2]&0xff)==0x4e)&&((b[pos+3]&0xff)==0x47)) {
          f->APIC.type=PNG;
          break;
        }
        if((b[pos]&0xff)==0x42&&((b[pos+1])&0xff)==0x4d) {
          f->APIC.type=BMP;
          break;
        }
      }
      f->APIC.data=new char[Size-pos-1];
      f->APIC.size=Size-pos-1;
      for(int i=0;i<Size-pos-1;++i)
        f->APIC.data[i]=b[i+pos]&0xff;
    }
    else { //threw away
      file->seekg(Size,ios::cur);
    }
  }
  return 0;
}

bool ID3v2::CheckFrameID(char *ID) {
  for(int i=0;i<4;++i)
    if((ID[i]<'A'||ID[i]>'Z')&&(ID[i]<'0'||ID[i]>'9'))
      return false;
  return true;
}

int ID3v2::HeaderSize(char *a) {
  int sizecode[4];
  for(int i=0;i<4;++i) {
    sizecode[i]=0xff&a[i];
  }
  return (sizecode[0]&0x7F)*0x200000 + (sizecode[1]&0x7F)*0x400
  		+ (sizecode[2]&0x7F)*0x80 +(sizecode[3]&0x7F);
}

int ID3v2::FrameSize(char *a) {
  int sizecode[4];
  for(int i=0;i<4;++i) {
    sizecode[i]=0xff&a[i];
  }
  return sizecode[0]*0x100000000 + sizecode[1]*0x10000+
    sizecode[2]*0x100 + sizecode[3];
}
