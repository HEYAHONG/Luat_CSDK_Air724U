/*c����ʵ�֣�����linuxƽ̨�����ˣ���Windows�¿�����GetPrivateProfileString��GetPrivateProfileInt�����ȡ.ini�����ļ����ݣ�
������Linuxƽ̨�Ͼ�һ��Īչ�ˡ�Ϊ�˽�������⣬������C����ȡ.ini�����ɲ���ƽ̨��������*/
#include <string.h>
#ifdef WIN32
#include <Windows.h>
#include <stdio.h>
#else
#define  MAX_PATH 260
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#endif
char g_szConfigPath[MAX_PATH];

#include "am_openat.h"

#define fgetc getc

//��INI�ļ���ȡ�ַ�����������
char *GetIniKeyString(char *title,char *key,char *filename) 
{ 
	FILE *fp; 
	char szLine[256];
	static char tmpstr[256];
	int rtnval;
	int i = 0; 
	int flag = 0; 
	char *tmp;
	char* strend;
 
	if((fp = fopen(filename, "r")) == NULL) 
	{ 
		printf("have   no   such   file \n");
		return ""; 
	}
	while(!feof(fp)) 
	{ 
		rtnval = fgetc(fp); 
		if(rtnval == EOF) 
		{ 
			break; 
		} 
		else 
		{ 
			szLine[i++] = rtnval; 
		} 
		if(rtnval == '\n') 
		{ 
#ifndef WIN32
			i--;
#endif	
			szLine[--i] = '\0';
			i = 0; 
			tmp = strchr(szLine, '='); 

 
			if(( tmp != NULL )&&(flag == 1)) 
			{ 
				if(strstr(szLine,key)!=NULL) 
				{ 
					//ע����
					if ('#' == szLine[0])
					{
					}
					else if ( '/' == szLine[0] && '/' == szLine[1] )
					{
						
					}
					else
					{
						//�Ҵ�key��Ӧ����
						tmp++;
						while(*tmp && (*tmp == ' ' || *tmp == '\t')) tmp++;
						strend = tmp;
						while(*strend && *strend != ' ' && *strend != '\t') strend++;
						strncpy(tmpstr,tmp, strend - tmp); 
						tmpstr[strend - tmp] = 0;
						fclose(fp);
						return tmpstr; 
					}
				} 
			}
			else 
			{ 
				strcpy(tmpstr,"["); 
				strcat(tmpstr,title); 
				strcat(tmpstr,"]");
				if( strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 ) 
				{
					//�ҵ�title
					flag = 1; 
				}
			}
		}
	}
	fclose(fp); 
	return ""; 
}
 
//��INI�ļ���ȡ����������
int GetIniKeyInt(char *title,char *key,char *filename)
{
	return atoi(GetIniKeyString(title,key,filename));
}
 


