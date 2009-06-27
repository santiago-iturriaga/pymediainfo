/*
 * UDFReader.h
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 */

#ifndef UDFREADER_H_
#define UDFREADER_H_

#include <iostream>
#include <stdint.h>

namespace imediafinder {

struct charspec {
	uint8_t CharacterSetType; //Uint8
	uint8_t CharacterSetInfo[63]; //byte
};

struct timestamp {
	uint16_t TypeAndTimezone[2]; //Uint16
	uint16_t Year[2]; //Uint16
	uint8_t Month; //Uint8
	uint8_t Day; //Uint8
	uint8_t Hour; //Uint8
	uint8_t Minute; //Uint8
	uint8_t Second; //Uint8
	uint8_t Centiseconds; //Uint8
	uint8_t HundredsofMicroseconds; //Uint8
	uint8_t Microseconds; //Uint8
};

struct tag {
	uint16_t TagIdentifier; //Uint16
	uint16_t DescriptorVersion; //Uint16
	uint8_t TagChecksum; //Uint8
	uint8_t Reserved; //byte
	uint16_t TagSerialNumber; //Uint16
	uint16_t DescriptorCRC; //Uint16
	uint16_t DescriptorCRCLength; //Uint16
	uint32_t TagLocation; //Uint32
};

struct extent_ad {
	uint32_t Length; //Uint32
	uint32_t Location; //Uint32
};

struct AnchorVolumeDescriptorPointer {
	struct tag DescriptorTag;
	struct extent_ad MainVolumeDescriptorSequenceExtent;
	struct extent_ad ReserveVolumeDescriptorSequenceExtent;
	uint8_t Reserved[480]; //byte
};

struct VolumeDescriptorPointer {
	struct tag DescriptorTag;
	uint32_t VolumeDescriptorSequenceNumber;
	extent_ad Next_Volume_Descriptor_Sequence_Extent;
	uint8_t Reserved[484]; //byte
};

struct PrimaryVolumeDescriptor {
	struct tag DescriptorTag;
	uint32_t VolumeDescriptorSequenceNumber; //Uint32
	uint32_t PrimaryVolumeDescriptorNumber; //Uint32
	char VolumeIdentifier[32]; //dstring
	uint16_t VolumeSequenceNumber; //Uint16
	uint16_t MaximumVolumeSequenceNumber; //Uint16
	uint16_t InterchangeLevel; //Uint16
	uint16_t MaximumInterchangeLevel; //Uint16
	uint32_t CharacterSetList; //Uint32
	uint32_t MaximumCharacterSetList; //Uint32
	char VolumeSetIdentifier[128]; //dstring
	struct charspec DescriptorCharacterSet;
	struct charspec ExplanatoryCharacterSet;
	struct extent_ad VolumeAbstract;
	struct extent_ad VolumeCopyrightNotice;
	/*
	 struct EntityID		ApplicationIdentifier;
	 struct timestamp	RecordingDateandTime;
	 struct EntityID		ImplementationIdentifier;
	 char				ImplementationUse[64];							//byte
	 char				PredecessorVolumeDescriptorSequenceLocation[4];	//Uint32
	 char				Flags[2];										//Uint16
	 char				Reserved[22];									//byte
	 */
};

struct LogicalVolumeDescriptor {
	struct tag DescriptorTag;
	uint32_t VolumeDescriptorSequenceNumber;
	struct charspec DescriptorCharacterSet;
	char LogicalVolumeIdentifier[128];
	uint32_t LogicalBlockSize;
	uint8_t DomainIdentifier[32]; //regid (1/7.4)
	uint8_t LogicalVolumeContentsUse[16]; //bytes
	uint32_t MapTableLength;
	uint32_t NumberOfPartitionMaps;
	uint8_t ImplementationIdentifier[32]; //regid (1/7.4)
	uint8_t ImplementationUse[128]; //bytes
	struct extent_ad IntegritySequenceExtent;
	//uint8_t PartitionMaps[??]; //bytes
};

class UDFReader {
public:
	UDFReader(std::string dev_name);
	virtual ~UDFReader();

	std::string GetDeviceName();
	std::string GetLabel();
private:
	bool _loaded;
	std::string _dev_name;
	std::string _label;

	void Load();

	int GetVolumeDescriptorTagId(uint32_t location_offset);
	PrimaryVolumeDescriptor* GetPrimaryVolumeDescriptor(uint32_t location_offset);
	VolumeDescriptorPointer* GetVolumeDescriptorPointer(uint32_t location_offset);
	LogicalVolumeDescriptor* GetLogicalVolumeDescriptor(uint32_t location_offset);
};

}

#endif /* UDFREADER_H_ */
