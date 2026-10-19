object Form4: TForm4
  Left = 201
  Top = 128
  Width = 651
  Height = 387
  Caption = ' '#1053#1072#1095#1072#1083#1100#1085#1072#1103' '#1090#1086#1095#1082#1072' '#1080' '#1072#1079#1080#1084#1091#1090
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 16
    Width = 585
    Height = 56
    BorderStyle = bsSingle
    TabOrder = 0
    object StaticText1: TStaticText
      Left = 8
      Top = 8
      Width = 211
      Height = 20
      Caption = '   '#1050#1086#1086#1088#1076#1080#1085#1072#1090#1099' '#1085#1072#1095#1072#1083#1100#1085#1086#1081' '#1090#1086#1095#1082#1080':'
      Color = clAqua
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clMaroon
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
    object X0t: TStaticText
      Left = 274
      Top = 9
      Width = 32
      Height = 20
      Caption = '  X0='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
    object Y0t: TStaticText
      Left = 422
      Top = 8
      Width = 30
      Height = 20
      Caption = ' Y0='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clRed
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 2
    end
    object X0: TEdit
      Left = 305
      Top = 8
      Width = 118
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      Text = '0.0'
      OnChange = X0Change
    end
    object Y0: TEdit
      Left = 450
      Top = 7
      Width = 121
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      Text = '0.0'
      OnChange = Y0Change
    end
  end
  object Panel2: TPanel
    Left = 8
    Top = 89
    Width = 585
    Height = 98
    TabOrder = 1
    object StaticText4: TStaticText
      Left = 12
      Top = 7
      Width = 340
      Height = 20
      Caption = '    '#1059#1075#1086#1083' '#1084#1077#1078#1076#1091' '#1086#1089#1100#1102'   '#1061'  '#1080' '#1085#1072#1095#1072#1083#1100#1085#1099#1084' '#1085#1072#1087#1088#1072#1074#1083#1077#1085#1080#1077#1084' :'
      Color = clAqua
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 0
    end
    object signt: TStaticText
      Left = 14
      Top = 40
      Width = 79
      Height = 20
      Caption = ' '#1047#1085#1072#1082' '#1091#1075#1083#1072'='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 1
    end
    object sign: TEdit
      Left = 95
      Top = 36
      Width = 21
      Height = 33
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = '+'
      OnChange = signChange
    end
    object grt: TStaticText
      Left = 116
      Top = 40
      Width = 69
      Height = 20
      Caption = ' '#1043#1088#1072#1076#1091#1089#1099'='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 3
    end
    object mint: TStaticText
      Left = 226
      Top = 41
      Width = 65
      Height = 20
      Caption = ' '#1052#1080#1085#1091#1090#1099'='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 4
    end
    object sect: TStaticText
      Left = 315
      Top = 40
      Width = 71
      Height = 20
      Caption = ' '#1057#1077#1082#1091#1085#1076#1099'='
      Color = clLime
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 5
    end
    object min: TEdit
      Left = 289
      Top = 40
      Width = 26
      Height = 21
      TabOrder = 6
      Text = '0'
      OnChange = minChange
    end
    object gr: TEdit
      Left = 185
      Top = 40
      Width = 41
      Height = 24
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
      Text = '0'
      OnChange = grChange
    end
    object sec: TEdit
      Left = 387
      Top = 41
      Width = 54
      Height = 21
      TabOrder = 8
      Text = '0'
      OnChange = secChange
    end
  end
  object Button1: TButton
    Left = 498
    Top = 197
    Width = 94
    Height = 26
    Caption = ' '#1047#1072#1087#1080#1089#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 1
    ParentFont = False
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 498
    Top = 232
    Width = 107
    Height = 26
    Caption = ' '#1042#1086#1079#1074#1088#1072#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = Button2Click
  end
  object Panel3: TPanel
    Left = 13
    Top = 264
    Width = 594
    Height = 161
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Visible = False
    object StaticText2: TStaticText
      Left = 4
      Top = 7
      Width = 264
      Height = 24
      Caption = ' '#1044#1083#1080#1085#1072' '#1089#1091#1097#1077#1089#1090#1074#1091#1102#1097#1077#1081' '#1090#1088#1072#1089#1089#1099' '#1074' '#1084'.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object Ls: TEdit
      Left = 319
      Top = 8
      Width = 74
      Height = 28
      TabOrder = 1
    end
    object StaticText3: TStaticText
      Left = 10
      Top = 35
      Width = 313
      Height = 24
      Caption = #1044#1083#1080#1085#1072' '#1087#1088#1086#1077#1082#1090#1085#1086#1081' '#1090#1088#1072#1089#1089#1099' '#1087#1086' '#1093#1086#1088#1076#1072#1084'  '#1074' '#1084'.'
      Color = clBtnFace
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentColor = False
      ParentFont = False
      TabOrder = 2
    end
    object StaticText5: TStaticText
      Left = 7
      Top = 64
      Width = 310
      Height = 24
      Caption = #1044#1083#1080#1085#1072' '#1087#1088#1086#1077#1082#1090#1085#1086#1081' '#1090#1088#1072#1089#1089#1099' '#1087#1086' '#1082#1088#1080#1074#1099#1084' '#1074' '#1084'.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
    end
    object StaticText6: TStaticText
      Left = 8
      Top = 88
      Width = 295
      Height = 24
      Caption = #1054#1090#1085#1086#1089#1080#1090#1077#1083#1100#1085#1086#1077' '#1080#1079#1084#1077#1085#1077#1085#1080#1077' '#1076#1083#1080#1085#1099' '#1074' %'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
    end
    object StaticText7: TStaticText
      Left = 6
      Top = 117
      Width = 300
      Height = 24
      Caption = #1053#1077#1074#1103#1079#1082#1072' '#1074' '#1082#1086#1085#1077#1095#1085#1086#1081' '#1090#1086#1095#1082#1077' ( '#1074' '#1084'.) '#1087#1086'  '#1061'='
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
    end
    object dx: TEdit
      Left = 304
      Top = 118
      Width = 93
      Height = 28
      TabOrder = 6
    end
    object StaticText8: TStaticText
      Left = 402
      Top = 116
      Width = 50
      Height = 24
      Caption = ' '#1087#1086' Y='
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -16
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 7
    end
    object dy: TEdit
      Left = 453
      Top = 112
      Width = 102
      Height = 28
      TabOrder = 8
    end
    object Lp: TEdit
      Left = 321
      Top = 37
      Width = 70
      Height = 28
      TabOrder = 9
    end
    object Lm: TEdit
      Left = 323
      Top = 64
      Width = 69
      Height = 28
      TabOrder = 10
    end
    object dL: TEdit
      Left = 322
      Top = 90
      Width = 73
      Height = 28
      TabOrder = 11
    end
  end
end
