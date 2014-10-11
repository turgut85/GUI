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

#include <stdio.h>
#include "NetworkEvents.h"
#include "NetworkEventsEditor.h"
#include "../../UI/UIComponent.h"

const int MAX_MESSAGE_LENGTH = 64000;

/*********************************************/
NetworkEvents::NetworkEvents()
	: GenericProcessor("Network Events"), Thread("NetworkThread"), threshold(200.0), bufferZone(5.0f), state(false)

{
	zmqcontext =  zmq_ctx_new();
	firstTime = true;
	responder = nullptr;
	urlport = 5005;
	threadRunning = false;
	opensocket();

	sendSampleCount = false; // disable updating the continuous buffer sample counts,
							 // since this processor only sends events
	shutdown = false;
}

void NetworkEvents::setNewListeningPort(int port)
{
	// first, close existing thread.
	disable();
	// allow some time for thread to quit
	sleep(300);
	urlport = port;
	opensocket();
}

NetworkEvents::~NetworkEvents()
{
	shutdown = true;
	disable();
}

bool NetworkEvents::disable()
{
	if (threadRunning)
	{
		zmq_close(responder);
		zmq_ctx_destroy(zmqcontext); // this will cause the thread to exit

		if (!shutdown)
			zmqcontext = zmq_ctx_new();
	}

	return true;
}

AudioProcessorEditor* NetworkEvents::createEditor(
){
    editor = new NetworkEventsEditor(this, true);

    return editor;

}


void NetworkEvents::setParameter(int parameterIndex, float newValue)
{


}


void NetworkEvents::handleEvent(int eventType, juce::MidiMessage& event, int samplePosition)
{

}

void NetworkEvents::sendMessage(String s, MidiBuffer& eventBuffer)
{

    CharPointer_UTF8 data = s.toUTF8();

    addEvent(eventBuffer,
            MESSAGE,
            0,
            0,
            0,
            data.length()+1, //It doesn't hurt to send the end-string null and can help avoid issues
            (uint8*) data.getAddress());
}


String NetworkEvents::handleSpecialMessages(String msg)
{
	// any special commands can go here (e.g., start/stop recording)
	return "Hello good sir.";
}

void NetworkEvents::process(AudioSampleBuffer& buffer,
							MidiBuffer& events,
							int& nSamples)
{

	checkForEvents(events);

	lock.enter();

	while (!networkMessagesQueue.empty()) 
	{
		std::cout << "Received message!" << std::endl;
		String msg = networkMessagesQueue.front();
		sendMessage(msg, events);
		sendActionMessage("Network events received: " + msg);
		networkMessagesQueue.pop();
	}

	lock.exit();

	nSamples = -10; // make sure this is not processed;

}


void NetworkEvents::opensocket()
{
	startThread();
}

void NetworkEvents::run() {

	responder = zmq_socket (zmqcontext, ZMQ_REP);
	String url = String("tcp://*:") + String(urlport);
	int rc = zmq_bind (responder, url.toRawUTF8());

	if (rc != 0) {
		// failed to open socket?
		return;
	}

	threadRunning = true;
	char* buffer = new char[MAX_MESSAGE_LENGTH];
	int result = -1;

	while (threadRunning) 
	{

		result = zmq_recv (responder, buffer, MAX_MESSAGE_LENGTH-1, 0); // blocking

		if (result < 0) // will only happen when responder dies.
			break;

		String msg(buffer, result);


		if (result > 0) 
		{
			lock.enter();
			networkMessagesQueue.push(msg);
			lock.exit();

			std::cout << "Received message!" << std::endl;

			String response = handleSpecialMessages(msg);

			// echo for now:
			zmq_send (responder, buffer, 1, 0);
			//zmq_send (responder, response.getCharPointer(), response.length(), 0);
		} 
		else 
		{
			String zeroMessageError = "Received Zero Message?!?!?";
			zmq_send (responder, zeroMessageError.getCharPointer(), zeroMessageError.length(), 0);
		}

	}

	zmq_close(responder);

	delete buffer;

	threadRunning = false;

	return;
}


bool NetworkEvents::isReady()
{
   
        return true;
    
}


float NetworkEvents::getDefaultSampleRate()
{
    return 30000.0f;
}

int NetworkEvents::getDefaultNumOutputs()
{
    return 0;
}

float NetworkEvents::getDefaultBitVolts()
{
    return 0.05f;
}

void NetworkEvents::enabledState(bool t)
{

    isEnabled = t;

}

bool NetworkEvents::isSource()
{
	return true;
}

void NetworkEvents::saveCustomParametersToXml(XmlElement* parentElement)
{
    XmlElement* mainNode = parentElement->createNewChildElement("NETWORKEVENTS");
    mainNode->setAttribute("port", urlport);
}


void NetworkEvents::loadCustomParametersFromXml()
{

	if (parametersAsXml != nullptr)
	{
		forEachXmlChildElement(*parametersAsXml, mainNode)
		{
			if (mainNode->hasTagName("NETWORKEVENTS"))
			{
				setNewListeningPort(mainNode->getIntAttribute("port"));
			}
		}
	}
}

