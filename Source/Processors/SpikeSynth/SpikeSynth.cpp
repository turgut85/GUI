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
      synthBuffer(2,5000)
{

}

SpikeSynth::~SpikeSynth()
{

}


void SpikeSynth::updateSettings()
{
	// turns N channels into 2 audio outputs
	settings.numOutputs = 2;

    // 128 inputs, 2 outputs (left and right channel)
    setPlayConfigDetails(getNumInputs(), getNumOutputs(), 44100.0, 128);
}

bool SpikeSynth::enable()
{

}

bool SpikeSynth::disable()
{

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

}



void SpikeSynth::process(AudioSampleBuffer& buffer, MidiBuffer& events)
{
	
}