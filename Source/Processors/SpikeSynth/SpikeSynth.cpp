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
#include <cmath>
#include "SpikeSynth.h"


SpikeSynth::SpikeSynth()
    : GenericProcessor("Spike Synth"),
      synthBuffer(1, 80000), overflowBuffer(1, 80000), noteLength(100.0f), threshold(40)
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

    startFreq = 0;
    modeID = 0;
    
    noteLength = 250.0f; // ms
    attack = 25.0; // ms
    decay = 25.0; // ms
    sustain = 0.2; // fraction
    release = 200.0; // ms

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

    filter.setup(3, // order
    	         getSampleRate(), // sampleRate
    	         2550, // center frequency
    	         2500, // bandwidth
    	         1);   // ripple dB

}

bool SpikeSynth::enable()
{
	overflowBufferIndex = 0;
	return true;
}

bool SpikeSynth::disable()
{
	return true;
}

void SpikeSynth::updateWaveforms()
{

    std::cout << "Sample rate: " << getSampleRate() << std::endl;

	waveformBuffer.setSize(numElectrodes, (int) (noteLength / 1000.0f * getSampleRate()));
	waveformBuffer.clear();

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
    	//std::cout << "Note number: " << noteNum << ", freq: " << freq << std::endl;

    	// create triangle waves at each frequency:
    	float period = ((1./freq) * getSampleRate());
    	float n = 0;
    	float a;

    	while (n < (float) waveformBuffer.getNumSamples())
    	{

    		float rem = fmod(n, period);

    		if (rem <= (period / 2))
    			a = 0.25 - rem / float(period);
    		else
    			a = rem / float(period) - 0.75;
    		//else if (n > waveformBuffer.getNu)
    		//a = 0.5 - (n % period) / float(period);
    		//if (a > 0.5)
    		//	a = 1 - a;
    		//a -= 0.5;

    		waveformBuffer.setSample(i, n, a*400);
    		n++;
    	}
    }


    //Create waveform envelope:
	//std::cout << "Waveform size: " << waveformBuffer.getNumSamples() << std::endl;

	int startIndex = 0;
	int endIndex = (int) (attack / 1000.0f * getSampleRate());
	//std::cout << "Attack: " << startIndex << " to " << endIndex << std::endl;
	waveformBuffer.applyGainRamp(startIndex, endIndex - startIndex, 0.0, 1.0);

	startIndex = endIndex;
	endIndex += (int) (decay / 1000.0f * getSampleRate());
	//std::cout << "Decay: " << startIndex << " to " << endIndex << std::endl;
	waveformBuffer.applyGainRamp(startIndex, endIndex - startIndex, 1.0, sustain);

	startIndex = endIndex;
	endIndex = waveformBuffer.getNumSamples() - (int) (release / 1000.0f * getSampleRate());
	//std::cout << "Sustain: " << startIndex << " to " << endIndex << std::endl;
	waveformBuffer.applyGainRamp(startIndex, endIndex - startIndex, sustain, sustain);

	startIndex = endIndex;
	endIndex = waveformBuffer.getNumSamples();
	//std::cout << "Release: " << startIndex << " to " << endIndex << std::endl;
	waveformBuffer.applyGainRamp(startIndex, endIndex - startIndex, sustain, 0.0);


    synthBuffer.clear();
    overflowBuffer.clear();
}

AudioProcessorEditor* SpikeSynth::createEditor()
{
	editor = new SpikeSynthEditor(this, true);
    return editor;
}


void SpikeSynth::saveCustomParametersToXml(XmlElement* parentElement)
{
    XmlElement* params = parentElement->createNewChildElement("PARAMETERS");
    params->setAttribute("startFreq", startFreq);\
    params->setAttribute("modeID", modeID);
    params->setAttribute("threshold", threshold);
    params->setAttribute("attack", attack);
    params->setAttribute("decay", decay);
    params->setAttribute("sustain", sustain);
    params->setAttribute("release", release);
    params->setAttribute("noteLength", noteLength);

}

void SpikeSynth::loadCustomParametersFromXml()
{

	if (parametersAsXml != nullptr) // prevent double-loading
    {
        // use parametersAsXml to restore state

		SpikeSynthEditor* ed = (SpikeSynthEditor*) getEditor();

        forEachXmlChildElement(*parametersAsXml, xmlNode)
        {
	        if (xmlNode->hasTagName("PARAMETERS"))
	        {
	            startFreq = xmlNode->getIntAttribute("startFreq", 1);
				modeID = xmlNode->getIntAttribute("modeID", 1);
				threshold = xmlNode->getDoubleAttribute("threshold", 0.0);
				attack = xmlNode->getDoubleAttribute("attack", 20.0);
				decay = xmlNode->getDoubleAttribute("decay", 20.0);
				sustain = xmlNode->getDoubleAttribute("sustain", 0.5);
				release = xmlNode->getDoubleAttribute("release", 20.0);
				noteLength = xmlNode->getDoubleAttribute("noteLength", 20.0);

				//std::cout << "START NOTE = " << startFreq << std::endl;

		    	ed->setParams(startFreq, modeID, threshold, attack, decay, sustain, release, noteLength);

	        }
	    }
    }

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
		case 7: // note length
			noteLength = newValue; break;
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
	                float gain = 2.0; // scale to spike amplitude?

	                if (sampleNum < synthBuffer.getNumSamples())
	                	//std::cout << sampleNum << ", " << electrodeNum << std::endl;
	                	//std::cout << synthBuffer.getNumSamples() << ", " << synthBuffer.getNumChannels() << std::endl;
	                	//std::cout << waveformBuffer.getNumSamples() << ", " << waveformBuffer.getNumChannels() << std::endl;
	                	synthBuffer.addFrom(0, sampleNum, waveformBuffer, electrodeNum, 0, waveformBuffer.getNumSamples(), gain);
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
	
	synthBuffer.copyFrom(0, 0, overflowBuffer, overflowBufferIndex, 0, synthBuffer.getNumSamples());
	overflowBuffer.clear();

	checkForEvents(events);

	int nSamples = getNumSamples(0);

	float* ptr = synthBuffer.getWritePointer(0);
	filter.process(nSamples, &ptr);

	buffer.copyFrom(0, 0, synthBuffer, 0, 0, nSamples);
	buffer.copyFrom(1, 0, synthBuffer, 0, 0, nSamples);

	// copy leftover samples to the beginning of the buffer
	overflowBuffer.copyFrom(0, 0, synthBuffer, 0, nSamples, synthBuffer.getNumSamples()-nSamples);
	synthBuffer.clear();


}

