object fGonik: TfGonik
  Left = 311
  Top = 244
  Width = 1035
  Height = 469
  Color = clSkyBlue
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -14
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 120
  TextHeight = 16
  object Label1: TLabel
    Left = 256
    Top = 10
    Width = 238
    Height = 31
    AutoSize = False
    Caption = '           '#1059#1075#1086#1083' (+'#1074#1083#1077#1074#1086')                '
    Color = clRed
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label3: TLabel
    Left = 492
    Top = 10
    Width = 130
    Height = 31
    AutoSize = False
    Caption = #1057#1090#1088#1077#1083#1072' (+'#1074#1083#1077#1074#1086')'
    Color = clGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveCaption
    Font.Height = -18
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object sgGonik: TStringGrid
    Left = 10
    Top = 39
    Width = 680
    Height = 376
    ColCount = 7
    FixedCols = 0
    RowCount = 2
    FixedRows = 0
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clActiveBorder
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSizing, goColSizing, goEditing]
    ParentFont = False
    TabOrder = 0
    ColWidths = (
      49
      83
      64
      66
      62
      61
      104)
    RowHeights = (
      24
      25)
  end
  object Button1: TButton
    Left = 719
    Top = 69
    Width = 119
    Height = 31
    Caption = #1044#1086#1073#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 719
    Top = 118
    Width = 119
    Height = 31
    Caption = #1042#1089#1090#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 719
    Top = 167
    Width = 119
    Height = 31
    Caption = #1059#1076#1072#1083#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnClick = Button3Click
  end
  object Button4: TButton
    Left = 719
    Top = 217
    Width = 119
    Height = 30
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -20
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    OnClick = Button4Click
  end
  object Button5: TButton
    Left = 719
    Top = 266
    Width = 129
    Height = 31
    Caption = #1053#1077' '#1079#1072#1087#1080#1089#1099#1074#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -17
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = Button5Click
  end
end
