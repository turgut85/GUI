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
	desiredWidth = 150;

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
