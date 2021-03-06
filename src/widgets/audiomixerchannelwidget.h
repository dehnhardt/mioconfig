#ifndef AUDIOMIXERCHANNELWIDGET_H
#define AUDIOMIXERCHANNELWIDGET_H

#include <QFrame>
#include <QMenu>
#include <QObject>
#include <QTimer>
#include <QToolButton>

#include "../device.h"

namespace Ui {
class AudioChannelFeatureWidget;
}

class AudioMixerChannelWidget : public QFrame {
	Q_OBJECT
  public:
	AudioMixerChannelWidget(Device *device, AudioPortId portId,
							AudioChannelId channelNumber,
							pk::ChannelDirection channelDirection, QWidget *parent);

	AudioMixerChannelWidget(QWidget *parent);
	~AudioMixerChannelWidget();

	QFrame *getFirstConnectionFrame();
	QFrame *getSecondConnectionFrame();
	QToolButton *getConnectionButton();

	virtual void setMaster(bool isMaster, QString channel2Name) = 0;
	virtual void refreshStatus() = 0;
	virtual QString getChannelName();

	int getPanValue();
	void setPanValue(int value);
	int calculateBalance(int panL, int panR);
	int calculatePanL(int balance);
	int calculatePanR(int balance);
	bool master();
	bool slave();
	void setSlave(bool b_slave);

  protected:
	Ui::AudioChannelFeatureWidget *ui;
	QTimer *m_pUpdateTimer = nullptr;
	QToolButton *m_pBtnSelectConnection = nullptr;
	QMenu *m_pConnectionMenu = nullptr;
	pk::ChannelDirection m_channelDirection = pk::ChannelDirection::CD_NONE;
	Device *m_pDevice = nullptr;
	bool m_bChannelInit = false;
	AudioPortId m_iPortId = 0;
	pk::AudioPortType m_iPortType = pk::AudioPortType::APT_NONE;
	AudioChannelId m_iMixerChannelId = 0;
	bool m_bIsMaster = false;
	bool m_bIsSlave = false;
	unsigned int m_iPanSplit = 0;

	virtual void initControls();

  signals:
	void linkStatusChanged(AudioChannelId m_iMixerChannelId,
						   pk::ChannelDirection portDirection, bool status);
	void soloStatusChanged(bool status);
	void soloPFLStatusChanged(bool status);
	void invertStatusChanged(bool status);
	void muteStatusChanged(bool status);
	void volumeChanged(int volume);
	void panChanged(int trim);
	void channelConnectionChanged();

  private slots:
	virtual void audioChannelValueChanged() = 0;

  public slots:
	virtual void changeLinkStatus(bool enabled) = 0;
	void changeSoloStatus(bool enabled);
	void changePFLStatus(bool enabled);
	void changeInvertStatus(bool enabled);
	void changeMuteStatus(bool enabled);
	void changeVolume(int volume);
	void changePan(int pan);
	void changeMeterVolume(unsigned int channel, int value);
};

#endif // AUDIOMIXERCHANNELWIDGET_H
