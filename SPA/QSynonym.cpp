#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QSynonym.h"

void QArgument::setQArgumentType(std::string name) {
    QSynonymType = name;
}

void QArgument::setQArgumentName(std::string type) {
    QSynonymName = type;
}

std::string QArgument::getQArgumentType() const {
    return QSynonymType;
}

std::string QArgument::getQArgumentName() const {
    return QSynonymName;

}