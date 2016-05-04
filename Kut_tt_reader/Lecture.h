#ifndef _LECTURE_H_
#define _LECTURE_H_

#include <windows.h>

typedef unsigned short u_srt;
typedef int TIME_INFO;
#define MON		0x80000000
#define TUS		0x40000000
#define WED		0x20000000
#define THU		0x10000000
#define FRI		0x08000000
#define SAT		0x04000000
#define t01A	0x00000001
#define t01B	0x00000002
#define t02A	0x00000004
#define t02B	0x00000008
#define t03A	0x00000010
#define t03B	0x00000020
#define t04A	0x00000040
#define t04B	0x00000080
#define t05A	0x00000100
#define t05B	0x00000200
#define t06A	0x00000400
#define t06B	0x00000800
#define t07A	0x00001000
#define t07B	0x00002000
#define t08A	0x00004000
#define t08B	0x00008000
#define t09A	0x00010000
#define t09B	0x00020000
#define t10A	0x00040000
#define t10B	0x00080000
#define t11A	0x00100000
#define t11B	0x00200000
#define t12A	0x00400000
#define t12B	0x00800000
#define tovr_12 0x01000000

#define IS_DAY_VALIED(day) (day & 0xFC000000)
#define GET_ONLY_TIME(day) (day & 0x01FFFFFF)

#define TI_COUNT	25

const int tt_inf[TI_COUNT] = {t01A, t01B, t02A, t02B, t03A, t03B, t04A, t04B, t05A, t05B,
								t06A, t06B, t07A, t07B, t08A, t08B, t09A, t09B, t10A, t10B,
								t11A, t11B, t12A, t12B, tovr_12};

/*
#define MAJOR_COUNT 21	// ���� ����

#define car_eng 0		// �ڿ�
#define com_prat 1		// ����
#define inf_tech 2		// IT
#define prod_sys 3		// ����
#define dig_sys 4		// ���
#define	con_sys 5		// ����
#define	electron 6		// ����
#define	elrock 7 		// ����
#define	el_tele 8		// ����
#define	com_tech 9		// �İ�
#define	multi_media 10	// ��Ƽ
#define	intern_soft 11	// ����
#define	inf_protect 12	// ��ȣ
#define	design_tech 13	// ���
#define	construct_tech 14	// ���
#define	construct_edu 15	// ����
#define	chemi_eng 16	// ����
#define	new_metrial 17	// �Ű�
#define	hum_man 18		// �η�
#define	tech_man 19		// ���
#define	e_bz 20			// e����


const WCHAR MajorName[MAJOR_COUNT][5] = {L"�ڿ�", L"����", L"IT", L"����", L"���", L"����", 
							L"����", L"����",
							L"����", L"�İ�", L"��Ƽ", L"����", L"��ȣ", L"���", L"���", L"����",
							L"����", L"�Ű�", L"�η�", L"���", L"eBiz"};
*/

// 2010�� 1�б���� �ٲ� �� ������ ���� ����
/*
const WCHAR MajorName[MAJOR_COUNT][5] = {L"�ڿ�", L"����", L"IT��", L"����", L"���", L"����", 
							L"����", L"����", L"����",
							L"�İ�", L"�ι�", L"��Ƽ", L"����", L"��ȣ", L"��HW", L"��SW", L"�Ľ�",
                            L"����", L"����", L"����", L"�İ�",
                            L"�Ű�", L"��ȭ", L"����", 
							L"����", L"���", L"����",
							L"����", L"�η�", L"���", L"eBiz"};									
*/

//2013�⵵ 1�б������ 2012�� 12�� 17�� ���� 10�� ���� �������׿� �ö�� xls ���� ����

#define MAJOR_COUNT 30

const WCHAR MajorName[MAJOR_COUNT][12] = {                            
                            L"�ڵ���������������",
                            L"��ǻ����������",
                            L"IT��������",
                            L"����ý�������",
                            L"�����нý�������",
                            L"����ý�������",
                            L"�����������",
                            L"���ڰ�������",
                            L"������Ű�������",
                            L"�����������",
                            L"���ڰ�������",
                            L"������Ű�������",
                            L"��ǻ�Ͱ�������",
                            L"��ǻ��HW����",
                            L"��ǻ��SW����",
                            L"��ǻ�ͽý�����������",
                            L"��Ƽ�̵������",
                            L"���ͳݰ�������",
                            L"������ȣ��������",
                            L"�ż����������",
                            L"����ȭ�а�������",
                            L"��������������",
                            L"�ż�����а�",
                            L"����ȭ�а��а�",
                            L"�����ΰ��а�",
                            L"�����������",
                            L"����������",
                            L"�η°濵����",
                            L"����濵����",
                            L"EBusiness����"};

////////////////////////////////////////// �̼�����
////////////////////////////////////////// Ȧ���� �ʼ�, ¦���� �����̴�
////////////////////////////////////////// �о���̸鼭 �Ϲݼ����� 11���� 12�� ��ȯ�ϵ��� �Ѵ�
#define CT_COUNT		15

#define CT_sp_cul_req	01	// ���������ʼ�
#define CT_sp_cul_sel	02	// �������缱��
#define CT_HRD_req		03	// HRD�ʼ�
#define CT_HRD_sel		04	// HRD����
#define CT_tech_req	05	// ���б����ʼ�
#define CT_tech_sel	06	// ���б��ʼ���
#define CT_depart_req	07	// �к��ʼ� 
#define CT_depart_sel	08	// �кμ���
#define CT_major_req	09	// �����ʼ�
#define CT_major_sel	10	// ��������
//#define CT_norm_sel	11	// �Ϲݼ���(*)
#define CT_norm_dummy	11	
#define CT_norm_sel	12	// �Ϲݼ���
#define CT_MSC_req		13	// MSC�ʼ�
#define CT_MSC_sel		14	// MSC���� 
#define CT_MULTI_UNMAJOR	0x80000000	// ����/������ ����/�������ش�������� S�� ǥ��
#define CT_MDTEACHER_LECTURE	0x40000000 // T�� �ߵ��������.

#define CT__ONLYTYPE(dat) (((dat)<<2)>>2)

const WCHAR CTName[CT_COUNT][13] = {L"(null)", L"���������ٽ�", L"���������Ϲ�", L"HRD�ٽ�", L"HRD�Ϲ�",
								L"���б����ٽ�", L"���б����Ϲ�", L"�кΰ����ٽ�", L"�кΰ����Ϲ�",
								L"�а�(����)�ٽ�", L"�а�(����)�Ϲ�", L"(null)", L"��������",
								L"MSC�ٽ�", L"MSC�Ϲ�"};

//#define DEP_COUNT	10
//#define YEAR_COUNT	48
#define DAY_COUNT	6
#define TT_COUNT	15
/*
//									��ü  ����  ����  ���  ��ī  ���  �ż�  ��ȭ  �ι�  ����
const char dep_inf[DEP_COUNT][3] = {"��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};
const char year_inf[YEAR_COUNT][3] = {"��", "", "", "", "", "", "", "", 
							"1A", "1B", "1C", "1D", "1E", "", "", "1\t",
							"2A", "2B", "2C", "2D", "", "", "", "2\t",
							"3A", "3B", "3C", "3D", "", "", "", "3\t",
							"4A", "4B", "4C", "4D", "", "", "", "4\t",
							"",   "",   "",   "",   "", "", "", "5\t"};
*/
const wchar_t day_inf[DAY_COUNT][2] = {L"��", L"ȭ", L"��", L"��", L"��", L"��"};

class LECTURE;
int Check_TT_Duplicate(LECTURE *a, LECTURE *b); // ��ġ�� 0, �Ȱ�ġ�� 1
int tonumber(wchar_t ch, wchar_t ch2);

class LECTURE
{
//private:
public:
	wchar_t code[7];		// �ڵ�
	wchar_t* name;			// �̸�
	u_srt pou_srt;		// ����
	u_srt lecture;		// ����
	u_srt exp;			// ����
	u_srt div_class;	// �й�
	float plan_point;	// ��������
	wchar_t target_eng;	// �����
    int capacity;       // ����
    int fixed_capacity;  // ��������

//	int target_department;// ����а� 0�̸� ��� �а� ���, 0�� �ƴϸ� 1~9�� �ش��ϴ� �а�
//	int target_year;	// ����г� 0�̸� ��� �г� ���, 0�� �ƴϸ� 1~5<<3 �г�. ���� 3�� ��Ʈ�� 000~100 ���� ABCDE, 111�� ��� ���ĺ��� �پ����� ����
	wchar_t* target_dep_year;	// ����а�/�г�. ���ڿ� �״�� ����
	wchar_t* teacher;	// ��米��

	u_srt major[MAJOR_COUNT];// ���� ������

	int time_info[3];		// ���Ϲױ���

	int valid;

	LECTURE* next;
public:
	LECTURE() {name = NULL; next = NULL; valid = 0; teacher = NULL;};
	~LECTURE()
	{
		if(name != NULL)
			delete[] name; 
		if(teacher != NULL)
			delete[] teacher;
		if(next != NULL)
			delete next;
	}
	int Open_Lecture(const wchar_t* const Filename, LECTURE* Lecture);

//	friend ostream& operator<< (ostream& os, const LECTURE &lt);
};


class SameList
{
private:
	wchar_t code[7];
	SameList* next;

	SameList() {next = NULL;};
public:
	SameList(wchar_t cd[7]);
	~SameList() {if(next != NULL) delete next;};
	int AddList(wchar_t cd[7]);	// �̹� ������ 0, ������ 1�� ����
	void GetCode(int n, wchar_t cd[7]);
};

class MyLecture
{
private:
	LECTURE *Lecture;
	
	MyLecture *next;

	MyLecture() {Lecture=NULL; next=  NULL;};
public:
	MyLecture(LECTURE *lc);
	~MyLecture();
	int AddLecture(LECTURE *lc);	// �ߺ��Ǵ� �ð��� ������ 0, ������ �߰��ϰ� 1�� ����
	int CheckLecture(LECTURE *lc);	// �ڵ尡 ��ġ�ϴ� ���� ������ 1, ������ 0�� ����
	void DelLecture(LECTURE *lc);	// lc �� ���� ���� ������ ����
//	int GetTTA() {return Lecture->time_info[0];};
//	int GetTTB() {return Lecture->time_info[1];};
	LECTURE* GetLecture();
	MyLecture* GetNext();
};

#endif
