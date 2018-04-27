object MainForm: TMainForm
  Left = 0
  Top = 0
  Caption = 'MainForm'
  ClientHeight = 166
  ClientWidth = 635
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
  object LoadKeyButton: TButton
    Left = 89
    Top = 32
    Width = 91
    Height = 25
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
    TabOrder = 0
  end
  object GenerateKeyButton: TButton
    Left = 89
    Top = 63
    Width = 91
    Height = 25
    Caption = #1043#1077#1085#1077#1088#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 1
    OnClick = GenerateKeyButtonClick
  end
  object ExportKeyButton: TButton
    Left = 89
    Top = 94
    Width = 91
    Height = 25
    Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 2
    OnClick = ExportKeyButtonClick
  end
  object Edit1: TEdit
    Left = 186
    Top = 77
    Width = 121
    Height = 21
    TabOrder = 3
    Text = 'Edit1'
  end
  object Edit2: TEdit
    Left = 186
    Top = 104
    Width = 121
    Height = 21
    TabOrder = 4
    Text = 'Edit2'
  end
  object Edit3: TEdit
    Left = 313
    Top = 77
    Width = 121
    Height = 21
    TabOrder = 5
    Text = 'Edit3'
  end
  object EncryptFileButton: TButton
    Left = 186
    Top = 32
    Width = 121
    Height = 41
    Caption = #1064#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 6
  end
  object DecryptFileButton: TButton
    Left = 313
    Top = 32
    Width = 121
    Height = 39
    Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
    TabOrder = 7
  end
  object CreateContainerButton: TButton
    Left = 8
    Top = 32
    Width = 75
    Height = 25
    Caption = #1057#1086#1079#1076#1072#1090#1100
    TabOrder = 8
    OnClick = CreateContainerButtonClick
  end
  object LoadContainerButton: TButton
    Left = 8
    Top = 63
    Width = 75
    Height = 25
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100
    TabOrder = 9
    OnClick = LoadContainerButtonClick
  end
  object DeleteContainerButton: TButton
    Left = 8
    Top = 94
    Width = 75
    Height = 25
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 10
    OnClick = DeleteContainerButtonClick
  end
  object Memo1: TMemo
    Left = 440
    Top = 32
    Width = 187
    Height = 93
    Lines.Strings = (
      'Memo1')
    TabOrder = 11
  end
  object UsernameEdit: TEdit
    Left = 8
    Top = 8
    Width = 121
    Height = 21
    TabOrder = 12
    Text = 'UsernameEdit'
  end
  object OpenDialog1: TOpenDialog
    Left = 320
    Top = 104
  end
  object SaveExKeyDialog: TSaveDialog
    Left = 392
    Top = 104
  end
end
