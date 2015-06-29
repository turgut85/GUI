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

NeuropixThread::NeuropixThread(SourceNode* sn) : DataThread(sn)
{

}

NeuropixThread::~NeuropixThread()
{

}


/** Returns true if the data source is connected, false otherwise.*/
bool NeuropixThread::foundInputSource()
{
	return true;
}

/** Initializes data transfer.*/
bool NeuropixThread::startAcquisition()
{
	return true;
}

/** Stops data transfer.*/
bool NeuropixThread::stopAcquisition()
{
	return true;
}

/** Returns the number of continuous headstage channels the data source can provide.*/
int NeuropixThread::getNumHeadstageOutputs()
{
	return 16;
}

/** Returns the number of continuous aux channels the data source can provide.*/
int NeuropixThread::getNumAuxOutputs()
{
	return 0;
}

/** Returns the number of continuous ADC channels the data source can provide.*/
int NeuropixThread::getNumAdcOutputs()
{
	return 0;
}

/** Returns the sample rate of the data source.*/
float NeuropixThread::getSampleRate()
{
	return 25000;
}

/** Returns the volts per bit of the data source.*/
float NeuropixThread::getBitVolts(Channel* chan)
{
	return 10.0;
}

/** Returns the number of event channels of the data source.*/
int NeuropixThread::getNumEventChannels()
{
	return 0;
}


bool NeuropixThread::updateBuffer()
{
	return true;
}