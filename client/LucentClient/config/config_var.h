#pragma once
#include "config_manager.h"
#include <iostream>

template <typename T>
class ConfigVar {
private:
	std::string path, key;
	mutable T value;
	mutable bool initialized = false; // Lazy loading flag

	void Load() const {
		if (!initialized) {
			value = ConfigManager::GetValue<T>(path, key, value);
			initialized = true;
		}
	}

public:
	ConfigVar(const std::string& path, const std::string& key, const T& defaultValue)
		: path(path), key(key), value(defaultValue) {}

	operator T() const { Load(); return value; }

	ConfigVar& operator=(const T& newValue) {
		value = newValue;
		ConfigManager::SetValue<T>(path, key, value);
		return *this;
	}

	ConfigVar& operator=(const ConfigVar<T>& other) {
		if (this != &other) {
			this->value = other.value;
			this->path = other.path;
			this->key = other.key;
			this->initialized = other.initialized;
		}
		return *this;
	}

	const std::string& GetPath() const { return path; }
	const std::string& GetKey() const { return key; }

	T* GetPointer() { Load(); return &value; }  // Pointer accessor
	T& GetValue() { Load(); return value; }  // Value accessor
	void SetValue(const T& v) { Load(); value = v; }
};

