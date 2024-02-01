#pragma once
class DefinitionCounter
{
public:
	static size_t getDefinitionID();
private:
	static size_t definitionCount;
};

