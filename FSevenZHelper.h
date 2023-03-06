#pragma once
#pragma warning(disable : 4530)

//7Z����

//enum class ESPSevenZType 
//{
//	OpType_None,
//	OpType_Compress,		//ѹ��
//	OpType_Decompression,	//��ѹ
//};

//#ifndef FSEVENZHELPER_H
//#define FSEVENZHELPER_H


#include <iostream>
#include <string>
#include <functional>
#include "G:/ѹ����/bit7z-master/bit7z-master/include/bit7z.hpp"
#include "G:/ѹ����/bit7z-master/bit7z-master/include/bit7zlibrary.hpp"
#include<qstring.h>
using namespace bit7z;

class FSevenZHelper
{
private:


public:
	FSevenZHelper();
	~FSevenZHelper();
	void Compress(const QString& Src, const QString& Dest);	//ѹ��
	void Extract(const QString& Src, const QString& Dest);		//��ѹ
	void GetFileSuffix(const QString& Path);//��ȡ�ļ���׺

	static std::wstring StringToWString(const std::string& str);
	/*
	*	�ⲿ��������������н�ѹ��
	*
	*	����1:	ESPSevenZType �����ǽ�ѹ����ѹ����ö��
	*	����2:	const FString& Src ��Ҫѹ�����ѹ��ȫ·��
	*	����3:	�������·��
	*			����ò�����ֵ��ʱ��Ϊ�յĻ�
	*			1>��ѹʱ�����ͬ���ļ����´���һ������Srcȥ��ѹ���ļ���׺���ļ���
	*			2>ѹ���Ļ�����ͬ���ļ�����Ĭ��ѹ����һ��.gspp���ļ�, ��ʵ����7z��ʽ
	*
	*	Notes:��֧��ѹ����rar
	*/
	//void CompressDecompression(ESPSevenZType type, const QString& Src, const QString& Dest);



private:
	QString m_SourcePath;		//Դ�ļ�·��
	QString m_DestDir;			//Ŀ��·��
	QString m_FileSuffix;		//����ԭ·����������ļ���׺
	QString DefaultSuffix;		//Ĭ�Ϻ�׺
	//ESPSevenZType m_OpType;		//��ǰ�ǽ�ѹ����ѹ��

};
//#endif

