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

#ifndef NEUROPIXEDITOR_H_INCLUDED
#define NEUROPIXEDITOR_H_INCLUDED

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "../Editors/GenericEditor.h"

/**

User interface for the "Neuropix" source node.

@see SourceNode, eCube

*/

class NeuropixEditor : public GenericEditor
{
public:
    NeuropixEditor(GenericProcessor* parentNode, NeuropixThread* thread, bool useDefaultParameterEditors);
    virtual ~NeuropixEditor();

    //void buttonEvent(Button* button);
    //virtual void sliderValueChanged(Slider* slider);
    //virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged);

    void saveEditorParameters(XmlElement*);
    void loadEditorParameters(XmlElement*);

private:
    //ScopedPointer<Label> volLabel;
    //ScopedPointer<Slider> volSlider;
    //ScopedPointer<Label> chanLabel;
    //ScopedPointer<ComboBox> chanComboBox;
    //ScopedPointer<Label> samplerateLabel;
    //ScopedPointer<Label> samplerateValueLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuropixEditor);

};


#endif  // NEUROPIXEDITOR_H_INCLUDED
