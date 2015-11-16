/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2015 Open Ephys and Allen Institute for Brain Science

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

#include <stdio.h>
#include "SpikeSynth.h"

SpikeSynth::SpikeSynth()
    : GenericProcessor("Spike Synth"),
      synthBuffer(2, 10000)
{

}

SpikeSynth::~SpikeSynth()
{

}


void SpikeSynth::updateSettings()
{
	// turns N channels into 2 audio outputs
	settings.numOutputs = 2;
    setPlayConfigDetails(getNumInputs(), getNumOutputs(), 44100.0, 128);

    numElectrodes = 0;

    // generate waveforms for each channel
    for (int i = 0; i < eventChannels.size(); i++)
    {
        ChannelType type = eventChannels[i]->getType();

        if (type == ELECTRODE_CHANNEL)
        {

        	numElectrodes++;

        }
    }
}

bool SpikeSynth::enable()
{

}

bool SpikeSynth::disable()
{

}

void SpikeSynth::updateWaveforms()
{
	waveformBuffer.setSize(numElectrodes, 1000);

    for (int i = 0; i < numElectrodes; i++)
    {
    	// set base frequency (plus interval per channel):
    	float freq = 1000. * (i+1);

    	// create saw waves at each frequency:
    	int period = (int) ((1./freq) * 44100.);
    	int n = 0;
    	float a = 1;
    	while (n < 1000)
    	{
    		a = 0.5 - (n % period) / float(period);
    		waveformBuffer.setSample(i, n, a);
    		n++;
    	}

    	// ADSR:
    	waveformBuffer.applyGainRamp(0, 100, 0.0, 1.0);
    	waveformBuffer.applyGainRamp(900, 100, 1.0, 0.0);
    }
}

AudioProcessorEditor* SpikeSynth::createEditor()
{
	editor = new SpikeSynthEditor(this, true);
    return editor;
}


void SpikeSynth::saveCustomParametersToXml(XmlElement* parentElement)
{

}

void SpikeSynth::loadCustomParametersFromXml()
{

}


void SpikeSynth::setParameter(int parameterIndex, float newValue)
{

}

void SpikeSynth::handleEvent(int eventType, MidiMessage& event, int sampleNum)
{
 	if (eventType == SPIKE)
    {

        const uint8_t* dataptr = event.getRawData();
        int bufferSize = event.getRawDataSize();

        if (bufferSize > 0)
        {

            SpikeObject newSpike;

            bool isValid = unpackSpike(&newSpike, dataptr, bufferSize);

            if (isValid)
            {
                int electrodeNum = newSpike.source;
                float gain = 1.0; // scale to spike amplitude?

                synthBuffer.addFrom(0, sampleNum, waveformBuffer, electrodeNum, 0, 1000, gain);

            }

        }
    }
}



void SpikeSynth::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
	synthBuffer.clear();

	checkForEvents(events);

	buffer.copyFrom(0, 0, synthBuffer, 0, 0, getNumSamples(0));
	buffer.copyFrom(1, 0, synthBuffer, 0, 0, getNumSamples(0));
}