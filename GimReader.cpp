#ifdef WIN32  
#pragma execution_character_set("utf-8")
#endif
#include "GimReader.h"
#include<qdatetime.h>
#include <iostream>
#include <QDomDocument>
#include <QXmlStreamReader>
#include<qtextcodec.h>
#include<qdiriterator.h>
#include <QtCore/qurl.h>
#define  PI 3.1415926
#include<xkeycheck.h>
#include <gl/GL.h>
#include "example12.h"
using namespace std;

QList<ModFile> gimModFileList;//���е�mod

ObjectModel objectModel;
QList<TripletPara> sysParas;

QString extractPath; //����7z��ѹ֮���·���ļ���:G:/Example C/example11/gimUncompress
QString sevenzPath; //����gim7z�ļ���:G:/Example C/example11/gim7zFiles
QString path;//7z�ļ���ѹλ�ã�����.7z�ļ�:gimUncompress/���Թ���
QString gimName;//gim�ļ�������������׺��
GimReader::GimReader()
{
    extractPath = "G:/Example C/example12/gimUncompress"; //����7z��ѹ֮���·���ļ���
    sevenzPath = "G:/Example C/example12/gim7zFiles";//����gim7z�ļ���

}

GimReader::~GimReader()
{
    //qDebug() << f1System.getSubSystems() << endl;
}
//����GIM
void GimReader::ImportGim(QString gimPath)
{
    QFile gimfile(gimPath);
    //  ָ���򿪷�ʽ
    bool isOk = gimfile.open(QFile::ReadOnly);
    if (!isOk) {
        throw"file open failed";
    }
    QByteArray gimBytes = gimfile.readAll();//��ȡgim�ļ����ֽ�������
    gimfile.close();
    if (gimBytes.size() == 0)
    {
        throw "gimBytes.size == 0";
    }
    QFileInfo gim(gimPath);
    gimName = gim.baseName();//��ȡ.gim�ļ�������׺���ļ���
    RemoveGimHeaders(gimName, gimBytes);//�Ƴ�gimheader ���7z

    //�����ļ���
    QDir dir(extractPath);
    if (!dir.exists()) {
        dir.mkpath(extractPath);
    }

    QDir sevenz(sevenzPath);
    if (!sevenz.exists()) {
        sevenz.mkpath(sevenzPath);
    }
    path = extractPath + "/" + gimName;
    QDir extractpath(path);
    if (!extractpath.exists()) {
        extractpath.mkpath(path);
    }
    //��ѹ��gimUncompressĿ¼��---------��ѹ·����gimUncompress�Ϳ�����
    //FSevenZHelper sevenzipextract;
    //sevenzipextract.Extract(sevenzPath + "/" + gimName + ".7z", path);//��ѹ7z�ļ�
    //7zѹ������ѹ֮���·��
    qDebug() << "��ѹ���" << endl;
}
//����GIM
void GimReader::ExportGim(QString outPath)
{
    FSevenZHelper sevenzipcompress;
    sevenzipcompress.Compress(path, sevenzPath + "/" + gimName + ".7z");
    WriteGimFile(sevenzPath + "/" + gimName + ".7z", outPath);
}
//����.7z�ļ�
void GimReader::RemoveGimHeaders(QString gimName, QByteArray gimBytes)
{
    if (gimBytes.size() < 876)
    {
        throw "gim�ļ����ֽ�������" + gimBytes.size();
    }
    qDebug() << "�ļ���ʶ��" << gimBytes.mid(0, 16).data() << endl;
    qDebug() << "�ļ����ƣ�" << gimBytes.mid(16, 256).data() << endl;
    qDebug() << "����ߣ�" << gimBytes.mid(272, 64).data() << endl;
    qDebug() << "��֯��λ��" << gimBytes.mid(336, 256).data() << endl;
    qDebug() << "������ƣ�" << gimBytes.mid(592, 128).data() << endl;
    qDebug() << "����ʱ�䣺" << gimBytes.mid(720, 16).data() << endl;
    qDebug() << "������汾�ţ�" << gimBytes.mid(736, 8).data() << endl;
    qDebug() << "����ΰ汾�ţ�" << gimBytes.mid(744, 8).data() << endl;
    qDebug() << "��׼���汾�ţ�" << gimBytes.mid(752, 8).data() << endl;
    qDebug() << "��׼�ΰ汾�ţ�" << gimBytes.mid(760, 8).data() << endl;
    qDebug() << "�洢���С��";
    for (int i = 768; i < 776; i++) {
        int t = (int)gimBytes[i];
        cout << t << " ";
    }
    qDebug() << endl;
    qDebug() << "�洢��ǰ100Byte��7z��ʼ����";
    for (int i = 776; i < 876; i++) {
        int t = (int)gimBytes[i];
        cout << t << " ";
    }
    qDebug() << endl;
    QByteArray gim7zBytes;
    gim7zBytes.resize(gimBytes.size() - 776);
    for (int i = 776, j = 0; i < gimBytes.size(); i++, j++)
    {
        gim7zBytes[j] = gimBytes[i];
    }

    //����ļ��Ѵ��� ɾ���ļ�
    QString gim7zPath = sevenzPath + "/" + gimName + ".7z";
    QFile gim7zfile(gim7zPath);
    if (gim7zfile.exists()) {
        gim7zfile.remove();
    }
    //д7z
    gim7zfile.open(QFile::WriteOnly);
    gim7zfile.write(gim7zBytes);
    gim7zfile.close();
}
//����.gim�ļ�
void GimReader::WriteGimFile(QString gim7zPath, QString outPath)
{
    QFile gim7zfile(gim7zPath);
    if (!gim7zfile.exists()) {
        throw"gim.7z������";
    }
    QByteArray gim7zBytes = gim7zfile.readAll();
    qint32 size = gim7zBytes.size();
    QByteArray gimBytes;
    gimBytes.resize(size + 776);
    WriteGimHeaders(gimBytes, size);
    for (int i = 776, j = 0; j < gim7zBytes.size(); i++, j++)
    {
        gimBytes[i] = gim7zBytes[j];
    }
    QFile gimfile(outPath);
    if (gimfile.open(QFile::WriteOnly)) {
        gimfile.write(gimBytes);
    }
}
//дͷ����Ϣ
void GimReader::WriteGimHeaders(QByteArray gimBytes, int values)
{
    //��ʶ,0
    SetHeaders(gimBytes, QByteArray("GIMPKGS"), 0);

    //�ļ�����,16
    SetHeaders(gimBytes, QByteArray("��Ѷ"), 16);

    //�����,272
    SetHeaders(gimBytes, QByteArray("��Ȼ"), 272);

    //��֯��λ,336
    SetHeaders(gimBytes, QByteArray("�Ͼ��ʵ��ѧ"), 336);

    //�������,592
    SetHeaders(gimBytes, QByteArray("�������"), 592);

    //����ʱ��,720
    QDateTime date = QDateTime::currentDateTime();
    QString datetime = date.toString("yyyy-MM-dd hh:mm:ss");
    SetHeaders(gimBytes, datetime.toUtf8(), 720);


    //������汾��,736
    SetHeaders(gimBytes, QByteArray("V1"), 736);

    //����ΰ汾��,744
    SetHeaders(gimBytes, QByteArray("01"), 744);


    //��׼���汾��,752
    SetHeaders(gimBytes, QByteArray("01"), 752);


    //��׼�ΰ汾��,760
    SetHeaders(gimBytes, QByteArray("01"), 760);


    //�洢���С,768
    SetHeaders(gimBytes, intToByte(values), 768);
}

void GimReader::SetHeaders(QByteArray headerContent, QByteArray item, int begin)
{
    for (int i = 0; i < item.size(); i++)
    {
        headerContent[begin] = item[i];
        begin++;
    }
}

QByteArray GimReader::intToByte(int values)
{
    QByteArray arr = QByteArray::number(values, 16);
    return arr;
    /* QByteArray abyte0;
     abyte0.resize(4);
     abyte0[0] = (uchar)(0xff & values);
     abyte0[1] = (uchar)((0xff00 & values) >> 8);
     abyte0[2] = (uchar)((0xff0000 & values) >> 16);
     abyte0[3] = (uchar)((0xff000000 & values) >> 24);
     return abyte0;*/
}

void GimReader::run()
{
    //ReadProjectFile();
    //qDebug() << "F1System:" << projectFile.getSubSystem() << endl;
    //qDebug() << "F2System:" << f1System.getSubSystems() << endl;
    //qDebug() << "F3�ĸ�����" << f2System.getSubSystems().size() << endl;
    //for (int i = 0; i < f2subsystems.size(); i++) {
    //    qDebug() << "F3System:" << f2subsystems[i] << endl;
    //}
    ////qDebug() << "F3System:" << f2System.getSubSystems() << endl;
    //qDebug() << "F4�ĸ�����" << f3System.getSubSystems().size() << endl;
    //qDebug() << "F4System:" << f3System.getSubSystems() << endl;
    //qDebug() << "F5�ĸ�����" << f4subdevices.size() << endl;
    //GetGimModFileList2();
    //GetGimModFileList();
    getFlSystemNode();
    //qDebug() << "F1�ĸ�����" << FlSystemNode.size() << endl;
    //qDebug() << "F2�ĸ�����" << F2SystemNode.size() << endl;
    //qDebug() << "F3�ĸ�����" << F3SystemNode.size() << endl;
    //
    qDebug() << "F3toF4�ĸ�����" << F3toF4Node.size() << endl;
    qDebug() << "F3toF4��" << F3toF4Node << endl;
    qDebug() << "F3toObj�ĸ�����" << F3toObjNode.size() << endl;
    qDebug() << "F3toObj��" << F3toObjNode << endl;
    qDebug() << "allF4�ĸ�����" << allF4SystemNode.size() << endl;
    qDebug() << "F4�ĸ�����" << F4SystemNode.size() << endl;
    qDebug() << "F4�в��������ĸ���" << havasubdevices.size() << endl;
    qDebug() << "F4�в���������" << havasubdevices << endl;
    //qDebug() << "F5�ĸ�����" << PartIndexNode.size() << endl;

    GetGimModFileList1();
    //qDebug() << "�豸������" << devices.size() << endl;
    //qDebug() << "�豸����" << devices << endl;
    //readModFile("G:/idea/gim/src/main/resources/gimUncompress/������/MOD/03aa690f-f38c-4bd9-97cf-c4b97e896294.mod");

    /*ModFile modFile=readModFile("G:/idea/gim/src/main/resources/gimUncompress/������/MOD/03aa690f-f38c-4bd9-97cf-c4b97e896294.mod");
    ModFileList.append(modFile);*/

    //qDebug() << ModFileList.size() << endl;

    //for (ModFile modFile : ModFileList) {
    //    //qDebug() << modFile.getSimpleEntities().length() << endl;
    //    //qDebug() <<111111<< endl;
    //    for (SimpleEntity simpleEntity : modFile.getSimpleEntities()) {
    //        //qDebug() << 111111 << endl;
    //        if (!simpleEntity.getGraphicElement().isNull()) {
    //            //qDebug() << 111111 << endl;
    //            //qDebug() << simpleEntity.getGraphicElement().nodeName() << endl;
    //            if (simpleEntity.getGraphicElement().nodeName() == ("Cuboid")) {
    //                //qDebug() << 111111 << endl;
    //                QString  vs = simpleEntity.getTransformMatrix();
    //                //qDebug() << vs << endl;
    //                QStringList splits = vs.split(",");
    //                float mat[16];
    //                for (int i = 0; i < 16; i++)
    //                {
    //                    mat[i] = (splits[i]).toFloat();
    //                    //qDebug() << mat[i] << endl;
    //
    //                }
    //                ////��ȡ��ת�Ƕ�
    //                float rX = (atan2(mat[9], mat[10])) * PI / 180;
    //                float rY = (atan2(-mat[8], sqrt(pow(mat[9], 8) + pow(mat[10], 8)))) * PI / 180;
    //                float rZ = (atan2(mat[4], mat[0])) * PI / 180;
    //                //��ȡ����
    //                float X = mat[12];
    //                float Y = mat[13];
    //                float Z = mat[14];
    //                //��ȡ��ɫ
    //                float R = (float)simpleEntity.getR() / 255;
    //                float G = (float)simpleEntity.getG() / 255;
    //                float B = (float)simpleEntity.getB() / 255;
    //                float A = (float)simpleEntity.getA() / 255;
    //                //��ȡ�����
    //                QDomElement n = simpleEntity.getGraphicElement().toElement();
    //                float L = (n.attribute("L")).toFloat();
    //                float W = (n.attribute("W")).toFloat();
    //                float H = (n.attribute("H")).toFloat();

    //                GLuint id1 = glGenLists(1);
    //                glNewList(id1, GL_COMPILE);
    //                glPushMatrix();
    //                Cuboid(L, W, H);
    //                glPopMatrix();
    //                glEndList();
    //                 //qDebug() << L << " " << H << "" << W << " " << endl;
    //            }
    //        }
    //    }
    //}

    //setmodfile(ModFileList);
    //qDebug() << getmodfile().size() << endl;
    //readXmlPara("G:/idea/gim/src/main/resources/gimUncompress/������/MOD/03aa690f-f38c-4bd9-97cf-c4b97e896294.mod");
}
//��ȡproject.cbm  --- ��ȡһ��ProjectFile����
ProjectFile GimReader::ReadProjectFile()
{
    ProjectFile  projectFile;
    //��ȡ��ѹ����ļ����� ��project.cbm��ʼ
    QString readPath = path + "/CBM/" + "project.cbm";
    //qDebug() << readPath << endl;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (content.isEmpty()) {
        qDebug() << "��ȡproject.cbmʧ��" << endl;
    }
    if (!content.isEmpty()) {
        projectFile.setEntityName(EntityName::ProjectFile);
        projectFile.setFileName("project.cbm");
        projectFile.setBLHA(content.value("BLHA"));
        projectFile.setSubSystem(content.value("SUBSYSTEM"));
    }
    // readF1System(content.value("SUBSYSTEM"));
     //
     ///*QMapIterator<QString, QString> iterator(content);
     //while (iterator.hasNext()) {
     //    iterator.next();
     //    qDebug() << iterator.key() << ":" << iterator.value();
     //}*/
     //qDebug() << projectFile.getSubSystem()<< endl;
    return  projectFile;
}
//һ��.cbm
F1System GimReader::readF1System(QString fileName)
{
    F1System f1System;
    if (fileName.isEmpty()) {
        qDebug() << "F1System.cbm������" << endl;
        throw"һ��cbm�ļ�������";
    }
    //��ȡ��Ӧ��ȫվ��.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    //qDebug() << content << endl;
    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F1System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        //��ʼ��װentity
        //F1System f1System;
        f1System.setEntityName(EntityName::F1System);
        //*** 1 *** һ��ȫվ��.cbm���ļ���
        f1System.setFileName(fileName);
        //*** 2 *** һ��ȫվ��fam������
        f1System.setFamFileName(content.value("BASEFAMILY"));
        //*** 3 *** ���õĶ���ϵͳ��
        //QList<F2System>subSysyems;
        if (content.count("SUBSYSTEMS.NUM") > 0) {
            if (content.value("SUBSYSTEMS.NUM") != "0") {
                int i;
                QList<QString> f1subsystems;
                for (i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++) {
                    //qDebug() << content.value("SUBSYSTEM" + QString::number(i)) << endl;
                    //f1System.setSubSystems(subsystems);
                    f1subsystems.append(content.value("SUBSYSTEM" + QString::number(i)));
                    //readF2System(content.value("SUBSYSTEM" + QString::number(i)));
                }
                f1System.setSubSystems(f1subsystems);
                //f2System.setSubSystems(f2subsystems);
            }
        }
        //*** 4 *** ���õ��߼�ģ��
        if (content.count("SUBLOGICALMODELS.NUM") > 0) {
            if (content.value("SUBLOGICALMODELS.NUM") != "0") {
                int i;
                QList<QString> sublogicalmodels;
                for (i = 0; i < (content.value("SUBLOGICALMODELS.NUM")).toInt(); i++) {
                    sublogicalmodels.append(content.value("SUBLOGICALMODEL" + QString::number(i)));
                }
                f1System.setSubLogicalModels(sublogicalmodels);
            }
        }
        //*** 5 *** ���õĲ������xml
        f1System.setMaterialSheet(content.value("MATERIALSHEET"));
        //*** 6 *** ���õ�ifc
        if (content.count("IFC.NUM") > 0) {
            if (content.value("IFC.NUM") != "0") {
                int i;
                QList<QString> ifcfiles;
                for (i = 0; i < (content.value("IFC.NUM")).toInt(); i++) {

                    ifcfiles.append(content.value("IFC" + QString::number(i)));
                }
                f1System.setIfcFiles(ifcfiles);
            }
        }
    }
    //qDebug() << f1System.getSubSystems() << endl;
    return f1System;
}
//����.cbm
F2System GimReader::readF2System(QString fileName)
{
    F2System f2System;
    if (fileName.isEmpty()) {
        qDebug() << "����cbm������" << endl;
        throw"����cbm�ļ�������";
    }
    //��ȡ��Ӧ�Ķ���ϵͳ��.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    //qDebug() << content << endl;
    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        if (content.value("ENTITYNAME") != ("F2System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;

            throw "CODE_ERROR";
        }
        //��ʼ��װentity
        //F2System f2System;
        f2System.setEntityName(EntityName::F2System);
        //*** 1 *** ����ϵͳ��.cbm���ļ���
        f2System.setFileName(fileName);
        //*** 2 *** ����ϵͳ��������
        f2System.setSysClassifyName(content.value("SYSCLASSIFYNAME"));
        //*** 3 *** ����ϵͳ��.fam������
       //QList<TripletPara> basefamily = readTripletPara(path + "/CBM/" + content.key("BASEFAMILY"));
        f2System.setFamFileName(content.value("BASEFAMILY"));
        //*** 4 *** ���õ�������ϵͳ��
        //QList<F3System> subSystems;
        if (content.count("SUBSYSTEMS.NUM") > 0) {
            if (content.value("SUBSYSTEMS.NUM") != ("0")) {
                //qDebug() << content.value("SUBSYSTEMS.NUM") << endl;
                QList<QString> f2subsystems;
                for (int i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++) {
                    //F3System f3System = this->readF3System(content.key("SUBSYSTEM" + i));
                    f2subsystems.append(content.value("SUBSYSTEM" + QString::number(i)));
                    // readF3System(content.value("SUBSYSTEM" + QString::number(i)));
                }
                f2System.setSubSystems(f2subsystems);
                //f3System.setSubSystems(f3subsystems);
            }
        }
        //*** 5 *** ���õ�ifc�ļ�
        if (content.count("IFC.NUM") > 0) {
            if (content.value("IFC.NUM") != "0") {
                int i;
                QList<QString> ifcfiles;
                for (i = 0; i < (content.value("IFC.NUM")).toInt(); i++) {
                    ifcfiles.append(content.value("IFC" + QString::number(i)));
                }
                f2System.setIfcFiles(ifcfiles);
            }
        }
        //*** 6 *** �������xml
        f2System.setMaterialSheet(content.value("MATERIALSHEET"));
    }
    //qDebug() << f2System.getSubSystems().size() << endl;
    return f2System;
}
//����.cbm
F3System GimReader::readF3System(QString fileName)
{
    F3System f3System;
    if (fileName.isEmpty()) {
        qDebug() << "����cbm������" << endl;
        throw"����cbm�ļ�������";
    }
    //��ȡ��Ӧ��������ϵͳ��.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    //qDebug() << content << endl;
    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        if (content.value("ENTITYNAME") != ("F3System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        //��ʼ��װentity
        f3System.setEntityName(EntityName::F3System);
        //*** 1 *** ������ϵͳ��.cbm���ļ���
        f3System.setFileName(fileName);
        //*** 2 *** ������ϵͳ��������
        f3System.setSysClassifyName(content.value("SYSCLASSIFYNAME"));
        //�ж���F3����F4
        if (content.count("BASEFAMILY1") > 0) {
            //��F3������F4
            GetF3System(content, f3System);
        }
        else {
            //��F3�豸
            //qDebug() << "F3ASF4" << endl;
            GetF3SystemAsF4(content, f3System);
        }
    }
    return f3System;
}
//�ļ�.cbm
F4System GimReader::readF4System(QString fileName)
{
    F4System f4System;
    if (fileName.isEmpty()) {
        qDebug() << "�ļ�cbm������" << endl;
        throw"�ļ�cbm�ļ�������";
    }
    //��ȡ��Ӧ���ļ��豸��.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F4System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        //��ʼ��װentity
        f4System.setEntityName(EntityName::F4System);
        //*** 1 *** �ļ��豸��.cbm���ļ���
        f4System.setFileName(fileName);

        //*** 2 *** �ļ��豸��������
        f4System.setSysClassifyName(content.value("SYSCLASSIFYNAME"));

        //*** 3 *** �ļ��豸��.fam������
        //QList<TripletPara> basefamily = readTripletPara(path + "/CBM/" + content.key("BASEFAMILY"));
        f4System.setFamFileName(content.value("BASEFAMILY"));

        //*** 4 *** �豸����ڱ��վԭ��Ŀռ�任����
        f4System.setTransformMatrix(content.value("TRANSFORMMATRIX"));

        //*** 5 *** ���õ�����ģ��dev
        f4System.setObjectModel(content.value("OBJECTMODELPOINTER"));

        //*** 6 *** ���õĲ�������
        //QList<PartIndex> partIndices();
        if (content.count("SUBDEVICES.NUM") > 0) {
            if (content.value("SUBDEVICES.NUM") != ("0")) {
                QList<QString> f4subdevices;
                for (int i = 0; i < (content.value("SUBDEVICES.NUM")).toInt(); i++) {
                    f4subdevices.append(content.value("SUBDEVICE" + QString::number(i)));
                    //readPartIndex(content.value("SUBDEVICE" + QString::number(i)));

                }
                f4System.setSubDevice(f4subdevices);
            }
        }
        //*** 7 *** ���õ��߼�ģ��cbm
        if (content.count("SUBLOGICALMODELS.NUM") > 0) {
            if (content.value("SUBLOGICALMODELS.NUM") != ("0")) {
                QList<QString> sublogicalmodels;
                for (int i = 0; i < content.value("SUBLOGICALMODELS.NUM"); i++) {
                    sublogicalmodels.append(content.value("SUBLOGICALMODEL" + QString::number(i)));
                }
                f4System.setSubLogicalModels(sublogicalmodels);
            }
        }
    }
    //qDebug() << f4System.getSubDevice() << endl;
    return f4System;
}

void GimReader::GetF3System(QMap<QString, QString> content, F3System& f3System)
{
    //qDebug() << content << endl;
    //F3
    f3System.setEntityName(EntityName::F3System);
    //*** 3 *** ������ϵͳ�������� 1 2 3
    f3System.setSysName1(content.value("SYSTEMNAME1"));
    f3System.setSysName2(content.value("SYSTEMNAME2"));
    f3System.setSysName3(content.value("SYSTEMNAME3"));

    //*** 4 *** ������ϵͳ��.fam������ 1 2 3
    f3System.setFamFileName1(content.value("BASEFAMILY1"));
    f3System.setFamFileName2(content.value("BASEFAMILY2"));
    f3System.setFamFileName3(content.value("BASEFAMILY3"));

    //*** 5 *** ���õ��ļ��豸��
    if (content.count("SUBSYSTEMS.NUM") > 0)
    {
        if (content.value("SUBSYSTEMS.NUM") != ("0"))
        {
            QList<QString> f3subsystems;//f4cbm
            for (int i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++)
            {
                f3subsystems.append(content.value("SUBSYSTEM" + QString::number(i)));
                //readF4System(content.value("SUBSYSTEM" + QString::number(i)));
            }
            f3System.setSubSystems(f3subsystems);
            //f4System.setSubDevice(f4subdevices);
        }
    }
    //*** 6 *** ���õ�ifc�ļ�
    if (content.count("IFC.NUM") > 0)
    {
        if (content.value("IFC.NUM") != ("0"))
        {
            QList<QString> ifcfiles;
            for (int i = 0; i < (content["IFC.NUM"]).toInt(); i++)
            {
                ifcfiles.append(content.value("IFC" + QString::number(i)));
            }
            f3System.setIfcFiles(ifcfiles);
        }
    }

    //*** 7 *** �������xml
    f3System.setMaterialSheet(content.value("MATERIALSHEET"));
}

void GimReader::GetF3SystemAsF4(QMap<QString, QString> content, F3System& f3System)
{
    //F4
    f3System.setEntityName(EntityName::F3SystemAsF4);
    //*** 3 *** �ļ��豸��.fam������
    // List<TripletPara> basefamily = ReadTripletPara(gimDirectory + @"\CBM\" + content["BASEFAMILY"]);
    f3System.setFamFileName(content.value("BASEFAMILY"));

    //*** 4 *** �豸����ڱ��վԭ��Ŀռ�任����
    f3System.setTransformMatrix(content.value("TRANSFORMMATRIX"));

    //*** 5 *** ���õ�����ģ��dev
    f3System.setObjectModel(content.value("OBJECTMODELPOINTER"));


    //*** 6 *** ���õĲ�������
    if (content.count("SUBDEVICES.NUM") > 0)
    {
        if (content.value("SUBDEVICES.NUM") != ("0"))
        {
            QList<QString> subdevices;
            for (int i = 0; i < (content.value("SUBDEVICES.NUM")).toInt(); i++)
            {
                subdevices.append(content.value("SUBDEVICE" + QString::number(i)));
                //readPartIndex(content.value("SUBDEVICE" + QString::number(i)));
            }
            f3System.setSubDevice(subdevices);
        }
    }
}
//��������.cbm
PartIndex GimReader::readPartIndex(QString fileName)
{
    PartIndex partIndex;
    if (fileName.isEmpty()) {
        throw"��������cbm�ļ�������";
    }
    //��ȡ��Ӧ�Ĳ�������.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    //qDebug() << content << endl;
    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        //�����ʶ
        if ((content.value("ENTITYNAME") == ("PartIndex"))
            || content.value("ENTITYNAME") == ("PARTINDEX")) { //�ĵ�����ȫ��д���������Ǵ�Сд


        //��ʼ��װentity
        //PartIndex partIndex();
            partIndex.setEntityName(EntityName::PartIndex);

            //*** 1 *** ��������.cbm���ļ���
            partIndex.setFileName(fileName);

            //*** 2 *** ��������������
            partIndex.setPartName(content.value("PARTNAME"));

            //*** 3 *** ��������.fam������
           // QList<TripletPara> basefamily = readTripletPara(path + "/CBM/" + content.key("BASEFAMILY"));
            partIndex.setPartName(content.value("BASEFAMILY"));

            //*** 5 *** ���õ�����ģ��dev
            partIndex.setObjectModel(content.value("OBJECTMODELPOINTER"));
        }
        else {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }

    }
    return partIndex;
}
//.dev
ObjectModel GimReader::readObjectModel(QString fileName) {
    //qDebug() << 11111 << endl;
    ObjectModel objectModel;
    QString readPath = path + "/DEV/" + fileName;
    //qDebug() << readPath << endl;
    if (readPath.isEmpty()) {
        qDebug() << "dev�ļ�������" << endl;
        throw"dev�ļ�������";
    }
    //��ȡ��Ӧ������ģ��.dev
    QMap<QString, QString> content = readDoublePara(readPath);

    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        //��ʼ��װentity
       // ObjectModel<class T> objectModel;
        objectModel.setEntityName(EntityName::ObjectModel);
        //*** 1 *** ����ģ��.dev���ļ���
        objectModel.setFileName(fileName);
        //*** 2 *** ����ģ��.fam������
        //qDebug() << "famfile"<<content.value("BASEFAMILYPOINTER") << endl;
        objectModel.setFamFileName(content.value("BASEFAMILYPOINTER"));
        //*** 3 *** ����ģ��dev��Ӧ�ĵ����豸��ģ������
        objectModel.setSymbolName(content.value("SYMBOLNAME"));
        //*** 4 *** ���õĴμ�����ģ��dev �� �μ�����ģ�͵Ŀռ�任����
        //QList<ObjectModel<class T>> subObjectModel();
        QList<QString> subobjmodels;
        QList<QString> subobjtransformmatrix;
        if (content.count("SUBDEVICES.NUM") > 0) {
            if (content.value("SUBDEVICES.NUM") != ("0")) {
                for (int i = 0; i < (content.value("SUBDEVICES.NUM")).toInt(); i++) {
                    subobjmodels.append(content.value("SUBDEVICE" + QString::number(i)));
                    subobjtransformmatrix.append(content.value("TRANSFORMMATRIX" + QString::number(i)));
                }
            }
        }
        objectModel.setSubObjectModels(subobjmodels);
        objectModel.setSubObjTransformMatrix(subobjtransformmatrix);
        //*** 5 *** ���õ�phm�ļ� �� phmģ�͵Ŀռ�任����
        QList<QString> subsolidmodels;
        QList<QString> subsolidtransformmatrix;
        if (content.count("SOLIDMODELS.NUM") > 0) {
            if (content.value("SOLIDMODELS.NUM") != ("0")) {
                
                for (int i = 0; i < (content.value("SOLIDMODELS.NUM")).toInt(); i++) {
                    //SolidModel<class T> solidModel = this->readSolidModel(content.key("SOLIDMODEL" + i));
                    subsolidmodels.append(content.value("SOLIDMODEL" + QString::number(i)));
                    subsolidtransformmatrix.append(content.value("#TRANSFORMMATRIX" + QString::number(i)));
                }
            }
        }
        objectModel.setSolidModels(subsolidmodels);
        objectModel.setSolidTransformMatrix(subsolidtransformmatrix);
    }
    return objectModel;
}
//.phm
SolidModel GimReader::readSolidModel(QString fileName)
{
    if (fileName.isEmpty()) {
        qDebug() << "phm�ļ�������" << endl;
        throw "phm�ļ�������";
    }
    //��ȡ��Ӧ�����ģ���ļ�phm
    QString readPath = path + "/PHM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);

    //�ѻ�õ�contentװ��entity
    SolidModel solidModel;
    if (!content.isEmpty()) {
        //SolidModel<class T> solidModel;
        //*** 1 *** ���ģ���ļ�phm���ļ���
        solidModel.setFileName(fileName);
        //*** 2 *** ���õ�phm��mod��stl�ļ�
        if (content.count("SOLIDMODELS.NUM") > 0) {
            if (content.value("SOLIDMODELS.NUM") != ("0")) {
                QList<QString> colorList;
                QList<QString> transformMatrixList;
                QList<QString> children;
                for (int i = 0; i < (content.value("SOLIDMODELS.NUM")).toInt(); i++) {
                    colorList.append(content.value("COLOR" + QString::number(i)));
                    transformMatrixList.append(content.value("TRANSFORMMATRIX" + QString::number(i)));
                    children.append(content.value("SOLIDMODEL" + QString::number(i)));
                }
                solidModel.setcolorList(colorList);
                solidModel.settransformMatrixList(transformMatrixList);
                solidModel.setchildren(children);
            }
        }
    }
    return solidModel;
}
//.cbm
LogicalModel GimReader::readLogicalModel(QString fileName)
{
    LogicalModel logicalModel;
    if (fileName.isEmpty()) {
        throw "cbm�ļ�������";
    }
    //��ȡ��Ӧ���߼�ģ��.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);

    //�ѻ�õ�contentװ��entity
    if (!content.isEmpty()) {
        if (content.value("ENTITYNAME") != ("LOGICALMODEL")) { //�ĵ�����ȫ��д��������
            qDebug() << extractPath << "/DEV/" << fileName << "��ʶ����" << endl;

            throw "CODE_ERROR";
        }
        //��ʼ��װentity
        //*** 1 *** �߼�ģ��.cbm���ļ���
        logicalModel.setFileName(fileName);
        //*** 2 *** ���õ�sch�ļ�
        if (content.count("LOGICALMODELS.NUM") > 0) {
            QList<QString> schFiles;
            if (content.value("LOGICALMODELS.NUM") != ("0")) {
                for (int i = 0; i < (content.value("LOGICALMODELS.NUM")).toInt(); i++) {
                    schFiles.append(content.value("LOGICALMODEL" + QString::number(i)));
                }
                logicalModel.setSchFiles(schFiles);
            }
        }

    }
    return logicalModel;

}
//.ifc
IfcFile GimReader::readIfcFile(QString fileName)
{
    IfcFile ifcFile;
    if (fileName.isEmpty()) {
        throw "ifc�ļ�������";
    }
    return ifcFile;
}
//.stl
StlFile GimReader::readStlFile(QString fileName)
{
    StlFile stlFile;
    if (fileName.isEmpty()) {
        throw "stl�ļ�������";
    }
    return stlFile;
}
//.sch
SchFile GimReader::readSchFile(QString fileName)
{
    SchFile schFile;
    if (fileName.isEmpty()) {
        throw "sch�ļ�������";
    }
    return schFile;
}
//mod
ModFile GimReader::readModFile(QString fileName)
{
    if (fileName.isEmpty()) {
        qDebug() << "mod�ļ��Ҳ���" << endl;
        throw ".mod�ļ�������";
    }
    //��ȡ��Ӧ��xml
    QString readPath = path + "/MOD/" + fileName;
    QDomNodeList content = readXmlPara(readPath);//////���ԣ��ǵøĵ�ַ
    ModFile modFile;
    if (!content.isEmpty()) {
        modFile.setFileName(fileName);
        //��Ϊ<Entities>ֻ��һ����ֱ�ӻ�ȡ��һ��
        QDomNode entities = content.item(0);
        //�ٻ�ȡ<Entities>�����е��¼���ǩList ��<Entity>,<Entity>,<Entity>,...
        QDomNodeList entityList = entities.childNodes();
        //qDebug() << entityList.size() << endl;
        QList<SimpleEntity> simpleentitylist;
        QList<BoolEntity> boolentitylist;
        //����һ��xml�е�entity
        for (int i = 0; i < entityList.size(); i++) {
            //��ȡ��i��<Entity>
            //ת��ΪԪ��
            QDomElement entity = entityList.item(i).toElement();
            QString  type = entity.attribute("Type");

            if (type == ("simple")) {

                SimpleEntity simpleEntity;

                simpleEntity.setEntityId((entity.attribute("ID")).toInt());
                //QVariant t = entity.attribute("Visible");
                //simpleEntity.setVisible(t.toBool());
                simpleEntity.setVisible(entity.attribute("Visible"));
                //��ȡ<Entity>�е��¼���ǩList ��<ͼԪ/>,<Color/>,<TransformMatrix/>
                QDomNodeList entityParas = entity.childNodes();
                //����<ͼԪ/>,<Color/>,<TransformMatrix/>
                for (int j = 0; j < entityParas.size(); j++) {
                    QDomElement item = entityParas.item(j).toElement();
                    if (item.nodeName() == ("Color")) {
                        simpleEntity.setR((item.attribute("R")).toInt());
                        simpleEntity.setG((item.attribute("G")).toInt());
                        simpleEntity.setB((item.attribute("B")).toInt());
                        simpleEntity.setA((item.attribute("A")).toInt());
                    }
                    else if (item.nodeName() == ("TransformMatrix")) {
                        simpleEntity.setTransformMatrix(item.attribute("Value"));
                    }
                    else {
                        simpleEntity.setGraphicElement(item);
                    }

                }
                simpleentitylist.append(simpleEntity);
                // modFile.setSimpleEntities(simpleentitylist);
                  //qDebug() << modFile.getSimpleEntities().length() << endl;
            }
            else if (type == ("boolean")) {
                BoolEntity boolEntity;
                boolEntity.setEntityId((entity.attribute("ID")).toInt());
                boolEntity.setVisible(entity.attribute("Visible"));
                //QVariant t = entity.attribute("Visible");
                //boolEntity.setVisible(t.toBool());
                //��ȡ<Entity>�е��¼���ǩList ��<Boolean/>,<Color/>,<TransformMatrix/>
                QDomNodeList entityChildren = entity.childNodes();
                //����<Boolean/>,<Color/>,<TransformMatrix/>
                for (int j = 0; j < entityChildren.size(); j++) {
                    QDomElement item = entityChildren.item(j).toElement();

                    if (item.nodeName() == ("Color")) {
                        boolEntity.setR((item.attribute("R")).toInt());
                        boolEntity.setG((item.attribute("G")).toInt());
                        boolEntity.setB((item.attribute("B")).toInt());
                        boolEntity.setA((item.attribute("A")).toInt());
                    }
                    else if (item.nodeName() == ("TransformMatrix")) {
                        QString mat = item.attribute("Value");
                        boolEntity.setTransformMatrix(item.attribute("Value"));
                    }
                    else if (item.nodeName() == ("Boolean")) {
                        boolEntity.setBoolEntityId1((item.attribute("Entity1")).toInt());
                        boolEntity.setBoolEntityId2((item.attribute("Entity2")).toInt());
                        boolEntity.setBoolType(item.attribute("Type"));

                    }

                }
                boolentitylist.append(boolEntity);
                //modFile.setBoolEntities(boolentitylist);
            }


            //qDebug() << modFile.getSimpleEntities().length() << endl;
        }
        modFile.setSimpleEntities(simpleentitylist);
        //qDebug() << modFile.getSimpleEntities().length() << endl;
        modFile.setBoolEntities(boolentitylist);
    }
    return modFile;
}

//����������Ԫ�����ݵ��ļ�,fam
QList<TripletPara> GimReader::readTripletPara(QString fileName, EntityName entityName)
{
    if (fileName.isEmpty()) {
        qDebug() << "fam�ļ�������" << endl;
        throw"fam������";
    }

    QString readPath;

    if (entityName == EntityName::ObjectModel)
    {
        readPath = path + "/DEV/" + fileName;
        //qDebug() << "����dev�е�fam�ļ�" << endl;
    }
    else
    {
        readPath = path + "/CBM/" + fileName;
    }
    QList<TripletPara> content;
    QFile file(readPath);
    try {
        if (file.open(QIODevice::ReadOnly)) {
            //qDebug() << "��ʼ��ȡfam����" << endl;
            QTextStream reader(&file);
            QTextCodec* code = QTextCodec::codecForName("utf8");//���˲��õ���UTF8��ʽ ��ʾ��
            reader.setCodec(code);
            //QBuffer reader(&file.readAll());
            QString tStr;
            while (!reader.atEnd()) {
                tStr = reader.readLine();
                if (tStr.isEmpty() || tStr == ("[��Ʋ���]")) {
                    continue;
                }
                QStringList splits = tStr.split("=");

                if (splits.length() == 3) {
                    TripletPara para;

                    para.setParaNameEN(splits[0]);
                    para.setParaNameCH(splits[1]);
                    para.setParameter(splits[2]);
                    content.append(para);
                    //TripletPara tripletPara(splits[0], splits[1], splits[2]);
                }
                else if (splits.length() == 2) {
                    TripletPara para;
                    para.setParaNameEN(splits[0]);
                    para.setParaNameCH(splits[1]);
                    para.setParameter("");
                    content.append(para);

                }

            }
        }
    }
    catch (std::exception e) {
        qDebug() << "�ļ���ȡʧ��" << endl;
        throw ("exception", path + "�ļ���ȡʧ��");
        throw ("exception", e.what());

    }
    return content;
}
//�������Ƕ�Ԫ�����ݵ��ļ�,cbm
QMap<QString, QString> GimReader::readDoublePara(QString readPath)
{
    //qDebug() << 1111111111 << endl;
    if (readPath.isEmpty()) {
        qDebug() << "��ȡ��Ԫ���ļ�ʧ�ܣ���ַ������" << endl;
        throw"project.cbm������";
    }
    //qDebug() << readPath << endl;
    QFile file(readPath);

    //QBuffer reader;
    QMap<QString, QString> content;
    try {
        // QBuffer reader(&file.readAll());
        if (file.open(QIODevice::ReadOnly)) {
            QTextStream reader(&file);
            QTextCodec* code = QTextCodec::codecForName("utf8");//���˲��õ���UTF8��ʽ ��ʾ��
            reader.setCodec(code);
            QString tStr;
            int flag = 0;  //�ж��Ƿ���solidmodel+i
            while (!reader.atEnd()) {
                tStr = reader.readLine();
                //qDebug() << tStr << endl;
                QStringList splits = tStr.split("=");

                if (flag == 1 && splits[0] == "TRANSFORMMATRIX0")
                {
                    content.insert("#" + splits[0], splits[1]);
                }
                else
                {
                    if (splits[0] == "TRANSFORMMATRIX0")
                    {
                        flag = 1;
                    }
                    content.insert(splits[0], splits[1]);
                }

            }

        }
    }
    catch (std::exception e) {
        qDebug() << "����Ԫ��ʧ��" << endl;
    }
    return content;
}
//��ȡxml��ʽ���ļ�,mod
QDomNodeList GimReader::readXmlPara(QString readPath)
{
    if (readPath.isEmpty())
    {
        throw "�ļ�������";
    }
    QFile file(readPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {  // ��ֻ��ģʽ��
        qDebug() << QString("Cannot read file %1(%2).").arg(readPath).arg(file.errorString());

    }
    QDomDocument doc;
    //���ļ����ݶ���doc��
    QDomNodeList entitiesList;
    if (!doc.setContent(&file)) {
        qDebug() << "parse file failed at line" << endl;
        file.close();

    }
    else {

        //Ȼ����firstChild�õ����Ľ��
        QDomElement entity = doc.documentElement();
        //��ȡ�ýڵ�������ӽ��
        entitiesList = entity.childNodes();//�ӽ����Entities ֻ��һ�� �����ӽ����Entity

    }
    //qDebug() << "entities��"<<entitiesList.length() << endl;
    return  entitiesList;
}

//ֱ�Ӷ�ȡȫ��mod�ļ������ڻ�ͼ
QList<ModFile> GimReader::GetGimModFileList2()
{
    QString a = path + "/CBM";
    QDir dir(a);
    QString filter;
    QStringList nameFilters;
    nameFilters << "*.cbm";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    qDebug() << files.size() << endl;
    //qDebug() << files << endl;

    //for (int i = 0; i < files.size(); i++) {
        //QString ap=files[i];

        //qDebug() << ap << endl;
        //QString p = a +"/" + ap;
        //qDebug() << p << endl;
    QList<ModFile> ModFileList;
    /* ModFile modFile = readModFile("da247fc1-443b-4e56-9596-5f8755f0852c.mod");

     ModFileList.append(modFile);
     objectModel = readObjectModel("25fd64c8-0475-49d2-b8bc-bfda6e72141d.dev");
     sysParas = readTripletPara("16e3c189-4538-4f44-bff0-2434039fbe2a.fam", EntityName::ObjectModel);*/

     /*qDebug() << sysParas[1].getParaNameEN() << endl;
     qDebug() << sysParas[1].getParaNameCH() << endl;
     qDebug() << sysParas[1].getParameter() << endl;*/
     //}
     /*QString ap;
     ModFile modFile=readModFile(ap);
     ModFileList.append(modFile);*/
     //qDebug() << ModFileList.size() << endl;

    return ModFileList;
}
//ID������
void GimReader::read()
{   //ID��
    if (name > 0) {
        QString devname;
        if (name <= gimModFileList.size()) {
            devname = gimModFileList[name - 1].getObjFileName();//�豸dev�ļ���
        }
        else {
            devname = gimModFileList[name - gimModFileList.size() - 1].getObjFileName();//�豸dev�ļ���
        }
        //qDebug() << "devname:"<<devname << endl;
        objectModel = readObjectModel(devname);//���豸
        QString famname = objectModel.getFamFileName();
        //qDebug() << "fam:" << famname << endl;
        sysParas = readTripletPara(famname, EntityName::ObjectModel);
        //qDebug() << "ѡ��sys:" << sysParas.size() << endl;
    }
}
//��project.cbm��f1�ļ���
QList<QString> GimReader::getFlSystemNode()
{

    QString readPath = path + "/CBM/" + "project.cbm";
    //qDebug() << readPath << endl;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (!content.isEmpty()) {
        FlSystemNode.append(content.value("SUBSYSTEM"));
        getF2SystemNode(content.value("SUBSYSTEM"));
    }
    else {
        qDebug() << "project.cbmΪ��" << endl;
    }
    //readF1System(content.value("SUBSYSTEM"));

    //qDebug() << "f1�ļ�����"<<FlSystemNode << endl;
    return FlSystemNode;
}
//��f1.cbm��f2�ļ���
QList<QString> GimReader::getF2SystemNode(QString fileName)
{
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F1System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        if (content.count("SUBSYSTEMS.NUM") > 0) {
            if (content.value("SUBSYSTEMS.NUM") != "0") {
                for (int i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++) {
                    //qDebug() << content.value("SUBSYSTEM" + QString::number(i)) << endl;
                    //f1System.setSubSystems(subsystems);
                    F2SystemNode.append(content.value("SUBSYSTEM" + QString::number(i)));
                    getF3SystemNode(content.value("SUBSYSTEM" + QString::number(i)));
                    //qDebug() << "f2�ļ�����" << content.value("SUBSYSTEM" + QString::number(i)) << endl;
                }
            }
        }
    }
    else {
        qDebug() << "һ��.cbmΪ�գ�"  << fileName << endl;
    }
    return F2SystemNode;
}
//��f2.cbm��f3�ļ���
QList<QString> GimReader::getF3SystemNode(QString fileName)
{
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F2System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        if (content.count("SUBSYSTEMS.NUM") > 0) {
            if (content.value("SUBSYSTEMS.NUM") != "0") {
                for (int i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++) {
                    //qDebug() << content.value("SUBSYSTEM" + QString::number(i)) << endl;
                    //f1System.setSubSystems(subsystems);
                    F3SystemNode.append(content.value("SUBSYSTEM" + QString::number(i)));
                    F3asF4(content.value("SUBSYSTEM" + QString::number(i)));
                    //qDebug() << "f3�ļ�����" << content.value("SUBSYSTEM" + QString::number(i)) << endl;
                }
            }
        }
    }
    else {
        qDebug() << "����.cbmΪ�գ�" << fileName << endl;
    }
    return F3SystemNode;
}
//��f3.cbm��f4�ļ���
void GimReader::F3asF4(QString fileName)
{
    //������.cbm
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F3System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        if (content.count("BASEFAMILY1") > 0) {
            //F3toF4
            F3toF4Node.append(fileName);
            if (content.count("SUBSYSTEMS.NUM") > 0) {
                if (content.value("SUBSYSTEMS.NUM") != "0") {
                    for (int i = 0; i < (content.value("SUBSYSTEMS.NUM")).toInt(); i++) {
                        allF4SystemNode.append(content.value("SUBSYSTEM" + QString::number(i)));
                        getF4SystemNode(content.value("SUBSYSTEM" + QString::number(i)));
                    }
                }
            }
        }
        else {
            //F3toObj
            F3toObjNode.append(fileName);
        }
    }
    else {
        qDebug() << "����.cbmΪ�գ�" << fileName << endl;
    }
}
//��f4.cbm�޳�
QList<QString> GimReader::getF4SystemNode(QString fileName)
{
    QString readPath = path + "/CBM/" + fileName;
    QMap<QString, QString> content = readDoublePara(readPath);
    if (!content.isEmpty()) {
        //�����ʶ
        if (content.value("ENTITYNAME") != ("F4System")) {
            qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
            throw "CODE_ERROR";
        }
        F4SystemNode.append(fileName);
        if (content.count("SUBDEVICES.NUM") > 0)
        {
            if (content.value("SUBDEVICES.NUM") != ("0"))
            {
                havasubdevices.append(fileName);
                for (int i = 0; i < (content.value("SUBDEVICES.NUM")).toInt(); i++)
                {
                    PartIndexNode.append(content.value("SUBDEVICE" + QString::number(i)));
                    //readPartIndex(content.value("SUBDEVICE" + QString::number(i)));
                }
            }
        }
    }
    else {
        qDebug() << "�ļ�.cbmΪ�գ�" + fileName << endl;
    }
    return F4SystemNode;
}

//��F4��PartIndex
QList<QString> GimReader::getPartIndexNode(QString fileName)
{
    //QString readPath = path + "/CBM/" + fileName;
    //QMap<QString, QString> content = readDoublePara(readPath);
    //if (!content.isEmpty()) {
    //    //�����ʶ
    //    if (content.value("ENTITYNAME") != ("F4System")) {
    //        qDebug() << path << "/CBM/ " << fileName << "��ʶ����" << endl;
    //        throw "CODE_ERROR";
    //    }
    //    if (content.count("SUBDEVICES.NUM") > 0)
    //    {
    //        if (content.value("SUBDEVICES.NUM") != ("0"))
    //        {
    //            for (int i = 0; i < (content.value("SUBDEVICES.NUM")).toInt(); i++)
    //            {
    //                PartIndexNode.append(content.value("SUBDEVICE" + QString::number(i)));
    //                //readPartIndex(content.value("SUBDEVICE" + QString::number(i)));
    //            }
    //        }
    //    }
    //}
    //else {
    //    qDebug() << "��ȡ�ļ�.cbmʧ��"+ fileName << endl;
    //}
    return PartIndexNode;
}

//һ��dev��ȡ��Ӧ��mod�ļ�
QList<ModFile> GimReader::GetGimModFileList()
{
    //��ȡ���е�F4System�ļ�
    //qDebug() << f3subsystems.size() << endl;
    //for (int i = 0; i < 1; i++) {
        //QString ap = f3subsystems[i];                  //  һ��f4�ļ���
        //QString f4path = extractPath + "/CBM/" + ap;
        //qDebug() << "f4:" << f4path << endl;
        //F4System f4System = readF4System("a1323088-1084-4a23-a0b4-52c72369365f.cbm");
        ////qDebug() << "f4���ò���������" << f4System.getSubDevice().size() << endl;
        //if (f4System.getSubDevice().size() != 0) {//f4���ò���
        //    //QList<QString> modname;
        //    qDebug() << "f4���õ�dev��" << f4System.getObjectModel() << endl;
        //    ObjectModel objectModel = readObjectModel(f4System.getObjectModel());//��dev�ļ�
        //    //1�����豸dev��mod��
        //    QList<QString> solidModels = objectModel.getSolidModels();  //��ȡdev���õ�phm�ļ�
        //    for (int i = 0; i < solidModels.size(); i++)
        //    {
        //        SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
        //        QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ��б�
        //        for (int i = 0; i < children.size(); i++) {
        //            if (children[i].endsWith(".mod")) {
        //                //modname.append(children[i]);
        //                ModFile modFile = readModFile(children[i]);
        //                modFile.setObjFileName(f4System.getObjectModel());//��mod�ļ���dev��Ӧ��
        //                gimModFileList.append(modFile);
        //            }
        //        }
        //    }
        //    //2����������mod��
        //    QList<QString> subvices = objectModel.getSubObjectModels();
        //    for (int i = 0; i < subvices.size(); i++) {
        //        //qDebug() << subvices[i] << endl;
        //        ObjectModel objectModel = readObjectModel(subvices[i]);//��dev�ļ�
        //        QList<QString> solidModels = objectModel.getSolidModels();//��ȡdev���õ�phm�ļ�
        //        for (int i = 0; i < solidModels.size(); i++)
        //        {
        //            SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
        //            QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ�
        //            for (int i = 0; i < children.size(); i++) {
        //                if (children[i].endsWith(".mod")) {
        //                    //modname.append(children[i]);
        //                    ModFile modFile = readModFile(children[i]);
        //                    modFile.setObjFileName(f4System.getObjectModel());//��mod�ļ���dev��Ӧ��
        //                    gimModFileList.append(modFile);
        //                }
        //            }
        //        }
        //    }
        //}
        //else {//ֻ��һ��dev
        //    qDebug() << "f4���õ�dev��" << f4System.getObjectModel() << endl;
        //    ObjectModel objectModel = readObjectModel(f4System.getObjectModel());//��dev�ļ�
        //    QList<QString> solidModels = objectModel.getSolidModels();  //��ȡdev���õ�phm�ļ�
        //    for (int i = 0; i < solidModels.size(); i++)
        //    {
        //        SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
        //        QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ�
        //        for (int i = 0; i < children.size(); i++) {
        //            if (children[i].endsWith(".mod")) {
        //                //��ȡ��Ӧ��xml
        //                ModFile modFile = readModFile(children[i]);
        //                modFile.setObjFileName(f4System.getObjectModel());//��mod�ļ���dev��Ӧ��
        //                gimModFileList.append(modFile);
        //            }
        //        }
        //    }
        //}

    //�ҳ����е�dev�ļ��������е��豸
    QString a = path + "/DEV";
    QDir dir(a);
    QString filter;
    QStringList nameFilters;
    nameFilters << "*.dev";
    QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
    //qDebug() << "files�ĸ�����"<<files.size() << endl;
    //qDebug() <<"files:"<< files << endl;
    //QList<QString> devname = files;
    //qDebug() << "devname�ĸ�����" << devname.size() << endl;
    //qDebug() <<"devname:"<< devname << endl;
    for (int i = 0; i < files.size(); i++) {
        ObjectModel objectModel = readObjectModel(files[i]);        //��ȡdev�ļ�
        if (!objectModel.getSubObjectModels().isEmpty()) //dev�в���dev
        {
            //qDebug() << "files�ĸ�����" << files.size() << endl;
            //qDebug() << "�в�����" << files[i] ;
            //qDebug() << "�в�����" << i;
            files.removeOne(files[i]);
            QList<QString> subdevices = objectModel.getSubObjectModels();//��ȡ����dev
            //��ȡ��������dev
            for (int i = 0; i < subdevices.size(); i++) {
                //qDebug() << "���õĲ���dev��" << subdevices[i] << endl;
                ObjectModel objectModel1 = readObjectModel(subdevices[i]);//������dev
                files.removeOne(subdevices[i]);
                QList<QString> solidModels1 = objectModel1.getSolidModels();  //��ȡdev���õ�phm�ļ�
                for (int i = 0; i < solidModels1.size(); i++)
                {
                    SolidModel solidModel1 = readSolidModel(solidModels1[i]); //��ȡphm�ļ�
                    QList<QString> children1 = solidModel1.getchildren();     //��ȡphm���õ�mod�ļ�
                    for (int i = 0; i < children1.size(); i++) {
                        if (children1[i].endsWith(".mod")) {
                            ModFile modFile1 = readModFile(children1[i]);
                            modFile1.setObjFileName(objectModel.getFileName());//��mod�ļ���dev�ļ�����Ӧ��
                            //modFile1.setSymbolName(objectModel.getSymbolName());//��mod�ļ�д���豸��
                            gimModFileList.append(modFile1);
                            //devices.append(objectModel.getSymbolName());
                        }
                    }
                }
            }
        }
    }
    /*qDebug() << "files�ĸ�����" << files.size() << endl;
    qDebug() << "files��" << files << endl;*/
    
    for (int i = 0; i < files.size(); i++) {
        //qDebug() << "dev:" << files[i];
        ObjectModel objectModel = readObjectModel(files[i]);        //��ȡdev�ļ�
        if (objectModel.getSubObjectModels().isEmpty()) //devû�в���dev
        {
            QList<QString> solidModels = objectModel.getSolidModels();  //��ȡdev���õ�phm�ļ�
            for (int i = 0; i < solidModels.size(); i++)
            {
                SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
                QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ�
                for (int i = 0; i < children.size(); i++) {
                    if (children[i].endsWith(".mod")) {
                        ModFile modFile = readModFile(children[i]);
                        //qDebug() << "mod:" << children[i] << endl;
                        modFile.setObjFileName(objectModel.getFileName());//��mod�ļ���dev�ļ�����Ӧ��
                        //modFile.setSymbolName(objectModel.getSymbolName());//��mod�ļ�д���豸��
                        gimModFileList.append(modFile);
                    }
                }
            }
        }
    }
    qDebug() << "modFile�ļ��ĸ���:" << gimModFileList.size();
    //for (int i = 0; i < gimModFileList.size(); i++) {
    //    //qDebug() << i << endl;
    //    QString modname=gimModFileList[i].getFileName();
    //    QString devname=gimModFileList[i].getObjFileName();
    //    ObjectModel objectModel = readObjectModel(devname);
    //    QString famname=objectModel.getFamFileName();
    //    //QList<TripletPara> sysParas=readTripletPara(famname, objectModel.getEntityName());
    //    //qDebug() << "mod:" << modname <<" "<<"dev:"<<devname<< " " << "fam:" << famname << endl;
    //    qDebug() << "mod:" << modname<<" "<<objectModel.getSymbolName() << endl;
    //}
    //qDebug() << "dev" << gimModFileList[0].getObjFileName();
    //qDebug() << devname << endl;
    //ObjectModel objectModel1=readObjectModel(gimModFileList[0].getObjFileName());
    //qDebug() << objectModel1.getFamFileName() << endl;
    return gimModFileList;
}

//�豸����mod�ļ���Ӧ
QList<ModFile> GimReader::GetGimModFileList1()
{
    //1���豸����F3.cbm
    if (!F3toObjNode.isEmpty()) {
        for (int i = 0; i < F3toObjNode.size(); i++) {
            //QString readPath = path + "/CBM/" + F3toObjNode[i];
            F3System f3System = readF3System(F3toObjNode[i]);//��f3.cbm�ļ�
            QString f3objectModel = f3System.getObjectModel();//���f3�豸����Ӧ��dev
            QString transformMatrix2 = f3System.getTransformMatrix();//��ȡf3�豸��Ӧ������
            //qDebug() << f3objectModel << endl;
            ObjectModel objectModel = readObjectModel(f3objectModel);//��dev�ļ�
            QList<QString> solidModels = objectModel.getSolidModels();  //��ȡdev���õ�phm�ļ�
            for (int i = 0; i < solidModels.size(); i++)
            {
                SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
                QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ�
                for (int i = 0; i < children.size(); i++) {
                    if (children[i].endsWith(".mod")) {
                        ModFile modFile = readModFile(children[i]);
                        modFile.setObjFileName(objectModel.getFileName());//��mod�ļ���dev�ļ�����Ӧ��
                        modFile.setSymbolName(objectModel.getSymbolName());//�豸��
                        modFile.settransformMatrix2(transformMatrix2);//�豸����
                        gimModFileList.append(modFile);
                        //devices.append(objectModel.getSymbolName());
                    }
                }
            }
        }
    }
    //2���豸����F4.cbm
    if (!F4SystemNode.isEmpty()) {
        for (int i = 0; i < F4SystemNode.size(); i++) {
            F4System f4System = readF4System(F4SystemNode[i]);//��f4.cbm�ļ�
            QString f4objectModel = f4System.getObjectModel();//���f4�豸����Ӧ��dev
            QString transformMatrix2 = f4System.getTransformMatrix();//��ȡf4�豸��Ӧ���豸����
            ObjectModel objectModel = readObjectModel(f4objectModel);//��dev�ļ�
            //1�������ڲ���dev
            if (objectModel.getSubObjectModels().isEmpty()) {
                QList<QString> solidModels = objectModel.getSolidModels();  //��ȡdev���õ�phm�ļ�
                for (int i = 0; i < solidModels.size(); i++)
                {
                    SolidModel solidModel = readSolidModel(solidModels[i]); //��ȡphm�ļ�
                    QList<QString> children = solidModel.getchildren();     //��ȡphm���õ�mod�ļ�
                    for (int i = 0; i < children.size(); i++) {
                        if (children[i].endsWith(".mod")) {
                            ModFile modFile = readModFile(children[i]);
                            modFile.setObjFileName(objectModel.getFileName());//��mod�ļ���dev�ļ�����Ӧ��
                            modFile.setSymbolName(objectModel.getSymbolName());//�豸��
                            modFile.settransformMatrix2(transformMatrix2);//�豸����
                            gimModFileList.append(modFile);
                            //devices.append(objectModel.getSymbolName());
                        }
                    }
                }
            }
            else {//2�����ڲ���dev
                QList<QString> subdevices = objectModel.getSubObjectModels();//��ȡ����dev
                QList<QString>transformMatrix1 = objectModel.getSubObjTransformMatrix();//��ȡ��������
                for (int i = 0; i < subdevices.size(); i++) {
                    ObjectModel objectModel1 = readObjectModel(subdevices[i]);//������dev
                    QList<QString> solidModels1 = objectModel1.getSolidModels();  //��ȡdev���õ�phm�ļ�
                    for (int a = 0; a < solidModels1.size(); a++)
                    {
                        SolidModel solidModel1 = readSolidModel(solidModels1[a]); //��ȡphm�ļ�
                        QList<QString> children1 = solidModel1.getchildren();     //��ȡphm���õ�mod�ļ�
                        for (int j = 0; j < children1.size(); j++) {
                            if (children1[j].endsWith(".mod")) {
                                ModFile modFile1 = readModFile(children1[j]);
                                modFile1.setObjFileName(objectModel.getFileName());//��mod�ļ���dev�ļ�����Ӧ��
                                modFile1.setSymbolName(objectModel.getSymbolName());//�豸��
                                modFile1.settransformMatrix1(transformMatrix1[i]);//�豸��������
                                modFile1.settransformMatrix2(transformMatrix2);//�豸����
                                gimModFileList.append(modFile1);
                                //devices.append(objectModel.getSymbolName());
                            }
                        }
                    }
                }
            }
        }
    }
    qDebug() << "gimModFileList�ĸ�����" << gimModFileList.size() << endl;
    for (int i = 0; i < gimModFileList.size(); i++) {
        qDebug() <<"�豸���� "<<gimModFileList[i].getSymbolName()<<";" <<"mod:"<<gimModFileList[i].getFileName() << "�������꣺" << gimModFileList[i].gettransformMatrix1() << ";" << "�豸���꣺" << gimModFileList[i].gettransformMatrix2() << endl;
    }
    return gimModFileList;
}