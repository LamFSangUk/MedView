#include "volume.h"
#include "gdcmScanner.h"
#include "gdcmImageReader.h"
#include "gdcmPixmap.h"


namespace vdcm {
	Volume* read(const char* dirname) {

		// Directory 
		gdcm::Directory dir;
		dir.Load(dirname, false);

		// File List of Directory
		gdcm::Directory::FilenamesType files = dir.GetFilenames();

		if (files.size() == 0) {
			throw std::runtime_error("ERROR! Empty dir\n");
		}

		// For debug
		printf("files %d\n", files.size());

		// Read multi-frame DICOMs
		Volume *volume = new Volume();

		gdcm::ImageReader reader;
		for (auto filename : files) {
			
			// Read DICOM file
			reader.SetFileName(filename.c_str());
			if (!reader.Read()) {
				throw std::runtime_error("ERROR! None file\n");
			}

			const gdcm::Pixmap &image = reader.GetPixmap();
			int length = image.GetBufferLength();
			
			uint16_t* buffer = new uint16_t[length/2];
			char* temp_buffer = new char[length];
			image.GetBuffer(temp_buffer);

			for (int i = 0; i < length; i+=2) {
				uint16_t val = temp_buffer[i] | (uint16_t)temp_buffer[i+1] << 8;
				buffer[i/2] = val;
			}
			
			std::vector<uint16_t> slice(buffer, buffer + length/2);

			volume->m_volume.push_back(slice);
		}

		return volume;
	}

	Volume::Volume() {

	}

	std::vector<uint16_t> Volume::getAxialSlice(int idx) {
		printf("%d\n", m_volume.size());
		return m_volume[idx];
	}
}