#include "getmidiportroute.h"
#include "retsetmidiportroute.h"

GetMidiPortRoute::GetMidiPortRoute(Device *device)
	: SysExMessage(Command::GET_MIDI_PORT_ROUTE, SysExMessage::QUERY, device) {}

BYTE_VECTOR *GetMidiPortRoute::getMessageData() {
	BYTE_VECTOR *messageData = MIDI::byteSplit7bit(m_iPortNumber, 2);
	return messageData;
}

void GetMidiPortRoute::createAnswer(Command cmd, BYTE_VECTOR *message,
									Device *device) {
	m_pAnswer = std::make_shared<RetSetMidiPortRoute>(cmd, message, device);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}
