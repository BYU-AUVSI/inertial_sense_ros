/*
MIT LICENSE

Copyright 2014 Inertial Sense, LLC - http://inertialsense.com

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ISComm.h"

#if defined(RTK_EMBEDDED)

#include "../../hdw-src//hw-libs/rtklib/src/rtklib.h"

#else

/**
* Calculate 24 bit crc used in formats like RTCM3 - note that no bounds checking is done on buffer
* @param buffer the buffer to calculate the CRC for
* @param len the number of bytes to calculate the CRC for
* @return the CRC value
*/
unsigned int calculate24BitCRCQ(unsigned char* buffer, unsigned int len)
{
	static const unsigned int TABLE_CRC24Q[] =
	{
		0x000000,0x864CFB,0x8AD50D,0x0C99F6,0x93E6E1,0x15AA1A,0x1933EC,0x9F7F17,
		0xA18139,0x27CDC2,0x2B5434,0xAD18CF,0x3267D8,0xB42B23,0xB8B2D5,0x3EFE2E,
		0xC54E89,0x430272,0x4F9B84,0xC9D77F,0x56A868,0xD0E493,0xDC7D65,0x5A319E,
		0x64CFB0,0xE2834B,0xEE1ABD,0x685646,0xF72951,0x7165AA,0x7DFC5C,0xFBB0A7,
		0x0CD1E9,0x8A9D12,0x8604E4,0x00481F,0x9F3708,0x197BF3,0x15E205,0x93AEFE,
		0xAD50D0,0x2B1C2B,0x2785DD,0xA1C926,0x3EB631,0xB8FACA,0xB4633C,0x322FC7,
		0xC99F60,0x4FD39B,0x434A6D,0xC50696,0x5A7981,0xDC357A,0xD0AC8C,0x56E077,
		0x681E59,0xEE52A2,0xE2CB54,0x6487AF,0xFBF8B8,0x7DB443,0x712DB5,0xF7614E,
		0x19A3D2,0x9FEF29,0x9376DF,0x153A24,0x8A4533,0x0C09C8,0x00903E,0x86DCC5,
		0xB822EB,0x3E6E10,0x32F7E6,0xB4BB1D,0x2BC40A,0xAD88F1,0xA11107,0x275DFC,
		0xDCED5B,0x5AA1A0,0x563856,0xD074AD,0x4F0BBA,0xC94741,0xC5DEB7,0x43924C,
		0x7D6C62,0xFB2099,0xF7B96F,0x71F594,0xEE8A83,0x68C678,0x645F8E,0xE21375,
		0x15723B,0x933EC0,0x9FA736,0x19EBCD,0x8694DA,0x00D821,0x0C41D7,0x8A0D2C,
		0xB4F302,0x32BFF9,0x3E260F,0xB86AF4,0x2715E3,0xA15918,0xADC0EE,0x2B8C15,
		0xD03CB2,0x567049,0x5AE9BF,0xDCA544,0x43DA53,0xC596A8,0xC90F5E,0x4F43A5,
		0x71BD8B,0xF7F170,0xFB6886,0x7D247D,0xE25B6A,0x641791,0x688E67,0xEEC29C,
		0x3347A4,0xB50B5F,0xB992A9,0x3FDE52,0xA0A145,0x26EDBE,0x2A7448,0xAC38B3,
		0x92C69D,0x148A66,0x181390,0x9E5F6B,0x01207C,0x876C87,0x8BF571,0x0DB98A,
		0xF6092D,0x7045D6,0x7CDC20,0xFA90DB,0x65EFCC,0xE3A337,0xEF3AC1,0x69763A,
		0x578814,0xD1C4EF,0xDD5D19,0x5B11E2,0xC46EF5,0x42220E,0x4EBBF8,0xC8F703,
		0x3F964D,0xB9DAB6,0xB54340,0x330FBB,0xAC70AC,0x2A3C57,0x26A5A1,0xA0E95A,
		0x9E1774,0x185B8F,0x14C279,0x928E82,0x0DF195,0x8BBD6E,0x872498,0x016863,
		0xFAD8C4,0x7C943F,0x700DC9,0xF64132,0x693E25,0xEF72DE,0xE3EB28,0x65A7D3,
		0x5B59FD,0xDD1506,0xD18CF0,0x57C00B,0xC8BF1C,0x4EF3E7,0x426A11,0xC426EA,
		0x2AE476,0xACA88D,0xA0317B,0x267D80,0xB90297,0x3F4E6C,0x33D79A,0xB59B61,
		0x8B654F,0x0D29B4,0x01B042,0x87FCB9,0x1883AE,0x9ECF55,0x9256A3,0x141A58,
		0xEFAAFF,0x69E604,0x657FF2,0xE33309,0x7C4C1E,0xFA00E5,0xF69913,0x70D5E8,
		0x4E2BC6,0xC8673D,0xC4FECB,0x42B230,0xDDCD27,0x5B81DC,0x57182A,0xD154D1,
		0x26359F,0xA07964,0xACE092,0x2AAC69,0xB5D37E,0x339F85,0x3F0673,0xB94A88,
		0x87B4A6,0x01F85D,0x0D61AB,0x8B2D50,0x145247,0x921EBC,0x9E874A,0x18CBB1,
		0xE37B16,0x6537ED,0x69AE1B,0xEFE2E0,0x709DF7,0xF6D10C,0xFA48FA,0x7C0401,
		0x42FA2F,0xC4B6D4,0xC82F22,0x4E63D9,0xD11CCE,0x575035,0x5BC9C3,0xDD8538
	};

	unsigned int crc = 0;
	for (uint32_t i = 0; i != len; i++)
	{
		crc = ((crc << 8) & 0xFFFFFF) ^ TABLE_CRC24Q[(crc >> 16) ^ buffer[i]];
	}
	return crc;
}

/**
* Retrieve the 32 bit unsigned integer value of the specified bits - note that no bounds checking is done on buffer
* @param buffer the buffer containing the bits
* @param pos the start bit position in buffer to read at
* @param len the number of bits to read
* @return the 32 bit unsigned integer value
*/
unsigned int getBitsAsUInt32(const unsigned char* buffer, unsigned int pos, unsigned int len)
{
	unsigned int bits = 0;
	for (unsigned int i = pos; i < pos + len; i++)
	{
		bits = (bits << 1) + ((buffer[i / 8] >> (7 - i % 8)) & 1u);
	}
	return bits;
}

#endif

const unsigned int g_validBaudRates[IS_BAUDRATE_COUNT] = { IS_BAUDRATE_3000000, IS_BAUDRATE_921600, IS_BAUDRATE_460800, IS_BAUDRATE_230400, IS_BAUDRATE_115200, IS_BAUDRATE_57600, IS_BAUDRATE_38400, IS_BAUDRATE_19200 };

// Replace special character with encoded equivalent and add to buffer
static uint8_t* encodeByteAddToBuffer(unsigned val, uint8_t* ptrDest)
{
	switch (val)
	{
	case PSC_ASCII_START_BYTE:
	case PSC_ASCII_END_BYTE:
	case PSC_START_BYTE:
	case PSC_END_BYTE:
	case PSC_RESERVED_KEY:
	case UBLOX_START_BYTE1:
	case RTCM3_START_BYTE:
		*ptrDest++ = PSC_RESERVED_KEY;
		*ptrDest++ = ~val;
		break;
	default:
		*ptrDest++ = val;
		break;
	}

	return ptrDest;
}

static int dataIdShouldSwap(uint32_t dataId)
{
	switch (dataId)
	{
	case DID_GPS1_VERSION:
	case DID_GPS2_VERSION:
		return 0;
	}
	return 1;
}

static void swapPacket(packet_t* pkt)
{
	if (pkt->hdr.flags & CM_PKT_FLAGS_RAW_DATA_NO_SWAP)
	{
		if ((pkt->hdr.pid == PID_DATA || pkt->hdr.pid == PID_SET_DATA) && pkt->body.size >= sizeof(p_data_hdr_t))
		{
			// swap the data header only
			flipEndianess32(pkt->body.ptr, sizeof(p_data_hdr_t));
		}
	}
	else if (pkt->body.size < sizeof(p_data_hdr_t) || (pkt->hdr.pid != PID_DATA && pkt->hdr.pid != PID_SET_DATA))
	{
		// swap entire packet, not a data packet
		flipEndianess32(pkt->body.ptr, pkt->body.size);
	}
	else
	{
		// swap header
		flipEndianess32(pkt->body.ptr, sizeof(p_data_hdr_t));

		// get header
		p_data_hdr_t* dataHdr = (p_data_hdr_t*)pkt->body.ptr;

		// if dev_info_t, swap only the uint32 fields, this data structure is handled special as it contains char[] arrays and uint32_t in the same struct
		if (dataHdr->id == DID_DEV_INFO && pkt->body.size == sizeof(p_data_hdr_t) + sizeof(dev_info_t))
		{
			// swap only the pieces that need swapping
			dev_info_t* devInfo = (dev_info_t*)(pkt->body.ptr + sizeof(p_data_hdr_t));
			devInfo->buildNumber = SWAP32(devInfo->buildNumber);
			devInfo->repoRevision = SWAP32(devInfo->repoRevision);
			devInfo->serialNumber = SWAP32(devInfo->serialNumber);
		}
		else if (dataIdShouldSwap(dataHdr->id))
		{
			// swap entire packet body
			flipEndianess32(pkt->body.ptr + sizeof(p_data_hdr_t), pkt->body.size - sizeof(p_data_hdr_t));

			// flip doubles
			uint16_t* offsets;
			uint16_t offsetsLength;
			uint8_t* dataBuf = pkt->body.ptr + sizeof(p_data_hdr_t);

			// flip doubles back if needed
			if ((offsets = getDoubleOffsets(dataHdr->id, &offsetsLength)))
			{
				flipDoubles(dataBuf, dataHdr->size, dataHdr->offset, offsets, offsetsLength);
			}

			// flip strings back if needed
			if ((offsets = getStringOffsetsLengths(dataHdr->id, &offsetsLength)))
			{
				flipStrings(dataBuf, dataHdr->size, dataHdr->offset, offsets, offsetsLength);
			}
		}
	}
}

void is_comm_init(is_comm_instance_t* instance)
{
	instance->errors = 0;
	instance->counter = 0;
	instance->bufferPtr = 0;
	instance->hasStartByte = 0;
	instance->bufferPtrEnd = instance->buffer + instance->bufferSize;
	instance->externalDataIdentifier = 0;
}

static uint32_t processInertialSenseByte(is_comm_instance_t* instance, uint8_t byte)
{
	// append byte
	*instance->bufferPtr++ = byte;

	uint32_t dataId = DID_NULL;

	// if we appended the end byte, decode the packet
	if (byte == PSC_END_BYTE)
	{
		// reset position to beginning
		instance->hasStartByte = 0;

		// create a packet to decode into
		packet_t packet;
		packet.body.ptr = instance->buffer;
		packet.body.size = instance->bufferSize;

		// decode the packet in place on top of the receive buffer to save memory
		if (is_decode_binary_packet(&packet, instance->buffer, (int)(instance->bufferPtr - instance->buffer)) == 0 && packet.hdr.pid == PID_DATA)
		{
			p_data_hdr_t* dataHdr = (p_data_hdr_t*)packet.body.ptr;
			dataId = dataHdr->id;
			uint32_t dataSize = dataHdr->size;
			uint32_t dataOffset = dataHdr->offset;
			if (dataId > DID_NULL && dataId < DID_COUNT && dataSize > 0 && dataOffset + dataSize <= MAX_DATASET_SIZE)
			{
				// copy over the actual data structure to the start of the buffer
				memmove(instance->buffer + dataOffset, packet.body.ptr + sizeof(p_data_hdr_t), dataSize);
			}
			else
			{
				// corrupt data
				dataId = DID_NULL;
				instance->errors++;
			}
		}
		else
		{
			// corrupt data
			instance->errors++;
		}
	}

	// return data id, this will be a valid data id if the packet size and offset look good, otherwise DID_NULL
	return dataId;
}

static uint32_t processAsciiByte(is_comm_instance_t* instance, uint8_t byte)
{
	// append byte
	*instance->bufferPtr++ = byte;

	if (byte == PSC_ASCII_END_BYTE)
	{
		// calculate checksum, if pass return special data id
		instance->hasStartByte = 0;
		if (instance->bufferPtr - instance->buffer > 7)
		{
			// parse out checksum, put in temp null terminator
			uint8_t tmp = *(instance->bufferPtr - 2);
			*(instance->bufferPtr - 2) = 0;
			int actualCheckSum = strtol(instance->bufferPtr - 4, 0, 16);
			*(instance->bufferPtr - 2) = tmp;
			int dataCheckSum = 0;
			for (uint8_t* ptr = instance->buffer + 1, *ptrEnd = instance->bufferPtr - 5; ptr < ptrEnd; ptr++)
			{
				dataCheckSum ^= (int)*ptr;
			}
			if (actualCheckSum == dataCheckSum)
			{
				// valid ASCII Data
				instance->externalDataIdentifier = EXTERNAL_DATA_ID_ASCII;
				return _DID_EXTERNAL;
			}
			else
			{
				// checksum failure
				instance->errors++;
			}
		}
		else
		{
			// corrupt data
			instance->errors++;
		}
	}

	return DID_NULL;
}

static uint32_t processUbloxByte(is_comm_instance_t* instance, uint8_t byte)
{
	// append byte
	*instance->bufferPtr++ = byte;

	switch (instance->externalParseState)
	{
	case 1: // preamble 2
		if (byte != UBLOX_START_BYTE2)
		{
			// corrupt data
			instance->errors++;
			instance->hasStartByte = 0;
			return DID_NULL;
		}
	case 2: // class id
	case 3: // message id
	case 4: // length byte 1
	case 0:
		instance->externalParseState++;
		break;

	case 5: // length byte 2
	{
		uint16_t len = BE_SWAP16(*(uint16_t*)(instance->bufferPtr - 2));

		// if length is greater than available buffer, we cannot parse this ublox packet - ublox header is 6 bytes
		if (len > instance->bufferSize - 6)
		{
			instance->errors++;
			instance->hasStartByte = 0;
			return DID_NULL;
		}
		instance->externalParseState = -(len + 2);
	} break;
	default:
		if (++instance->externalParseState == 0)
		{
			// end of ublox packet, if checksum passes, send the external id
			instance->hasStartByte = 0;
			uint8_t actualChecksum1 = *(instance->bufferPtr - 2);
			uint8_t actualChecksum2 = *(instance->bufferPtr - 1);
			uint8_t calcChecksum1 = 0;
			uint8_t calcChecksum2 = 0;

			// calculate checksum, skipping the first two preamble bytes and the last two bytes which are the checksum
			for (uint8_t* ptr = instance->buffer + 2, *ptrEnd = instance->bufferPtr - 2; ptr < ptrEnd; ptr++)
			{
				calcChecksum1 += *ptr;
				calcChecksum2 += calcChecksum1;
			}
			if (actualChecksum1 == calcChecksum1 && actualChecksum2 == calcChecksum2)
			{
				// valid ublox packet
				instance->externalDataIdentifier = EXTERNAL_DATA_ID_UBLOX;
				return _DID_EXTERNAL;
			}
			else
			{
				// checksum failure
				instance->errors++;
			}
		}
	}

	return DID_NULL;
}

static uint32_t processRtcm3Byte(is_comm_instance_t* instance, uint8_t byte)
{
	// append byte
	*instance->bufferPtr++ = byte;

	switch (instance->externalParseState)
	{
	case 0:
	case 1:
		instance->externalParseState++;
		break;

	case 2:
	{
		int msgLength = getBitsAsUInt32(instance->buffer, 14, 10);

		// if message is too small or too big for rtcm3 or too big for buffer, fail
		if (msgLength > 1023 || msgLength > instance->bufferSize - 6)
		{
			// corrupt data
			instance->errors++;
			instance->hasStartByte = 0;
			return DID_NULL;
		}

		// parse the message plus 3 crc24 bytes
		instance->externalParseState = -(msgLength + 3);
	} break;
	default:
		if (++instance->externalParseState == 0)
		{
			// end of rtcm3 data, parse out
			instance->hasStartByte = 0;

			// get len without 3 crc bytes
            int lenWithoutCrc = (int)((instance->bufferPtr - instance->buffer) - 3);
			uint32_t actualCRC = calculate24BitCRCQ(instance->buffer, lenWithoutCrc);
			uint32_t correctCRC = getBitsAsUInt32(instance->buffer + lenWithoutCrc, 0, 24);

			// if valid CRC, forward mesasge to uINS
			if (actualCRC == correctCRC)
			{
				instance->externalDataIdentifier = EXTERNAL_DATA_ID_RTCM3;
				return _DID_EXTERNAL;
			}
			else
			{
				// checksum failure
				instance->errors++;
			}
		}
	}
	return DID_NULL;
}

uint32_t is_comm_parse(is_comm_instance_t* instance, uint8_t byte)
{
	if (instance == 0 || instance->buffer == 0 || instance->bufferSize < 16)
	{
		return DID_NULL;
	}
	else if (instance->bufferPtr == instance->bufferPtrEnd)
	{
		// end of buffer, reset state
		instance->hasStartByte = 0;
		instance->errors++;
	}

	// check for start byte if we haven't found it yet
	if (instance->hasStartByte == 0)
	{
		switch (byte)
		{
		case PSC_START_BYTE:
		case PSC_ASCII_START_BYTE:
		case UBLOX_START_BYTE1:
		case RTCM3_START_BYTE:
			// set flag and reset pos to beginning
			instance->hasStartByte = byte;
			instance->bufferPtr = instance->buffer;
			break;
		}
		instance->externalParseState = 0;
		instance->externalDataIdentifier = 0;
	}

	// if we have a start byte, process the data type
	switch (instance->hasStartByte)
	{
	case PSC_START_BYTE: return processInertialSenseByte(instance, byte);
	case PSC_ASCII_START_BYTE: return processAsciiByte(instance, byte);
	case UBLOX_START_BYTE1: return processUbloxByte(instance, byte);
	case RTCM3_START_BYTE: return processRtcm3Byte(instance, byte);
	}

	// no valid data yet...
	return DID_NULL;
}

int is_comm_get_data(is_comm_instance_t* instance, uint32_t dataId, uint32_t offset, uint32_t size, uint32_t periodMS)
{
	p_data_get_t request;

	request.id = dataId;
	request.offset = offset;
	request.size = size;
	request.bc_period_ms = periodMS;

	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_GET_DATA;
	hdr.counter = instance->counter++;

	return is_encode_binary_packet(&request, sizeof(request), &hdr, 0, instance->buffer, instance->bufferSize);
}

int is_comm_get_data_rmc(is_comm_instance_t* instance, uint64_t rmcBits)
{
	return 	is_comm_set_data(instance, DID_RMC, offsetof(rmc_t,bits), sizeof(uint64_t), (void*)&rmcBits);
}

int is_comm_set_data(is_comm_instance_t* instance, uint32_t dataId, uint32_t offset, uint32_t size, void* data)
{
	int dataSize = size + sizeof(p_data_hdr_t);
	uint8_t* toSend = (uint8_t*)MALLOC(dataSize);
	memcpy(toSend + sizeof(p_data_hdr_t), data, size);
	p_data_hdr_t* dataHdr = (p_data_hdr_t*)toSend;
	dataHdr->id = dataId;
	dataHdr->size = size;
	dataHdr->offset = offset;

	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_SET_DATA;
	hdr.counter = instance->counter++;

	int result = is_encode_binary_packet(toSend, dataSize, &hdr, 0, instance->buffer, instance->bufferSize);
	FREE(toSend);
	return result;
}

int is_comm_stop_broadcasts(is_comm_instance_t* instance)
{
	packet_hdr_t hdr;
	hdr.flags = 0;
	hdr.pid = PID_STOP_ALL_BROADCASTS;
	hdr.counter = instance->counter++;

	return is_encode_binary_packet(0, 0, &hdr, 0, instance->buffer, instance->bufferSize);
}

void is_decode_binary_packet_footer(packet_ftr_t* ftr, uint8_t* ptrSrc, uint8_t** ptrSrcEnd, uint32_t* checksum)
{
	int state = 0;
	uint8_t* currentPtr = (*ptrSrcEnd) - 1;
	*(uint32_t*)ftr = 0;

	// we need a state machine to ensure we don't overrun ptrSrcEnd
	while (state != 7 && currentPtr > ptrSrc)
	{
		switch (state)
		{
		case 0: // packet end byte
			ftr->stopByte = *currentPtr--;
			state = 1;
			break;

		case 1: // packet checksum 1
			ftr->cksum1 = *currentPtr--;
			state = (3 - (*currentPtr == PSC_RESERVED_KEY));
			break;

		case 2: // packet checksum 1 is encoded
			ftr->cksum1 = ~ftr->cksum1;
			currentPtr--;
			state = 3;
			break;

		case 3: // packet checksum 2
			ftr->cksum2 = *currentPtr--;
			state = (5 - (*currentPtr == PSC_RESERVED_KEY));
			break;

		case 4: // packet checksum 2 is encoded
			ftr->cksum2 = ~ftr->cksum2;
			currentPtr--;
			state = 5;
			break;

		case 5: // packet checksum 3
			ftr->cksum3 = *currentPtr;
			state = (7 - (*(currentPtr - 1) == PSC_RESERVED_KEY));
			break;

		case 6: // packet checksum 3 is encoded
			ftr->cksum3 = ~ftr->cksum3;
			currentPtr--;
			state = 7;
			break;

		default:
			break;
		}
	}
	*ptrSrcEnd = currentPtr;
	*checksum = ((uint32_t)ftr->cksum1) | (0x0000FF00 & ((uint32_t)ftr->cksum2 << 8)) | (0x00FF0000 & ((uint32_t)ftr->cksum3 << 16));
}

int is_decode_binary_packet_byte(uint8_t** _ptrSrc, uint8_t** _ptrDest, uint32_t* checksum, uint32_t shift)
{
	uint8_t* ptrSrc = *_ptrSrc;

	// packet id byte
	uint32_t val = *ptrSrc++;
	switch (val)
	{
	case PSC_ASCII_START_BYTE:
	case PSC_ASCII_END_BYTE:
	case PSC_START_BYTE:
	case PSC_END_BYTE:
	case RTCM3_START_BYTE:
	case UBLOX_START_BYTE1:
		// corrupt data
		return -1;

	case PSC_RESERVED_KEY:
		// skip special byte
		val = (~(*ptrSrc++) & 0x000000FF);
		// fall through intentional

	default:
		*checksum ^= (val << shift);
		*((*_ptrDest)++) = val;
	}
	*_ptrSrc = ptrSrc;

	return 0;
}

int is_encode_binary_packet(void* srcBuffer, unsigned int srcBufferLength, packet_hdr_t* hdr, uint8_t additionalPktFlags, void* encodedPacket, int encodedPacketLength)
{
	// Ensure data size is small enough, assuming packet size could double after encoding.
	if (srcBufferLength > MAX_PKT_BODY_SIZE)
	{
		return -1;
	}

	// Update Packet Counter
	uint8_t* ptrSrc;
	uint8_t* ptrSrcEnd;
	uint8_t* ptrDest = (uint8_t*)encodedPacket;
	uint8_t* ptrDestEnd = ptrDest + encodedPacketLength;
	uint32_t shifter = 0;
	uint32_t checkSumValue = CHECKSUM_SEED;
	uint32_t val;

	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	// Packet header -------------------------------------------------------------------------------------------
	*ptrDest++ = PSC_START_BYTE;

	// PID
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->pid;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= val;

	// Counter
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->counter;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= (val << 8);

	// Flags
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = hdr->flags | additionalPktFlags | CPU_IS_LITTLE_ENDIAN | CM_PKT_FLAGS_CHECKSUM_24_BIT;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);
	checkSumValue ^= (val << 16);

	// Packet body ----------------------------------------------------------------------------------------------
	if (srcBufferLength > 0)
	{
		ptrSrc = (uint8_t*)srcBuffer;
		ptrSrcEnd = ptrSrc + srcBufferLength;

		// copy body bytes, doing encoding and checksum
		while (ptrSrc != ptrSrcEnd && ptrDest < ptrDestEnd)
		{
			val = *ptrSrc++;
			checkSumValue ^= (val << shifter);

			// increment shifter
			shifter += 8;

			// reset if shifter equals 24
			shifter *= (shifter != 24);

			ptrDest = encodeByteAddToBuffer(val, ptrDest);
		}
	}

	// footer ----------------------------------------------------------------------------------------------------

	// checksum byte 3
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)((checkSumValue >> 16) & 0xFF);
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// checksum byte 2
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)(checkSumValue >> 8) & 0xFF;
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// checksum byte 1
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	val = (uint8_t)(checkSumValue & 0xFF);
	ptrDest = encodeByteAddToBuffer(val, ptrDest);

	// packet end byte
	if (ptrDest >= ptrDestEnd)
	{
		return -1;
	}
	*ptrDest++ = PSC_END_BYTE;
    return (int)(ptrDest - (uint8_t*)encodedPacket);
}

int is_decode_binary_packet(packet_t* pkt, unsigned char* pbuf, int pbufSize)
{
	// before we even get in this method, we can be assured that pbuf starts with a packet start byte and ends with a packet end byte
	// all other data can potentially be garbage
	if (pbufSize < 8)
	{
		// corrupt data
		return -1;
	}

	// decode the body and calculate checksum
	uint8_t* ptrSrc = pbuf;
	uint8_t* ptrDest = (uint8_t*)&pkt->hdr;
	uint8_t* ptrSrcEnd = pbuf + pbufSize;
	packet_ftr_t ftr;
	uint32_t actualCheckSumValue;

	is_decode_binary_packet_footer(&ftr, ptrSrc, &ptrSrcEnd, &actualCheckSumValue);
	uint32_t shifter = 0;
	uint32_t checkSumValue = CHECKSUM_SEED;

	// start packet byte
	*ptrDest++ = *ptrSrc++;

	if
	(
		// packet id
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 0) ||

		// packet counter
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 8) ||

		// packet flags
		is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, 16)
	)
	{
		return -1;
	}

	// decode the body - start shift 0
	ptrDest = pkt->body.ptr;
	while (ptrSrc < ptrSrcEnd)
	{
		if (is_decode_binary_packet_byte(&ptrSrc, &ptrDest, &checkSumValue, shifter))
		{
			return -1;
		}

		shifter += 8;

		// reset if shifter equals 24
		shifter *= (shifter != 24);
	}

	if (actualCheckSumValue != checkSumValue)
	{
		// corrupt data
		return -1;
	}

	pkt->body.size = (uint32_t)(ptrDest - pkt->body.ptr);
	if (pkt->body.size > MAX_PKT_BODY_SIZE)
	{
		return -1;
	}

	// if the endianness of the packet doesn't match our CPU, we need to flip the data so it will be correct for our CPU architecture
	else if (pkt->body.size != 0 && (pkt->hdr.flags & CM_PKT_FLAGS_ENDIANNESS_MASK) != CPU_IS_LITTLE_ENDIAN)
	{
		swapPacket(pkt);
	}

	return 0;
}
