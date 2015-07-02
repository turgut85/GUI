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
#include "../Editors/VisualizerEditor.h"
#include "NeuropixThread.h"

class NeuropixCanvas;
class NeuropixInterface;

/**

	User interface for the "Neuropix" source node.

	@see SourceNode, eCube

*/


class NeuropixEditor : public VisualizerEditor, public ComboBox::Listener
{
public:
    NeuropixEditor(GenericProcessor* parentNode, NeuropixThread* thread, bool useDefaultParameterEditors);
    virtual ~NeuropixEditor();

    //void buttonEvent(Button* button);
    //virtual void sliderValueChanged(Slider* slider);
    //virtual void comboBoxChanged(ComboBox* comboBoxThatHasChanged);
    void comboBoxChanged(ComboBox* comboBox);

    void saveEditorParameters(XmlElement*);
    void loadEditorParameters(XmlElement*);

    Visualizer* createNewCanvas(void);

private:
    //ScopedPointer<Label> volLabel;
    //ScopedPointer<Slider> volSlider;
    //ScopedPointer<Label> chanLabel;
    ScopedPointer<ComboBox> optionComboBox;
    //ScopedPointer<Label> samplerateLabel;
    //ScopedPointer<Label> samplerateValueLabel;
    Viewport* viewport;
    NeuropixCanvas* canvas;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NeuropixEditor);

};

class NeuropixCanvas : public Visualizer, public Button::Listener
{
public:
	NeuropixCanvas(GenericProcessor* p);
	~NeuropixCanvas();

	void paint(Graphics& g);

	void refresh();

	void beginAnimation();
	void endAnimation();

	void refreshState();
	void update();

	void setOption(int);

	void setParameter(int, float);
    void setParameter(int, int, int, float);
    void buttonClicked(Button* button);

	void resized();

	SourceNode* processor;
	ScopedPointer<Viewport> neuropixViewport;
	ScopedPointer<NeuropixInterface> neuropixInterface;

};

class NeuropixInterface: public Component, public Button::Listener, public ComboBox::Listener
{
public:
	NeuropixInterface();
	~NeuropixInterface();

	void setOption(int);

	void paint(Graphics& g);

	void mouseMove(const MouseEvent& event);
    void mouseDown(const MouseEvent& event);
    void mouseDrag(const MouseEvent& event);
    void mouseUp(const MouseEvent& event);
    void mouseWheelMove(const MouseEvent&  event, const MouseWheelDetails&   wheel) ;
    MouseCursor getMouseCursor();

    void buttonClicked(Button*);
    void comboBoxChanged(ComboBox*);

private:
	int option;
	//NeuropixThread* thread;

	ScopedPointer<ComboBox> lfpGainComboBox;
	ScopedPointer<ComboBox> apGainComboBox;
	ScopedPointer<ComboBox> referenceComboBox;
	ScopedPointer<ComboBox> filterComboBox;

	ScopedPointer<UtilityButton> enableButton;
	ScopedPointer<UtilityButton> selectAllButton;

	ScopedPointer<Label> lfpGainLabel;
	ScopedPointer<Label> apGainLabel;
	ScopedPointer<Label> referenceLabel;
	ScopedPointer<Label> filterLabel;

	ScopedPointer<UtilityButton> enableViewButton;
	ScopedPointer<UtilityButton> lfpGainViewButton;
	ScopedPointer<UtilityButton> apGainViewButton;
	ScopedPointer<UtilityButton> referenceViewButton;
	
	Array<int> channelStatus;
	Array<int> channelReference;
	Array<int> channelApGain;
	Array<int> channelLfpGain;
	Array<int> channelSelectionState;

	Array<int> option1and2refs;
	Array<int> option3refs;
	Array<int> option4refs;

	bool isOverZoomRegion;
	bool isOverUpperBorder;
	bool isOverLowerBorder;
	bool isOverChannel;

	int zoomHeight;
	int zoomOffset;
	int initialOffset;
	int initialHeight;
	int lowerBound;
	int dragZoneWidth;

	int lowestChan;
	int highestChan;

	float channelHeight;

	int visualizationMode;

	Rectangle<int> selectionBox;
	bool isSelectionActive;

	MouseCursor::StandardCursorType cursorType;

	Path shankPath;

	String channelInfoString;

	Colour getChannelColour(int chan);
	int getNearestChannel(int x, int y);
	String getChannelInfoString(int chan);

	void drawLegend(Graphics& g);

};


#endif  // NEUROPIXEDITOR_H_INCLUDED
