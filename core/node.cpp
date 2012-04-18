/**
 *
 * File: node.cpp
 * Author: Yarygin Alexander <zpp0@mail.ru>
 *
 **/

#include "node.h"

IModule* Node::getInterface(IModule* module, QString interface)
{
    // qDebug() << "test0";    
    // FIXME: duplicate code in node.cpp
    IModule* Interface = NULL;
    // if (module->moduleImportInterfaces().indexOf(interface) != -1)
    Interface = m_interfaces_TEMP[interface];

    return Interface;
}

void Node::inter(QString type)
{
    qDebug() << "in timer interrupt node" << ID;

    if (type == "trySendMessage") {

        qDebug() << "try send message with CSMA-CA";

        QByteArray message;
        
        char string[] = "hello";

        message += "0000"; // преамбула
        message += 0x7A; // SFD?
        message += strlen(string); // length
        message += string; // data
        
        qDebug() << "start CSMA-CA with message" << message;

        sendMessageCSMACA(message);
    }

    if (type == "CSMA-CA") {

        qDebug() << "CSMA-CA";

        if (m_rtx->CCA()) {
            qDebug() << "CCA is true, send message";
            m_rtx->startTX(m_CSMA_CA_message);
            m_timer->start(sendMessagePeriod, timerInterruptHelper, this, "trySendMessage");
        }

        else {
            NB++;
            BE = std::min(BE + 1, aMaxBE);

            if (NB < macMaxCSMABackoffs)
                m_timer->start(CSMA_CA_delay(), timerInterruptHelper, this, "CSMA-CA");

            else {
                // qDebug() << "";
                m_timer->start(sendMessagePeriod, timerInterruptHelper, this, "trySendMessage");
            }
        }

    }
}

void Node::timerInterruptHelper(void* object, QString type)
{
    Node* node = (Node*) object;

    node->inter(type);
}

void Node::init()
{
    qDebug() << "in node init" << ID;

    foreach (IModule* imodule, m_interfaces_TEMP) {
        // qDebug() << "choose module" << imodule;
        if (imodule->moduleName() == "Timer") {
            // qDebug() << "get timer interface";
            m_timer = (ITimer*) imodule;
        }
        else if (imodule->moduleName() == "RTX") {
            // qDebug() << "get rtx interface";
            m_rtx = (Irtx*) imodule;
        }
    }

    // qDebug() << "timer" << m_timer;
    // qDebug() << "rtx" << m_rtx;

    m_timer->start(sendMessagePeriod, timerInterruptHelper, this, "trySendMessage");
}

VirtualTime Node::CSMA_CA_delay()
{
    int periods = ((double)rand() / (double)RAND_MAX) * (pow(2,BE)-1);
    VirtualTime wait = periods * pauseTime;
    return wait;
}
    
void Node::sendMessageCSMACA(byteArray message)
{
    m_CSMA_CA_message = message;

    BE = macMinBE;
    NB = 0;
    
    VirtualTime wait = CSMA_CA_delay();

    qDebug() << "CSMA-CA wait"
             << "BE" << BE
             << "NB" << NB
             << "wait" << wait;

    m_timer->start(wait, timerInterruptHelper, this, "CSMA-CA");
}

// #include "IRadioChannel.h"
// #include "IScene.h"


