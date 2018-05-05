object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'CryTool'
  ClientHeight = 206
  ClientWidth = 459
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
    Top = 42
    Width = 55
    Height = 13
    Caption = #1050#1086#1085#1090#1077#1081#1085#1077#1088
  end
  object Label2: TLabel
    Left = 105
    Top = 42
    Width = 73
    Height = 13
    Caption = #1050#1083#1102#1095#1080' '#1086#1073#1084#1077#1085#1072
  end
  object InfoLabel: TLabel
    Left = 8
    Top = 187
    Width = 3
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clMenuHighlight
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LoadKeyButton: TButton
    Left = 105
    Top = 60
    Width = 91
    Height = 41
    Caption = #1048#1084#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1090#1082#1088#1099#1090#1099#1081' '#1082#1083#1102#1095
    TabOrder = 0
    WordWrap = True
    OnClick = LoadKeyButtonClick
  end
  object GenerateKeyButton: TButton
    Left = 105
    Top = 102
    Width = 91
    Height = 42
    Caption = #1043#1077#1085#1077#1088#1080#1088#1086#1074#1072#1090#1100' '#1082#1083#1102#1095#1077#1074#1091#1102' '#1087#1072#1088#1091
    ParentShowHint = False
    ShowHint = False
    TabOrder = 1
    WordWrap = True
    OnClick = GenerateKeyButtonClick
  end
  object ExportKeyButton: TButton
    Left = 105
    Top = 145
    Width = 91
    Height = 36
    Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1090#1082#1088#1099#1090#1099#1081' '#1082#1083#1102#1095
    TabOrder = 2
    WordWrap = True
    OnClick = ExportKeyButtonClick
  end
  object EnPasswordEdit: TEdit
    Left = 202
    Top = 85
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 3
    Text = '111'
  end
  object PassConfirmEdit: TEdit
    Left = 202
    Top = 112
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 4
    Text = '111'
  end
  object DePasswordEdit: TEdit
    Left = 329
    Top = 85
    Width = 121
    Height = 21
    PasswordChar = '*'
    TabOrder = 5
    Text = '111'
  end
  object EncryptFileButton: TButton
    Left = 202
    Top = 8
    Width = 121
    Height = 71
    Caption = #1064#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 6
    OnClick = EncryptFileButtonClick
  end
  object DecryptFileButton: TButton
    Left = 329
    Top = 9
    Width = 121
    Height = 71
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 7
    OnClick = DecryptFileButtonClick
  end
  object CreateContainerButton: TButton
    Left = 8
    Top = 60
    Width = 91
    Height = 41
    Caption = #1057#1086#1079#1076#1072#1090#1100
    TabOrder = 8
    OnClick = CreateContainerButtonClick
  end
  object LoadContainerButton: TButton
    Left = 8
    Top = 102
    Width = 91
    Height = 42
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
    TabOrder = 9
    OnClick = LoadContainerButtonClick
  end
  object DeleteContainerButton: TButton
    Left = 8
    Top = 145
    Width = 91
    Height = 36
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 10
    OnClick = DeleteContainerButtonClick
  end
  object UsernameEdit: TEdit
    Left = 8
    Top = 8
    Width = 188
    Height = 21
    TabOrder = 11
    Text = 'UsernameEdit'
  end
  object SessionExBtn: TButton
    Left = 202
    Top = 139
    Width = 121
    Height = 42
    Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1089#1077#1089#1089#1080#1086#1085#1085#1099#1081' '#1082#1083#1102#1095
    Enabled = False
    TabOrder = 12
    WordWrap = True
    OnClick = SessionExBtnClick
  end
  object SessionLoadBtn: TButton
    Left = 329
    Top = 139
    Width = 121
    Height = 42
    Caption = #1048#1084#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1089#1077#1089#1089#1080#1086#1085#1085#1099#1081' '#1082#1083#1102#1095
    TabOrder = 13
    WordWrap = True
    OnClick = SessionLoadBtnClick
  end
  object UsingImportKeyCheck: TCheckBox
    Left = 329
    Top = 112
    Width = 121
    Height = 21
    Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1082#1083#1102#1095
    TabOrder = 14
    WordWrap = True
  end
  object OpenFileDialog: TOpenDialog
    Left = 320
    Top = 200
  end
  object SaveExKeyDialog: TSaveDialog
    Left = 254
    Top = 207
  end
end
