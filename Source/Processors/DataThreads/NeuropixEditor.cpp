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

#include "NeuropixThread.h"
#include "NeuropixEditor.h"



NeuropixEditor::NeuropixEditor(GenericProcessor* parentNode, NeuropixThread* thread, bool useDefaultParameterEditors)
 : VisualizerEditor(parentNode, useDefaultParameterEditors)
{
	desiredWidth = 200;
	tabText = "Neuropix";

	optionComboBox = new ComboBox("Option Combo Box");
	optionComboBox->setBounds(20,50,100,25);
	optionComboBox->addListener(this);

	for (int k = 1; k < 5; k++)
	{
		optionComboBox->addItem("Option " + String(k),k);
	}
	optionComboBox->setSelectedId(1);
	addAndMakeVisible(optionComboBox);
}

NeuropixEditor::~NeuropixEditor()
{

}

void NeuropixEditor::comboBoxChanged(ComboBox* comboBox)
{
	if (comboBox == optionComboBox)
	{
		int selectedOption = comboBox->getSelectedId();

		canvas->setOption(selectedOption);
	}
}

void NeuropixEditor::saveEditorParameters(XmlElement* xml)
{

}

void NeuropixEditor::loadEditorParameters(XmlElement* xml)
{

}

Visualizer* NeuropixEditor::createNewCanvas(void)
{
	GenericProcessor* processor = (GenericProcessor*) getProcessor();
    canvas = new NeuropixCanvas(processor);
    return canvas;
}

/********************************************/

NeuropixCanvas::NeuropixCanvas(GenericProcessor* p)
{

	processor = (SourceNode*) p;

	neuropixViewport = new Viewport();
	neuropixInterface = new NeuropixInterface();
	neuropixViewport->setViewedComponent(neuropixInterface, false);
	addAndMakeVisible(neuropixViewport);

	resized();
	update();
}

NeuropixCanvas::~NeuropixCanvas()
{

}

void NeuropixCanvas::setOption(int option)
{
	neuropixInterface->setOption(option);
}

void NeuropixCanvas::paint(Graphics& g)
{
	g.fillAll(Colours::darkgrey);
}

void NeuropixCanvas::refresh()
{
	repaint();
}

void NeuropixCanvas::refreshState()
{
	resized();
}

void NeuropixCanvas::update()
{

}

void NeuropixCanvas::beginAnimation()
{
}

void NeuropixCanvas::endAnimation()
{
}

void NeuropixCanvas::resized()
{

    neuropixViewport->setBounds(0,0,getWidth(),getHeight());
    neuropixInterface->setBounds(0,0,getWidth()-neuropixViewport->getScrollBarThickness(), 600);
}

void NeuropixCanvas::setParameter(int x, float f)
{

}

void NeuropixCanvas::setParameter(int a, int b, int c, float d)
{
}

void NeuropixCanvas::buttonClicked(Button* button)
{

}


/*****************************************************/
NeuropixInterface::NeuropixInterface()
{
	cursorType = MouseCursor::NormalCursor;

	isOverZoomRegion = false;
	isOverUpperBorder = false;
	isOverLowerBorder = false;
	isSelectionActive = false;
	isOverChannel = false;
	
	for (int i = 0; i < 966; i++)
	{
		channelStatus.add(-1);
		channelReference.add(0);
		channelLfpGain.add(0);
		channelApGain.add(0);
		channelSelectionState.add(0);
	}



	visualizationMode = 0;

	addMouseListener(this, true);

	zoomHeight = 50;
	lowerBound = 530;
	zoomOffset = 0;
	dragZoneWidth = 10;

	option1and2refs.add(37);
	option1and2refs.add(76);
	option1and2refs.add(113);
	option1and2refs.add(152);
	option1and2refs.add(189);
	option1and2refs.add(228);
	option1and2refs.add(265);

	option4refs = option1and2refs;

	option1and2refs.add(304);
	option1and2refs.add(341);

	option3refs = option1and2refs;

	option3refs.add(421);
	option3refs.add(460);
	option3refs.add(497);
	option3refs.add(536);
	option3refs.add(573);
	option3refs.add(612);
	option3refs.add(649);
	option3refs.add(688);
	option3refs.add(725);
	option3refs.add(805);
	option3refs.add(844);
	option3refs.add(881);
	option3refs.add(920);
	option3refs.add(957);

	option1and2refs.add(380);

	option4refs.add(313);
	option4refs.add(352);
	option4refs.add(389);
	option4refs.add(428);
	option4refs.add(465);
	option4refs.add(504);
	option4refs.add(541);
	option4refs.add(589);
	option4refs.add(628);
	option4refs.add(665);
	option4refs.add(704);
	option4refs.add(741);
	option4refs.add(780);
	option4refs.add(817);
	option4refs.add(865);
	option4refs.add(904);
	option4refs.add(941);


	apGainComboBox = new ComboBox("apGainComboBox");
	apGainComboBox->setBounds(400, 150, 65, 22);
	apGainComboBox->addListener(this);

	lfpGainComboBox = new ComboBox("lfpGainComboBox");
	lfpGainComboBox->setBounds(400, 200, 65, 22);
	lfpGainComboBox->addListener(this);

	for (int i = 0; i < 8; i++)
	{
		lfpGainComboBox->addItem(String(i), i+1);
		apGainComboBox->addItem(String(i), i+1);
	}

	lfpGainComboBox->setSelectedId(1);
	apGainComboBox->setSelectedId(1);

	referenceComboBox = new ComboBox("ReferenceComboBox");
	referenceComboBox->setBounds(400, 250, 65, 22);
	referenceComboBox->addListener(this);

	for (int i = 0; i < 11; i++)
	{
		referenceComboBox->addItem(String(i), i+1);
	}

	referenceComboBox->setSelectedId(1);

	filterComboBox = new ComboBox("FilterComboBox");
	filterComboBox->setBounds(400, 300, 65, 22);
	filterComboBox->addListener(this);

	for (int i = 0; i < 3; i++)
	{
		filterComboBox->addItem(String(i), i+1);
	}

	filterComboBox->setSelectedId(1);

	enableButton = new UtilityButton("ENABLE", Font("Small Text", 13, Font::plain));
    enableButton->setRadius(3.0f);
    enableButton->setBounds(400,95,65,22);
    enableButton->addListener(this);
    enableButton->setTooltip("Enable selected channel(s)");

    selectAllButton = new UtilityButton("SELECT ALL", Font("Small Text", 13, Font::plain));
    selectAllButton->setRadius(3.0f);
    selectAllButton->setBounds(400,50,95,22);
    selectAllButton->addListener(this);
    selectAllButton->setTooltip("Select all channels");

    enableViewButton = new UtilityButton("VIEW", Font("Small Text", 12, Font::plain));
    enableViewButton->setRadius(3.0f);
    enableViewButton->setBounds(480,97,45,18);
    enableViewButton->addListener(this);
    enableViewButton->setTooltip("View channel enabled state");

    lfpGainViewButton = new UtilityButton("VIEW", Font("Small Text", 12, Font::plain));
    lfpGainViewButton->setRadius(3.0f);
    lfpGainViewButton->setBounds(480,202,45,18);
    lfpGainViewButton->addListener(this);
    lfpGainViewButton->setTooltip("View LFP gain of each channel");

    apGainViewButton = new UtilityButton("VIEW", Font("Small Text", 12, Font::plain));
    apGainViewButton->setRadius(3.0f);
    apGainViewButton->setBounds(480,152,45,18);
    apGainViewButton->addListener(this);
    apGainViewButton->setTooltip("View AP gain of each channel");

    referenceViewButton = new UtilityButton("VIEW", Font("Small Text", 12, Font::plain));
    referenceViewButton->setRadius(3.0f);
    referenceViewButton->setBounds(480,252,45,18);
    referenceViewButton->addListener(this);
    referenceViewButton->setTooltip("View reference of each channel");

    addAndMakeVisible(lfpGainComboBox);
    addAndMakeVisible(apGainComboBox);
    addAndMakeVisible(referenceComboBox);
    addAndMakeVisible(filterComboBox);

    addAndMakeVisible(enableButton);
    addAndMakeVisible(selectAllButton);
    addAndMakeVisible(enableViewButton);
    addAndMakeVisible(lfpGainViewButton);
    addAndMakeVisible(apGainViewButton);
    addAndMakeVisible(referenceViewButton);

    lfpGainLabel = new Label("LFP GAIN","LFP GAIN");
    lfpGainLabel->setFont(Font("Small Text", 13, Font::plain));
    lfpGainLabel->setBounds(396,180,100,20);
    lfpGainLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(lfpGainLabel);

    apGainLabel = new Label("AP GAIN","AP GAIN");
    apGainLabel->setFont(Font("Small Text", 13, Font::plain));
    apGainLabel->setBounds(396,130,100,20);
    apGainLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(apGainLabel);

    referenceLabel = new Label("REFERENCE", "REFERENCE");
    referenceLabel->setFont(Font("Small Text", 13, Font::plain));
    referenceLabel->setBounds(396,230,100,20);
    referenceLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(referenceLabel);

    filterLabel = new Label("FILTER", "FILTER CUT (GLOBAL)");
    filterLabel->setFont(Font("Small Text", 13, Font::plain));
    filterLabel->setBounds(396,280,200,20);
    filterLabel->setColour(Label::textColourId, Colours::grey);
    addAndMakeVisible(filterLabel);

    shankPath.startNewSubPath(27, 28);
    shankPath.lineTo(27, 514);
    shankPath.lineTo(27+5, 522);
    shankPath.lineTo(27+10, 514);
    shankPath.lineTo(27+10, 28);
    shankPath.closeSubPath();

   	setOption(1);

}

NeuropixInterface::~NeuropixInterface()
{

}

void NeuropixInterface::comboBoxChanged(ComboBox* comboBox)
{
	if (comboBox == apGainComboBox)
	{
		int gainSetting = comboBox->getSelectedId() - 1;

		for (int i = 0; i < 966; i++)
		{
			if (channelSelectionState[i] == 1)
				channelApGain.set(i, gainSetting);

			// inform the thread of the new settings
		}
	} else if (comboBox == lfpGainComboBox)
	{
		int gainSetting = comboBox->getSelectedId() - 1;

		for (int i = 0; i < 966; i++)
		{
			if (channelSelectionState[i] == 1)
				channelLfpGain.set(i, gainSetting);

			// inform the thread of the new settings
		}
	} else if (comboBox == referenceComboBox)
	{
		int refSetting = comboBox->getSelectedId() - 1;

		for (int i = 0; i < 966; i++)
		{
			if (channelSelectionState[i] == 1)
				channelReference.set(i,refSetting);

			// inform the thread of the new settings
		}
	} else if (comboBox == filterComboBox)
	{
		// inform the thread of the new settings
	}

	repaint();
}

void NeuropixInterface::buttonClicked(Button* button)
{
	if (button == selectAllButton)
	{
		for (int i = 0; i < 966; i++)
		{
			channelSelectionState.set(i, 1);
		}

		repaint();

	} else if (button == enableViewButton)
	{
		visualizationMode = 0;
		repaint();
	} 
	 else if (button == apGainViewButton)
	{
		visualizationMode = 1;
		repaint();
	} else if (button == lfpGainViewButton)
	{
		visualizationMode = 2;
		repaint();
	} else if (button == referenceViewButton)
	{
		visualizationMode = 3;
		repaint();
	} else if (button == enableButton)
	{
		for (int i = 0; i < 966; i++)
		{
			if (channelSelectionState[i] == 1) // channel is currently selected
			{
				if (channelStatus[i] > -1) // channel can be turned on
				{
					channelStatus.set(i, 1); // turn channel on

					int startPoint;
					int jump;

					if (option == 3)
					{
						startPoint = -768;
						jump = 384;
					} else {
						startPoint = -828;
						jump = 276;
					}

					for (int j = startPoint; j <= -startPoint; j += jump)
					{
						//std::cout << "Checking channel " << j + i << std::endl;

						int newChan = j + i;

						if (newChan >= 0 && newChan < 966 && newChan != i)
						{
							//std::cout << "  In range" << std::endl;

							if (channelStatus[newChan] > -1)
							{
								//std::cout << "    Turning off." << std::endl;
								channelStatus.set(newChan, 0); // turn connected channel off
							}
						}
					}
				}
			}
		}

		repaint();
	}
	
}

void NeuropixInterface::setOption(int o)
{
	option = o;

	for (int i = 0; i < 276; i++)
	{
		channelStatus.set(i, 1);
	}

	for (int i = 276; i < 384; i++)
	{
		if (option != 4)
			channelStatus.set(i, 1);
		else
			channelStatus.set(i, 0);
	}

	for (int i = 384; i < 960; i++)
	{
		if (option < 3)
		{
			channelStatus.set(i, -1);
		} else {
			channelStatus.set(i, 0);
		}
	}

	for (int i = 960; i < 966; i++)
	{
		if (option == 4)
		{
			channelStatus.set(i, 0);
		} else {
			channelStatus.set(i, -1);
		}
	}

	Array<int> refs;

	if (option < 3)
		refs = option1and2refs;
	else if (option == 3)
		refs = option3refs;
	else
		refs = option4refs;

	for (int i = 0; i < refs.size(); i++)
	{
		channelStatus.set(refs[i]-1, -2);
	}

	if (option < 3)
	{
		enableButton->setEnabledState(false);
	} else {
		enableButton->setEnabledState(true);
	}


	repaint();
}

void NeuropixInterface::mouseMove(const MouseEvent& event)
{
	float y = event.y;
	float x = event.x;

	//std::cout << x << " " << y << std::endl;

	bool isOverZoomRegionNew;
	bool isOverUpperBorderNew;
	bool isOverLowerBorderNew;

	if (y > lowerBound - zoomOffset - zoomHeight - dragZoneWidth/2 
	 && y < lowerBound - zoomOffset + dragZoneWidth/2 &&  x > 9 && x < 54)
	{
		isOverZoomRegionNew = true;
	} else {
		isOverZoomRegionNew = false;
	}

	if (isOverZoomRegionNew)
	{
		if (y > lowerBound - zoomHeight - zoomOffset - dragZoneWidth/2
			&& y <  lowerBound - zoomHeight - zoomOffset + dragZoneWidth/2 )
		{
			isOverUpperBorderNew = true;

		} else if (y > lowerBound  - zoomOffset - dragZoneWidth/2
			&& y <  lowerBound  - zoomOffset + dragZoneWidth/2)
		{
			isOverLowerBorderNew = true;

		} else {
			isOverUpperBorderNew = false;
			isOverLowerBorderNew = false;
		}
	}

	if (isOverZoomRegionNew != isOverZoomRegion ||
		isOverLowerBorderNew != isOverLowerBorder ||
		isOverUpperBorderNew != isOverUpperBorder)
	{
		isOverZoomRegion = isOverZoomRegionNew;
		isOverUpperBorder = isOverUpperBorderNew;
		isOverLowerBorder = isOverLowerBorderNew;

		if (!isOverZoomRegion)
		{
			cursorType = MouseCursor::NormalCursor;
		} else {

			if (isOverUpperBorder)
				cursorType = MouseCursor::TopEdgeResizeCursor;
			else if (isOverLowerBorder)
				cursorType = MouseCursor::BottomEdgeResizeCursor;
			else
				cursorType = MouseCursor::NormalCursor;
		}

		repaint();
	}

	if (x > 225 - channelHeight && x < 225 + channelHeight && y < lowerBound && y > 18)
	{
		int chan = getNearestChannel(x, y);
		isOverChannel = true;
		channelInfoString = getChannelInfoString(chan);

		//std::cout << channelInfoString << std::endl;

		repaint();
	} else {
		bool isOverChannelNew = false;

		if (isOverChannelNew != isOverChannel)
		{
			isOverChannel = isOverChannelNew;
			repaint();
		}
	}
}

int NeuropixInterface::getNearestChannel(int x, int y)
{
	int chan = ((lowerBound - y) * 2 / channelHeight) + lowestChan + 2;

	if (chan % 2 == 1)
		chan += 1;

	if (x > 225)
		chan += 1;

	return chan;
}

String NeuropixInterface::getChannelInfoString(int chan)
{
	String a;
	a += "Channel ";
	a += String(chan + 1);
	a += "\n\nType: ";
	
	if (channelStatus[chan] == -2)
	{
		a += "REF";
		return a;
	}
	else
	{
		a += "SIGNAL";
	}

	a += "\nEnabled: ";

	if (channelStatus[chan] == 1)
		a += "YES";
	else
		a += "NO";

	a += "\nAP Gain: ";
	a += String(channelApGain[chan]);

	a += "\nLFP Gain: ";
	a += String(channelLfpGain[chan]);

	a += "\nReference: ";
	a += String(channelReference[chan]);

	return a;
}

void NeuropixInterface::mouseUp(const MouseEvent& event)
{
	if (isSelectionActive)
	{

		isSelectionActive = false;
		repaint();
	}
	
}

void NeuropixInterface::mouseDown(const MouseEvent& event)
{
	initialOffset = zoomOffset;
	initialHeight = zoomHeight;

	//std::cout << event.x << std::endl;

	if (event.x > 150 && event.x < 400)
	{

		if (!event.mods.isShiftDown())
		{
			for (int i = 0; i < 966; i++)
				channelSelectionState.set(i, 0);
		}

		if (event.x > 225 - channelHeight && event.x < 225 + channelHeight)
		{
			int chan = getNearestChannel(event.x, event.y);

			//std::cout << chan << std::endl;

			if (chan >= 0 && chan < 966)
			{
				channelSelectionState.set(chan, 1);
			}

		}
		repaint();
	}
}

void NeuropixInterface::mouseDrag(const MouseEvent& event)
{
	if (isOverZoomRegion)
	{
		if (isOverUpperBorder)
		{
			zoomHeight = initialHeight - event.getDistanceFromDragStartY();

			if (zoomHeight > lowerBound - zoomOffset - 18)
				zoomHeight = lowerBound - zoomOffset - 18;
		}
		else if (isOverLowerBorder)
		{
			
			zoomOffset = initialOffset - event.getDistanceFromDragStartY();
			
			if (zoomOffset < 0)
			{
				zoomOffset = 0;
			} else {
				zoomHeight = initialHeight + event.getDistanceFromDragStartY();
			}

		}
		else {
			zoomOffset = initialOffset - event.getDistanceFromDragStartY();
		}
		//std::cout << zoomOffset << std::endl;
	} else if (event.x > 150 && event.x < 450)
	{
		int w = event.getDistanceFromDragStartX();
		int h = event.getDistanceFromDragStartY();
		int x = event.getMouseDownX();
		int y = event.getMouseDownY();

		if (w < 0)
		{
			x = x + w; w = -w;
		}

		if (h < 0)
		{
			y = y + h; h = -h;
		}

		selectionBox = Rectangle<int>(x, y, w, h);
		isSelectionActive = true;

		//if (x < 225)
		//{
		int chanStart = getNearestChannel(224, y + h);
		int chanEnd = getNearestChannel(224, y) + 1;

		//std::cout << chanStart << " " << chanEnd << std::endl;

		if (x < 225 + channelHeight)
		{
			for (int i = 0; i < 966; i++)
			{
				if (i >= chanStart && i <= chanEnd)
				{
					if (i % 2 == 1)
					{
						if ((x + w > 225) || (x > 225 && x < 225 + channelHeight))
							channelSelectionState.set(i, 1);
						else
							channelSelectionState.set(i, 0);
					} else {
						if ((x < 225) && (x + w > (225 - channelHeight)))
							channelSelectionState.set(i, 1);
						else
							channelSelectionState.set(i, 0);
					}
				} else {
					if (!event.mods.isShiftDown())
						channelSelectionState.set(i, 0);
				}
			}
		} else {
			for (int i = 0; i < 966; i++)
			{
				if (!event.mods.isShiftDown())
					channelSelectionState.set(i, 0);
			}
		}

		repaint();
	}

	if (zoomOffset > lowerBound - zoomHeight - 18)
		zoomOffset = lowerBound - zoomHeight - 18;
	else if (zoomOffset < 0)
		zoomOffset = 0;

	if (zoomHeight < 10)
		zoomHeight = 10;
	if (zoomHeight > 100)
		zoomHeight = 100;

	repaint();
}

void NeuropixInterface::mouseWheelMove(const MouseEvent&  event, const MouseWheelDetails&   wheel)
{

	if (event.x > 100 && event.x < 450)
	{

		if (wheel.deltaY > 0)
			zoomOffset += 2;
		else
			zoomOffset -= 2;

		//std::cout << wheel.deltaY << " " << zoomOffset << std::endl;

		if (zoomOffset < 0)
		{
			zoomOffset = 0;
		} else if (zoomOffset + 18 + zoomHeight > lowerBound)
		{
			zoomOffset = lowerBound - zoomHeight - 18;
		}

		repaint();
	}

    

}

MouseCursor NeuropixInterface::getMouseCursor()
{
    MouseCursor c = MouseCursor(cursorType);

    return c;
}

void NeuropixInterface::paint(Graphics& g)
{

	int xOffset = 27;

	// draw zoomed-out channels channels

	for (int i = 0; i < channelStatus.size(); i++)
	{
		g.setColour(getChannelColour(i));

		g.setPixel(xOffset + 3 + ((i % 2)) * 2, 513 - (i / 2));
		g.setPixel(xOffset + 3 + ((i % 2)) * 2 + 1, 513 - (i / 2));
	}

	// channel 1 = pixel 513
	// channel 966 = pixel 30
	// 483 pixels for 966 channels

	// draw channel numbers

	g.setColour(Colours::grey);
	g.setFont(12);

	int ch = 0;

	for (int i = 513; i > 30; i -= 50)
	{
		g.drawLine(6, i, 18, i);
		g.drawLine(44, i, 54, i);
		g.drawText(String(ch), 59, int(i) - 6, 100, 12, Justification::left, false);
		ch += 100;
	}

	// draw shank outline
	g.setColour(Colours::lightgrey);
	g.strokePath(shankPath, PathStrokeType(1.0));

	// draw zoomed channels

	lowestChan = (513 - (lowerBound - zoomOffset)) * 2 - 1;
	highestChan = (513 - (lowerBound - zoomOffset - zoomHeight)) * 2 + 10;

	float totalHeight = float(lowerBound + 100);
	channelHeight = totalHeight / ((highestChan - lowestChan) / 2);

	for (int i = lowestChan; i <= highestChan; i++)
	{
		if (i >= 0 && i < 966)
		{

			float xLoc = 225 - channelHeight * (1 - (i % 2));
			float yLoc = lowerBound - ((i - lowestChan - (i % 2)) / 2 * channelHeight);

			if (channelSelectionState[i])
			{
				g.setColour(Colours::white);
				g.fillRect(xLoc, yLoc, channelHeight, channelHeight);
			}

			g.setColour(getChannelColour(i));

			g.fillRect(xLoc+1, yLoc+1, channelHeight-2, channelHeight-2);

		}
		
	}

	// draw borders around zoom area

	g.setColour(Colours::darkgrey.withAlpha(0.7f));
	g.fillRect(25, 0, 15, lowerBound - zoomOffset - zoomHeight);
	g.fillRect(25, lowerBound-zoomOffset, 15, zoomOffset+10);

	g.setColour(Colours::darkgrey);
	g.fillRect(100, 0, 250, 22);
	g.fillRect(100, lowerBound + 10, 250, 100);

	if (isOverZoomRegion)
		g.setColour(Colour(25,25,25));
	else
		g.setColour(Colour(55,55,55));

	Path upperBorder;
	upperBorder.startNewSubPath(5, lowerBound - zoomOffset - zoomHeight);
	upperBorder.lineTo(54, lowerBound - zoomOffset - zoomHeight);
	upperBorder.lineTo(100, 16);
	upperBorder.lineTo(350, 16);

	Path lowerBorder;
	lowerBorder.startNewSubPath(5, lowerBound - zoomOffset);
	lowerBorder.lineTo(54, lowerBound - zoomOffset);
	lowerBorder.lineTo(100, lowerBound + 16);
	lowerBorder.lineTo(350, lowerBound + 16);

	g.strokePath(upperBorder, PathStrokeType(2.0));
	g.strokePath(lowerBorder, PathStrokeType(2.0));

	// draw selection zone

	if (isSelectionActive)
	{
		g.setColour(Colours::white.withAlpha(0.5f));
		g.drawRect(selectionBox);
	}

	if (isOverChannel)
	{
		//std::cout << "YES" << std::endl;
		g.setColour(Colour(55, 55, 55));
		g.setFont(15);
		g.drawMultiLineText(channelInfoString, 280, 310, 250);
	}

	drawLegend(g);

}

void NeuropixInterface::drawLegend(Graphics& g)
{
	g.setColour(Colour(55, 55, 55));
	g.setFont(15);

	int xOffset = 100;
	int yOffset = 310;

	switch (visualizationMode)
	{
		case 0: // ENABLED STATE
			g.drawMultiLineText("ENABLED?", xOffset, yOffset, 200);
			g.drawMultiLineText("YES", xOffset+30, yOffset+22, 200);
			g.drawMultiLineText("NO", xOffset+30, yOffset+42, 200);
			g.drawMultiLineText("N/A", xOffset+30, yOffset+62, 200);
			g.drawMultiLineText("REF", xOffset+30, yOffset+82, 200);

			g.setColour(Colours::yellow);
			g.fillRect(xOffset+10, yOffset + 10, 15, 15);

			g.setColour(Colours::orange);
			g.fillRect(xOffset+10, yOffset + 30, 15, 15);

			g.setColour(Colours::grey);
			g.fillRect(xOffset+10, yOffset + 50, 15, 15);

			g.setColour(Colours::black);
			g.fillRect(xOffset+10, yOffset + 70, 15, 15);

			break;

		case 1: // AP GAIN
			g.drawMultiLineText("AP GAIN", xOffset, yOffset, 200);

			for (int i = 0; i < 8; i++)
			{
				g.drawMultiLineText(String(i), xOffset+30, yOffset+22 + 20*i, 200);
			}

			for (int i = 0; i < 8; i++)
			{
				g.setColour(Colour(25*i,25*i,50));
				g.fillRect(xOffset+10, yOffset + 10 + 20*i, 15, 15);
			}



			break;

		case 2: // LFP GAIN
			g.drawMultiLineText("LFP GAIN", xOffset, yOffset, 200);

			for (int i = 0; i < 8; i++)
			{
				g.drawMultiLineText(String(i), xOffset+30, yOffset+22 + 20*i, 200);
			}

			for (int i = 0; i < 8; i++)
			{
				g.setColour(Colour(66,25*i,35*i));
				g.fillRect(xOffset+10, yOffset + 10 + 20*i, 15, 15);
			}

			break;

		case 3: // REFERENCE
			g.drawMultiLineText("REFERENCE", xOffset, yOffset, 200);

			for (int i = 0; i < 11; i++)
			{
				g.drawMultiLineText(String(i), xOffset+30, yOffset+22 + 20*i, 200);
			}

			for (int i = 0; i < 11; i++)
			{
				g.setColour(Colour(20*i,200,20*i));
				g.fillRect(xOffset+10, yOffset + 10 + 20*i, 15, 15);
			}

			break;
	}
}

Colour NeuropixInterface::getChannelColour(int i)
{
	if (visualizationMode == 0) // ENABLED STATE
	{
		if (channelStatus[i] == -1) // not available
		{
			return Colours::grey;
		} 
		else if (channelStatus[i] == 0) // disabled
		{
			return Colours::orange;
		} 
		else if (channelStatus[i] == 1) // enabled
		{
			return Colours::yellow;
		} 
		else if (channelStatus[i] == -2) // reference
		{
			return Colours::black;
		} else 
		{
			return Colours::green; // wtf?
		}
	} else if (visualizationMode == 1) // AP GAIN
	{
		if (channelStatus[i] == -1) // not available
		{
			return Colours::grey;
		} 
		else if (channelStatus[i] == -2) // reference
		{
			return Colours::black;
		}
		else
		{
			return Colour(25*channelApGain[i],25*channelApGain[i],50);
		} 
	} else if (visualizationMode == 2) // LFP GAIN
	{
		if (channelStatus[i] == -1) // not available
		{
			return Colours::grey;
		} 
		else if (channelStatus[i] == -2) // reference
		{
			return Colours::black;
		}
		else
		{
			return Colour(66,25*channelLfpGain[i],35*channelLfpGain[i]);
		} 
	} else if (visualizationMode == 3) // REFERENCE
	{
		if (channelStatus[i] == -1) // not available
		{
			return Colours::grey;
		} 
		else if (channelStatus[i] == -2) // reference
		{
			return Colours::black;
		}
		else
		{
			return Colour(20*channelReference[i], 200, 20*channelReference[i]);
		} 
	}

	
}