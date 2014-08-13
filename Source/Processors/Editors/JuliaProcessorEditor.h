/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2013 Open Ephys

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

#ifndef JULIAPROCESSOREDITOR_H_INCLUDED
#define JULIAPROCESSOREDITOR_H_INCLUDED


#include "../../../JuceLibraryCode/JuceHeader.h"
#include "GenericEditor.h"
#include "ImageIcon.h"

class ImageIcon;
class JuliaProcessor;


/**

  User interface for the "JuliaProcessor" source node.

  @see SourceNode, JuliaProcessorThread

*/

class JuliaProcessorEditor : public GenericEditor,
public Label::Listener
{
public:
    JuliaProcessorEditor(GenericProcessor* parentNode, bool useDefaultParameterEditors);
    virtual ~JuliaProcessorEditor();

    void buttonEvent(Button* button);
	void labelTextChanged(Label* te);

    void setFile(String file);

    void saveEditorParameters(XmlElement*);

    void loadEditorParameters(XmlElement*);

	ImageIcon* icon;

private:

    ScopedPointer<UtilityButton> fileButton;
    ScopedPointer<UtilityButton> reloadFileButton;
    ScopedPointer<Label> fileNameLabel;
	
	ScopedPointer<Label> bufferSizeSelection;
	ScopedPointer<Label> bufferSizeSelectionLabel;

    JuliaProcessor* juliaProcessor;

    File lastFilePath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JuliaProcessorEditor);

};




#endif  // JULIAPROCESSOREDITOR_H_INCLUDED
