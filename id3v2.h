#ifndef _ID3V2_
#define _ID3V2_

using namespace std;
/****************************/
#define ISO 0
#define UTF16 1
#define UTF16BE 2
#define UTF8 3
/****************************/
#define JPEG 0
#define PNG 1
#define BMP 2
/****************************/
struct ID3v2_Header
{
	ID3v2_Header()
	{
		memset(Header,0,sizeof(Header));
		Ver=0;
		Revision=0;
		Flag=0;
		Size=0;
	}
	char Header[4];
	int Ver;
	int Revision;
	int Flag;
	int Size;
};
struct Frames {
	Frames() {
		TIT2 = NULL;
		TPE1 = NULL;
		TALB = NULL;
		TLAN = NULL;
		TYER = NULL;
		TCON = NULL;
		COMM = NULL;
	}
  wchar_t *TIT2; //Title
  wchar_t *TPE1; //Arthur
  wchar_t *TALB; //Album
  wchar_t *TLAN; //Language
  struct _TRAK
  {
		_TRAK() {
			Num = 0;
			AlbumNum = 0;
		}
    int Num;
    int AlbumNum;
  }TRAK; //Unknown?
  char *TYER; //Year
  char *TCON; //Style
  char *COMM;
	struct _APIC {
		_APIC() {
			type=-1;
		}
		char *data;
		char type;
		int size;
	}APIC;
};

class ID3v2 {
public:
  int GetHeader(fstream*,ID3v2_Header*);
  int GetFrames(fstream*,Frames*);
private:
  int HeaderSize(char*);
  int FrameSize(char*);
  bool CheckFrameID(char*);
};

#endif
