/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2015 Allen Institute for Brain Science

    ------------------------------------------------------------------

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef NEUROPIXTHREAD_H_INCLUDED
#define NEUROPIXTHREAD_H_INCLUDED

#include "../../../JuceLibraryCode/JuceHeader.h"

#include <stdio.h>
#include <string.h>

#include "DataThread.h"
#include "../GenericProcessor/GenericProcessor.h"


/**

  Communicates with imec Neuropix probes.

  @see DataThread, SourceNode

*/

class NeuropixThread : public DataThread
{

public:

    NeuropixThread(SourceNode* sn);
    ~NeuropixThread();

    bool updateBuffer();

    /** Returns true if the data source is connected, false otherwise.*/
    bool foundInputSource();

    /** Initializes data transfer.*/
    bool startAcquisition();

    /** Stops data transfer.*/
    bool stopAcquisition();

    /** Returns the number of continuous headstage channels the data source can provide.*/
    int getNumHeadstageOutputs();

    /** Returns the number of continuous aux channels the data source can provide.*/
	int getNumAuxOutputs();

    /** Returns the number of continuous ADC channels the data source can provide.*/
	int getNumAdcOutputs();

    /** Returns the sample rate of the data source.*/
    float getSampleRate();

    /** Returns the volts per bit of the data source.*/
    float getBitVolts(Channel* chan);

    /** Returns the number of event channels of the data source.*/
	int getNumEventChannels();

	/** Selects which electrode is connected to each channel. */
	void selectElectrode(int chNum, int connection);

	/** Selects which reference is used for each channel. */
	void setReference(int chNum, int refSetting);

	/** Sets the gain for each channel. */
	void setGain(int ch, int apGain, int lfpGain);

	/** Sets the gain for each channel. */
	void setFilter(int filter);


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuropixThread);

private:
    bool baseStationAvailable;

};

#endif  // NEUROPIXTHREAD_H_INCLUDED
