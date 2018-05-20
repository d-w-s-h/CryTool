object MainForm: TMainForm
  Left = 0
  Top = 0
  BorderStyle = bsSingle
  Caption = 'CryTool'
  ClientHeight = 367
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object InfoLabel: TLabel
    Left = 8
    Top = 345
    Width = 3
    Height = 13
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object UsernameEdit: TEdit
    Left = 8
    Top = 13
    Width = 261
    Height = 21
    TabOrder = 0
    Text = 'UsernameEdit'
  end
  object UpdateContainerButton: TButton
    Left = 8
    Top = 40
    Width = 261
    Height = 33
    Caption = #1054#1073#1085#1086#1074#1080#1090#1100' '#1082#1086#1085#1090#1077#1081#1085#1077#1088' '#1080' '#1082#1083#1102#1095#1080
    TabOrder = 1
    WordWrap = True
    OnClick = UpdateContainerButtonClick
  end
  object PageControl1: TPageControl
    Left = 8
    Top = 79
    Width = 265
    Height = 260
    ActivePage = TabSheet1
    MultiLine = True
    TabOrder = 2
    object TabSheet1: TTabSheet
      Caption = #1056#1072#1073#1086#1090#1072' '#1089' '#1092#1072#1081#1083#1072#1084#1080
      object EncryptFileButton: TButton
        Left = 3
        Top = 7
        Width = 121
        Height = 71
        Caption = #1064#1080#1092#1088#1086#1074#1072#1090#1100
        TabOrder = 0
        OnClick = EncryptFileButtonClick
      end
      object EnPasswordEdit: TEdit
        Left = 3
        Top = 85
        Width = 121
        Height = 21
        PasswordChar = '*'
        TabOrder = 1
        Text = '12345678'
      end
      object PassConfirmEdit: TEdit
        Left = 3
        Top = 111
        Width = 121
        Height = 21
        PasswordChar = '*'
        TabOrder = 2
        Text = '12345678'
      end
      object SessionExBtn: TButton
        Left = 3
        Top = 138
        Width = 121
        Height = 42
        Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1089#1077#1089#1089#1080#1086#1085#1085#1099#1081' '#1082#1083#1102#1095
        Enabled = False
        TabOrder = 3
        WordWrap = True
        OnClick = SessionExBtnClick
      end
      object SessionLoadBtn: TButton
        Left = 130
        Top = 138
        Width = 119
        Height = 42
        Caption = #1048#1084#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1089#1077#1089#1089#1080#1086#1085#1085#1099#1081' '#1082#1083#1102#1095
        TabOrder = 4
        WordWrap = True
        OnClick = SessionLoadBtnClick
      end
      object UsingImportKeyCheck: TCheckBox
        Left = 130
        Top = 111
        Width = 121
        Height = 21
        Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1082#1083#1102#1095
        TabOrder = 5
        WordWrap = True
        OnClick = UsingImportKeyCheckClick
      end
      object DePasswordEdit: TEdit
        Left = 130
        Top = 84
        Width = 119
        Height = 21
        PasswordChar = '*'
        TabOrder = 6
        Text = '12345678'
      end
      object DecryptFileButton: TButton
        Left = 130
        Top = 7
        Width = 121
        Height = 71
        Caption = #1056#1072#1089#1096#1080#1092#1088#1086#1074#1072#1090#1100
        TabOrder = 7
        OnClick = DecryptFileButtonClick
      end
      object ExportKeyButton: TButton
        Left = 3
        Top = 186
        Width = 121
        Height = 36
        Caption = #1069#1082#1089#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1090#1082#1088#1099#1090#1099#1081' '#1082#1083#1102#1095
        TabOrder = 8
        WordWrap = True
        OnClick = ExportKeyButtonClick
      end
      object LoadKeyButton: TButton
        Left = 130
        Top = 186
        Width = 119
        Height = 36
        Caption = #1048#1084#1087#1086#1088#1090#1080#1088#1086#1074#1072#1090#1100' '#1086#1090#1082#1088#1099#1090#1099#1081' '#1082#1083#1102#1095
        TabOrder = 9
        WordWrap = True
        OnClick = LoadKeyButtonClick
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1059#1076#1072#1083#1077#1085#1085#1086#1077' '#1087#1086#1076#1082#1083#1102#1095#1077#1085#1080#1077
      ImageIndex = 1
      object ConnectButton: TButton
        Left = 3
        Top = 32
        Width = 126
        Height = 21
        Caption = 'Connect'
        TabOrder = 0
        OnClick = ConnectButtonClick
      end
      object IPEdit: TEdit
        Left = 4
        Top = 5
        Width = 245
        Height = 21
        TabOrder = 1
        Text = '192.168.200.50'
      end
      object SendButton: TButton
        Left = 3
        Top = 57
        Width = 246
        Height = 85
        Caption = 'Send'
        TabOrder = 2
        OnClick = SendButtonClick
      end
      object DownloadProgressBar: TProgressBar
        Left = 3
        Top = 148
        Width = 246
        Height = 13
        TabOrder = 3
      end
      object DisconnectButton: TButton
        Left = 129
        Top = 32
        Width = 120
        Height = 21
        Caption = 'Disconnect'
        TabOrder = 4
        OnClick = DisconnectButtonClick
      end
    end
  end
  object OpenFileDialog: TOpenDialog
    Left = 168
    Top = 400
  end
  object SaveExKeyDialog: TSaveDialog
    Left = 110
    Top = 391
  end
  object ClientSocket: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 29900
    OnConnect = ClientSocketConnect
    OnDisconnect = ClientSocketDisconnect
    OnRead = ClientSocketRead
    OnError = ClientSocketError
    Left = 24
    Top = 320
  end
  object ServerSocket: TServerSocket
    Active = True
    Port = 29900
    ServerType = stNonBlocking
    OnClientConnect = ServerSocketClientConnect
    OnClientDisconnect = ServerSocketClientDisconnect
    OnClientRead = ServerSocketClientRead
    Left = 88
    Top = 320
  end
end
