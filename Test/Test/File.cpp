#include "File.h"// Добавляем для преобразования String^ в const wchar_t*

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;
using namespace std;

// Функция для парсинга файла с форматом [PropertyName] {PropertyInformation};
Dictionary<String^,String^>^ ParsePropertyFile(String^ filePath)
{
    Dictionary<String^, String^>^ propertyList = gcnew Dictionary<String^, String^>();

    try
    {
        StreamReader^ sr = gcnew StreamReader(filePath);

        String^ currentLine;

        while ((currentLine = sr->ReadLine()) != nullptr)
        {
            if (currentLine->StartsWith("[") && currentLine->EndsWith("};"))
            {
                String^ propertyName = currentLine->Substring(1, currentLine->IndexOf("]") - 1)->Trim();
                String^ propertyInformation = currentLine->Substring(currentLine->IndexOf("{") + 1, currentLine->IndexOf("}") - currentLine->IndexOf("{") - 1)->Trim();

                propertyList->Add(propertyName, propertyInformation);
            }
        }

        sr->Close();
    }
    catch (Exception^ e)
    {
        Console::WriteLine("Error reading from file: " + e->Message);
    }

    return propertyList;
}

// Функция для парсинга файла формата "username:password"
Dictionary<String^,String^>^ ParseCredentialsFile(String^ filePath)
{
    Dictionary<String^, String^>^ userCredentialsList = gcnew Dictionary<String^, String^>();

    try
    {
        // Используем StreamReader для чтения из файла
        StreamReader^ sr = gcnew StreamReader(filePath);

        // Читаем файл построчно
        while (!sr->EndOfStream)
        {
            String^ line = sr->ReadLine();

            // Разбиваем строку на части, используя разделитель ":"
            System::Array^ parts = line->Split(':');
            // Проверяем, что у нас есть две части (username и password)
            if (parts->Length == 2)
            {
                String^ username = parts->GetValue(0)->ToString()->Trim();
                String^ password = parts->GetValue(1)->ToString()->Trim();
                userCredentialsList->Add(username, password);
            }
        }

        sr->Close();
    }
    catch (Exception^ e)
    {
        // Обработка исключений, если что-то пошло не так
        Console::WriteLine("Error reading from file: " + e->Message);
    }

    return userCredentialsList;
}

bool writeFile(String^ usr, String^ data, String^ feedBK, bool phone, String^ filePath) {
    try
    {
        // Используем StreamWriter для записи строки в файл
        StreamWriter^ sw = gcnew StreamWriter(filePath,true);
        sw->Write("["+usr+"]");
        sw->Write("{");
        if (phone) sw->WriteLine("phone: " + data);
        else sw->WriteLine("email: " + data);
        sw->Write(feedBK);
        sw->Write("};\n");
        sw->Close();
        return 1;
    }
    catch (Exception^ e)
    {
        // Обработка исключений, если что-то пошло не так
        Console::WriteLine("Error writing to file: " + e->Message);
    }
    return 0;
}

//bool writeLikeBD(String^ str, String^ filePath) {
//    writeFile(str,filePath)
//}


String^ readFile(String^ filePath) {
    try
    {
        // Используем StreamReader для чтения из файла
        StreamReader^ sr = gcnew StreamReader(filePath);
        String^ content = sr->ReadToEnd();
        sr->Close();
        return content;
    }
    catch (Exception^ e)
    {
        // Обработка исключений, если что-то пошло не так
        Console::WriteLine("Error reading from file: " + e->Message);
        return nullptr; // Возвращаем nullptr в случае ошибки
    }
}