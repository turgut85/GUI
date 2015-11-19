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
      synthBuffer(2, 10000), noteLength(1000), threshold(100)
{
    
    float octave0 [12] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87};

    for (int i = 0; i < 12; i++)
    	frequencies.add(octave0[i]);

    int a[7] = {0, 2, 4, 5, 7, 9, 11};
    int b[7] = {0, 2, 3, 5, 7, 9, 10};
    int c[7] = {0, 1, 3, 5, 7, 8, 10};
    int d[7] = {0, 2, 4, 6, 7, 9, 11};
    int e[7] = {0, 2, 4, 5, 7, 9, 10};
    int f[7] = {0, 2, 3, 5, 7, 8, 10};
    int g[7] = {0, 1, 3, 5, 6, 8, 10};

    for (int i = 0; i < 7; i++)
    {
    	ionian.add(a[i]);
    	dorian.add(b[i]);
    	phrygian.add(c[i]);
    	lydian.add(d[i]);
    	mixolydian.add(e[i]);
    	aeolian.add(f[i]);
    	locrian.add(g[i]);
    }

    startFreq = 2;
    modeID = 2;
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

    updateWaveforms();

}

bool SpikeSynth::enable()
{

}

bool SpikeSynth::disable()
{

}

void SpikeSynth::updateWaveforms()
{
	waveformBuffer.setSize(numElectrodes, noteLength);

    for (int i = 0; i < numElectrodes; i++)
    {
    	// set base frequency (plus interval per channel):
    	
    	Array<int> mode;

    	switch (modeID)
    	{
    		case 0:
    			mode = ionian; break;
    		case 1:
    			mode = dorian; break;
    		case 2:
    			mode = phrygian; break;
    		case 3:
    			mode = lydian; break;
    		case 4:
    			mode = mixolydian; break;
    		case 5:
    			mode = aeolian; break;
    		case 6:
    			mode = locrian; break;
    		default:
    			mode = ionian;
    	}

    	int noteNum = mode[i % 7];

    	float freq = frequencies[noteNum] * pow(2, startFreq + i/7);
    	std::cout << "Note number: " << noteNum << ", freq: " << freq << std::endl;

    	// create saw waves at each frequency:
    	int period = (int) ((1./freq) * getSampleRate());
    	int n = 0;
    	float a;
    	while (n < noteLength)
    	{
    		a = 0.5 - (n % period) / float(period);
    		waveformBuffer.setSample(i, n, a*2);
    		n++;
    	}

    	// ADSR (simplified for now):
    	waveformBuffer.applyGainRamp(0, 100, 0.0, 1.0);
    	waveformBuffer.applyGainRamp(noteLength-100, 100, 1.0, 0.0);
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

	switch (parameterIndex)
	{
		case 0: // threshold
			threshold = newValue; break;
		case 1: // start note
			startFreq = (int) newValue; break;
		case 2: // mode
			modeID = (int) newValue; break;
		case 3: // attack
			attack = newValue; break;
		case 4: // decay
			decay = newValue; break;
	case 5: // sustain
			sustain = newValue; break;
		case 6: // release
			release = newValue; break;
		default:
			break;
	}

	if (parameterIndex > 0)
	{
		updateWaveforms();
	}

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

            	if (checkThreshold(newSpike))
            	{

	                int electrodeNum = newSpike.source;
	                float gain = 1.0; // scale to spike amplitude?

	                if (sampleNum < synthBuffer.getNumSamples())
	                	//std::cout << sampleNum << ", " << electrodeNum << std::endl;
	                	//std::cout << synthBuffer.getNumSamples() << ", " << synthBuffer.getNumChannels() << std::endl;
	                	//std::cout << waveformBuffer.getNumSamples() << ", " << waveformBuffer.getNumChannels() << std::endl;
	                	synthBuffer.addFrom(0, sampleNum, waveformBuffer, electrodeNum, 0, 1000, gain);
	            	}
            }

        }
    }
}

bool SpikeSynth::checkThreshold(SpikeObject& s)
{
    for (int i = 0; i < s.nSamples*s.nChannels; i++)
    {

        if (float(s.data[i]-32768)/float(*s.gain)*1000.0f > threshold)
        {
            return true;
        }
    }

    return false;
}


void SpikeSynth::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
	synthBuffer.clear();

	checkForEvents(events);

	buffer.copyFrom(0, 0, synthBuffer, 0, 0, getNumSamples(0));
	buffer.copyFrom(1, 0, synthBuffer, 0, 0, getNumSamples(0));
}

