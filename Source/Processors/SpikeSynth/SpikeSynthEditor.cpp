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

#include "SpikeSynthEditor.h"
#include "SpikeSynth.h"

#include <stdio.h>


SpikeSynthEditor::SpikeSynthEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors=true)
    : GenericEditor(parentNode, useDefaultParameterEditors)

{
	desiredWidth = 300;

	startNoteComboBox = new ComboBox("Start Note");

    for (int i = 0; i < 9; i++)
    {
        startNoteComboBox->addItem("C" + String(i), i+1);
    }

    startNoteComboBox->setEditableText(false);
    startNoteComboBox->setJustificationType(Justification::centredLeft);
    startNoteComboBox->addListener(this);
    startNoteComboBox->setBounds(10,50,80,20);
    startNoteComboBox->setSelectedId(1);
    addAndMakeVisible(startNoteComboBox);

    startNoteLabel = new Label("Start Note:","Start Note:");
    startNoteLabel->setBounds(6, 30, 90, 15);
    startNoteLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(startNoteLabel);

    modeComboBox = new ComboBox("Scale");
    modeComboBox->addItem("Ionian", 1);
    modeComboBox->addItem("Dorian", 2);
    modeComboBox->addItem("Phrygian", 3);
    modeComboBox->addItem("Lydian", 4);
    modeComboBox->addItem("Mixolydian", 5);
    modeComboBox->addItem("Aeolian", 6);
    modeComboBox->addItem("Locrian", 7);

	modeComboBox->setEditableText(false);
    modeComboBox->setJustificationType(Justification::centredLeft);
    modeComboBox->addListener(this);
    modeComboBox->setBounds(10,95,80,20);
    modeComboBox->setSelectedId(1);
    addAndMakeVisible(modeComboBox);

    modeLabel = new Label("Mode:","Mode:");
    modeLabel->setBounds(6, 75, 90, 15);
    modeLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(modeLabel);

    float octave0 [12] = {16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87};
    float octave1 [12] = {32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74};
    float octave2 [12] = {65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.8, 110.0, 116.5, 123.5};
    float octave3 [12] = {130.8, 138.6, 146.8, 155.6, 164.8, 174.6, 185.0, 196.0, 207.7, 220.0, 233.1, 246.9};
    float octave4 [12] = {261.6, 277.2, 293.7, 311.1, 329.6, 349.2, 370.0, 392.0, 415.3, 440.0, 466.2, 493.9};
    float octave5 [12] = {523.3, 554.4, 587.3, 622.3, 659.3, 698.5, 740.0, 784.0, 830.6, 880.0, 932.3, 987.8};
    float octave6 [12] = {1047,  1109,  1175,  1245,  1319,  1397,  1480,  1568,  1661,  1760,  1865,  1976};
    float octave7 [12] = {2093,  2217,  2349,  2489,  2637,  2794,  2960,  3136,  3322,  3520,  3729,  3951};
    float octave8 [12] = {4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902};

    frequencies.add(Array<float>(octave0));
    frequencies.add(Array<float>(octave1));
    frequencies.add(Array<float>(octave2));
    frequencies.add(Array<float>(octave3));
    frequencies.add(Array<float>(octave4));
    frequencies.add(Array<float>(octave5));
    frequencies.add(Array<float>(octave6));
    frequencies.add(Array<float>(octave7));
    frequencies.add(Array<float>(octave8));

    float ionian [8] = {0, 2, 4, 5, 7, 9, 11, 12};
    float dorian [8] = {0, 2, 3, 5, 7, 9, 10, 12};
    float phrygian [8] = {0, 1, 3, 5, 7, 8, 10, 12};
    float lydian [8] = {0, 2, 4, 6, 7, 9, 11, 12};
    float mixolydian [8] = {0, 2, 4, 5, 7, 9, 10, 12};
    float aeolian [8] = {0, 2, 3, 5, 7, 8, 10, 12};
    float locrian [8] = {0, 1, 3, 5, 6, 8, 10, 12};

    modes.add(Array<float>(ionian));
    modes.add(Array<float>(dorian));
    modes.add(Array<float>(phrygian));
    modes.add(Array<float>(lydian));
    modes.add(Array<float>(mixolydian));
    modes.add(Array<float>(aeolian));
    modes.add(Array<float>(locrian));

    MemoryInputStream mis(BinaryData::silkscreenserialized, BinaryData::silkscreenserializedSize, false);
    Typeface::Ptr typeface = new CustomTypeface(mis);
    Font font = Font(typeface);

    thresholdSlider = new ThresholdSlider(font);
    thresholdSlider->setBounds(100,50,60,60);
    addAndMakeVisible(thresholdSlider);
    thresholdSlider->addListener(this);
    thresholdSlider->setActive(true);
    Array<double> v;
    thresholdSlider->setValues(v);

    adsrInterface = new ADSRInterface();
    adsrInterface->setBounds(170, 40, 110, 75);
    addAndMakeVisible(adsrInterface);
}


SpikeSynthEditor::~SpikeSynthEditor()
{


}

void SpikeSynthEditor::buttonEvent(Button* button)
{


}


void SpikeSynthEditor::labelTextChanged(Label* label)
{


}

void SpikeSynthEditor::comboBoxChanged(ComboBox* comboBox)
{


}


// ----------------------------------

ADSRInterface::ADSRInterface()
{

}

ADSRInterface::~ADSRInterface()
{

}

void ADSRInterface::paint(Graphics& g)
{

	float w = getWidth();
	float h = getHeight();

	float totalWidth = 250; // ms
	float offset = 10; // px

	float attack = 50; // ms
	float decay = 50; // ms
	float sustain = 0.8; // fraction
	float release = 50; // ms


	g.fillAll(Colours::darkgrey);

	g.setColour(Colours::grey);
	g.drawLine(attack/totalWidth*w, 0, attack/totalWidth*w, h);
    g.drawLine((attack+decay)/totalWidth*w, 0, (attack+decay)/totalWidth*w, h);
    g.drawLine(w - release/totalWidth*w, 0, w - release/totalWidth*w, h);

	g.setColour(Colours::yellow);
	g.drawLine(0, h-offset, attack/totalWidth*w, offset);
	g.drawLine(attack/totalWidth*w, offset, attack/totalWidth*w + decay/totalWidth*w, h - sustain*(h-offset*2) - offset);
	g.drawLine(attack/totalWidth*w + decay/totalWidth*w, h - sustain*(h-offset*2) - offset, w - release/totalWidth*w, h - sustain*(h-offset*2) - offset);
	g.drawLine( w - release/totalWidth*w, h - sustain*(h-offset*2) - offset, w, h - offset);

}