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
#include "neuropix-api/ElectrodePacket.h"

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
	std::cout << "  Basestation version number: " << bs_version << std::endl;
	std::cout << "  Basestation revision number: " << bs_revision << std::endl;
	std::cout << "  API version number: " << vn.major << "." << vn.minor << std::endl;
	std::cout << "  Asic info: " << asicId.probeType << std::endl;

	dataBuffer = new DataBuffer(384, 10000); // start with 384 channels and automatically resize

	// channel selections:
	// Options 1 & 2 -- fixed 384 channels
	// Option 3 -- select 384 of 960 shank electrodes
	// Option 4 -- select 276 of 966 shank electrodes

	// void neuropix_selectElectrodes(int chNum, int connection)
	// chNum = 0 to 383
	// connection = 0-2 for Option 3 [1 to 768, chNum + 384*connection]
	//              0-3 for Option 4 [1 to 828, chNum + 276*chNum]

	// int neuropix_setReference(int chNum, int refSetting)
	//  Options 1, 2, and 3: ch 0-383, 0-10 refSetting
	//  Option 4: 0-275, 0-7 refSetting
	//  Each channel can have a separate reference input
	//  

	// int neuropix_setGain(int ch, int apGain, int lfpGain, bool standby)
	//   ch = 0-383
	//   apGain = 0-7
	//   lfpGain = 0-7
	//   standby = 0 or 1

	// int neuropix_setFilter (int filter)
	// 00 = 300 Hz
	// 01 = 500 Hz
	// 11 = 1 kHz

	// int neuropix_setMode (unsigned char mode)
	// Mode 2: impedance measurement, 3: recording

	// status codes:
	// 0 = success
	// 1 = fail
	// 2 = no device
	// 3 = device info fail
	// 4 = open fail
	// 5 = FIFO fail
	// 6 = write fail
	// 7 = read fail
	// 8 = no data to read
	// 9 = shank option error
}

NeuropixThread::~NeuropixThread()
{
	//neuropix_close(); // closes the data and configuration link 
}


/** Returns true if the data source is connected, false otherwise.*/
bool NeuropixThread::foundInputSource()
{
	return true;
	//return baseStationAvailable;
}

/** Initializes data transfer.*/
bool NeuropixThread::startAcquisition()
{
	// clear the neuropix buffer
    neuropix.neuropix_nrst(false);
    neuropix.neuropix_resetDatapath();
    neuropix.neuropix_nrst(true);

	// clear the internal buffer
	dataBuffer->clear();

	ElectrodePacket packet;

	if (internalTrigger)
	  ConfigAccessErrorCode caec = neuropix.neuropix_setNeuralStart();

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
	return 0;
}

void NeuropixThread::selectElectrode(int chNum, int connection)
{
	// ShankConfigErrorCode scec = neuropix_selectElectrode(chNum, connection);

	// if (scec == SHANK_SUCCESS)
	// {
	// 	// all good!
	// }
}

void NeuropixThread::setReference(int chNum, int refSetting)
{
	// BaseConfigErrorCode bcec = neuropix_setReference(chNum, refSetting);
}

void NeuropixThread::setGain(int chNum, int apGain, int lfpGain)
{
	// BaseConfigErrorCode bcec = neuropix_setGain(ch, apGain, lfpGain, 0);
}

void NeuropixThread::setFilter(int filter)
{
	// BaseConfigErrorCode bcec = neuropix_setFilter(filter);
}

void NeuropixThread::setTriggerMode(bool trigger)
{
	// ConfigAccessErrorCode caec = neuropix_triggermode(trigger);
}


bool NeuropixThread::updateBuffer()
{

	ElectrodePacket packet = ElectrodePacket();

	ReadErrorCode rec = neuropix.neuropix_readElectrodeData(packet);

//	ReadErrorCode rec = DATA_ERROR;

	if (rec == READ_SUCCESS)
	{
		int64 timestamp = 0;
		uint64 eventCode = 0;
		float data[384] = { 0.0 };

		for (int i = 0; i < 12; i++)
		{
			eventCode = (uint64)packet.synchronization[i];
			timestamp = (int64)packet.ctrs[i][0];

			for (int ch = 0; ch < 384; ch++)
			{
				float dataPt = half_float::half_cast<float, half_float::half>(packet.apData[i][ch]);
				data[ch] = dataPt;
			}
			
			dataBuffer->addToBuffer(data, &timestamp, &eventCode, 1);
		}

		std::cout << "READ SUCCESS!" << std::endl;
	
	}
	else {
		if (rec == NO_DATA_LINK)
		{
			std::cout << "NO DATA LINK" << std::endl;
		}
		else if (rec == WRONG_DATA_MODE)
		{
			std::cout << "WRONG DATA MODE" << std::endl;
		}
		else if (rec == DATA_BUFFER_EMPTY)
		{
			std::cout << "DATA BUFFER EMPTY" << std::endl;
		}
		else if (rec == DATA_ERROR)
		{
			std::cout << "DATA ERROR" << std::endl;
		}
		else {
			std::cout << "ERROR CODE: " << rec << std::endl;
		}
	}
	 
	return true;
}