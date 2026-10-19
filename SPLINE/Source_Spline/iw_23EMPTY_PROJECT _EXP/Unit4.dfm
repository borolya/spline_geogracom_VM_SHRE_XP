object fElems: TfElems
  Left = 220
  Top = 359
  Width = 731
  Height = 460
  Caption = 'fElems'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 72
    Top = 8
    Width = 305
    Height = 20
    Caption = '                               '#1055#1077#1088#1077#1093#1086#1076#1085#1072#1103'                      '
    Color = clGrayText
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label2: TLabel
    Left = 136
    Top = 40
    Width = 170
    Height = 20
    Caption = '           '#1053#1072#1095#1072#1083#1086'                 '
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label3: TLabel
    Left = 264
    Top = 40
    Width = 114
    Height = 20
    Caption = '        '#1050#1086#1085#1077#1094'         '
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label4: TLabel
    Left = 384
    Top = 8
    Width = 126
    Height = 16
    Caption = '  '#1050#1088#1091#1075#1086#1074#1072#1103'/'#1055#1088#1103#1084#1072#1103'  '
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object sgElems: TStringGrid
    Left = 8
    Top = 64
    Width = 673
    Height = 345
    ColCount = 8
    FixedCols = 0
    RowCount = 2
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing]
    TabOrder = 0
    OnClick = sgElemsClick
    ColWidths = (
      64
      62
      76
      45
      66
      50
      68
      70)
  end
  object Button1: TButton
    Left = 560
    Top = 104
    Width = 89
    Height = 25
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 560
    Top = 136
    Width = 89
    Height = 25
    Caption = #1042#1089#1090#1072#1074#1080#1090#1100
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 560
    Top = 168
    Width = 89
    Height = 25
    Caption = #1059#1076#1072#1083#1080#1090#1100
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 560
    Top = 200
    Width = 89
    Height = 25
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 560
    Top = 232
    Width = 89
    Height = 25
    Caption = #1053#1077' '#1079#1072#1087#1080#1089#1099#1074#1072#1090#1100
    TabOrder = 5
    OnClick = Button5Click
  end
end
