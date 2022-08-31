#include "RandomDeviceId.h"
#include <combaseapi.h>

RandomDeviceId::RandomDeviceId() : IModule(0, Category::MISC, "Random DeviceId Spoof Servers") {
}

RandomDeviceId::~RandomDeviceId() {
}

const char* RandomDeviceId::getModuleName() {
	return ("RandomDeviceId");
}

std::string RandomDeviceId::GetUUID() {
	constexpr size_t GUID_LEN = 64;
	char buffer[GUID_LEN];

	GUID guid;
	if (bool(CoCreateGuid(&guid)))
		return std::string();

	_snprintf(buffer, GUID_LEN,
		"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return buffer;
} //�������UUID