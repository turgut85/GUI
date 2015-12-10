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
// #include Neuropix_basestation_api

NeuropixThread::NeuropixThread(SourceNode* sn) : DataThread(sn), baseStationAvailable(false)
{

	// OpenErrorCode errorCode = neuropix_open(); // establishes a data connection with the basestation

	// if (errorCode == OPEN_SUCCESS)
	// {
	// 	errorCode = neuropix_initialize(); // initializes the headstage with default values

	// 	baseStationAvailable = true;

	// 	// // GET SYSTEM INFO:
	// 	// ErrorCode neuropix_getHardwareVersion( version_number_version); ??
	// 	// ConfigAccessErrorCode caec = neuropix_getBSversion( unsigned char& version major);
	// 	// ConfigAccessErrorCode caec = neuropix_getBSrevision( unsigned char& version minor);
	// 	// struct Version_number = neuropix_getAPIVersion();
	// 	// EepromErrorCode eeec = neuropix_readid(AsicID& id);
	// } else {
	// 	baseStationAvailable = false;
	// }

	// void neuropix_readId(unsigned char* id) // reads 32-bit ID
	// update the editor 

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
	return baseStationAvailable;
}

/** Initializes data transfer.*/
bool NeuropixThread::startAcquisition()
{
	//if (internalTrigger)
	// ConfigAccessErrorCode caec neuropix_setNeuralStart();
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
	return 25000.;
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

	// ElectrodePacket packet;

	// ReadErrorCode rec = neuropix_readelectrodedata(packet);

	// // packet size = 152 x 32-bit = 4864 bits
	// // 32 LFP channels???
	// // 384 AP channels

	// float lfpData[32];
	// float spikeData[384];
	
	// int ctr[12];
	
	// int ch = 0;
	// int ctrIndex = 0;

	// int startBit = 32;

	// while (startBit < 24*32)
	// {
	// 	ctr[ctrIndex] = packet[startBit+22:startBit+32]; // lsb
	// 	ctr[ctrIndex] += packet[startBit+12:startBit+22] << 8; // msb

	// 	ctrIndex++;

	// 	if (startBit == 32)
	// 		startBit = 12*32;
	// 	else
	// 		startBit += 32;
	// }

	// // convert counter to timestamp?

	// while (startBit < 152*32)
	// {
	// 	spikeData[ch] = packet[startBit+2:startBit+12];
	// 	spikeData[ch+1] = packet[startBit+12:startBit+22];
	// 	spikeData[ch+2] = pcket[startBit+22:startBit+32];

	// 	ch += 3;
	// 	startBit += 32;
	// }

	// dataBuffer->addToBuffer(spikeData, &timestamp, &eventCode, 1);

	// return true;
}