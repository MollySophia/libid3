#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "id3v2.h"
using namespace std;

int main(int argc,char **argv) {
  if(argc==1) {
    cout<<"Usage: mp3info file1 file2 ..."<<endl;
    return -1;
  }
  setlocale(LC_ALL,"");
  ID3v2 ID3;
  ID3v2_Header h;
  Frames f;
  for (int i=1;i<argc;++i) {
    fstream mp3file;
    mp3file.open(argv[i],ios::in|ios::binary);
    if(!mp3file) {
      cout<<"File "<<argv[i]<<" input error!"<<endl;
      mp3file.clear();
      continue;
    }
    cout<<argv[i]<<endl;
    mp3file.seekg(0,ios::end);
    cout<<"File size: "<<mp3file.tellg()/(1024.0*1024.0)<<" MBytes."<<endl;
    mp3file.seekg(0,ios::beg);
    if(ID3.GetHeader(&mp3file,&h)==-1) {
      cout<<"Not an mp3 file!"<<endl;
      continue;
    }
    ID3.GetFrames(&mp3file,&f);
    printf("Album: %ls ",f.TALB);
    cout<<endl;
    printf("Title: %ls ",f.TIT2);
    cout<<endl;
    printf("Arthur: %ls ",f.TPE1);
    cout<<endl;
    printf("Year: %s \n",f.TYER);
    printf("StyleCode: %s \n",f.TCON);
    fstream img;
    string imgf=argv[i];
    imgf.erase(imgf.size()-3,3);
    switch ((int)f.APIC.type) {
      case JPEG: {
        imgf+="jpg";
        break;
      }
      case PNG: {
        imgf+="png";
        break;
      }
      case BMP: {
        imgf+="bmp";
        break;
      }
      default:break;
    }
    if(f.APIC.type!=4) {
      img.open(imgf.c_str(),ios::out|ios::binary);
      img.write(f.APIC.data,f.APIC.size);
      img.close();
      cout<<"Cover image saved to: "<<imgf<<'.'<<endl;
    }
    cout<<endl<<endl;
    mp3file.close();
  }
  exit(0);
}
