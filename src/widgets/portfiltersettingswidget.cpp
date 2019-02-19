#include "portfiltersettingswidget.h"
#include "controls/midicontrollercombodelegate.h"
#include "ui_portfiltersettingswidget.h"

#include <QLabel>

PortFilterSettingsWidget::PortFilterSettingsWidget(QWidget *parent)
	: QWidget(parent), ui(new Ui::PortFilterSettingsWidget) {
	ui->setupUi(this);
	MidiControllerComboDelegate *comboDelegate =
		new MidiControllerComboDelegate();
	ui->m_pTblMidiControllerFilter->setItemDelegateForColumn(0, comboDelegate);
	ui->m_pTblMidiControllerFilter->horizontalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiControllerFilter->verticalHeader()->setSectionResizeMode(
		QHeaderView::ResizeToContents);
	ui->m_pTblMidiChannelMessageFilter->horizontalHeader()
		->setSectionResizeMode(QHeaderView::ResizeToContents);
}

PortFilterSettingsWidget::~PortFilterSettingsWidget() { delete ui; }

void PortFilterSettingsWidget::setMIDISystemMessagesFilter(
	MIDISystemMessagesFilter *midiSystemMessagesFilter) {
	ui->m_pCbFilterMidiActiveSensingEvents->setChecked(
		midiSystemMessagesFilter->filterMidiActiveSensingEvents);
	ui->m_pCbFilterMidiRealtimeEvents->setChecked(
		midiSystemMessagesFilter->filterMidiRealtimeEvents);
	ui->m_pCbFilterMidiResetEvents->setChecked(
		midiSystemMessagesFilter->filterMidiResetEvents);
	ui->m_pCbFilterMidiSongPositionPointer->setChecked(
		midiSystemMessagesFilter->filterMidiSongPositionPointerEvents);
	ui->m_pCbFilterMidiSongSelectEvents->setChecked(
		midiSystemMessagesFilter->filterMidiSongSelectEvents);
	ui->m_pCbFilterMidiSysexEvents->setChecked(
		midiSystemMessagesFilter->filterMidiSysExEvents);
	ui->m_pCbFilterMidiTimeCodeEvents->setChecked(
		midiSystemMessagesFilter->filterMidiTimeCodeEvents);
	ui->m_pCbFilterMidiTuneRequestEvents->setChecked(
		midiSystemMessagesFilter->filterMidiTuneRequestEvents);
}

void PortFilterSettingsWidget::setMidiControllerFilter(
	MIDIControllerFilter **midiControllerFilter) {

	ui->m_pTblMidiControllerFilter->setModel(
		new MidiControllerFilterTM(midiControllerFilter));
	for (int i = 0; i < static_cast<int>(sizeof(*midiControllerFilter)); i++) {
		QModelIndex modelIndex =
			ui->m_pTblMidiControllerFilter->model()->index(i, 0, QModelIndex());
		ui->m_pTblMidiControllerFilter->openPersistentEditor(modelIndex);
	}
}

void PortFilterSettingsWidget::setMidiChannelMessagesFilter(
	MIDIChannelMessagesFilter **midiChannelMessagesFilter) {
	ui->m_pTblMidiChannelMessageFilter->setModel(
		new MidiChannelMessagesFilterTM(midiChannelMessagesFilter));
}

MIDISystemMessagesFilter *
PortFilterSettingsWidget::getMIDISystemMessagesFilter() {
	MIDISystemMessagesFilter *midiSystemMessagesFilter =
		new MIDISystemMessagesFilter();
	midiSystemMessagesFilter->filterMidiActiveSensingEvents =
		ui->m_pCbFilterMidiActiveSensingEvents->isChecked();
	midiSystemMessagesFilter->filterMidiRealtimeEvents =
		ui->m_pCbFilterMidiRealtimeEvents->isChecked();
	midiSystemMessagesFilter->filterMidiResetEvents =
		ui->m_pCbFilterMidiResetEvents->isChecked();
	midiSystemMessagesFilter->filterMidiSongPositionPointerEvents =
		ui->m_pCbFilterMidiSongPositionPointer->isChecked();
	midiSystemMessagesFilter->filterMidiSongSelectEvents =
		ui->m_pCbFilterMidiSongSelectEvents->isChecked();
	midiSystemMessagesFilter->filterMidiSysExEvents =
		ui->m_pCbFilterMidiSysexEvents->isChecked();
	midiSystemMessagesFilter->filterMidiTimeCodeEvents =
		ui->m_pCbFilterMidiTimeCodeEvents->isChecked();
	midiSystemMessagesFilter->filterMidiTuneRequestEvents =
		ui->m_pCbFilterMidiTuneRequestEvents->isChecked();
	return midiSystemMessagesFilter;
}

MIDIControllerFilter **PortFilterSettingsWidget::getMidiControllerFiler() {
	return nullptr;
}

QTableWidgetItem *PortFilterSettingsWidget::getCheckStateItem(bool checked) {
	QTableWidgetItem *item = new QTableWidgetItem();
	item->setCheckState(checked ? Qt::Checked : Qt::Unchecked);
	return item;
}

// MidiChannelMessagesFilterTM

MidiControllerFilterTM::MidiControllerFilterTM(
	MIDIControllerFilter **midiControllerFilter) {
	this->m_ppMidiControllerFilter = midiControllerFilter;
}

int MidiControllerFilterTM::rowCount(const QModelIndex &parent
									 __attribute__((unused))) const {
	return sizeof(m_ppMidiControllerFilter);
}

int MidiControllerFilterTM::columnCount(const QModelIndex &) const {
	return MIDI_CHANNELS + 2;
}

QVariant MidiControllerFilterTM::data(const QModelIndex &index,
									  int role) const {

	MIDIControllerFilter *midiControllerFilter =
		m_ppMidiControllerFilter[index.row()];
	switch (role) {
	case Qt::DisplayRole:
		if (index.column() == 0) {
			return midiControllerFilter->midiContollerNumber;
		}
		break;
	case Qt::EditRole:
		if (index.column() == 0) {
			return midiControllerFilter->midiContollerNumber;
		}
		break;
	case Qt::CheckStateRole:
		if (index.column() == 1) {
			for (int channel = 0; channel < MIDI_CHANNELS; channel++) {
				if (!midiControllerFilter->channel[channel])
					return Qt::Unchecked;
			}
			return Qt::Checked;
		}
		if (index.column() > 1)
			return midiControllerFilter->channel[index.column() - 2]
					   ? Qt::Checked
					   : Qt::Unchecked;
		break;
	}
	return QVariant();
}

QVariant MidiControllerFilterTM::headerData(int section,
											Qt::Orientation orientation,
											int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString(tr("MIDI-Controller"));
		case 1:
			return QString(tr("Filter all Channels"));
		default:
			return QString::number(section - 1);
		}
	}
	return QVariant();
}

bool MidiControllerFilterTM::setData(const QModelIndex &index,
									 const QVariant &value, int role) {
	MIDIControllerFilter *midiControllerFilter =
		m_ppMidiControllerFilter[index.row()];
	if (role == Qt::EditRole) {
		if (!checkIndex(index))
			return false;
		if (index.column() == 0) {
			midiControllerFilter->midiContollerNumber = value.toUInt();
		}
	}
	if (role == Qt::CheckStateRole) {
		if (index.column() == 1) {
			for (int column = 0; column < MIDI_CHANNELS; column++) {
				midiControllerFilter->channel[column] = value.toBool();
			}
			emit dataChanged(createIndex(index.row(), 2),
							 createIndex(index.row(), MIDI_CHANNELS + 1));
		} else if (index.column() > 1) {
			midiControllerFilter->channel[index.column() - 2] = value.toBool();
			emit dataChanged(createIndex(index.row(), 1),
							 createIndex(index.row(), 1));
		}
		return true;
	}
	return false;
}

Qt::ItemFlags MidiControllerFilterTM::flags(const QModelIndex &index) const {
	if (index.column() > 0)
		return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			   Qt::ItemIsSelectable;
	return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}

MidiChannelMessagesFilterTM::MidiChannelMessagesFilterTM(
	MIDIChannelMessagesFilter **midiChannelMessagesFilter) {
	this->m_ppMidiChannelMessagesFilter = midiChannelMessagesFilter;
}

int MidiChannelMessagesFilterTM::rowCount(const QModelIndex &) const {
	return MIDI_CHANNELS + 1;
}

int MidiChannelMessagesFilterTM::columnCount(const QModelIndex &) const {
	return 7;
}

QVariant MidiChannelMessagesFilterTM::data(const QModelIndex &index,
										   int role) const {
	MIDIChannelMessagesFilter *midiChannelMessagesFilter =
		this->m_ppMidiChannelMessagesFilter[index.row()];
	switch (role) {
	case Qt::DisplayRole:
		if (index.column() == 0) {
			return QString::number(index.row());
		}
		break;
	case Qt::EditRole:
		if (index.column() == 0) {
			return QString::number(index.row());
		}
		break;
	case Qt::CheckStateRole:
		if (index.column() > 0) {
			switch (index.column()) {
			case 1:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiPitchBendEvents);
			case 2:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiChannelPressureEvents);
			case 3:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiProgrammChangeEvents);
			case 4:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiControlChangeEvents);
			case 5:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiPolyKeyPressureEvents);
			case 6:
				return boolToCheckState(
					midiChannelMessagesFilter->filterMidiNoteOnOffEvents);
			}
		}
	}
	return QVariant();
}

QVariant MidiChannelMessagesFilterTM::headerData(int section,
												 Qt::Orientation orientation,
												 int role) const {
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
		switch (section) {
		case 0:
			return QString(tr("MIDI-Channel"));
		case 1:
			return QString(tr("Pitch Bend"));
		case 2:
			return QString(tr("Channel Pressure"));
		case 3:
			return QString(tr("Program Change"));
		case 4:
			return QString(tr("Control Change"));
		case 5:
			return QString(tr("Poly Key Pressure"));
		case 6:
			return QString(tr("Note On / Note Off"));
		default:
			return QVariant();
		}
	}
	return QVariant();
}

bool MidiChannelMessagesFilterTM::setData(const QModelIndex &index,
										  const QVariant &value, int role) {
	MIDIChannelMessagesFilter *midiChannelMessagesFilter =
		this->m_ppMidiChannelMessagesFilter[index.row()];
	int column = index.column();
	if (role == Qt::CheckStateRole && column > 0 && column < 7)
		switch (column) {
		case 1:
			midiChannelMessagesFilter->filterMidiPitchBendEvents =
				value.toBool();
			break;
		case 2:
			midiChannelMessagesFilter->filterMidiChannelPressureEvents =
				value.toBool();
			break;
		case 3:
			midiChannelMessagesFilter->filterMidiProgrammChangeEvents =
				value.toBool();
			break;
		case 4:
			midiChannelMessagesFilter->filterMidiControlChangeEvents =
				value.toBool();
			break;
		case 5:
			midiChannelMessagesFilter->filterMidiPolyKeyPressureEvents =
				value.toBool();
			break;
		case 6:
			midiChannelMessagesFilter->filterMidiNoteOnOffEvents =
				value.toBool();
			break;
		}
	return false;
}

Qt::ItemFlags
MidiChannelMessagesFilterTM::flags(const QModelIndex &index) const {
	if (index.column() > 0)
		return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled |
			   Qt::ItemIsSelectable;
	return QAbstractTableModel::flags(index);
}

Qt::CheckState MidiChannelMessagesFilterTM::boolToCheckState(bool value) const {
	return value ? Qt::Checked : Qt::Unchecked;
}
