/*
	Yelo: Open Sauce SDK

	See license\OpenSauce\OpenSauce for specific license information
*/
#pragma once

#include <string>
#include "base.h"
#include "../math/integer_math.h"
#include "../math/color_math.h"
#include "../math/real_math.h"

namespace Yelo {
	namespace ValueConversion {
		template <typename ... Arguments>
		static bool FromStringImpl(const char * format, int member_count, const char * input, Arguments ... args) {
			return member_count == sscanf_s(input, format, args ...);
		}

		bool FromString(const char * string, bool &output) {
			auto std_string = std::string(string);

			if ((std_string == "True") || (std_string == "true") || (std_string == "on") || (std_string == "1")) {
				output = true;
				return true;
			} else if ((std_string == "False") || (std_string == "false") || (std_string == "off") || (std_string == "0")) {
				output = false;
				return true;
			}

			return false;
		}

		bool FromString(const char * string, byte &output) {
			uint value;
			auto   result = FromStringImpl("%u", 1, string, &value);
			output = (byte) value;

			return result;
		}

		bool FromString(const char * string, sbyte &output) {
			long value;
			auto  result = FromStringImpl("%d", 1, string, &value);
			output = (sbyte) value;

			return result;
		}

		bool FromString(const char * string, unsigned short &output) {
			uint value;
			auto   result = FromStringImpl("%u", 1, string, &value);
			output = (unsigned short) value;

			return result;
		}

		bool FromString(const char * string, short &output) {
			long value;
			auto  result = FromStringImpl("%d", 1, string, &value);
			output = (short) value;

			return result;
		}

		bool FromString(const char * string, uint &output) {
			return FromStringImpl("%u", 1, string, &output);
		}

		bool FromString(const char * string, long &output) {
			return FromStringImpl("%d", 1, string, &output);
		}

		bool FromString(const char * string, uint64 &output) {
			return FromStringImpl("%lu", 1, string, &output);
		}

		bool FromString(const char * string, __int64 &output) {
			return FromStringImpl("%ld", 1, string, &output);
		}

		bool FromString(const char * string, real &output) {
			return FromStringImpl("%f", 1, string, &output);
		}

		bool FromString(const char * string, std::string &output) {
			output.assign(string);
			return true;
		}

		bool FromString(const char * string, point2d &output) {
			long x, y;
			auto  result = FromStringImpl("%d %d", 2, string, &x, &y);
			output.x = (short) x;
			output.y = (short) y;

			return result;
		}

		bool FromString(const char * string, rectangle2d &output) {
			long top, left, bottom, right;
			auto  result = FromStringImpl("%d %d %d %d", 4, string, &top, &left, &bottom, &right);
			output.top    = (short) top;
			output.left   = (short) left;
			output.bottom = (short) bottom;
			output.right  = (short) right;

			return result;
		}

		bool FromString(const char * string, real_vector2d &output) {
			return FromStringImpl("%f %f", 2, string, &output.i, &output.j);
		}

		bool FromString(const char * string, real_vector3d &output) {
			return FromStringImpl("%f %f %f", 3, string, &output.i, &output.j, &output.k);
		}

		bool FromString(const char * string, real_point2d &output) {
			return FromStringImpl("%f %f", 2, string, &output.x, &output.y);
		}

		bool FromString(const char * string, real_point3d &output) {
			return FromStringImpl("%f %f %f", 3, string, &output.x, &output.y, &output.z);
		}

		bool FromString(const char * string, real_quaternion &output) {
			return FromStringImpl("%f %f %f %f", 4, string, &output.i, &output.j, &output.k, &output.w);
		}

		bool FromString(const char * string, rgb_color &output) {
			uint red, green, blue;
			auto   result = FromStringImpl("%u %u %u", 3, string, &red, &green, &blue);
			output.red   = (byte) red;
			output.green = (byte) green;
			output.blue  = (byte) blue;

			return result;
		}

		bool FromString(const char * string, argb_color &output) {
			uint red, green, blue, alpha;
			auto   result = FromStringImpl("%u %u %u %u", 4, string, &red, &green, &blue, &alpha);
			output.red   = (byte) red;
			output.green = (byte) green;
			output.blue  = (byte) blue;
			output.alpha = (byte) alpha;

			return result;
		}

		bool FromString(const char * string, real_rgb_color &output) {
			return FromStringImpl("%f %f %f", 3, string, &output.red, &output.green, &output.blue);
		}

		bool FromString(const char * string, real_argb_color &output) {
			return FromStringImpl("%f %f %f %f", 4, string, &output.red, &output.green, &output.blue, &output.alpha);
		}
	};
};
