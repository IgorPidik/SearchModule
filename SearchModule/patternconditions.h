#ifndef PATTERNCONDITIONS_H
#define PATTERNCONDITIONS_H

#include "conditionsinterface.h"
#include <QString>

class PatternConditions: public ConditionsInterface
{
public:
    PatternConditions(QString namePattern, QString contextPattern):namePattern(namePattern), contextPattern(contextPattern){}
    QString namePattern;
    QString contextPattern;

};

#endif // PATTERNCONDITIONS_H
