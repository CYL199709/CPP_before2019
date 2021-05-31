#pragma pack(1)
typedef struct BMPHeader
{
	unsigned char bfType[2];//�ļ���ʽ
	unsigned long bfSize;//�ļ���С
	unsigned short bfReserved1;//����
	unsigned short bfReserved2;
	unsigned long bfOffBits; //DIB�������ļ��е�ƫ����
}Header;
typedef struct BMPInfo
{
	unsigned long biSize;//�ýṹ�Ĵ�С
	long biWidth;//�ļ����
	long biHeight;//�ļ��߶�
	unsigned short biPlanes;//ƽ����
	unsigned short biBitCount;//��ɫλ��
	unsigned long biCompression;//ѹ������
	unsigned long biSizeImage;//DIB��������С
	long biXPixPerMeter;
	long biYPixPerMeter;
	unsigned long biClrUsed;//������ɫ������
	unsigned long biClrImporant;//������Ҫ��ɫ
}Info;
typedef struct RGBQuad
{
	unsigned char rgbBlue; //��ɫ��������
	unsigned char rgbGreen;//��ɫ��������
	unsigned char rgbRed;//��ɫ��������
	unsigned char rgbReserved;
}Quad;
#pragma pack()
typedef struct{
	unsigned char uch;
	unsigned long weight;
}TmpNode;
typedef struct Node{
	unsigned char uch;
	unsigned long weight;
	char *code;						// �ַ���Ӧ�Ĺ���������
	int parent, lchild, rchild;
	Node(){parent=0;}
} HufNode,*HufTree;
class Compressor
{
private:
	unsigned char RGBData[4000][3];
	unsigned char GrayData[4000];
	FILE * fpBMP,* fpGray;
	Header * header;
	Info * info;
	Quad * quad;
public:
	int BmpToGrayR(const char *,const char *);
	int BmpToGrayG(const char *,const char *);
	int BmpToGrayB(const char *,const char *);
	int BmpToGrayC(const char *,const char *);
	int decode(const char *,const char *);
	int encode(const char *,const char *);
	int openFile(const char *,const char *);
	void select(HufNode*,unsigned int,int *);
	void createTree(HufNode *,unsigned int,unsigned int);
	void createCode(HufNode *,unsigned);
	void  closeFile();
};
