object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'CryTool'
  ClientHeight = 181
  ClientWidth = 504
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 35
    Width = 55
    Height = 13
    Caption = #1050#1086#1085#1090#1077#1081#1085#1077#1088
  end
  object Label2: TLabel
    Left = 105
    Top = 35
    Width = 67
    Height = 13
    Caption = #1050#1083#1102#1095' '#1086#1073#1084#1077#1085#1072
  end
  object LoadKeyButton: TButton
    Left = 105
    Top = 53
    Width = 91
    Height = 41
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
    TabOrder = 0
  end
  object GenerateKeyButton: TButton
    Left = 105
    Top = 95
    Width = 91
    Height = 42
    Caption = #1043#1077#1085#1077#1088#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 1
    OnClick = GenerateKeyButtonClick
  end
  object ExportKeyButton: TButton
    Left = 105
    Top = 138
    Width = 91
    Height = 35
    Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 2
    OnClick = ExportKeyButtonClick
  end
  object EnPasswordEdit: TEdit
    Left = 226
    Top = 115
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 3
    Text = '111'
  end
  object PassConfirmEdit: TEdit
    Left = 226
    Top = 142
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 4
    Text = '111'
  end
  object DePasswordEdit: TEdit
    Left = 353
    Top = 115
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 5
    Text = '111'
  end
  object EncryptFileButton: TButton
    Left = 226
    Top = 23
    Width = 121
    Height = 86
    Caption = #1064#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 6
    OnClick = EncryptFileButtonClick
  end
  object DecryptFileButton: TButton
    Left = 353
    Top = 23
    Width = 121
    Height = 86
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 7
    OnClick = DecryptFileButtonClick
  end
  object CreateContainerButton: TButton
    Left = 8
    Top = 53
    Width = 91
    Height = 41
    Caption = #1057#1086#1079#1076#1072#1090#1100
    TabOrder = 8
    OnClick = CreateContainerButtonClick
  end
  object LoadContainerButton: TButton
    Left = 8
    Top = 95
    Width = 91
    Height = 42
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
    TabOrder = 9
    OnClick = LoadContainerButtonClick
  end
  object DeleteContainerButton: TButton
    Left = 8
    Top = 138
    Width = 91
    Height = 35
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 10
    OnClick = DeleteContainerButtonClick
  end
  object UsernameEdit: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 11
    Text = 'UsernameEdit'
  end
  object OpenFileDialog: TOpenDialog
    Left = 360
    Top = 142
  end
  object SaveExKeyDialog: TSaveDialog
    Left = 432
    Top = 142
  end
end
