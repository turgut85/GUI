/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Allen Institute for Brain Science

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

#include "neuropix-api/half.hpp"


NeuropixThread::NeuropixThread(SourceNode* sn) : DataThread(sn), baseStationAvailable(false)
{
	OpenErrorCode errorCode = neuropix.neuropix_open(); // establishes a data connection with the basestation

	if (errorCode == OPEN_SUCCESS)
	{
		std::cout << "Open success!" << std::endl;
		neuropix.neuropix_close();
	}
	else {
		CoreServices::sendStatusMessage("Failure with error code " + String(errorCode));
		std::cout << "Failure with error code " << String(errorCode) << std::endl;
		baseStationAvailable = false;
		return;
	}

	baseStationAvailable = true;
	internalTrigger = true;

	// // GET SYSTEM INFO:
	VersionNumber hw_version;
	ErrorCode error1 = neuropix.neuropix_getHardwareVersion(&hw_version);

	unsigned char bs_version;
	ConfigAccessErrorCode error2 = neuropix.neuropix_getBSVersion(bs_version);

	unsigned char bs_revision;
	ConfigAccessErrorCode error3 = neuropix.neuropix_getBSRevision(bs_revision);

	VersionNumber vn = neuropix.neuropix_getAPIVersion();

	AsicID asicId;
	EepromErrorCode error4 = neuropix.neuropix_readId(asicId);

	std::cout << "  Hardware version number: " << hw_version.major << "." << hw_version.minor << std::endl;
	std::cout << "  Basestation version number: " << String(bs_version) << "." << String(bs_revision) << std::endl;
	std::cout << "  API version number: " << vn.major << "." << vn.minor << std::endl;
	std::cout << "  Asic info: " << asicId.probeType << std::endl;

	dataBuffer = new DataBuffer(384, 10000); // start with 384 channels and automatically resize

	// channel selections:
	// Options 1 & 2 -- fixed 384 channels
	// Option 3 -- select 384 of 960 shank electrodes
	// Option 4 -- select 276 of 966 shank electrodes

}

NeuropixThread::~NeuropixThread()
{
	//neuropix_close(); // closes the data and configuration link 
}


/** Returns true if the data source is connected, false otherwise.*/
bool NeuropixThread::foundInputSource()
{
	return baseStationAvailable;
}

/** Initializes data transfer.*/
bool NeuropixThread::startAcquisition()
{

	// set into recording mode
	neuropix.neuropix_mode(ASIC_RECORDING);

	// clear the neuropix buffer
    neuropix.neuropix_nrst(false);
    neuropix.neuropix_resetDatapath();
    neuropix.neuropix_nrst(true);

	// clear the internal buffer
	dataBuffer->clear();

	if (internalTrigger)
	{
		ConfigAccessErrorCode caec = neuropix.neuropix_setNeuralStart();

		if (caec != CONFIG_SUCCESS)
		{
			std::cout << "start failed with error code " << caec << std::endl;
			return false;
		}
	}
	  
	startThread();

	return true;
}

/** Stops data transfer.*/
bool NeuropixThread::stopAcquisition()
{

	if (isThreadRunning())
	{
		signalThreadShouldExit();
	}

	neuropix.neuropix_nrst(false);

	return true;
}

/** Returns the number of continuous headstage channels the data source can provide.*/
int NeuropixThread::getNumHeadstageOutputs()
{
	return 384;
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
	return 30000.;
}

/** Returns the volts per bit of the data source.*/
float NeuropixThread::getBitVolts(Channel* chan)
{
	return 10.0;
}

/** Returns the number of event channels of the data source.*/
int NeuropixThread::getNumEventChannels()
{
	return 16;
}

void NeuropixThread::selectElectrode(int chNum, int connection)
{
	ShankConfigErrorCode scec = neuropix.neuropix_selectElectrode(chNum, connection);
}

void NeuropixThread::setReference(int chNum, int refSetting)
{
	BaseConfigErrorCode bcec = neuropix.neuropix_setReference(chNum, refSetting);
}

void NeuropixThread::setGain(int chNum, int apGain, int lfpGain)
{
	BaseConfigErrorCode bcec = neuropix.neuropix_setGain(chNum, apGain, lfpGain);
}

void NeuropixThread::setFilter(int filter)
{
	BaseConfigErrorCode bcec = neuropix.neuropix_setFilter(filter);
}

void NeuropixThread::setTriggerMode(bool trigger)
{
	ConfigAccessErrorCode caec = neuropix.neuropix_triggerMode(trigger);
}

bool NeuropixThread::updateBuffer()
{

	ElectrodePacket packet;

	ReadErrorCode rec = neuropix.neuropix_readElectrodeData(packet);

	if (rec == READ_SUCCESS)
	{
		int64 timestamp = 0;
		uint64 eventCode = 0;

		for (int i = 0; i < 12; i++)
		{
			eventCode = (uint64) packet.synchronization[i];
			timestamp = (int64) packet.ctrs[i][0];
			
			dataBuffer->addToBuffer(packet.apData[i], &timestamp, &eventCode, 1);
		}

		//std::cout << "READ SUCCESS!" << std::endl;
	
	}
	else {
		if (rec == NO_DATA_LINK)
		{
			//std::cout << "NO DATA LINK" << std::endl;
		}
		else if (rec == WRONG_DATA_MODE)
		{
			//std::cout << "WRONG DATA MODE" << std::endl;
		}
		else if (rec == DATA_BUFFER_EMPTY)
		{
			//std::cout << "DATA BUFFER EMPTY" << std::endl;
		}
		else if (rec == DATA_ERROR)
		{
			//std::cout << "DATA ERROR" << std::endl;
		}
		else {
			//std::cout << "ERROR CODE: " << rec << std::endl;
		}
	}
	 
	return true;
}