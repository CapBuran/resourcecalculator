#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string>
#include <vector>
#include <list>
#include <map>

namespace ResourceCalculator
{

template <class Type>
class SerializerDataCustom{
private:
	std::vector<Type> _data;
	Type* _pdata;

public:
	SerializerDataCustom(size_t Size):
		_data(Size),
		_pdata(Size > 0 ? &_data[0] : NULL)
	{		
	}

	SerializerDataCustom():
		_pdata(NULL)
	{		
	}

	SerializerDataCustom& operator++(){
		_pdata++;
		return *this;
	}

	size_t Size(){
		return _data.size();
	}

	void ReiInit(size_t Size){
		_data.init_clean();
		_data.set_size(Size);
	}

	SerializerDataCustom& operator+=(const size_t& add_val) {
		_pdata += add_val;
		return *this;
	}

	operator Type*(){
		if(_pdata == NULL) _pdata = _data.size() > 0 ? &_data[0] : NULL;
		return _pdata;
	}

	void *getPData(){
		_pdata = _data.size() > 0 ? &_data[0] : NULL;
		return _pdata;
	}

  std::vector<Type> &getData(){
		return _data;
	}

	void swap(std::vector<Type> &data){
		_pdata = data.size() > 0 ? &data[0] : NULL;
		data.swap(_data);
	}

	void swap(SerializerDataCustom<Type> &data){
		data._data.swap(data._data);
		_pdata = *this;
		data._pdata = data;
	}

	bool Check(size_t sizeBuffer) const{
		if(_data.size() == 0) return true;
		return _pdata - &_data[0] == sizeBuffer;
	}

	bool Check() const{
		if(_data.size() == 0) return true;
		return _pdata - &_data[0] == _data.size();
	}
};

typedef SerializerDataCustom<char> SerializerData;

class Serializable{
public:
	virtual size_t get_size_pack() const = 0;
	virtual void pack(SerializerData &) const = 0;
	virtual void unpack(SerializerData &) = 0;
};

//!Функция тяжелая: если объект занимает большое количество памяти лучше реализовать у него метод clone_to
template <class SerializerClassType>
void SerializableCopy(const SerializerClassType &ClassFrom, SerializerClassType &ClassTo) {
	{
		const SerializerClassType &Check1 = ClassFrom;
		const SerializerClassType &Check2 = ClassTo;
	}
	SerializerData data(ClassFrom.get_size_pack());
	ClassFrom.pack(data);
	assert(data.Check());
	data.getPData();
	ClassTo.unpack(data);
	assert(data.Check());
}

//!Функция тяжелая: если объекты занимают большое количество памяти лучше реализовать у него метод swap
template <class SerializerClassType>
void SerializableSwap(SerializerClassType &Class1, SerializerClassType &Class2) {
	{
		const SerializerClassType &Check1 = Class1;
		const SerializerClassType &Check2 = Class2;
	}
	SerializerData data1(Class1.get_size_pack());
	SerializerData data2(Class2.get_size_pack());
	Class1.pack(data1);
	Class2.pack(data2);
	assert(data1.Check());
	assert(data2.Check());
	data1.swap(data2);
	Class1.unpack(data1);
	Class2.unpack(data2);
	assert(data1.Check());
	assert(data2.Check());
}

#pragma region Шаблоны для простых типов
template <class Type>
inline void UnPackValAndSeekData(SerializerData &data, Type &Value) {
	Type *DataPTR = (Type*)data.operator char *();
	Value = *DataPTR;
	data += sizeof(Type);
	return;
}

template <class Type>
inline void PackValAndSeekData(SerializerData &data, const Type &Value) {
	Type *DataPTR = (Type*)data.operator char *();
	*DataPTR = Value;
	data += sizeof(Type);
	return;
}
#pragma endregion

#pragma region Шаблоны для std

template <class Type>
inline size_t GetSize(const std::string &Str)
{
	size_t SizeStr = sizeof(size_t);
	if(Str.length() > 0) SizeStr += Str.length()*sizeof(char)+sizeof(char);
	return SizeStr;
}

template <class Type>
inline void UnPackAndSeek(SerializerData &data, std::string &Str)
{
	size_t SizeStr = -1;
	UnPackValAndSeekData(data, SizeStr);
	Str.clear();
	if(SizeStr > 0){
		Str.reserve(SizeStr+sizeof(char));
		Str.resize(SizeStr);
		char *c_str = &Str[0];
		SizeStr += sizeof(char);
		memcpy(c_str, data, SizeStr);
		data += SizeStr;
	}
	return;
}

template <class Type>
inline void PackAndSeek(SerializerData &data, const std::string &Str)
{
	const char *c_str = Str.c_str();
	size_t SizeStr = Str.length();
	PackValAndSeekData(data, SizeStr);
	if(SizeStr == 0) return;
	SizeStr += sizeof(char);
	memcpy(data, c_str, SizeStr);
	data += SizeStr;
	return;
}


template <class Type>
inline size_t GetSize(const std::vector<Type> &vec)
{
	size_t SizeStr = sizeof(size_t) + vec.size()*sizeof(Type);
	return SizeStr;
}

template <class Type>
inline void UnPackAndSeek(SerializerData &data, std::vector<Type> &vec)
{
	size_t SizeVec = -1;
	UnPackValAndSeekData(data, SizeVec);
	vec.clear();
	if(SizeVec > 0){
		vec.resize(SizeVec);
		Type *DataVec = &vec[0];
		memcpy(DataVec, data, SizeVec*sizeof(Type));
		data += SizeVec*sizeof(Type);
	}
	return;
}

template <class Type>
inline void PackAndSeek(SerializerData &data, const std::vector<Type> &vec)
{
	size_t SizeVec = (size_t)vec.size();
	PackValAndSeekData(data, SizeVec);
	if(SizeVec == 0) return;
	const Type *DataVec = &vec[0];
	memcpy(data, DataVec, SizeVec*sizeof(Type));
	data += SizeVec*sizeof(Type);
	return;
}


template <class Type>
inline size_t GetSize(const std::list<Type> &list)
{
  size_t SizeStr = sizeof(size_t) + list.size() * sizeof(Type);
  return SizeStr;
}

template <class Type>
inline void UnPackAndSeek(SerializerData &data, std::list<Type> &list) 
{
  size_t SizeList = -1;
  UnPackValAndSeekData(data, SizeList);
  list.clear();
  if (SizeList > 0) {
    for (size_t i = 0; i < SizeList; i++){
      Type elem;
      UnPackValAndSeekData(data, elem);
      list.push_back(elem);
    }
  }
  return;
}

template <class Type>
inline void PackAndSeek(SerializerData &data, const std::list<Type> &list)
{
  size_t SizeList = list.size();
  PackValAndSeekData(data, SizeList);
  if (SizeList == 0)
    return;
  for (auto &elem : list) {
    PackValAndSeekData(data, elem);
  }
  return;
}


template <class Type1, class Type2>
inline size_t GetSize(const std::map<Type1, Type2> &map)
{
	size_t SizeStr = sizeof(size_t) + map.size()*(sizeof(Type1) + sizeof(Type2));
	return SizeStr;
}

template <class Type1, class Type2>
inline void UnPackAndSeek(SerializerData &data, std::map<Type1, Type2> &map) 
{
	size_t SizeMap = -1;
	UnPackValAndSeekData(data, SizeMap);
	map.clear();
	if(SizeMap > 0){
		for (size_t i = 0; i < SizeMap; i++){
			Type1 First; Type2 Second;
			UnPackValAndSeekData(data, First);
			UnPackValAndSeekData(data, Second);
			map.operator[](First) = Second;
		}
	}
	return;
}

template <class Type1, class Type2>
inline void PackAndSeek(SerializerData &data, const std::map<Type1, Type2> &map)
{
	size_t SizeMap = map.size();
	PackValAndSeekData(data, SizeMap);
	if(SizeMap == 0) return;
	for (auto &i : map){
		Type1 First = i.first;
		Type2 Second = i.second; 
		PackValAndSeekData(data, First);
		PackValAndSeekData(data, Second);
	}
	return;
}

#pragma endregion

}
