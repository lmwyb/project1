#include "FSevenZHelper.h"
#include <QtCore/qstring.h>
#include <QtCore/qfileinfo.h>
#include <QtCore/qdir.h>
#include<qdebug.h>
#include <QtCore/qurl.h>
//#include "GetRegValue.h"
//#include "RegionRiskReadLibrary.h"

using namespace std;
FSevenZHelper::FSevenZHelper()
{
}

FSevenZHelper::~FSevenZHelper()
{
}

//void FSevenZHelper::CompressDecompression(ESPSevenZType type, const QString& Src, const QString& Dest)
//{
//	m_OpType = type;
//	if (type == ESPSevenZType::OpType_Compress)//ѹ��
//	{
//		Compress(type, Src, Dest);
//	}
//	else if (type == ESPSevenZType::OpType_Decompression)//��ѹ
//	{
//		Extract(type, Src, Dest);
//	}
//}
//��ȡ�ļ���׺
void FSevenZHelper::GetFileSuffix(const QString& Path)
{
	QFileInfo fileinfo;
	fileinfo = QFileInfo(Path);
	m_FileSuffix = fileinfo.suffix();
	////Ĭ�Ϻ�׺��ʽ
	//m_FileSuffix = DefaultSuffix;
	//QString Temp = FPaths::ConvertRelativePathToFull(Path);
	////��ȡ�ļ���׺
	////���䰴��·����ֿ�
	//TArray<QString> arrSplit;
	//Temp.ParseIntoArray(arrSplit, TEXT("/"), true);
	//if (arrSplit.Num() > 1)
	//{
	//	Temp.RemoveFromEnd(arrSplit[arrSplit.Num() - 1]);
	//	//�õ����һ��, �ٰ���.���
	//	std::string Fin = arrSplit[arrSplit.Num() - 1];
	//	TArray<std::string> arrSplitPoint;
	//	Fin.ParseIntoArray(arrSplitPoint, TEXT("."), true);
	//	if (arrSplitPoint.Num() == 2)
	//	{
	//		m_FileSuffix = arrSplitPoint[1];
	//	}
	//}
}

//ѹ��
void FSevenZHelper::Compress(const QString& Src, const QString& Dest)
{
	if (Src.isEmpty()) return;

	//1>Դ�ļ�(·�����ߵ����ļ�) ��Ҫ����ѹ��
	QFileInfo dir(Src);
	m_SourcePath = dir.absoluteFilePath();

	//2>��ȡ�ļ���׺
	GetFileSuffix(Dest);

	//3>�жϵ�ǰ������ļ�·���Ƿ�ͽ����ĺ�׺һ��
	if (!Dest.isEmpty())
	{
		QFileInfo dir(Dest);
		m_DestDir = dir.absoluteFilePath();
		QFile file(m_DestDir);
		if (file.exists()) {
			file.remove();
		}
		file.open(QIODevice::ReadWrite);
		file.close();
		if (!m_DestDir.endsWith(m_FileSuffix))
		{
			m_DestDir.append(".");
			m_DestDir.append(m_FileSuffix);
		}
	}
	else
	{
		//3>���Ŀ��·�������õĻ�, ����ݵ�ǰ��ѹ���ļ����ƻ���ѹ���ļ�·�����ƴ���һ��.gspp(7z)��ѹ���ļ�����
		m_DestDir = m_SourcePath;
		if (!m_FileSuffix.isEmpty())
		{
			//����һ��ͬ���ļ���·��
			QString RemoveEndStr = (".");
			RemoveEndStr.append(m_FileSuffix);
			m_DestDir.remove(RemoveEndStr);
		}
		m_DestDir.append(".");
		m_DestDir.append(DefaultSuffix);
	}

	try {
		bit7z::Bit7zLibrary lib(L"7z.dll");
		std::shared_ptr<BitCompressor> compressor = nullptr;
		if (m_FileSuffix.compare(TEXT("7z")) == 0)
		{
			//��׺L".7z"
			compressor = std::make_shared<BitCompressor>(lib, BitFormat::SevenZip);
		}

		//��QStringת��Ϊstd::string
		std::string msSourcePath = m_SourcePath.toStdString();
		std::string msDestDir = m_DestDir.toStdString();

		std::wstring src = StringToWString(msSourcePath.c_str());
		std::wstring dest = StringToWString(msDestDir.c_str());

		compressor->compressDirectory(src, dest);
		compressor->setUpdateMode(true);
	}
	catch (const BitException& ex) {
		//do something with ex.what()...
		qDebug() << ex.what() << endl;

	}
}

//��ѹ
void FSevenZHelper::Extract(const QString& Src, const QString& Dest)
{
	//Src����Ϊ��
	if (Src.isEmpty()) return;

	//1>Դ�ļ�(ѹ���ļ���ʽ) ��Ҫ�����ѹ,��ȡ����·��
	QFileInfo dir(Src);
	m_SourcePath = dir.absoluteFilePath();

	//2>��ȡ�ļ���׺
	GetFileSuffix(Src);

	//3>����Ŀ��·��
	if (!Dest.isEmpty())
	{
		QFileInfo dir(Dest);
		m_DestDir = dir.absoluteFilePath();
		QFile file(m_DestDir);
		if (file.exists()) {
			file.remove();
		}
		file.open(QIODevice::ReadWrite);
		file.close();
	}
	else
	{
		//3>���Ŀ��·�������õĻ�, �����ͬ��Ŀ¼����һ��ͬ���ļ��г���
		m_DestDir = m_SourcePath;
		if (!m_FileSuffix.isEmpty())
		{
			//����һ��ͬ���ļ���·��
			QString RemoveEndStr = (".");
			RemoveEndStr.append(m_FileSuffix);
			m_DestDir.remove(RemoveEndStr);
		}

	}

	try {
		bit7z::Bit7zLibrary lib(L"7z.dll");
		std::shared_ptr<bit7z::BitExtractor> extractor = nullptr;
		if (m_FileSuffix.compare(TEXT("7z")) == 0)//7z��Сд������
		{
			//��׺L".7z"
			extractor = std::make_shared<bit7z::BitExtractor>(lib, bit7z::BitFormat::SevenZip);
		}
		//��QStringת��Ϊstd::string
		std::string msSourcePath = m_SourcePath.toStdString();
		std::string msDestDir = m_DestDir.toStdString();

		std::wstring src = StringToWString(msSourcePath.c_str());
		std::wstring dest = StringToWString(msDestDir.c_str());
		extractor->extract(src, dest);
	}
	catch (const BitException& ex) {
		//do something with ex.what()...
		qDebug() << ex.what() << endl;

	}
}
std::wstring FSevenZHelper::StringToWString(const std::string& str) {
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return std::wstring(w_str);
}

