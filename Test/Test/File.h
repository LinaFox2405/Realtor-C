#pragma once
#include <iostream>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;
Dictionary<String^, String^>^ ParseCredentialsFile(String^ filePath);
bool writeFile(String^ usr, String^ data, String^ feedBK, bool phone, String^ filePath);
String^ readFile(String^ path);
Dictionary<String^, String^>^ ParsePropertyFile(String^ filePath);