/***************************************************************************
                          enginepregain.cpp  -  description
                             -------------------
    copyright            : (C) 2002 by Tue and Ken Haste Andersen
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "enginepregain.h"
#include "controllogpotmeter.h"
#include "controlpotmeter.h"
#include "controlpushbutton.h"
#include "configobject.h"
#include "controlobject.h"

#include "sampleutil.h"


/*----------------------------------------------------------------
   A pregaincontrol is ... a pregain.
   ----------------------------------------------------------------*/
EnginePregain::EnginePregain(const char * group)
{
    potmeterPregain = new ControlLogpotmeter(ConfigKey(group, "pregain"), 4.);
    //Replay Gain things
    m_pControlReplayGain = new ControlObject(ConfigKey(group, "replaygain"));


    if(ControlObject::getControl(ConfigKey("[ReplayGain]", "InitialReplayGainBoost"))==NULL)
    {
        m_pReplayGainBoost = new ControlPotmeter(ConfigKey("[ReplayGain]", "InitialReplayGainBoost"),0., 15.);
        m_pEnableRG = new ControlPotmeter(ConfigKey("[ReplayGain]", "ReplayGainEnabled"));
    }
    else
    {
        m_pReplayGainBoost = (ControlPotmeter*)ControlObject::getControl(ConfigKey("[ReplayGain]", "InitialReplayGainBoost"));
        m_pEnableRG = (ControlPotmeter*)ControlObject::getControl(ConfigKey("[ReplayGain]", "ReplayGainEnabled"));
    }

}

EnginePregain::~EnginePregain()
{
    delete potmeterPregain;
    delete m_pControlReplayGain;
}

void EnginePregain::process(const CSAMPLE * pIn, const CSAMPLE * pOut, const int iBufferSize)
{

    float fEnableRG = m_pEnableRG->get();
    float fReplayGainBoost = m_pReplayGainBoost->get();
    CSAMPLE * pOutput = (CSAMPLE *)pOut;
    float fGain = potmeterPregain->get();
    float fRGain = m_pControlReplayGain->get();
    m_fReplayGainCorrection=1;
    if(fRGain*fEnableRG != 0)
    {

        //Passing a user defined boost
        m_fReplayGainCorrection=fRGain*pow(10, fReplayGainBoost/20);
    }
    fGain = (fGain/2)*m_fReplayGainCorrection;;
    if (fGain == 1.)
    {
        if (pIn!=pOut)
        {
            for (int i=0; i<iBufferSize; ++i)
                pOutput[i] = pIn[i];
            //memcpy(pOutput, pIn, sizeof(CSAMPLE) * iBufferSize);
        }
        return;
    }


    // SampleUtil deals with aliased buffers and gains of 1 or 0.
    SampleUtil::copyWithGain(pOutput, pIn, fGain, iBufferSize);
}
