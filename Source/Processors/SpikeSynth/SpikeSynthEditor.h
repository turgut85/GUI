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

#ifndef SPIKESYNTHEDITOR_H_INCLUDED
#define SPIKESYNTHEDITOR_H_INCLUDED

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Editors/GenericEditor.h"
#include "../Editors/ElectrodeButtons.h"
#include "../SpikeDetector/SpikeDetectorEditor.h"


class TriangleButton;
class UtilityButton;
class ADSRInterface;

/**

  User interface for the SpikeSynth processor.

  @see SpikeSynth

*/


class SpikeSynthEditor : public GenericEditor,
    public Label::Listener,
    public ComboBox::Listener

{
public:
    SpikeSynthEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~SpikeSynthEditor();
    void buttonEvent(Button* button);
    void labelTextChanged(Label* label);
    void comboBoxChanged(ComboBox* comboBox);

    ScopedPointer<Label> startNoteLabel;
    ScopedPointer<Label> modeLabel;
    ScopedPointer<Label> thresholdLabel;
    ScopedPointer<ComboBox> startNoteComboBox;
    ScopedPointer<ComboBox> modeComboBox;
    ScopedPointer<ThresholdSlider> thresholdSlider;

    ScopedPointer<ADSRInterface> adsrInterface;

private:

	Array<Array<float>> frequencies;
	Array<Array<float>> modes;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpikeSynthEditor);

};

class ADSRInterface : public Component
{
public:
	ADSRInterface();
	virtual ~ADSRInterface();

	void paint(Graphics& g);
};

#endif  // SPIKESYNTHEDITOR_H_INCLUDED
