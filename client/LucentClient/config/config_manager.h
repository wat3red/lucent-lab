#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <filesystem>
#include <oxorany/oxorany_include.h>

#include "json.hpp"
#include "../vector3.h"
#include "../vector2.h"
#include <imgui/imgui.h> 

namespace nlohmann {
	template <>
	struct adl_serializer<ImColor> {
		static void to_json(json& j, const ImColor& color) {
			j = json{ {oxorany("r"), color.Value.x}, {oxorany("g"), color.Value.y}, {oxorany("b"), color.Value.z}, {oxorany("a"), color.Value.w} };
		}

		static void from_json(const json& j, ImColor& color) {
			color = ImColor(j.value(oxorany("r"), 1.0f), j.value(oxorany("g"), 1.0f), j.value(oxorany("b"), 1.0f), j.value(oxorany("a"), 1.0f));
		}
	};

	template <>
	struct adl_serializer<ImVec4> {
		static void to_json(json& j, const ImVec4& color) {
			j = json{ {oxorany("x"), color.x}, {oxorany("y"), color.y}, {oxorany("z"), color.z}, {oxorany("w"), color.w} };
		}

		static void from_json(const json& j, ImVec4& color) {
			color.x = j.value(oxorany("x"), 1.0f);
			color.y = j.value(oxorany("y"), 1.0f);
			color.z = j.value(oxorany("z"), 1.0f);
			color.w = j.value(oxorany("w"), 1.0f);
		}
	};

	template <>
	struct adl_serializer<Vector3> {
		static void to_json(json& j, const Vector3& vector) {
			j = json{ {oxorany("x"), vector.x}, {oxorany("y"), vector.y}, {oxorany("z"), vector.z} };
		}

		static void from_json(const json& j, Vector3& vector) {
			vector.x = j.value(oxorany("x"), 1.0f);
			vector.y = j.value(oxorany("y"), 1.0f);
			vector.z = j.value(oxorany("z"), 1.0f);
		}
	};

	template <>
	struct adl_serializer<Vector2> {
		static void to_json(json& j, const Vector2& vector) {
			j = json{ {oxorany("x"), vector.x}, {oxorany("y"), vector.y} };
		}

		static void from_json(const json& j, Vector2& vector) {
			vector.x = j.value(oxorany("x"), 1.0f);
			vector.y = j.value(oxorany("y"), 1.0f);
		}
	};
}

using json = nlohmann::json;

namespace ConfigManager {
	extern std::string CurrentConfig;

	void Save(const json& config);
	json Load();
	void RemoveSection(const std::string& path);
	std::vector<std::string> SplitPath(const std::string& path);

	template<typename T>
	void SetValue(const std::string& path, const std::string& key, const T& value) {
		json config = Load();
		json* section = &config;
		for (const auto& part : SplitPath(path)) {
			section = &((*section)[part]);
		}
		(*section)[key] = value;
		Save(config);
	}

	template<typename T>
	T GetValue(const std::string& path, const std::string& key, const T& defaultValue) {
		json config = Load();
		json* section = &config;
		for (const auto& part : SplitPath(path)) {
			if (!section->contains(part)) return defaultValue;
			section = &((*section)[part]);
		}
		return section->contains(key) ? (*section)[key].get<T>() : defaultValue;
	}
}