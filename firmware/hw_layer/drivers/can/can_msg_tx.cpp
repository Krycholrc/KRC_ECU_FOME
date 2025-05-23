/**
 * @file	can_msg_tx.cpp
 *
 * CAN message transmission
 * 
 * @date Mar 13, 2020
 * @author Matthew Kennedy, (c) 2012-2020
 */

#include "pch.h"

#include "can_msg_tx.h"

#include "can.h"

#if EFI_CAN_SUPPORT
/*static*/ CANDriver* CanTxMessage::s_devices[2] = {nullptr, nullptr};

/*static*/ void CanTxMessage::setDevice(CANDriver* device1, CANDriver* device2) {
	s_devices[0] = device1;
	s_devices[1] = device2;
}
#endif // EFI_CAN_SUPPORT

CanTxMessage::CanTxMessage(uint32_t eid, uint8_t dlc, CanBusIndex bus, bool isExtended)
	: m_busIndex(bus)
{
#if HAL_USE_CAN || EFI_UNIT_TEST
#ifndef STM32H7XX
	// ST bxCAN device
	m_frame.IDE = isExtended ? CAN_IDE_EXT : CAN_IDE_STD;
	m_frame.RTR = CAN_RTR_DATA;
#else /* if STM32H7XX */
	// Bosch M_CAN FDCAN device
	m_frame.common.XTD = isExtended;
	m_frame.common.RTR = 0;
#endif

	if (isExtended) {
		CAN_EID(m_frame) = eid;
	} else {
		CAN_SID(m_frame) = eid;
	}

	setDlc(dlc);

	setArrayValues(m_frame.data8, 0);
#endif // HAL_USE_CAN || EFI_UNIT_TEST
}

#if EFI_CAN_SUPPORT
CanTxMessage::~CanTxMessage() {
	size_t busIndex = static_cast<size_t>(m_busIndex);
	auto device = s_devices[busIndex];

	if (!device) {
		warning(ObdCode::CUSTOM_ERR_CAN_CONFIGURATION, "Send: CAN configuration issue %d", busIndex);
		return;
	}

	if (!engine->allowCanTx) {
		return;
	}

	if (engineConfiguration->verboseCan) {
		#ifndef STM32H7XX
			int id = (unsigned int)((m_frame.IDE == CAN_IDE_EXT) ? CAN_EID(m_frame) : CAN_SID(m_frame));
		#else
			int id = (unsigned int)(m_frame.common.XTD ? CAN_EID(m_frame) : CAN_SID(m_frame));
		#endif

		efiPrintf("CAN TX bus %d ID %x(%d) DLC %x: %x %x %x %x %x %x %x %x",
				busIndex,
				id, id,
				m_frame.DLC,
				m_frame.data8[0], m_frame.data8[1],
				m_frame.data8[2], m_frame.data8[3],
				m_frame.data8[4], m_frame.data8[5],
				m_frame.data8[6], m_frame.data8[7]);
	}

	// 100 ms timeout
	msg_t msg = canTransmit(device, CAN_ANY_MAILBOX, &m_frame, TIME_MS2I(100));
#if EFI_TUNER_STUDIO
	if (msg == MSG_OK) {
		engine->outputChannels.canWriteOk++;
	} else {
		engine->outputChannels.canWriteNotOk++;
	}
#endif // EFI_TUNER_STUDIO
}
#endif /* EFI_CAN_SUPPORT */

#if HAL_USE_CAN || EFI_UNIT_TEST
void CanTxMessage::setDlc(uint8_t dlc) {
	m_frame.DLC = dlc;
}

void CanTxMessage::setShortValue(uint16_t value, size_t offset) {
	m_frame.data8[offset] = value & 0xFF;
	m_frame.data8[offset + 1] = value >> 8;
}

void CanTxMessage::setBit(size_t byteIdx, size_t bitIdx) {
	m_frame.data8[byteIdx] |= 1 << bitIdx;
}

uint8_t& CanTxMessage::operator[](size_t index) {
	return m_frame.data8[index];
}

#endif // HAL_USE_CAN

#if EFI_UNIT_TEST

static ICanTransmitMock* mockCan;

CanTxMessage::~CanTxMessage() {
	if (mockCan) {
		mockCan->onTx(
			m_frame.SID, m_frame.DLC,
			m_frame.data8[0], m_frame.data8[1], m_frame.data8[2], m_frame.data8[3],
			m_frame.data8[4], m_frame.data8[5], m_frame.data8[6], m_frame.data8[7]
		);
	}
}

void setCanTxMockHandler(ICanTransmitMock* mock) {
	mockCan = mock;
}

#endif // EFI_UNIT_TEST
