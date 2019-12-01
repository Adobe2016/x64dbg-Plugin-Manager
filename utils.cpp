// Copyright (c) 2019 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "utils.h"

Utils::Utils(QObject *parent) : QObject(parent)
{

}

QList<Utils::RECORD> Utils::getRecords(QString sRootPath)
{
    QList<Utils::RECORD> listResult;

    QFileInfo fi(sRootPath);

    sRootPath=fi.absoluteDir().path()+QDir::separator()+fi.fileName();

    _getRecords(sRootPath,sRootPath,&listResult);

    return listResult;
}

bool Utils::checkMData(Utils::MDATA *pMData, QString *psErrorString)
{
    bool bResult=true;

    if(pMData->sName=="")
    {
        *psErrorString=tr("Invalid name");

        bResult=false;
    }
    // TODO
    return bResult;
}

QString Utils::createBundleName(Utils::MDATA *pMData)
{
    QString sResult;

    sResult+=pMData->sName;

    if(pMData->sVersion!="")
    {
        sResult+=QString("_%1").arg(pMData->sVersion);
    }

    if(pMData->sDate!="")
    {
        sResult+=QString("_%1").arg(pMData->sDate);
    }

    return sResult;
}

bool Utils::isPluginValid(QString sFileName)
{
    bool bResult=false;

    QFile file;

    file.setFileName(sFileName);

    if(file.open(QIODevice::ReadOnly))
    {
        XZip xzip(&file);

        if(xzip.isVaild())
        {
            QList<XArchive::RECORD> listRecords=xzip.getRecords();

            bResult=XArchive::isArchiveRecordPresent("plugin_info.json",&listRecords);
        }

        file.close();
    }

    return bResult;
}

void Utils::_getRecords(QString sRootPath, QString sCurrentPath, QList<Utils::RECORD> *pListRecords)
{
    QFileInfo fi(sCurrentPath);

    RECORD record={};

    record.bIsFile=true;
    record.sFullPath=fi.absoluteFilePath();
    record.sPath=record.sFullPath;
    record.bIsFile=fi.isFile();

    record.sPath=record.sPath.remove(0,sRootPath.size()+1);

    if(record.sPath!="")
    {
        pListRecords->append(record);
    }

    if(!record.bIsFile)
    {
        QDir dir(sCurrentPath);
        QFileInfoList eil=dir.entryInfoList();

        int nCount=eil.count();

        for(int i=0;i<nCount;i++)
        {
            QString sFN=eil.at(i).fileName();

            if((sFN!=".")&&(sFN!=".."))
            {
                _getRecords(sRootPath,eil.at(i).absoluteFilePath(),pListRecords);
            }
        }
    }
}