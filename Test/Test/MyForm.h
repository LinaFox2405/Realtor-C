// �������� ������������ ����� ��� CLR � Windows Forms
#pragma once
#include "File.h"
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Drawing;


// ����������� ����� � ������� �� ���������� �������� �����-�������
public ref class PurchaseRequestForm : public Form
{
private:
    TextBox^ textBoxMessage = gcnew TextBox();
    TextBox^ textBoxDate = gcnew TextBox();
    ComboBox^ comboBoxContactType = gcnew ComboBox();
    String^ usr;
public:
    PurchaseRequestForm(String^ username)
    {
        usr = username;
        // �������� ����������� �����
        Label^ labelContactType = gcnew Label();
        labelContactType->Text = "�������� ��� �����:";
        labelContactType->Location = System::Drawing::Point(10, 10);

        comboBoxContactType->Items->Add("�������");
        comboBoxContactType->Items->Add("Email");
        comboBoxContactType->Location = System::Drawing::Point(150, 10);

        textBoxDate->Location = System::Drawing::Point(150, 30);

        Label^ labelMessage = gcnew Label();
        labelMessage->Text = "������� ��� ������:";
        labelMessage->Location = System::Drawing::Point(10, 40);

        textBoxMessage->Multiline = true;
        textBoxMessage->Size = System::Drawing::Size(300, 100);
        textBoxMessage->Location = System::Drawing::Point(10, 70);

        Button^ buttonSubmit = gcnew Button();
        buttonSubmit->Text = "���������";
        buttonSubmit->Location = System::Drawing::Point(10, 180);
        buttonSubmit->Click += gcnew EventHandler(this, &PurchaseRequestForm::SubmitButton_Click);

        // ���������� ����������� �� �����
        this->Controls->Add(labelContactType);
        this->Controls->Add(comboBoxContactType);
        this->Controls->Add(labelMessage);
        this->Controls->Add(textBoxMessage);
        this->Controls->Add(buttonSubmit);
        this->Controls->Add(textBoxDate);

        // ��������� ���������
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->Text = "Purchase Request";
        this->Size = System::Drawing::Size(350, 250);
        this->StartPosition = FormStartPosition::CenterScreen;
    }

private:
    // ���������� ������� ������� ������ ���������
    void SubmitButton_Click(Object^ sender, EventArgs^ e)
    {
        // ��� ��� ��� ��������� ������ �� ���������� ��������
        if (!(textBoxDate->Text->Length == 0)) {
            bool phone = true;
            String^ feedBK = textBoxMessage->Text;
            if (comboBoxContactType->Text == "Email") phone = false;
            String^ data = textBoxDate->Text;
            writeFile(usr, data, feedBK, phone, "db.txt");
            MessageBox::Show("������ ����������!", "Info", MessageBoxButtons::OK, MessageBoxIcon::Information);
            this->Close();
            exit(1);
        }else MessageBox::Show("������ �� ����������!������� �������/Email", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
};

// ����������� ����� � ��������� ����������� � ������������
public ref class PropertyDetailsForm : public Form
{
    String^ usr;
    Dictionary<String^, String^>^ sellers = ParsePropertyFile("BD/sellers.txt");
    Dictionary<String^, String^>^ pics = ParsePropertyFile("BD/prop_images.txt");
public:
    PropertyDetailsForm(String^ propertyName, String^ propertyDescription, String^ sellerInfo, String^ username)
    {
        usr = username;
        // �������� ����������� ����� � ��������� �����������
        Label^ labelName = gcnew Label();
        labelName->Text = "�������� ����������: " + propertyName;
        labelName->Location = System::Drawing::Point(10, 10);
        labelName->AutoSize = true;

        TextBox^ textBoxDescription = gcnew TextBox();
        textBoxDescription->Multiline = true;
        textBoxDescription->ReadOnly = true;
        textBoxDescription->Text = propertyDescription;
        textBoxDescription->Size = System::Drawing::Size(300, 100);
        textBoxDescription->Location = System::Drawing::Point(10, 40);

        TextBox^ textBoxSellerInfo = gcnew TextBox();
        textBoxSellerInfo->Multiline = true;
        textBoxSellerInfo->ReadOnly = true;
        String^ val;
        sellers->TryGetValue(propertyName, val);
        textBoxSellerInfo->Text = "���������� � ��������:\n" + val;
        textBoxSellerInfo->Size = System::Drawing::Size(300, 60);
        textBoxSellerInfo->Location = System::Drawing::Point(10, 150);

        // ���������� ���� (������)
        PictureBox^ pictureBox = gcnew PictureBox();
        String^ path;
        pics->TryGetValue(propertyName, path);
        pictureBox->ImageLocation ="pics/"+ path + ".jpg";
        pictureBox->SizeMode = PictureBoxSizeMode::Zoom;
        pictureBox->Size = System::Drawing::Size(200, 150);
        pictureBox->Location = System::Drawing::Point(320, 40);

        Button^ buttonBack = gcnew Button();
        buttonBack->Text = "�����";
        buttonBack->Location = System::Drawing::Point(10, 210);
        buttonBack->Click += gcnew EventHandler(this, &PropertyDetailsForm::BackButton_Click);

        Button^ buttonBuy = gcnew Button();
        buttonBuy->Text = "���� ������";
        buttonBuy->Location = System::Drawing::Point(230, 210);
        buttonBuy->AutoSize = true;
        buttonBuy->Click += gcnew EventHandler(this, &PropertyDetailsForm::BuyButton_Click);

        // ���������� ����������� �� �����
        this->Controls->Add(labelName);
        this->Controls->Add(textBoxDescription);
        this->Controls->Add(textBoxSellerInfo);
        this->Controls->Add(pictureBox);
        this->Controls->Add(buttonBack);
        this->Controls->Add(buttonBuy);

        // ��������� ���������
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->Text = "Property Details";
        this->Size = System::Drawing::Size(550, 300);
        this->StartPosition = FormStartPosition::CenterScreen;
    }

private:
    // ���������� ������� ������� ������ �����
    void BackButton_Click(Object^ sender, EventArgs^ e)
    {
        // �������� ������� �����
        this->Close();
    }

    // ���������� ������� ������� ������ ���� ������
    void BuyButton_Click(Object^ sender, EventArgs^ e)
    {
        PurchaseRequestForm^ purchaseRequestForm = gcnew PurchaseRequestForm(usr);
        this->Hide();
        purchaseRequestForm->ShowDialog();
    }
};

public ref class PropertyListForm : public Form
{
private:
    ListBox^ listBoxProperties = gcnew ListBox();
    String^ usr;
    Dictionary<String^, String^>^ props = ParsePropertyFile("BD/property.txt");
public:
    PropertyListForm(String^ username)
    {
        usr = username;
        // �������� ����������� ����� �� ������� ������������
        
        listBoxProperties->Size = System::Drawing::Size(300, 200);
        listBoxProperties->Location = System::Drawing::Point(10, 10);

        // ���������� ������ ������������ (������)
        for each (KeyValuePair<String^, String^> pair in props) {
            listBoxProperties->Items->Add(pair.Key);
        }
        //listBoxProperties->Click += gcnew EventHandler(this, &PropertyListForm::SelectPropertyButton_Click);
        listBoxProperties->Click += gcnew EventHandler(this, &PropertyListForm::ListBoxProperties_SelectedIndexChanged);
        listBoxProperties->Font = gcnew System::Drawing::Font("Arial", 16, System::Drawing::FontStyle::Bold);

        Button^ buttonClose = gcnew Button();
        buttonClose->Text = "�����";
        buttonClose->Location = System::Drawing::Point(10, 220);
        buttonClose->Click += gcnew EventHandler(this, &PropertyListForm::CloseButton_Click);

        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;

        // ���������� ����������� �� �����
        this->Controls->Add(listBoxProperties);
        this->Controls->Add(buttonClose);

        // ��������� ���������
        this->Text = "Property List";
    }
private:
    void PropertyListForm::ListBoxProperties_SelectedIndexChanged(Object^ sender, EventArgs^ e)
    {
        // ��������, ������ �� ������� � ������
        if (listBoxProperties->SelectedIndex != -1)
        {
            // ��������� ��������� ������������
            String^ selectedProperty = listBoxProperties->SelectedItem->ToString();

            // ��� ��� ��� ��������� ��������� ���������� � ��������� ������������
            // ��������� ��� ���������� � ����������� PropertyDetailsForm

            String^ propertyName = selectedProperty;
            String^ propertyDescription;
            props->TryGetValue(selectedProperty, propertyDescription);
            String^ sellerInfo = "��� ��������, ���������� ����������...";

            // �������� � ����������� ����� � ��������� �����������
            PropertyDetailsForm^ propertyDetailsForm = gcnew PropertyDetailsForm(propertyName, propertyDescription, sellerInfo, usr);
            this->Hide();
            propertyDetailsForm->ShowDialog();
            this->Show();
        }
    }
    // ���������� ������� ������� ������ Close
    void CloseButton_Click(Object^ sender, EventArgs^ e)
    {
        // �������� ������� �����
        this->Hide();
    }
};

public ref class SuccessForm : public Form
{
public:
    String^ usr;
    SuccessForm(String^ username)
    {
        usr = username;
        Label^ labelWelcome = gcnew Label();
        labelWelcome->Text = "������������, " + username + "!";
        labelWelcome->AutoSize = true;
        labelWelcome->Font = gcnew System::Drawing::Font("Arial", 16, System::Drawing::FontStyle::Bold);
        int centerX = (this->Width - labelWelcome->Width) / 2;
        int centerY = (this->Height - labelWelcome->Height) / 2;
        labelWelcome->Location = System::Drawing::Point(centerX, centerY-40);

        //������ ���������
        Button^ buttonOK = gcnew Button();
        buttonOK->Text = "������� ������������";
        buttonOK->Font = gcnew System::Drawing::Font("Arial", 16, System::Drawing::FontStyle::Bold);
        buttonOK->Location = System::Drawing::Point(110, 180);
        buttonOK->Size = System::Drawing::Size(200, 60);
        buttonOK->Click += gcnew EventHandler(this, &SuccessForm::OKButton_Click);

        //������ ������
        Button^ buttonExit = gcnew Button();
        buttonExit->Text = "�����";
        Font = gcnew System::Drawing::Font("Arial", 16, System::Drawing::FontStyle::Bold);
        buttonExit->Location = System::Drawing::Point(300, 340);
        buttonExit->Size = System::Drawing::Size(90, 50);
        buttonExit->Click += gcnew EventHandler(this, &SuccessForm::ExitButton_Click);

        this->Controls->Add(labelWelcome);
        this->Controls->Add(buttonOK);
        this->Controls->Add(buttonExit);

        this->ClientSize = System::Drawing::Size(400, 400);
        this->Text = "";
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
    }

private:
    void OKButton_Click(Object^ sender, EventArgs^ e)
    {
        // �������� ����� ����� �� ������� ������������
        PropertyListForm^ propertyListForm = gcnew PropertyListForm(usr);
        propertyListForm->StartPosition = FormStartPosition::CenterScreen;
        this->Hide();
        // ����������� ����� �� ������� ������������
        propertyListForm->ShowDialog();
        this->Show();
    }
    void ExitButton_Click(Object^ sender, EventArgs^ e)
    {
        this->Close();
    }
private: System::Void InitializeComponent() {
    this->SuspendLayout();
    // 
    // SuccessForm
    // 
    this->ClientSize = System::Drawing::Size(400, 400);
    this->Name = L"SuccessForm";
    this->ResumeLayout(false);
}
};

// ����������� �����
public ref class MyForm : public Form
{
private:
    // ���������� ���������� ��� �������� ������
    Dictionary< String^, String^>^ users = ParseCredentialsFile("BD/users.txt");
    // ����������� ����������� �����
    Label^ labelUsername;
    TextBox^ textBoxUsername;
    Label^ labelPassword;
    TextBox^ textBoxPassword;
    Button^ buttonSubmit;

public:
    MyForm()
    {
        // ������������� ����������� �����
        labelUsername = gcnew Label();
        labelUsername->Text = "Username:";
        labelUsername->Location = Point(10, 10);

        textBoxUsername = gcnew TextBox();
        textBoxUsername->Location = Point(120, 10);

        labelPassword = gcnew Label();
        labelPassword->Text = "Password:";
        labelPassword->Location = Point(10, 40);

        textBoxPassword = gcnew TextBox();
        textBoxPassword->Location = Point(120, 40);
        textBoxPassword->PasswordChar = '*';

        buttonSubmit = gcnew Button();
        buttonSubmit->Text = "Submit";
        buttonSubmit->Location = Point(10, 70);
        buttonSubmit->Click += gcnew EventHandler(this, &MyForm::SubmitButton_Click);

        // ������������� ������ �������������

        //����������
        MyForm::StartPosition = FormStartPosition::CenterScreen;

        // ���������� ����������� �� �����
        this->Controls->Add(labelUsername);
        this->Controls->Add(textBoxUsername);
        this->Controls->Add(labelPassword);
        this->Controls->Add(textBoxPassword);
        this->Controls->Add(buttonSubmit);

        this->ClientSize = System::Drawing::Size(287, 100);
        this->Text = "Login";
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
    }

private:
    // ���������� ������� ������� ������ Submit
    void MyForm::SubmitButton_Click(Object^ sender, EventArgs^ e)
    {
        String^ usr = textBoxUsername->Text;
        String^ pwd = textBoxPassword->Text;
        String^ temp;
       if (users->ContainsKey(usr) && users->TryGetValue(usr, temp) && temp == pwd) {
            SuccessForm^ successForm = gcnew SuccessForm(textBoxUsername->Text);
            this->Hide();

            successForm->StartPosition = FormStartPosition::CenterScreen;
            successForm->ShowDialog();
            successForm->Hide();
            this->textBoxPassword->Clear();
            this->textBoxUsername->Clear();
            this->Show();
        }
        else {
            MessageBox::Show("Login failed. Invalid username or password. Try again.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            this->textBoxPassword->Clear();
            this->textBoxUsername->Clear();
        }
    }
private: System::Void InitializeComponent() {
    this->SuspendLayout();
    this->ClientSize = System::Drawing::Size(287, 60);
    this->Name = L"����";
    this->Text = L"����";
    this->ResumeLayout(false);
}
};

