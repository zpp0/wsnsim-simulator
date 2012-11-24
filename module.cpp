/**
 *
 * File: module.cpp
 * Description: modules functions
 * Author: Yarygin Alexander <yarygin.alexander@gmail.com>
b *
 **/

#include "module.h"

ModuleType getModuleType(QString typeName)
{
    ModuleType type;
    if (typeName == "environment")
        type = ModuleType_Environment;
    else if (typeName == "hardware")
            type = ModuleType_Hardware;
    else if (typeName == "software")
        type = ModuleType_Software;
    else
        type = ModuleType_Undefined;

    return type;
}

ModuleParamType getModuleParamType(QString typeName)
{
    ModuleParamType type;
    if (typeName == "int")
        type = ModuleParamType_INT;
    else if (typeName == "uint8")
        type = ModuleParamType_UINT8;
    else if (typeName == "uint16")
        type = ModuleParamType_UINT16;
    else if (typeName == "uint32")
        type = ModuleParamType_UINT32;
    else if (typeName == "uint64")
        type = ModuleParamType_UINT64;
    else if (typeName == "double")
        type = ModuleParamType_DOUBLE;
    else if (typeName == "string")
        type = ModuleParamType_STRING;
    else if (typeName == "nodes")
        type = ModuleParamType_NODES;
    else if (typeName == "table")
        type = ModuleParamType_TABLE;
    else
        type = ModuleParamType_Undefined;

    return type;
}

Module::Module(ModuleData& moduleData)
{
    name = moduleData.moduleInfo["name"];
    fileName = moduleData.fileName;

    // prepare module type
    type = getModuleType(moduleData.moduleInfo["type"]);

    // prepare module params
    foreach(ModuleParam param, moduleData.params) {
        ModuleParameter parameter;

        parameter.name = param.name;
        parameter.type = getModuleParamType(param.type);
        parameter.value = param.value;

        params += parameter;
    }

    // prepare moduleID
    ID = moduleData.moduleInfo["ID"].toInt();

    // prepare dependencies
    foreach(QString depName, moduleData.dependencies.keys()) {
        ModuleDependence dep;
        dep.name = depName;
        QString typeName = moduleData.dependencies[depName].first;
        dep.moduleID = moduleData.dependencies[depName].second;

        dep.type = getModuleType(typeName);

        dependencies += dep;
    }
}
