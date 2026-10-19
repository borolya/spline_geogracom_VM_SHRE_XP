object el: Tel
  Tag = 3
  Left = 40
  Top = 252
  Width = 980
  Height = 619
  BorderWidth = 6
  Caption = '  '#1069' '#1083' '#1077' '#1084' '#1077' '#1085' '#1090' '#1099'  '#1087' '#1083' '#1072' '#1085' '#1072
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 0
    Top = 0
    Width = 121
    Height = 25
    AutoSize = False
    Caption = ' '#1063#1080#1089#1083#1086' '#1082#1088#1080#1074#1099#1093':'
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clInfoText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object nach: TLabel
    Left = 151
    Top = -1
    Width = 131
    Height = 26
    AutoSize = False
    Caption = ' '#1053#1072#1095'. '#1076#1083#1080#1085#1072': min'
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label2: TLabel
    Left = 333
    Top = 0
    Width = 34
    Height = 25
    AutoSize = False
    Caption = 'max:'
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Lf: TLabel
    Left = 424
    Top = 0
    Width = 48
    Height = 27
    AutoSize = False
    Caption = '    fact:'
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object Label3: TLabel
    Left = 526
    Top = 0
    Width = 144
    Height = 28
    AutoSize = False
    Caption = ' min. '#1050#1086#1085#1077#1095#1085#1072#1103' '#1076#1083#1080#1085#1072':'
    Color = clSkyBlue
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object nkriv: TEdit
    Left = 121
    Top = 0
    Width = 30
    Height = 21
    ReadOnly = True
    TabOrder = 0
  end
  object Lmin: TEdit
    Left = 284
    Top = 0
    Width = 47
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = '  0'
    OnChange = LminChange
  end
  object Lmax: TEdit
    Left = 368
    Top = 0
    Width = 57
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = '700'
    OnChange = LmaxChange
  end
  object Lfact: TEdit
    Left = 472
    Top = 0
    Width = 53
    Height = 28
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    OnChange = LfactChange
  end
  object Lend: TEdit
    Left = 672
    Top = 0
    Width = 45
    Height = 27
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
  end
  object dob: TButton
    Left = 804
    Top = 88
    Width = 74
    Height = 25
    Caption = ' '#1044#1086#1073#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = dobClick
  end
  object vst: TButton
    Left = 805
    Top = 113
    Width = 73
    Height = 25
    Caption = ' '#1042#1089#1090#1072#1074#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    OnClick = vstClick
  end
  object udal: TButton
    Left = 805
    Top = 136
    Width = 73
    Height = 25
    Caption = ' '#1059#1076#1072#1083#1080#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    OnClick = udalClick
  end
  object Button4: TButton
    Left = 804
    Top = 160
    Width = 115
    Height = 25
    Caption = #1057#1092#1086#1088#1084#1080#1088#1086#1074#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clPurple
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    OnClick = Button4Click
  end
  object Panel1: TPanel
    Left = 73
    Top = 24
    Width = 333
    Height = 36
    Alignment = taLeftJustify
    BorderWidth = 6
    BorderStyle = bsSingle
    Caption = '                  '#1055' '#1077' '#1088' '#1077' '#1093' '#1086' '#1076' '#1085' '#1072' '#1103
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 9
  end
  object Panel2: TPanel
    Left = 404
    Top = 24
    Width = 197
    Height = 35
    Alignment = taLeftJustify
    BorderWidth = 6
    BorderStyle = bsSingle
    Caption = '             '#1056' '#1072' '#1076' '#1080' '#1091' '#1089' '
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
  end
  object Panel4: TPanel
    Left = 0
    Top = 56
    Width = 59
    Height = 35
    BorderWidth = 1
    BorderStyle = bsSingle
    Caption = ' '#8470
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 11
  end
  object Panel5: TPanel
    Left = 75
    Top = 55
    Width = 69
    Height = 37
    BorderWidth = 1
    Caption = 'min'
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
  end
  object Panel6: TPanel
    Left = 143
    Top = 54
    Width = 63
    Height = 38
    Caption = 'max'
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
  end
  object Panel7: TPanel
    Left = 271
    Top = 56
    Width = 68
    Height = 37
    Alignment = taLeftJustify
    Caption = ' '#1053#1072#1095#1072#1083#1086
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 14
  end
  object Panel8: TPanel
    Left = 338
    Top = 56
    Width = 67
    Height = 37
    Alignment = taLeftJustify
    Caption = '     '#1050#1086#1085#1077#1094
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
  end
  object Panel9: TPanel
    Left = 406
    Top = 56
    Width = 63
    Height = 35
    Alignment = taLeftJustify
    Caption = '    min'
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 16
  end
  object sgEl: TStringGrid
    Left = 9
    Top = 92
    Width = 798
    Height = 422
    ColCount = 12
    DefaultColWidth = 65
    FixedCols = 0
    RowCount = 2
    FixedRows = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goColMoving, goEditing]
    TabOrder = 17
    Visible = False
    OnClick = sgElClick
    OnSetEditText = sgElSetEditText
  end
  object Panel10: TPanel
    Left = 204
    Top = 55
    Width = 70
    Height = 37
    Caption = #1092#1072#1082#1090
    Color = clLime
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 18
  end
  object Panel11: TPanel
    Left = 466
    Top = 57
    Width = 68
    Height = 34
    Alignment = taLeftJustify
    Caption = '      max'
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 19
  end
  object Panel12: TPanel
    Left = 533
    Top = 57
    Width = 71
    Height = 34
    Alignment = taLeftJustify
    Caption = '     '#1092#1072#1082#1090
    Color = clYellow
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 20
  end
  object Panel13: TPanel
    Left = 604
    Top = 57
    Width = 66
    Height = 35
    Alignment = taLeftJustify
    Caption = '      min'
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 21
  end
  object Panel14: TPanel
    Left = 670
    Top = 57
    Width = 67
    Height = 35
    Caption = 'max'
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 22
  end
  object Panel15: TPanel
    Left = 735
    Top = 58
    Width = 68
    Height = 35
    Alignment = taLeftJustify
    Caption = '     '#1092#1072#1082#1090
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 23
  end
  object Panel3: TPanel
    Left = 600
    Top = 26
    Width = 200
    Height = 33
    Alignment = taLeftJustify
    Caption = '                  '#1044#1083#1080#1085#1072
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 24
  end
  object Button1: TButton
    Left = 805
    Top = 209
    Width = 89
    Height = 25
    Caption = #1053#1077' '#1079#1072#1087#1080#1089#1099#1074#1072#1090#1100
    TabOrder = 25
    OnClick = Button1Click
  end
  object MClose: TButton
    Left = 806
    Top = 184
    Width = 75
    Height = 25
    Caption = #1047#1072#1087#1080#1089#1072#1090#1100
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ModalResult = 1
    ParentFont = False
    TabOrder = 26
    OnClick = MCloseClick
  end
  object dop: TStaticText
    Left = 806
    Top = 335
    Width = 168
    Height = 25
    AutoSize = False
    Caption = '  '#8470' '#1101#1083'-'#1090#1072'   '#1055#1080#1082#1077#1090'     '#1055#1083#1102#1089' '
    Color = clFuchsia
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 27
    Visible = False
  end
  object numel: TEdit
    Left = 807
    Top = 360
    Width = 47
    Height = 24
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 28
    Visible = False
  end
  object pic: TEdit
    Left = 849
    Top = 359
    Width = 67
    Height = 25
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 29
    Visible = False
  end
  object pl: TEdit
    Left = 917
    Top = 360
    Width = 46
    Height = 24
    AutoSize = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 30
    Visible = False
  end
  object kon: TPanel
    Left = 804
    Top = 299
    Width = 149
    Height = 32
    Caption = #1050#1086#1085#1077#1094' '#1087#1077#1088#1077#1093#1086#1076#1085#1086#1081
    Color = clAqua
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 31
    Visible = False
  end
  object Button2: TButton
    Left = 806
    Top = 234
    Width = 75
    Height = 25
    Caption = #1057#1090#1072#1088#1090
    TabOrder = 32
    Visible = False
    OnClick = Button2Click
  end
  object Button3: TButton
    Left = 806
    Top = 258
    Width = 75
    Height = 25
    Caption = #1042#1086#1079#1074#1088#1072#1090
    TabOrder = 33
    Visible = False
    OnClick = Button3Click
  end
end
