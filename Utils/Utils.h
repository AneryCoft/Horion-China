#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <random>
#include <vector>

#include <array>
#include <string>
#include <future>
#include <windows.h>
#include <Psapi.h>
//#include "xorstr.h"

static const char* const KeyNames[] = {
	"Unknown",
	"VK_LBUTTON",
	"VK_RBUTTON",
	"VK_CANCEL",
	"VK_MBUTTON",
	"VK_XBUTTON1",
	"VK_XBUTTON2",
	"Unknown",
	"VK_BACK",
	"TAB",
	"Unknown",
	"Unknown",
	"VK_CLEAR",
	"VK_RETURN",
	"Unknown",
	"Unknown",
	"Shift",
	"CTRL",
	"VK_MENU",
	"VK_PAUSE",
	"VK_CAPITAL",
	"VK_KANA",
	"Unknown",
	"VK_JUNJA",
	"VK_FINAL",
	"VK_KANJI",
	"Unknown",
	"VK_ESCAPE",
	"VK_CONVERT",
	"VK_NONCONVERT",
	"VK_ACCEPT",
	"VK_MODECHANGE",
	"VK_SPACE",
	"VK_PRIOR",
	"VK_NEXT",
	"VK_END",
	"VK_HOME",
	"VK_LEFT",
	"VK_UP",
	"VK_RIGHT",
	"VK_DOWN",
	"VK_SELECT",
	"VK_PRINT",
	"VK_EXECUTE",
	"VK_SNAPSHOT",
	"INSERT",
	"DELETE",
	"VK_HELP",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"A",
	"B",
	"C",
	"D",
	"E",
	"F",
	"G",
	"H",
	"I",
	"J",
	"K",
	"L",
	"M",
	"N",
	"O",
	"P",
	"Q",
	"R",
	"S",
	"T",
	"U",
	"V",
	"W",
	"X",
	"Y",
	"Z",
	"VK_LWIN",
	"VK_RWIN",
	"VK_APPS",
	"Unknown",
	"VK_SLEEP",
	"NUMPAD0",
	"NUMPAD1",
	"NUMPAD2",
	"NUMPAD3",
	"NUMPAD4",
	"NUMPAD5",
	"NUMPAD6",
	"NUMPAD7",
	"NUMPAD8",
	"NUMPAD9",
	"VK_MULTIPLY",
	"VK_ADD",
	"VK_SEPARATOR",
	"VK_SUBTRACT",
	"VK_DECIMAL",
	"VK_DIVIDE",
	"F1",
	"F2",
	"F3",
	"F4",
	"F5",
	"F6",
	"F7",
	"F8",
	"F9",
	"F10",
	"F11",
	"F12",
	"VK_F13",
	"VK_F14",
	"VK_F15",
	"VK_F16",
	"VK_F17",
	"VK_F18",
	"VK_F19",
	"VK_F20",
	"VK_F21",
	"VK_F22",
	"VK_F23",
	"VK_F24",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"VK_NUMLOCK",
	"VK_SCROLL",
	"VK_OEM_NEC_EQUAL",
	"VK_OEM_FJ_MASSHOU",
	"VK_OEM_FJ_TOUROKU",
	"VK_OEM_FJ_LOYA",
	"VK_OEM_FJ_ROYA",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"Unknown",
	"LeftShift",
	"RightShift",
	"VK_LCONTROL",
	"VK_RCONTROL",
	"VK_LMENU",
	"VK_RMENU",
	"Browser Back",
	"Browser Forward",
	"Browser Refresh",
	"Browser Stop",
	"Browser Search",
	"Browser Favourites",
	"Browser Home",
	"Volume Mute",
	"Volume Down",
	"Volume Up",
	"Media_Next",
	"Media_Prev",
	"Media_Stop",
	"Media_Pause",
	"Mail",
	"Media",
	"App1",
	"App2",
	"Unknown",
	"Unknown",
	"OEM_1",
	"PLUS",
	"COMMA",
	"MINUS",
	"DOT",
	"OEM_2",
	"OEM_3"};

#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BYTE(x) (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? x - '0' : 0))

static inline float ImFmod(float x, float y) { return fmodf(x, y); }
static inline float ImFabs(float x) { return fabsf(x); }
template <typename T>
static inline void ImSwap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

#ifdef JM_XORSTR_HPP
#define FindSignatureOld(szSignature) Utils::FindSignatureModule("Minecraft.Windows.exe", szSignature)

#define FindSignature(x) Utils::FindSignatureModuleNew(Utils::StrToSignatureArray<Utils::StrToSignatureSize(x)>(x))

#define FindSignatureAsync(x) std::async(std::launch::async | std::launch::deferred, Utils::FindSignatureModuleNew<Utils::StrToSignatureSize(x)>, Utils::StrToSignatureArray<Utils::StrToSignatureSize(x)>(x))

#else
#define FindSignatureOld(szSignature) Utils::FindSignatureModule("Minecraft.Windows.exe", szSignature)

#define FindSignature(x) Utils::FindSignatureModuleNew(Utils::StrToSignatureArray<Utils::StrToSignatureSize(x)>(x))

#define FindSignatureAsync(x) std::async(std::launch::async | std::launch::deferred, Utils::FindSignatureModuleNew<Utils::StrToSignatureSize(x)>, Utils::StrToSignatureArray<Utils::StrToSignatureSize(x)>(x))

#endif

struct vec3_ti;

class Utils {
public:

	static __forceinline unsigned __int64 rotBy(int in, unsigned int by) {
		auto mut = static_cast<unsigned __int64>(in);
		return ((mut & 0x7FFFFFui64) | ((static_cast<unsigned int>(in) >> 8u) & 0x800000u) /*copy sign bit*/) << by;
	}

	static size_t posToHash(const vec3_ti& pos);

	template <typename type>
	static inline auto lerp(type a, type b, float t) -> type {
		return a + t * (b - a);
	};

	static inline unsigned int getCrcHash(const char* str, int seed = 0) {
		static unsigned int crc32_lut[256] = {0};
		if (!crc32_lut[1]) {
			const unsigned int polynomial = 0xEDB88320;
			for (unsigned int i = 0; i < 256; i++) {
				unsigned int crc = i;
				for (unsigned int j = 0; j < 8; j++)
					crc = (crc >> 1) ^ (((unsigned int)(-((int)(crc & 1)))) & polynomial);
				crc32_lut[i] = crc;
			}
		}

		seed = ~seed;
		unsigned int crc = seed;
		const unsigned char* current = (const unsigned char*)str;
		{
			// Zero-terminated string
			while (unsigned char c = *current++) {
				// We support a syntax of "label###id" where only "###id" is included in the hash, and only "label" gets displayed.
				// Because this syntax is rarely used we are optimizing for the common case.
				// - If we reach ### in the string we discard the hash so far and reset to the seed.
				// - We don't do 'current += 2; continue;' after handling ### to keep the code smaller.
				if (c == '#' && current[0] == '#' && current[1] == '#')
					crc = seed;
				crc = (crc >> 8) ^ crc32_lut[(crc & 0xFF) ^ c];
			}
		}
		return ~crc;
	}

	static inline const char* getKeybindName(int keybind) {
		return KeyNames[keybind];
	};
	// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
	// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
	static inline void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v) {
		float K = 0.f;
		if (g < b) {
			ImSwap(g, b);
			K = -1.f;
		}
		if (r < g) {
			ImSwap(r, g);
			K = -2.f / 6.f - K;
		}

		const float chroma = r - (g < b ? g : b);
		out_h = ImFabs(K + (g - b) / (6.f * chroma + 1e-20f));
		out_s = chroma / (r + 1e-20f);
		out_v = r;
	};

	// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
	// also http://en.wikipedia.org/wiki/HSL_and_HSV
	static inline void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
		if (s == 0.0f) {
			// gray
			out_r = out_g = out_b = v;
			return;
		}

		h = ImFmod(h, 1.0f) / (60.0f / 360.0f);
		int i = (int)h;
		float f = h - (float)i;
		float p = v * (1.0f - s);
		float q = v * (1.0f - s * f);
		float t = v * (1.0f - s * (1.0f - f));

		switch (i) {
		case 0:
			out_r = v;
			out_g = t;
			out_b = p;
			break;
		case 1:
			out_r = q;
			out_g = v;
			out_b = p;
			break;
		case 2:
			out_r = p;
			out_g = v;
			out_b = t;
			break;
		case 3:
			out_r = p;
			out_g = q;
			out_b = v;
			break;
		case 4:
			out_r = t;
			out_g = p;
			out_b = v;
			break;
		case 5:
		default:
			out_r = v;
			out_g = p;
			out_b = q;
			break;
		}
	};

	template <unsigned int IIdx, typename TRet, typename... TArgs>
	static inline auto CallVFunc(void* thisptr, TArgs... argList) -> TRet {
		//if (thisptr == nullptr)
		//return nullptr;
		using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, argList...);
	}

	template < typename ret>
	static inline auto FuncFromSigOffset(uintptr_t sig, int offset) -> ret {
		return reinterpret_cast<ret>(sig + offset + 4 + *reinterpret_cast<int*>(sig + offset));
	}

	// https://stackoverflow.com/a/34571089
	static std::string base64_encode(const std::string& in) {
		std::string out;

		int val = 0, valb = -6;
		for (unsigned char c : in) {
			val = (val << 8) + c;
			valb += 8;
			while (valb >= 0) {
				out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
				valb -= 6;
			}
		}
		if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
		while (out.size() % 4) out.push_back('=');
		return out;
	}

	static std::string base64_decode(const std::string& in) {
		std::string out;

		std::vector<int> T(256, -1);
		for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

		int val = 0, valb = -8;
		for (unsigned char c : in) {
			if (T[c] == -1) break;
			val = (val << 6) + T[c];
			valb += 6;
			if (valb >= 0) {
				out.push_back(char((val >> valb) & 0xFF));
				valb -= 8;
			}
		}
		return out;
	}

	static inline std::string randomString(const int size) {
		std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		std::random_device rd;
		std::mt19937 generator(rd());
		std::shuffle(str.begin(), str.end(), generator);
		return str.substr(0, size);
	}

	static std::string getClipboardText();

	static void setClipboardText(std::string& text);

	static std::string readFileContents(std::wstring filePath) {
		std::ifstream fileStr(filePath, std::ios::in | std::ios::binary);
		if (fileStr) {
			std::string contents;
			fileStr.seekg(0, std::ios::end);
			contents.resize(fileStr.tellg());
			fileStr.seekg(0, std::ios::beg);
			fileStr.read(&contents[0], contents.size());
			fileStr.close();
			return contents;
		}
		return "";
	}

	static std::wstring wreadFileContents(std::wstring filePath) {
		std::wifstream fileStr(filePath, std::ios::in | std::ios::binary);
		if (fileStr) {
			std::wstring contents;
			fileStr.seekg(0, std::ios::end);
			contents.resize(fileStr.tellg());
			fileStr.seekg(0, std::ios::beg);
			fileStr.read(&contents[0], contents.size());
			fileStr.close();
			return contents;
		}
		return L"";
	}

	static uintptr_t FindSignatureModule(const char* szModule, const char* szSignature);

	static void GetCurrentSystemTime(tm& timeInfo);

	static void ApplySystemTime(std::stringstream* ss);

	static uintptr_t getBase();

	static std::string sanitize(std::string text);

	static std::string onlyOneLine(std::string text);

	static std::wstring stringToWstring(std::string txt);
	
	static bool endsWith(std::wstring const& fullString, std::wstring const& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		} else {
			return false;
		}
	}

	static void ApplyRainbow(float* rcolors, const float modifier = 0.003f) {
		if (rcolors[3] < 1) {
			rcolors[0] = 1;
			rcolors[1] = 0.6f;
			rcolors[2] = 0.6f;
			rcolors[3] = 1;
		}

		Utils::ColorConvertRGBtoHSV(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);

		rcolors[0] += modifier;
		if (rcolors[0] >= 1)
			rcolors[0] = 0;

		Utils::ColorConvertHSVtoRGB(rcolors[0], rcolors[1], rcolors[2], rcolors[0], rcolors[1], rcolors[2]);
	}

	static std::string getRttiBaseClassName(void* ptr);
	static void patchBytes(unsigned char* dst, unsigned char* src, unsigned int size);
	static void nopBytes(unsigned char* dst, unsigned int size);
	
	_NODISCARD inline static constexpr size_t StrToSignatureSize(const _STD string_view szSignature) noexcept {
		size_t size = 0;

		constexpr auto check = [](uint8_t str) noexcept -> bool {
			return (str >= 'a' && str <= 'z') || (str >= 'A' && str <= 'Z') || (str >= '0' && str <= '9');
		};

		size_t reset = 0;  // Erase '?' at the end

		for (size_t i = 0; i < szSignature.length(); ++i) {
			if (szSignature[i] == ' ')
				continue;
			else if (check(szSignature[i])) {
				reset = 0;
				++size;
				if (i + 1 < szSignature.length() && check(szSignature[i + 1]))
					++i;
			}
			else if (szSignature[i] == '?') {
				++reset;
				++size;
				if (i + 1 < szSignature.length() && szSignature[i + 1] == '?')
					++i;
			}
		}
		return size - reset;
	}

	template <size_t Size>
	_NODISCARD inline static constexpr _STD array<uint16_t, Size> StrToSignatureArray(const _STD string_view szSignature) noexcept {
		_STD array<uint16_t, Size> signature_result{};

		constexpr auto check = [](uint8_t str) noexcept -> bool {
			return (str >= 'a' && str <= 'z') || (str >= 'A' && str <= 'Z') || (str >= '0' && str <= '9');
		};

		constexpr auto HexToOct = [](uint8_t str) noexcept -> uint8_t {
			if (str >= 'a' && str <= 'z')
				return str - 'a' + 10ui8;
			else if (str >= 'A' && str <= 'Z')
				return str - 'A' + 10ui8;
			else if (str >= '0' && str <= '9')
				return str - '0';
			else
				return 0ui8;
		};

		size_t s_i = 0;
		for (size_t i = 0; i < szSignature.length() && s_i < Size; ++i) {
			if (szSignature[i] == ' ')
				continue;
			else if (check(szSignature[i])) {
				if (i + 1 < szSignature.length() && check(szSignature[i + 1])) {
					signature_result[s_i++] = (HexToOct(szSignature[i]) << 4) | HexToOct(szSignature[i + 1]);
					++i;
				}
				else
					signature_result[s_i++] = HexToOct(szSignature[i]);
			}
			else if (szSignature[i] == '?') {
				if (i + 1 < szSignature.length() && szSignature[i + 1] == '?')  // "??"
					++i;
				signature_result[s_i++] = 0xffffui16;
			}
		}

		return signature_result;
	}

	template <size_t Size>
	_NODISCARD inline static constexpr _STD array<size_t, 256> Pretreatment(const _STD array<uint16_t, Size> szSignature) noexcept {
		_STD array<size_t, 256> result{};
		size_t res_counter = 0;
		for (size_t i = 0; i < Size; ++i) {
			if (res_counter >= 256)
				break;
			if (const auto str = szSignature[i]; str != 0xffffui16 && result[str] == 0) {
				++res_counter;
				size_t fallbackSize = 1;
				for (size_t f_i = Size; f_i > 0; --f_i)
					if (szSignature[f_i - 1] == str)  // alignment
						break;
					else  // '?' or others
						++fallbackSize;
				result[str] = fallbackSize;
			}
		}
		// '?' alignment
		size_t anysign_counter = Size;
		for (; anysign_counter > 0 && szSignature[anysign_counter - 1] != 0xffffui16; --anysign_counter)
			if (szSignature[anysign_counter - 1] == 0xffffui16)
				break;
		if (anysign_counter != 0)
			for (size_t i = 0; i < 256; ++i)
				if (result[i] == 0 || result[i] > Size - anysign_counter + 1)
					result[i] = Size - anysign_counter + 1;
		return result;
	}

	template <size_t Size>
	_NODISCARD static uintptr_t FindSignatureModuleNew(const std::array<uint16_t, Size> szSignature) noexcept {  // Find the 'times' occurrence in memory
		//======================
		// GetModuleHandleA
		static const auto rangeStart = (uintptr_t)GetModuleHandleA("Minecraft.Windows.exe");
		static MODULEINFO miModInfo;
		static bool init = false;
		if (!init) {
			init = true;
			GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		}
		static const uintptr_t rangeEnd = rangeStart + miModInfo.SizeOfImage;
		//======================

		if (Size >= rangeEnd - rangeStart + 1) 
			return 0;

		const auto Table = Pretreatment<Size>(szSignature);  // Pretreatment Table

		for (uintptr_t i = rangeStart; i <= rangeEnd;) {
			size_t move_back_size = 0;
			if (*reinterpret_cast<uint8_t*>(i) == szSignature.front()) {
				size_t fallbackSize = 1;
				uint8_t last_char = 0ui8;
				for (; fallbackSize < Size && i + fallbackSize <= rangeEnd; ++fallbackSize) {
					if (szSignature[fallbackSize] != 0xffffui16 && *reinterpret_cast<uint8_t*>(i + fallbackSize) != szSignature[fallbackSize]) {  // '?' or the same
						last_char = szSignature[fallbackSize];
						break;
					}
				}
				if (fallbackSize == Size)  // Match All
					return i;
				else {  // KMP
					for (uintptr_t i2 = i + fallbackSize + 1; i2 <= rangeEnd; ++i2) {
						if (*reinterpret_cast<uint8_t*>(i2) == last_char) { // Find the closest character alignment
							move_back_size = i2 - (i + fallbackSize);
							break;
						}
						if (i2 == rangeEnd)
							return 0;
					}
				}
			}
			if (i + Size <= rangeEnd)  { // Sunday
				// The maximum number of displacements required for a char after the end of the comparison
				if (const auto str = *reinterpret_cast<uint8_t*>(i + Size); Table[str] != 0) {  // Fallback
					if (move_back_size < Table[str]) // Take maximum alignment
						move_back_size = Table[str];
				} else  {
					if (move_back_size < Size + 1) // Take maximum alignment
						move_back_size = Size + 1;  
				}
			} else 
				return 0;
			i += move_back_size;
		}
		return 0;
	}
};
