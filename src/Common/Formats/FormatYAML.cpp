#include <Arduino.h>

#include "../Data/index.h"

#include "FormatYAML.h"

using namespace Qybercom;

String Formats::FormatYAML::_serialize (Bucket &bucket, int indentSize, bool first) {
	String out = "";
	String ind = indent(indentSize);

	if (bucket.IsValue())
		out += ind + bucket.AsValue().ToString() + "\n";

	if (bucket.IsArray()) {
		for (Bucket &v : bucket)
			out += ind + "- " + String(v.IsValue()
				? ": " + v.AsValue().ToString() + "\n"
				: Formats::FormatYAML::_serialize(v, indentSize + 1, false)
			);
	}

	if (bucket.IsObject()) {
		for (Bucket &v : bucket) {
			out += String(first ? ind : "") + v.Key() + String(v.IsValue()
				? ": " + v.AsValue().ToString() + "\n"
				: ":\n" + Formats::FormatYAML::_serialize(v, indentSize + 1)
			);

			first = true;
		}
	}

	return out;
}

String Formats::FormatYAML::BucketFormatSerialize (Bucket &bucket) {
	return Formats::FormatYAML::_serialize(bucket, 0);
}

Bucket Formats::FormatYAML::BucketFormatDeserialize (const String &raw) {
	// TODO: deserialization in progress
	return Bucket::Null();
}