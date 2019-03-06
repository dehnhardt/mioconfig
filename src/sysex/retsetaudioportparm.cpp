#include "retsetaudioportparm.h"
#include "retsetaudioglobalparm.h"

RetSetAudioPortParm::RetSetAudioPortParm(Device *device)
	: SysExMessage(RET_SET_AUDIO_PORT_PARM, QUERY, device) {}

RetSetAudioPortParm::~RetSetAudioPortParm() {}

void RetSetAudioPortParm::parseAnswerData() {
	m_iCommandVersionNumber = m_pData->at(0);
	m_iPortId = static_cast<int>(MIDI::byteJoin7bit(m_pData, 1, 2));
	m_audioPortType = static_cast<AudioPortType>(m_pData->at(3));
	m_iInputChannels = m_pData->at(4);
	m_iOutputChannels = m_pData->at(5);
	m_iNumberOfPortConfigurationBlocks = m_pData->at(6);
	m_pAudioPortConfigurations =
		new AudioPortConfiguration *[m_iNumberOfPortConfigurationBlocks];
	for (int i = 0; i < m_iNumberOfPortConfigurationBlocks; i++) {
		unsigned long offset = static_cast<unsigned long>(7 + i * 6);
		AudioPortConfiguration *audioPortConfiguration =
			new AudioPortConfiguration();
		audioPortConfiguration->audioConfigurationNumber = m_pData->at(offset);
		audioPortConfiguration->maxAudioChannelsSupported =
			m_pData->at(offset + 1);
		audioPortConfiguration->minInputChannelsSupported =
			m_pData->at(offset + 2);
		audioPortConfiguration->maxInputChannelsSupported =
			m_pData->at(offset + 3);
		audioPortConfiguration->minOutputChannelsSupported =
			m_pData->at(offset + 4);
		audioPortConfiguration->maxOutputChannelsSupported =
			m_pData->at(offset + 5);
		m_pAudioPortConfigurations[i] = audioPortConfiguration;
	}
	unsigned long offset =
		static_cast<unsigned long>(7 + m_iNumberOfPortConfigurationBlocks * 6);
	m_iMaxPortNameLength = m_pData->at(offset);
	offset += 1;
	m_iPortNameLength = m_pData->at(offset);
	offset += 1;
	m_sPortName = std::string(m_pData->begin() + static_cast<int>(offset),
							  m_pData->begin() + static_cast<int>(offset) +
								  m_iPortNameLength);
	offset += static_cast<unsigned long>(m_iPortNameLength);
	m_iDeviceSpecficPortNumer = m_pData->at(offset);
	offset += 1;
	unsigned char c;
	switch (m_audioPortType) {
	case APT_USB_DEVICE:
		c = m_pData->at(offset);
		m_bPortSupportsIOS = (c & 1) != 0;
		m_bPortSupportsPD = (c & 2) != 0;
		m_bPortIOSEnabled = (c & 4) != 0;
		m_bPortPCEnabled = (c & 8) != 0;
		break;
	case APT_USB_HOST:
	case APT_ETHERNET:
		c = m_pData->at(offset);
		m_iJackSpecificDeviceNumber = c;
		break;
	default:
		break;
	}
}

std::vector<unsigned char> *RetSetAudioPortParm::m_pGetMessageData() {
	return new BYTE_VECTOR();
}

std::string
RetSetAudioPortParm::getAudioPortTypeName(AudioPortType audioPortType) {
	switch (audioPortType) {
	case APT_NONE:
		return "none";
	case APT_USB_DEVICE:
		return "USB Device";
	case APT_USB_HOST:
		return "USB Host";
	case APT_ETHERNET:
		return "Network Device";
	case APT_ANALOGUE:
		return "Analogue";
	}
	return "none";
}

std::string RetSetAudioPortParm::getAudioPortTypeName() {
	return getAudioPortTypeName(m_audioPortType);
}

bool RetSetAudioPortParm::getPortNameWritable() {
	return m_iPortNameLength > 0;
}

AudioPortConfiguration *RetSetAudioPortParm::getCurrentAudioConfiguration() {
	unsigned int activeAudioConfiguration =
		this->m_pDevice->getAudioGlobalParm()
			->getNumberOfActiveAudioConfiguration();
	if (activeAudioConfiguration < sizeof(m_pAudioPortConfigurations))
		return m_pAudioPortConfigurations[activeAudioConfiguration];
	return nullptr;
}

std::string RetSetAudioPortParm::getCurrentAudioConfigurationString() {
	return m_pDevice->getAudioGlobalParm()->getAudioConfigurationString();
}

AudioPortType RetSetAudioPortParm::getAudioPortType() const {
	return m_audioPortType;
}

std::string RetSetAudioPortParm::getPortName() const { return m_sPortName; }

void RetSetAudioPortParm::setPortName(const std::string &sPortName) {
	m_iPortNameLength = static_cast<int>(sPortName.length());
	m_sPortName = sPortName;
}

bool RetSetAudioPortParm::getPortSupportsIOS() const {
	return m_bPortSupportsIOS;
}

bool RetSetAudioPortParm::getPortSupportsPC() const {
	return m_bPortSupportsPD;
}

bool RetSetAudioPortParm::getPortIOSEnabled() const {
	return m_bPortIOSEnabled;
}

void RetSetAudioPortParm::setPortIOSEnabled(bool bPortIOSEnabled) {
	m_bPortIOSEnabled = bPortIOSEnabled;
}

bool RetSetAudioPortParm::getPortPCEnabled() const { return m_bPortPCEnabled; }

void RetSetAudioPortParm::setPortPCEnabled(bool bPortPCEnabled) {
	m_bPortPCEnabled = bPortPCEnabled;
}

int RetSetAudioPortParm::getDeviceSpecficPortNumer() const {
	return m_iDeviceSpecficPortNumer;
}

int RetSetAudioPortParm::getJackSpecificDeviceNumber() const {
	return m_iJackSpecificDeviceNumber;
}

int RetSetAudioPortParm::getPortId() const { return m_iPortId; }

int RetSetAudioPortParm::getMaxPortNameLength() const {
	return m_iMaxPortNameLength;
}

int RetSetAudioPortParm::getInputChannels() const { return m_iInputChannels; }

void RetSetAudioPortParm::setInputChannels(int iInputChannels) {
	m_iInputChannels = iInputChannels;
}

int RetSetAudioPortParm::getOutputChannels() const { return m_iOutputChannels; }

void RetSetAudioPortParm::setOutputChannels(int iOutputChannels) {
	m_iOutputChannels = iOutputChannels;
}

int RetSetAudioPortParm::getNumberOfPortConfigurationBlocks() const {
	return m_iNumberOfPortConfigurationBlocks;
}
