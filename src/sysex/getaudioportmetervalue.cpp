#include "getaudioportmetervalue.h"
#include "retaudioportmetervalue.h"

GetAudioPortMeterValue::GetAudioPortMeterValue(Device *device)
	: PortSysExMessage(GET_AUDIO_PORT_METER_VALUE, SysExMessage::QUERY,
					   device) {}

GetAudioPortMeterValue::~GetAudioPortMeterValue() {}

void GetAudioPortMeterValue::createAnswer(Command m_Command,
										  std::vector<unsigned char> *message,
										  Device *m_pDevice) {
	m_pAnswer = new RetAudioPortMeterValue(m_Command, message, m_pDevice);
	if (debug)
		m_pAnswer->setDebug(true);
	m_pAnswer->parseAnswerData();
}

std::vector<unsigned char> *GetAudioPortMeterValue::m_pGetMessageData() {
	BYTE_VECTOR *data = new BYTE_VECTOR();
	BYTE_VECTOR *portId = getPortIdBytes();
	data->insert(data->begin(), portId->begin(), portId->end());
	data->push_back(static_cast<unsigned char>(m_portDirection));
	// MIDI::printMessage(data);
	return data;
}