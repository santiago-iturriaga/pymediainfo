/*
 * UDFReader.cpp
 *
 *  Created on: Jun 23, 2009
 *      Author: santiago
 *
 *  Clase encargada de obtener información de un volúmen UDF. La clase se encuentra
 *  bastante incompleta e implementa la correcta obtención de una cantidad mínima de
 *  informacón desde el volúmen.
 *
 *  Practicamente lo unico que se obtiene es el Label del volúmen.
 *
 */

#include "UDFReader.h"

#include <fcntl.h>
using namespace std;

#include "Utils.h"

#define SECTOR_SIZE									2048
#define AVDP_SECTOR									256

#define Primary_Volume_Descriptor_Id				1
#define Anchor_Volume_Descriptor_Pointer_Id			2
#define Volume_Descriptor_Pointer_Id				3
#define Implementation_Use_Volume_Descriptor_Id		4
#define Partition_Descriptor_Id						5
#define Logical_Volume_Descriptor_Id				6
#define Unallocated_Space_Descriptor_Id				7
#define Terminating_Descriptor_Id					8
#define Logical_Volume_Integrity_Descriptor_Id		9

#define Descriptor_Size								512

namespace imediafinder {

UDFReader::UDFReader(string dev_name) {
	this->_dev_name = dev_name;
	this->_label = "";
}

UDFReader::~UDFReader() {
	// TODO Auto-generated destructor stub
}

/*
 * Obtiene del Device Name del volumen UDF (p.ej.: /dev/sr0).
 */
string UDFReader::GetDeviceName() {
	return this->_dev_name;
}

/*
 * Retorna el Label del volumen UDF.
 */
string UDFReader::GetLabel() {
	if (!this->_loaded)
		Load();
	return this->_label;
}

void UDFReader::Load() {
	if (!this->_loaded) {
		// Leo el AVDP
		AnchorVolumeDescriptorPointer *avdp;
		{
			int avdp_off = AVDP_SECTOR * SECTOR_SIZE;
			int avdp_size = sizeof(AnchorVolumeDescriptorPointer);

#ifdef __DEBUG__
			cout << "AnchorVolumeDescriptorPointer size: " << avdp_size << endl;
			cout << "AnchorVolumeDescriptorPointer offset: " << avdp_off << endl;
#endif

			avdp = new AnchorVolumeDescriptorPointer;

			int avdp_fd = open(this->_dev_name.c_str(), O_RDONLY);
			if (avdp_fd != -1) {
				int status = lseek(avdp_fd, avdp_off, SEEK_SET);
				if (status != -1) {
					read(avdp_fd, avdp, avdp_size);
				}

				close(avdp_fd);
			}
		}

#ifdef __DEBUG__
		cout << "MainVolumeDescriptorSequenceExtent: " << endl;
		cout << "Length: ";
		cout << avdp->MainVolumeDescriptorSequenceExtent.Length;
		cout << endl;
		cout << "Locator: ";
		cout << avdp->MainVolumeDescriptorSequenceExtent.Location;
		cout << endl;

		cout << "ReserveVolumeDescriptorSequenceExtent: " << endl;
		cout << "Length: ";
		cout << avdp->ReserveVolumeDescriptorSequenceExtent.Length;
		cout << endl;
		cout << "Locator: ";
		cout << avdp->ReserveVolumeDescriptorSequenceExtent.Location;
		cout << endl;
#endif

		int32_t currentLocation = avdp->MainVolumeDescriptorSequenceExtent.Location
				* SECTOR_SIZE;
		int currentTagId = this->GetVolumeDescriptorTagId(currentLocation);
		delete avdp;

		bool finished = false;
		while ((currentTagId != Terminating_Descriptor_Id) && (!finished)) {
#ifdef __DEBUG__
			cout << endl << "=============> TagID: " << currentTagId << endl;
#endif

			if (currentTagId == Primary_Volume_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Logical_Volume_Descriptor_Id..." << endl;
#endif
				PrimaryVolumeDescriptor *pvd = this->GetPrimaryVolumeDescriptor(currentLocation);

				/* Obtengo la informacion */
				this->_label = Utils::CleanString(pvd->VolumeIdentifier, 32);

				delete pvd;
				currentLocation += SECTOR_SIZE;
			} else if (currentTagId == Implementation_Use_Volume_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Implementation_Use_Volume_Descriptor_Id... ignoring..."
						<< endl;
#endif
				currentLocation += SECTOR_SIZE;
			} else if (currentTagId == Partition_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Partition_Descriptor_Id... ignoring..." << endl;
#endif
				currentLocation += SECTOR_SIZE;
			} else if (currentTagId == Logical_Volume_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Logical_Volume_Descriptor_Id..." << endl;
#endif
				LogicalVolumeDescriptor *lvd = this->GetLogicalVolumeDescriptor(currentLocation);

				/* Obtengo la informacion */
				/* ¿? */

				delete lvd;
				currentLocation += SECTOR_SIZE;
			} else if (currentTagId == Unallocated_Space_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Unallocated_Space_Descriptor_Id... aborting..." << endl;
#endif
				return;
			} else if (currentTagId == Logical_Volume_Integrity_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Logical_Volume_Integrity_Descriptor_Id... ignoring..."
						<< endl;
#endif
				currentLocation += SECTOR_SIZE;
			} else if (currentTagId == Terminating_Descriptor_Id) {
#ifdef __DEBUG__
				cout << "Terminating_Descriptor_Id... aborting..." << endl;
#endif
				return;
			} else if (currentTagId == Volume_Descriptor_Pointer_Id) {
#ifdef __DEBUG__
				cout << "Volume_Descriptor_Pointer_Id..." << endl;
#endif

				VolumeDescriptorPointer *vdp = this->GetVolumeDescriptorPointer(currentLocation);
				currentLocation = vdp->Next_Volume_Descriptor_Sequence_Extent.Location * SECTOR_SIZE;
			}

			currentTagId = this->GetVolumeDescriptorTagId(currentLocation);
		}
	}
}

int UDFReader::GetVolumeDescriptorTagId(uint32_t location_offset) {
	// Leo el Tag
	tag *tag_instance;
	{
		uint32_t tag_size = sizeof(extent_ad);

#ifdef __DEBUG__
		cout << "extent_ad size: " << tag_size << endl;
		cout << "extent_ad offset: " << location_offset << endl;
#endif

		tag_instance = new tag;

		int tag_fd = open(this->_dev_name.c_str(), O_RDONLY);
		if (tag_fd != -1) {
			int status = lseek(tag_fd, location_offset, SEEK_SET);
			if (status != -1) {
				read(tag_fd, tag_instance, tag_size);
			}

			close(tag_fd);
		}
	}

	int16_t tagId = tag_instance->TagIdentifier;
	delete tag_instance;

	return tagId;
}

PrimaryVolumeDescriptor* UDFReader::GetPrimaryVolumeDescriptor(
		uint32_t location_offset) {
	// Leo el MVD
	PrimaryVolumeDescriptor *pvd;
	{
		uint32_t pvd_offset = location_offset;
		uint32_t pvd_size = sizeof(PrimaryVolumeDescriptor);

#ifdef __DEBUG__
		cout << endl;
		cout << "PrimaryVolumeDescriptor offset: " << pvd_offset << endl;
		cout << "PrimaryVolumeDescriptor size: " << pvd_size << endl;
#endif

		pvd = new PrimaryVolumeDescriptor;

		int pvd_fd = open(this->_dev_name.c_str(), O_RDONLY);
		if (pvd_fd != -1) {
			int status = lseek(pvd_fd, pvd_offset, SEEK_SET);
			if (status != -1)
				read(pvd_fd, pvd, pvd_size);

			close(pvd_fd);
		}
	}

	if (pvd->DescriptorTag.TagIdentifier == Primary_Volume_Descriptor_Id) {
		// Ok, esto es un Primary volume descriptor

#ifdef __DEBUG__
		cout << endl;
		cout << "Primary Volume Descriptor" << endl;
		cout << "TagIdentifier: " << pvd->DescriptorTag.TagIdentifier << endl;
		cout << "DescriptorVersion: " << pvd->DescriptorTag.DescriptorVersion
				<< endl;
		cout << "TagSerialNumber: " << pvd->DescriptorTag.TagSerialNumber
				<< endl;
		cout << "DescriptorCRC: " << pvd->DescriptorTag.DescriptorCRC << endl;
		cout << "DescriptorCRCLength: "
				<< pvd->DescriptorTag.DescriptorCRCLength << endl;
		cout << "TagLocation: " << pvd->DescriptorTag.TagLocation << endl;

		cout << "VolumeIdentifier: "
				<< Utils::CleanString(pvd->VolumeIdentifier, 32) << endl;
		cout << "VolumeSetIdentifier: " << Utils::CleanString(
				pvd->VolumeSetIdentifier, 128) << endl;
#endif

		return pvd;
	} else {
		delete pvd;

		return NULL;
	}
}

VolumeDescriptorPointer* UDFReader::GetVolumeDescriptorPointer(uint32_t location_offset) {
	// Leo el VDP
	VolumeDescriptorPointer *vdp;
	{
		uint32_t vdp_offset = location_offset;
		uint32_t vdp_size = sizeof(VolumeDescriptorPointer);

#ifdef __DEBUG__
		cout << endl;
		cout << "VolumeDescriptorPointer offset: " << vdp_offset << endl;
		cout << "VolumeDescriptorPointer size: " << vdp_size << endl;
#endif

		vdp = new VolumeDescriptorPointer;

		int vdp_fd = open(this->_dev_name.c_str(), O_RDONLY);
		if (vdp_fd != -1) {
			int status = lseek(vdp_fd, vdp_offset, SEEK_SET);
			if (status != -1)
				read(vdp_fd, vdp, vdp_size);

			close(vdp_fd);
		}
	}

	if (vdp->DescriptorTag.TagIdentifier == Volume_Descriptor_Pointer_Id) {
		// Ok, esto es un Volume descriptor pointer

#ifdef __DEBUG__
		cout << endl;
		cout << "VolumeDescriptorPointer" << endl;
		cout << "TagIdentifier: " << vdp->DescriptorTag.TagIdentifier << endl;
		cout << "DescriptorVersion: " << vdp->DescriptorTag.DescriptorVersion
				<< endl;
		cout << "TagSerialNumber: " << vdp->DescriptorTag.TagSerialNumber
				<< endl;
		cout << "DescriptorCRC: " << vdp->DescriptorTag.DescriptorCRC << endl;
		cout << "DescriptorCRCLength: "
				<< vdp->DescriptorTag.DescriptorCRCLength << endl;
		cout << "TagLocation: " << vdp->DescriptorTag.TagLocation << endl;
#endif

		return vdp;
	} else {
#ifdef __DEBUG__
		cout << endl;
		cout << "VolumeDescriptorPointer" << endl;
		cout << "TagIdentifier: " << vdp->DescriptorTag.TagIdentifier << endl;
#endif

		delete vdp;

		return NULL;
	}
}

LogicalVolumeDescriptor* UDFReader::GetLogicalVolumeDescriptor(uint32_t location_offset) {
	// Leo el LVD
	LogicalVolumeDescriptor *lvd;
	{
		uint32_t lvd_offset = location_offset;
		uint32_t lvd_size = sizeof(LogicalVolumeDescriptor);

#ifdef __DEBUG__
		cout << endl;
		cout << "LogicalVolumeDescriptor offset: " << lvd_offset << endl;
		cout << "LogicalVolumeDescriptor size: " << lvd_size << endl;

#endif

		lvd = new LogicalVolumeDescriptor;

		int lvd_fd = open(this->_dev_name.c_str(), O_RDONLY);
		if (lvd_fd != -1) {
			int status = lseek(lvd_fd, lvd_offset, SEEK_SET);
			if (status != -1)
				read(lvd_fd, lvd, lvd_size);

			close(lvd_fd);
		}
	}

	if (lvd->DescriptorTag.TagIdentifier == Logical_Volume_Descriptor_Id) {
		// Ok, esto es un Volume descriptor pointer

#ifdef __DEBUG__
		cout << endl;
		cout << "LogicalVolumeDescriptor" << endl;
		cout << "TagIdentifier: " << lvd->DescriptorTag.TagIdentifier << endl;
		cout << "DescriptorVersion: " << lvd->DescriptorTag.DescriptorVersion
				<< endl;
		cout << "TagSerialNumber: " << lvd->DescriptorTag.TagSerialNumber
				<< endl;
		cout << "DescriptorCRC: " << lvd->DescriptorTag.DescriptorCRC << endl;
		cout << "DescriptorCRCLength: "
				<< lvd->DescriptorTag.DescriptorCRCLength << endl;
		cout << "TagLocation: " << lvd->DescriptorTag.TagLocation << endl;
		cout << "LogicalVolumeIdentifier: " << Utils::CleanString(
				lvd->LogicalVolumeIdentifier, 128) << endl;
#endif

		return lvd;
	} else {
#ifdef __DEBUG__
		cout << endl;
		cout << "LogicalVolumeDescriptor" << endl;
		cout << "TagIdentifier: " << lvd->DescriptorTag.TagIdentifier << endl;
#endif

		delete lvd;

		return NULL;
	}
}

}
