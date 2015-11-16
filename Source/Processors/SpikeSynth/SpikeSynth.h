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

#ifndef SPIKESYNTH_H_INCLUDED
#define SPIKESYNTH_H_INCLUDED

#include "../../JuceLibraryCode/JuceHeader.h"

#include "../GenericProcessor/GenericProcessor.h"
#include "SpikeSynthEditor.h"

#include "../Visualization/SpikeObject.h"


/**

  Uses incoming spikes to trigger notes of a synthesizer

  @see GenericProcessor, SpikeSynthEditor

*/

class SpikeSynth : public GenericProcessor

{
public:

    // CONSTRUCTOR AND DESTRUCTOR //

    /** constructor */
    SpikeSynth();

    /** destructor */
    ~SpikeSynth();


    // PROCESSOR METHODS //

    /** Processes an incoming continuous buffer and places new
        spikes into the event buffer. */
    void process(AudioSampleBuffer& buffer, MidiBuffer& events);

    /** Used to alter parameters of data acquisition. */
    void setParameter(int parameterIndex, float newValue);

    /** Called whenever the signal chain is altered. */
    void updateSettings();

    /** Called prior to start of acquisition. */
    bool enable();

    /** Called after acquisition is finished. */
    bool disable();

    /** Creates the SpikeDetectorEditor. */
    AudioProcessorEditor* createEditor();


    // INTERNAL BUFFERS //

    /** Extra samples are placed in this buffer to allow seamless
        transitions between callbacks. */
    AudioSampleBuffer synthBuffer;

    void saveCustomParametersToXml(XmlElement* parentElement);
    void loadCustomParametersFromXml();

private:
  

    int sampleIndex;

    void handleEvent(int eventType, MidiMessage& event, int sampleNum);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpikeSynth);

};

#endif  // SPIKESYNTH_H_INCLUDED
