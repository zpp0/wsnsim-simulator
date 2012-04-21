/**
 *
 * IField.h
 *
 **/

#ifndef IFIELD_H
#define IFIELD_H

#include <QtCore>

#include "IEnvironment.h"

#include "types.h"

class IField : public IEnvironment
{
public:
    virtual double measure(double* coord, VirtualTime time) = 0;
};

Q_DECLARE_INTERFACE(IField,
                    "simulator.IField/0.1")

#endif // IFIELD_H
