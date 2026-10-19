object FManFiles: TFManFiles
  Left = 111
  Top = 93
  Width = 791
  Height = 518
  Caption = #1042#1074#1086#1076' '#1082#1086#1086#1088#1076#1080#1085#1072#1090' '#1090#1086#1095#1077#1082' '#1089#1098#1105#1084#1082#1080
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 16
  object Label2: TLabel
    Left = 74
    Top = 4
    Width = 153
    Height = 25
    AutoSize = False
    Caption = '  '#1048#1084#1077#1085#1072'  '#1082#1088#1080#1074#1099#1093
  end
  object ListBox1: TListBox
    Left = 0
    Top = 32
    Width = 298
    Height = 353
    Columns = 3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ItemHeight = 13
    Items.Strings = (
      '')
    ParentFont = False
    TabOrder = 0
    OnDblClick = ListBox1DblClick
  end
  object UDAL: TButton
    Left = 347
    Top = 68
    Width = 75
    Height = 33
    Caption = #1059#1076#1072#1083#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Visible = False
    OnClick = UDALClick
  end
  object Button1: TButton
    Left = 352
    Top = 128
    Width = 75
    Height = 30
    Caption = #1042#1086#1079#1074#1088#1072#1090
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = Button1Click
  end
end
