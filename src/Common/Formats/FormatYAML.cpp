#include <Arduino.h>

#include "../Value.h"
#include "../Utils.h"

#include "FormatYAML.h"

using namespace Qybercom;

String Formats::FormatYAML::_serialize (Value &value, int indentSize, bool first) {
	String out = "";
	String ind = indent(indentSize);

	// TODO: need refactor
	/*if (value.IsArray()) {
		for (Value &v : value)
			out += ind + "- " + String(v.IsValue()
				? ": " + v.ToString() + "\n"
				: Formats::FormatYAML::_serialize(v, indentSize + 1, false)
			);
	}

	if (value.IsObject()) {
		for (Value &v : value) {
			out += String(first ? ind : "") + v.Key() + String(v.IsValue()
				? ": " + v.ToString() + "\n"
				: ":\n" + Formats::FormatYAML::_serialize(v, indentSize + 1)
			);

			first = true;
		}
	}*/

	out += ind + value.ToString() + "\n";

	return out;
}

String Formats::FormatYAML::ValueFormatMIME () {
	return "application/yaml";
}

String Formats::FormatYAML::ValueFormatSerialize (Value &value) {
	return Formats::FormatYAML::_serialize(value, 0);
}

Value Formats::FormatYAML::ValueFormatDeserialize (const String &raw) {
	// TODO: deserialization in progress
	return Value();
}