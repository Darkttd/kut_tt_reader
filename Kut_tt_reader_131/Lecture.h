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
#define MAJOR_COUNT 21	// 전공 갯수

#define car_eng 0		// 자에
#define com_prat 1		// 컴응
#define inf_tech 2		// IT
#define prod_sys 3		// 생시
#define dig_sys 4		// 디시
#define	con_sys 5		// 제전
#define	electron 6		// 전기
#define	elrock 7 		// 전자
#define	el_tele 8		// 정통
#define	com_tech 9		// 컴공
#define	multi_media 10	// 멀티
#define	intern_soft 11	// 인터
#define	inf_protect 12	// 정호
#define	design_tech 13	// 디공
#define	construct_tech 14	// 축공
#define	construct_edu 15	// 축학
#define	chemi_eng 16	// 응공
#define	new_metrial 17	// 신공
#define	hum_man 18		// 인력
#define	tech_man 19		// 기술
#define	e_bz 20			// e비즈


const WCHAR MajorName[MAJOR_COUNT][5] = {L"자에", L"컴응", L"IT", L"생시", L"디시", L"제전", 
							L"전기", L"전자",
							L"정통", L"컴공", L"멀티", L"인터", L"정호", L"디공", L"축공", L"축학",
							L"응공", L"신공", L"인력", L"기술", L"eBiz"};
*/

// 2010년 1학기부터 바뀐 신 전공에 맞춰 수정
/*
const WCHAR MajorName[MAJOR_COUNT][5] = {L"자에", L"컴응", L"IT응", L"생시", L"디시", L"제전", 
							L"전기", L"전자", L"정통",
							L"컴공", L"인미", L"멀티", L"인터", L"정호", L"컴HW", L"컴SW", L"컴시",
                            L"전기", L"전자", L"정통", L"컴공",
                            L"신공", L"응화", L"에너", 
							L"디자", L"축공", L"축학",
							L"응공", L"인력", L"기술", L"eBiz"};									
*/

//2013년도 1학기용으로 2012년 12월 17일 오후 10시 기준 공지사항에 올라온 xls 파일 기준

#define MAJOR_COUNT 30

const WCHAR MajorName[MAJOR_COUNT][12] = {                            
                            L"자동차ㆍ에너지전공",
                            L"컴퓨터응용전공",
                            L"IT응용전공",
                            L"생산시스템전공",
                            L"디지털시스템전공",
                            L"제어시스템전공",
                            L"전기공학전공",
                            L"전자공학전공",
                            L"정보통신공학전공",
                            L"전기공학전공",
                            L"전자공학전공",
                            L"정보통신공학전공",
                            L"컴퓨터공학전공",
                            L"컴퓨터HW전공",
                            L"컴퓨터SW전공",
                            L"컴퓨터시스템응용전공",
                            L"멀티미디어전공",
                            L"인터넷공학전공",
                            L"정보보호공학전공",
                            L"신소재공학전공",
                            L"응용화학공학전공",
                            L"에너지공학전공",
                            L"신소재공학과",
                            L"응용화학공학과",
                            L"디자인공학과",
                            L"건축공학전공",
                            L"건축학전공",
                            L"인력경영전공",
                            L"기술경영전공",
                            L"EBusiness전공"};

////////////////////////////////////////// 이수구분
////////////////////////////////////////// 홀수는 필수, 짝수는 선택이다
////////////////////////////////////////// 읽어들이면서 일반선택은 11에서 12로 변환하도록 한다
#define CT_COUNT		15

#define CT_sp_cul_req	01	// 전문교양필수
#define CT_sp_cul_sel	02	// 전문교양선택
#define CT_HRD_req		03	// HRD필수
#define CT_HRD_sel		04	// HRD선택
#define CT_tech_req	05	// 공학기초필수
#define CT_tech_sel	06	// 공학기초선택
#define CT_depart_req	07	// 학부필수 
#define CT_depart_sel	08	// 학부선택
#define CT_major_req	09	// 전공필수
#define CT_major_sel	10	// 전공선택
//#define CT_norm_sel	11	// 일반선택(*)
#define CT_norm_dummy	11	
#define CT_norm_sel	12	// 일반선택
#define CT_MSC_req		13	// MSC필수
#define CT_MSC_sel		14	// MSC선택 
#define CT_MULTI_UNMAJOR	0x80000000	// ‘부/복’은 복수/부전공해당과목으로 S로 표시
#define CT_MDTEACHER_LECTURE	0x40000000 // T는 중등교직과목임.

#define CT__ONLYTYPE(dat) (((dat)<<2)>>2)

const WCHAR CTName[CT_COUNT][13] = {L"(null)", L"전문교양핵심", L"전문교양일반", L"HRD핵심", L"HRD일반",
								L"공학기초핵심", L"공학기초일반", L"학부공통핵심", L"학부공통일반",
								L"학과(전공)핵심", L"학과(전공)일반", L"(null)", L"자유선택",
								L"MSC핵심", L"MSC일반"};

//#define DEP_COUNT	10
//#define YEAR_COUNT	48
#define DAY_COUNT	6
#define TT_COUNT	15
/*
//									전체  건축  기정  디공  데카  산경  신소  응화  인미  정보
const char dep_inf[DEP_COUNT][3] = {"전", "건", "기", "디", "메", "산", "신", "응", "인", "정"};
const char year_inf[YEAR_COUNT][3] = {"전", "", "", "", "", "", "", "", 
							"1A", "1B", "1C", "1D", "1E", "", "", "1\t",
							"2A", "2B", "2C", "2D", "", "", "", "2\t",
							"3A", "3B", "3C", "3D", "", "", "", "3\t",
							"4A", "4B", "4C", "4D", "", "", "", "4\t",
							"",   "",   "",   "",   "", "", "", "5\t"};
*/
const wchar_t day_inf[DAY_COUNT][2] = {L"월", L"화", L"수", L"목", L"금", L"토"};

class LECTURE;
int Check_TT_Duplicate(LECTURE *a, LECTURE *b); // 겹치면 0, 안겹치면 1
int tonumber(wchar_t ch, wchar_t ch2);

class LECTURE
{
//private:
public:
	wchar_t code[7];		// 코드
	wchar_t* name;			// 이름
	u_srt pou_srt;		// 학점
	u_srt lecture;		// 강의
	u_srt exp;			// 실험
	u_srt div_class;	// 분반
	float plan_point;	// 설계학점
	wchar_t target_eng;	// 영어강좌
    int capacity;       // 정원
    int fixed_capacity;  // 수정정원

//	int target_department;// 대상학과 0이면 모든 학과 대상, 0이 아니면 1~9에 해당하는 학과
//	int target_year;	// 대상학년 0이면 모든 학년 대상, 0이 아니면 1~5<<3 학년. 뒤의 3개 비트는 000~100 까지 ABCDE, 111인 경우 알파벳이 붙어있지 않음
	wchar_t* target_dep_year;	// 대상학과/학년. 문자열 그대로 저장
	wchar_t* teacher;	// 담당교수

	u_srt major[MAJOR_COUNT];// 전공 데이터

	int time_info[3];		// 요일및교시

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
	int AddList(wchar_t cd[7]);	// 이미 있으면 0, 없으면 1을 리턴
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
	int AddLecture(LECTURE *lc);	// 중복되는 시간이 있으면 0, 없으면 추가하고 1을 리턴
	int CheckLecture(LECTURE *lc);	// 코드가 일치하는 것이 있으면 1, 없으면 0을 리턴
	void DelLecture(LECTURE *lc);	// lc 와 같은 것이 있으면 제거
//	int GetTTA() {return Lecture->time_info[0];};
//	int GetTTB() {return Lecture->time_info[1];};
	LECTURE* GetLecture();
	MyLecture* GetNext();
};

#endif
