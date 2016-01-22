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

    MemoryInputStream mis(BinaryData::silkscreenserialized, BinaryData::silkscreenserializedSize, false);
    Typeface::Ptr typeface = new CustomTypeface(mis);
    Font font = Font(typeface);

    thresholdSlider = new ThresholdSlider(font);
    thresholdSlider->setBounds(100,55,60,60);
    addAndMakeVisible(thresholdSlider);
    thresholdSlider->addListener(this);
    thresholdSlider->setActive(true);
    Array<double> v;
    thresholdSlider->setValues(v);
    thresholdSlider->setValue(40.0f);

    thresholdLabel = new Label("Threshold:","Threshold:");
    thresholdLabel->setBounds(94, 35, 90, 15);
    thresholdLabel->setColour(Label::textColourId, Colours::darkgrey);
    addAndMakeVisible(thresholdLabel);

    adsrInterface = new ADSRInterface((SpikeSynth*) getProcessor());
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
	if (comboBox == startNoteComboBox)
	{
		getProcessor()->setParameter(1, comboBox->getSelectedId());
	} else if (comboBox == modeComboBox)
	{
		getProcessor()->setParameter(2, comboBox->getSelectedId());
	}

}

void SpikeSynthEditor::sliderEvent(Slider* slider)
{
	getProcessor()->setParameter(0, slider->getValue());
}

void SpikeSynthEditor::setParams(int startNote, int mode, float thresh, float a, float d, float s, float r, float noteLength)
//(int startNote, int mode, float thresh, float a, float, d, float s, float r)
{

	//std::cout << "Setting startNote to " << startNote << std::endl;

	startNoteComboBox->setSelectedId(startNote, dontSendNotification);
	modeComboBox->setSelectedId(mode, dontSendNotification);
	thresholdSlider->setValue(thresh);

	adsrInterface->setParams(a,d,s,r, noteLength);
}

// ----------------------------------

ADSRInterface::ADSRInterface(SpikeSynth* p)
{
	processor = p;
}

ADSRInterface::~ADSRInterface()
{

}

void ADSRInterface::setParams(float a, float d, float s, float r, float noteLength)
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