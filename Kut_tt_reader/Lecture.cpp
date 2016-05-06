#include <iostream>
using namespace std;
#include <fstream>
#include "Lecture.h"
#include <cstring>
#include <locale>

bool isnumber(wchar_t ch)
{
	if(ch >= L'0' && ch <= L'9')
		return true;
	return false;
}

int tonumber(wchar_t ch)
{
	return int(ch) - int(L'0');
}

int tonumber(wchar_t ch, wchar_t ch2)
{
	return (int(ch) - int(L'0')) * 10 + (int(ch2) - int(L'0'));
}

int LECTURE::Open_Lecture(const wchar_t* const Filename, LECTURE *Lecture)
{
	LECTURE *Loop_Lecture;
	wchar_t ch;
	wchar_t ch2;
	wchar_t ch3;
	wchar_t dummy;
	wchar_t* temp;
	int i;
	int j;
	int to;
	int tmp;
	wifstream fin(Filename);
	if(!fin.is_open())
		return false;
	fin.imbue(locale("kor"));
	Loop_Lecture = Lecture;

	do
	{
		// 코드
		for(i = 0; i < 6; i++)
		{
			fin.get(ch);
				
			if(ch == L'\n' || ch == L'\t')
				fin.get(ch);
			Loop_Lecture->code[i] = ch;
		}
		Loop_Lecture->code[6] = L'\0';
		fin.get(ch);	// \t remover

		temp = new wchar_t[100];

		// 강좌이름
		fin.get(ch);
		i = 0;
		do
		{
			temp[i] = ch;

			i++;
			fin.get(ch);
		}while(ch != L'\t');
		temp[i] = L'\0';

		int lectureNameLen = wcslen(temp) + 1;
		Loop_Lecture->name = new wchar_t[lectureNameLen];
		wcscpy_s(Loop_Lecture->name, lectureNameLen, temp);
		Loop_Lecture->name[i] = L'\0';

		delete[] temp;

		// 학점
		fin >> Loop_Lecture->pou_srt;

		// 강의시간
		fin >> Loop_Lecture->lecture;

		// 실험시간
		fin >> Loop_Lecture->exp;

		// 설계학점
		fin >> Loop_Lecture->plan_point;

		// 분반
		fin >> Loop_Lecture->div_class;

        fin.get(ch);        // TAB REMOVER

        fin.get(ch);
        if(ch != '\t')
    		// 영어강좌
	    	Loop_Lecture->target_eng = ch;

		fin.get(ch);		// TAB REMOVER

		// 전공
		for(i = 0; i < MAJOR_COUNT; i++)
		{
			fin.get(ch);
			if(ch != L'\t')
			{
				fin.get(ch2);
				if(ch2 != L'\t')
				{
					fin.get(ch3);
					if(ch3 != L'\t')
					{
						fin.get(dummy);
						if(dummy != L'\t')
						{
							fin.get(dummy);
							if(dummy != L'\t')
								fin.get(dummy);
						}
					}
					else
						dummy = L'\t';
				}
				else
					ch3 = L'\t';
			}
			else
			{
				ch2 = L'\t';
				ch3 = L'\t';
			}

			if(ch != L'\t')
			{
				if(ch2 != L'\t')
				{
					if(isnumber(ch2))
					{
						Loop_Lecture->major[i] = tonumber(ch, ch2);
						if(Loop_Lecture->major[i] == CT_norm_dummy)
							Loop_Lecture->major[i] = CT_norm_sel;
						if(ch3 != L'\t')
						{
							if(ch3 == L'S')
							{
								Loop_Lecture->major[i] = Loop_Lecture->major[i] | CT_MULTI_UNMAJOR;
								ch3 = dummy;
							}
							if(ch3 == L'T')
							{
								Loop_Lecture->major[i] = Loop_Lecture->major[i] | CT_MDTEACHER_LECTURE;
								if(ch3 == dummy)
									fin.get(dummy);
							}
						}
					}
					else
					{
						Loop_Lecture->major[i] = tonumber(ch);
						if(ch2 == L'S')
						{
							Loop_Lecture->major[i] = Loop_Lecture->major[i] | CT_MULTI_UNMAJOR;
							ch2 = ch3;
						}
						if(ch2 == L'T')
						{
							Loop_Lecture->major[i] = Loop_Lecture->major[i] | CT_MDTEACHER_LECTURE;
						}
					}
				}
				else
				{
					Loop_Lecture->major[i] = tonumber(ch);
				}
			}
			else
			{
				Loop_Lecture->major[i] = 0;
			}
		}

		// 대상학과/학년

		temp = new wchar_t[200];
		fin.get(ch);
		while(ch == L'\t' || ch == L'"')
			fin.get(ch);
		i = 0;
		while(ch != L'\t' && ch != L'"')
		{
			temp[i] = ch;

			i++;
			fin.get(ch);
		}
		temp[i] = L'\0';

		int targetDepYearLen = wcslen(temp) + 1;
		Loop_Lecture->target_dep_year = new wchar_t[targetDepYearLen];
		wcscpy_s(Loop_Lecture->target_dep_year, targetDepYearLen, temp);
		Loop_Lecture->target_dep_year[i] = L'\0';

		delete[] temp;

		if(ch == L'"')
			fin.get(ch);	// tab remover

		// 담당교수
		temp = new wchar_t[20];
		fin.get(ch);
		if(ch == L'"')
			fin.get(ch);	// " remover
		i = 0;
		while(ch != L'\t' && ch != L'"')
		{
			temp[i] = ch;

			i++;
			fin.get(ch);
		}
		temp[i] = L'\0';
		if(ch == L'"')
			fin.get(ch);	// tab remover

		int teacherNameLen = wcslen(temp) + 1;
		Loop_Lecture->teacher = new wchar_t[teacherNameLen];
		wcscpy_s(Loop_Lecture->teacher, teacherNameLen, temp);
		Loop_Lecture->teacher[i] = L'\0';

		delete[] temp;


		// 시간표
		Loop_Lecture->time_info[0] = 0;
		Loop_Lecture->time_info[1] = 0;
		Loop_Lecture->time_info[2] = 0;

		while(true)
		{
			if(!fin.get(ch))
				break;
			if(ch == L'\t')
				continue;
			if(ch == L'/')
				continue;
            if(ch == L' ')
                continue;
			if(ch != L'\n')
			{
				fin.get(dummy);

				for(j = 0; j < DAY_COUNT; j++)
				{
					if(ch == day_inf[j][0])
					{
						break;
					}
				}

				if(j == DAY_COUNT)
				{
					cout << "Reading Failed.. (시간표:요일)";
					return 0;
				}

				if(!IS_DAY_VALIED(Loop_Lecture->time_info[0]))
				{
					Loop_Lecture->time_info[0] = MON >> j;
					to = 0;
				}
				else if(Loop_Lecture->time_info[0] & (MON >> j))
				{
					to = 0;
				}
				else if(!IS_DAY_VALIED(Loop_Lecture->time_info[1]))
				{
					Loop_Lecture->time_info[1] = MON >> j;
					to = 1;
				}
				else if(Loop_Lecture->time_info[1] & (MON >> j))
				{
					to = 1;
				}
				else if(!IS_DAY_VALIED(Loop_Lecture->time_info[2]))
				{
					Loop_Lecture->time_info[2] = MON >> j;
					to = 2;
				}
				else if(Loop_Lecture->time_info[2] & (MON >> j))
				{
					to = 2;
				}
				else
				{
					cout << "Reading Failed.. (시간표:시간)";
					return 0;
				}

				fin.get(ch);
				fin.get(ch2);
				fin.get(ch3);
				fin.get(dummy);

				tmp = tonumber(ch, ch2) - 1;
				tmp *= 2;

				if(ch3 == 'B')
					tmp++;

				if(tmp >= 24)
					tmp = 24;	// 13A 이상의 시간은 tovr로 저장

				Loop_Lecture->time_info[to] = Loop_Lecture->time_info[to] | tt_inf[tmp];

				if(dummy == L'\n')
					break;
			}
			else
				break;
		}

		Loop_Lecture->valid = 1;

		if(Loop_Lecture->next == NULL)
		{
			Loop_Lecture->next = new LECTURE;
			if(Loop_Lecture->next == NULL)
			{
				cout << "Reading Failed.. (Memory Overflow)";
				return false;
			}
			Loop_Lecture = Loop_Lecture->next;
		}
		else
			Loop_Lecture = Loop_Lecture->next;

		if(fin.eof())
			break;
	}while(1);
	fin.close();
	return true;
}


int SameList::AddList(wchar_t cd[7])
{
	if(next != NULL)
	{
		int i = 0;
		for(i = 0; i < 7; i++)
		{
			if(next->code[i] != cd[i])
				break;
		}
		if(i != 7)
			return next->AddList(cd);
		else
			return 0;
	}
	else
	{
		next = new SameList(cd);
		return 1;
	}
}

SameList::SameList(wchar_t cd[])
{
	wcscpy_s(code, cd);
	next= NULL;
}

void SameList::GetCode(int n, wchar_t cd[])
{
	if(n == 0)
	{
		if(next != NULL)
			wcscpy_s(cd, 7, next->code);
		else
			cd[0] = '\0';
	}
	else
		next->GetCode(n-1, cd);
}

MyLecture::MyLecture(LECTURE *lc)
{
	Lecture = lc;
	next = NULL;
}

int MyLecture::AddLecture(LECTURE *lc)
{
	if(next != NULL)
	{
		if(Check_TT_Duplicate(next->Lecture, lc) == 1)
			return next->AddLecture(lc);
		else
			return 0;
	}
	else
	{
		next = new MyLecture(lc);
		return 1;
	}
}

void MyLecture::DelLecture(LECTURE *lc)
{
	if(next != NULL)
	{
		if(next->Lecture == lc)
		{
			MyLecture* temp;
			temp = next;
			next = next->next;
			temp->next = NULL;
			delete temp;
		}
		else
			next->DelLecture(lc);
	}
}

int MyLecture::CheckLecture(LECTURE *lc)
{
	if(next != NULL)
	{
		if(next->Lecture == lc)
			return 1;
		else
			return next->CheckLecture(lc);
	}
	else
		return 0;
}

MyLecture::~MyLecture()
{
	Lecture = NULL;
	if(next != NULL)
		delete next;
}

int Check_TT_Duplicate(LECTURE *a, LECTURE *b)
{
	for(int i = 0; i < 10; i++)
	{
		if(IS_DAY_VALIED(a->time_info[i / 3] & b->time_info[i % 3]))
		{
			if(GET_ONLY_TIME(a->time_info[i / 3] & b->time_info[i % 3]))
				return 0;
		}
	}
	return 1;
}

LECTURE* MyLecture::GetLecture()
{
	return Lecture;
}

MyLecture* MyLecture::GetNext()
{
	return next;
}