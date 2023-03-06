#pragma once
#pragma once
#include <QtCore/qstring.h>
#include "EntityName.h"
class GimFile
{
public:
	QString getParentName() {
		return paratName;
	}
	void setParentName(QString paratName) {
		this->paratName = paratName;
	}
	QString getFileName() {
		return fileName;
	}
	void setFileName(QString fileName) {
		this->fileName = fileName;
	}
	EntityName getEntityName() {
		return entityName;
	}
	void setEntityName(EntityName entityName) {
		this->entityName = entityName;
	}
private:
	QString paratName;
	EntityName entityName;
	QString fileName;
};

